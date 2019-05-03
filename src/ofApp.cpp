#include "ofApp.h"

bool starInView(ofCamera &camera, glm::vec3 &star, nite::UserMap &user_map) {
	float windowWidth = ofGetWidth();
	float windowHeight = ofGetHeight();

	glm::vec3 screenCoordinates = camera.worldToScreen(star);
	if (
		screenCoordinates.x > 0 && screenCoordinates.x < ofGetWidth() &&
		screenCoordinates.y > 0 && screenCoordinates.y < ofGetHeight()
		) {
		int x = screenCoordinates.x / windowWidth * (float)user_map.getWidth();
		int y = screenCoordinates.y / windowHeight * (float)user_map.getHeight();
		if (x >= user_map.getWidth() || y >= user_map.getHeight()) {
			return false;
		}
		return user_map.getPixels()[y * user_map.getWidth() + x] > 0;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	if (oni_manager.setup(WIDTH, HEIGHT, FPS, true)) {
		cout << "Setup device and streams.\n" << endl;
	}
	else {
		cerr << openni::OpenNI::getExtendedError() << endl;
		return std::exit(1);
	}

	{
		float angle = ofDegToRad(23.4);
		celestialPole = glm::vec3(glm::sin(angle), glm::cos(angle), 0.0);
	}

	ofJson result = ofLoadJson("hyg.json");
	double r = 1000.0;
	glm::vec3 origin(0, 0, 0);
	glm::vec3 throwaway;
	for (auto star : result) {
		glm::vec3 xyz(star["x"], star["y"], star["z"]);
		glm::vec3 pos;
		bool b = glm::intersectLineSphere(origin, xyz, origin, r, pos, throwaway);
		if (b) {
			stars_pos.push_back(pos);
			stars_color.push_back(ofFloatColor(1.0, 1.0, 1.0));
		}
	}
	stars.addVertices(stars_pos);
	stars.addColors(stars_color);

	stars.setMode(OF_PRIMITIVE_POINTS);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	camera.lookAt(glm::vec3(0.0, 0.0, -1.0), celestialPole);
	//cout << "x: " << camera.getXAxis() << ", y: " << camera.getYAxis() << ", z: " << camera.getZAxis() << endl;
	glEnable(GL_POINT_SMOOTH);

	depthFrame.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
	userFrame.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
	userFrame.setColor(ofColor(1.0, 0.0, 0.0, 1.0));
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){
	camera.rotateDeg(0.002, celestialPole);
	//oni_manager.getDepthFrame(&depthFrame);
	nite::UserMap userMap = oni_manager.getUserMap();
	for (int i = 0; i < stars_pos.size(); i++) {
		if (starInView(camera, stars_pos[i], userMap)) {
			stars_color[i] = ofFloatColor(1.0, 0, 0);
		}
		else {
			stars_color[i] = ofFloatColor(1.0, 1.0, 1.0);
		}
	}

	stars.clearColors();
	stars.addColors(stars_color);
	//oni_manager.getUserFrame(&userFrame);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(0, 0, 0), ofColor(0, 8, 30), OF_GRADIENT_LINEAR);
	//userFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
	//ofSetColor(ofColor(0, 0, 0, 0));
	//ofDrawRectangle(0, 0, WIDTH, HEIGHT);
	//ofSetColor(ofColor::white);
	camera.begin();
		stars.draw();
	camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
