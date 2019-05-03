#pragma once

#include "ofMain.h"
#include "utils/ofJson.h"
#include "openni.h"
#include "OniManager.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		static const int WIDTH = 640;
		static const int HEIGHT = 480;
		static const int FPS = 30;

	private:
		ofRectangle canvasSpace = ofRectangle(0, 0, WIDTH, HEIGHT);
		ofImage depthFrame;
		ofImage userFrame;
		glm::vec3 celestialPole;
		vector<glm::vec3> stars_pos;
		vector<ofFloatColor> stars_color;
		ofMesh stars;
		ofCamera camera;
		double lastFrameTime;
		OniManager oni_manager;
};
