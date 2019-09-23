#include "OniManager.h"

bool OniManager::setup(int w, int h, int _fps, bool mirror) {
	width = w;
	height = h;
	fps = _fps;

	openni::Status ok = openni::STATUS_OK;
	if (openni::OpenNI::initialize() != ok) return false;

	if (device.open(openni::ANY_DEVICE) != ok) return false;

	if (depthStream.create(device, openni::SENSOR_DEPTH) == ok) {
		openni::VideoMode vm = depthStream.getVideoMode();
		vm.setFps(fps);
		vm.setResolution(w, h);
		if (depthStream.setVideoMode(vm) != ok) return false;
		if (depthStream.setMirroringEnabled(mirror) != ok) return false;
	}
	else return false;

	if (nite::NiTE::initialize() != ok) return false;
	if (userTracker.create(&device) != ok) return false;

	device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

	pDepthMap = new unsigned char[w * h];

	return true;
}

void OniManager::getDepthFrame(ofImage* image /* named `depthFrame` where it's invoked, ugh */) {
	userFrame.release(); // does this need a conditional?

	userTracker.readFrame(&userFrame);
	if (!userFrame.isValid()) {
		printf("err :(");
		return;
	}
	openni::VideoFrameRef depthFrame = userFrame.getDepthFrame();

	if (depthFrame.isValid()) {
		histogram(depthHistogram, depthFrame);
		int frameWidth = depthFrame.getWidth();
		int frameHeight = depthFrame.getHeight();
		memset(pDepthMap, 0, frameWidth * frameHeight);

		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)depthFrame.getData();
		unsigned char* pMap = pDepthMap + depthFrame.getCropOriginY() * frameWidth;
		int rowSize = depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

		for (int y = 0; y < frameHeight; ++y)
		{
			const openni::DepthPixel* pDepth = pDepthRow;
			unsigned char* pTex = pMap + depthFrame.getCropOriginX();

			for (int x = 0; x < frameWidth; ++x, ++pDepth, ++pTex)
			{
				if (*pDepth != 0)
				{
					int nHistValue = depthHistogram[*pDepth];
					*pTex = nHistValue;
				}
			}

			pDepthRow += rowSize;
			pMap += 640;
		}

		image->setFromPixels(pDepthMap, frameWidth, frameHeight, OF_IMAGE_GRAYSCALE);
	}

	depthFrame.release();
}

void OniManager::getColorFrame(ofImage* image) {
	colorFrame.release();
	colorStream.readFrame(&colorFrame);
	if (colorFrame.isValid()) {
		const openni::RGB888Pixel* pColor = (const openni::RGB888Pixel*)colorFrame.getData();
		image->setFromPixels((const unsigned char*)pColor, colorFrame.getWidth(), colorFrame.getHeight(), OF_IMAGE_COLOR);
	}
}

void OniManager::histogram(float *pHistogram, openni::VideoFrameRef& frame) {
	memset(pHistogram, 0, MAX_DEPTH * sizeof(float));
	const openni::DepthPixel* pDepth = (const openni::DepthPixel*)frame.getData();
	int restOfRow = frame.getStrideInBytes() / sizeof(openni::DepthPixel) - frame.getWidth();
	int height = frame.getHeight();
	int width = frame.getWidth();

	unsigned int nNumberOfPoints = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++, pDepth++) {
			if (*pDepth != 0) {
				pHistogram[*pDepth]++;
				nNumberOfPoints++;
			}

		}
		pDepth += restOfRow;
	}

	for (int nIndex = 1; nIndex < MAX_DEPTH; nIndex++) {
		pHistogram[nIndex] += pHistogram[nIndex - 1];
	}
	if (nNumberOfPoints) {
		for (int nIndex = 1; nIndex < MAX_DEPTH; nIndex++) {
			pHistogram[nIndex] = (float)(unsigned int)(256 * (1.0f - (pHistogram[nIndex] / nNumberOfPoints)));
		}
	}
}

nite::UserMap OniManager::getUserMap() {
	userFrame.release();
	userTracker.readFrame(&userFrame);
	return userFrame.getUserMap();
}

bool OniManager::usersPresent() {
	return !userFrame.getUsers().isEmpty();
}

void OniManager::getUserMask(bool *users) {
	if (userFrame.isValid()) {
		nite::UserMap userMap = userFrame.getUserMap();
		int length = userMap.getWidth() * userMap.getHeight();
		const nite::UserId* px = userMap.getPixels();
		for (int i = 0; i < length; i++) {
			users[i] = px[i] > 0;
		}
	}
}

void OniManager::getUserFrame(ofImage* image) {
	if (userFrame.isValid()) {
		nite::UserMap userMap = userFrame.getUserMap();
		int length = userMap.getWidth() * userMap.getHeight();
		const short* userPx = userMap.getPixels();
		ofPixels & pixels = image->getPixelsRef();
		for (int i = 0; i < length; i++) {
			pixels[i] = userPx[i] > 0 ? 255 : 0;
		}
		image->setFromPixels(pixels);
	}
}

void OniManager::updateSkeletonTracking() {
	auto &users = userFrame.getUsers();
	for (size_t i = 0; i < users.getSize(); i++) {
		if (users[i].isNew()) {
			userTracker.startSkeletonTracking(users[i].getId());
		}
		else if (users[i].isLost()) {
			userTracker.stopSkeletonTracking(users[i].getId());
		}
	}
}

vector<glm::vec3> OniManager::getSkeletonPoints() {
	vector<glm::vec3> points;
	if (userFrame.isValid()) {
		points.reserve(20); // arbitrary
		const nite::Array<nite::UserData>& users = userFrame.getUsers();
		for (size_t i = 0; i < users.getSize(); i++) {
			if (!users[i].isVisible()) {
				continue;
			}
			const nite::Skeleton& skeleton = users[i].getSkeleton();
			if (skeleton.getState() != nite::SKELETON_TRACKED) {
				continue;
			}
			for (size_t j = 0; j <= NITE_JOINT_COUNT; j++) {
				const nite::Point3f& pos = skeleton.getJoint((nite::JointType)j).getPosition();
				float x, y;
				userTracker.convertJointCoordinatesToDepth(pos.x, pos.y, pos.z, &x, &y);
				const glm::vec3 point(x / width, y / height, 0);
				points.push_back(point);
			}
		}
	}
	return points;
}
