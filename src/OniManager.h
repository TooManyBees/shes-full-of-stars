#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "NiTE.h"

# define MAX_DEPTH 10000

class OniManager {
public:
	bool setup(int w, int h, int fps, bool mirror = true);

	void getColorFrame(ofImage* image);
	void getDepthFrame(ofImage* image);
	nite::UserMap getUserMap();
	void getUserMask(bool *users);
	void getUserFrame(ofImage* image);
	bool usersPresent();
	void updateSkeletonTracking();
	vector<glm::vec3> getSkeletonPoints();
	int getWidth() { return width; };
	int getHeight() { return height; };
	int getFps() { return fps; };

private:
	int width;
	int height;
	int fps;
	openni::Device device;
	openni::VideoStream depthStream;
	openni::VideoStream colorStream;
	openni::VideoFrameRef colorFrame;

	nite::UserTracker userTracker;
	nite::UserTrackerFrameRef userFrame;

	unsigned char* pDepthMap;
	float depthHistogram[MAX_DEPTH];
	void histogram(float* pHistogram, openni::VideoFrameRef& frame);
};
