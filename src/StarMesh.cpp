#include "StarMesh.h"

void StarMesh::push(glm::vec3 position, float magnitude, ofFloatColor color) {
	positions.push_back(position);
	colors.push_back(color);
	magnitudes.push_back(magnitude);
	lastFocus.push_back(0);
}

void StarMesh::pushHour(size_t h, vector<glm::vec3> &ps, vector<float> &ms, vector<ofFloatColor> &cs) {
	for (auto &p : ps) {
		positions.push_back(p);
		lastFocus.push_back(0);
	}
	for (auto &m : ms) magnitudes.push_back(m);
	for (auto &c : cs) colors.push_back(c);

	if (h == 0) hourIndices[h] = 0;
	else hourIndices[h] = positions.size();
}

void StarMesh::init(ofShader &shader) {
	float size = positions.size();

	bufMagnitudes.allocate(magnitudes, GL_STATIC_DRAW);
	bufLastFocus.allocate(lastFocus, GL_DYNAMIC_DRAW);

	ofVbo &starVbo = star.getVbo();

	star.addVertices(positions);
	star.addColors(colors);
	star.setMode(OF_PRIMITIVE_POINTS);

	GLint magnitudeLocation = shader.getAttributeLocation("magnitude");
	lastFocusedLocation = shader.getAttributeLocation("lastFocused");

	starVbo.setAttributeBuffer(magnitudeLocation, bufMagnitudes, 1, 0);
	starVbo.setAttributeBuffer(lastFocusedLocation, bufLastFocus, 1, 0);
}

void updateStarFocus(size_t start, size_t end, const ofCamera &camera, const nite::UserMap &userMap, const vector<glm::vec3> &positions, vector<uint32_t> &lastFocus) {
	uint64_t focusTime = ofGetFrameNum();
	float windowWidth = ofGetWidth();
	float windowHeight = ofGetHeight();
	float mapWidth = userMap.getWidth();
	float mapHeight = userMap.getHeight();
	const nite::UserId* userPixels = userMap.getPixels();

	for (int i = start; i < end; i++) {
		glm::vec3 screenCoordinates = camera.worldToScreen(positions[i]);
		int x = screenCoordinates.x / windowWidth * mapWidth;
		if (x < 0 || x >= mapWidth) continue;
		int y = screenCoordinates.y / windowHeight * mapHeight;
		if (y < 0 || y >= mapHeight) continue;
		if (x >= 0 && y >= 0 && userPixels[y * userMap.getWidth() + x] > 0) {
			lastFocus[i] = (uint32_t)focusTime;
		}
	}
}

void StarMesh::updateFocus(ofCamera &camera, nite::UserMap &userMap) {
	ofVbo &starVbo = star.getVbo();

	// FIXME: Is the -7 correct? Is the 4 correct? Test.
	size_t hour = (HOURS + (size_t)camera.getHeadingDeg() / 15 - 7) % HOURS;
	size_t hoursHorizontal = 4;
	size_t start = hourIndices[(HOURS + hour - hoursHorizontal) % HOURS];
	size_t end = hourIndices[(hour + hoursHorizontal + 1) % HOURS];

	if (start < end) {
		updateStarFocus(start, end, camera, userMap, positions, lastFocus);
	}
	else {
		updateStarFocus(start, positions.size(), camera, userMap, positions, lastFocus);
		updateStarFocus(0, end, camera, userMap, positions, lastFocus);
	}

	starVbo.updateAttributeData(lastFocusedLocation, (float*)lastFocus.data(), positions.size());
	//starVbo.updateAttributeData(starColorLocation, (float*)colors.data(), size);
}

bool StarMesh::isStarInView(ofCamera &camera, glm::vec3 &star) {
	glm::vec3 screenCoordinates = camera.worldToScreen(star);
	return (
		screenCoordinates.x > 0 && screenCoordinates.x < ofGetWidth() &&
		screenCoordinates.y > 0 && screenCoordinates.y < ofGetHeight() &&
		screenCoordinates.z < 1 // weird; find out why
	);
}

void StarMesh::draw() {
	star.draw(OF_MESH_POINTS);
}