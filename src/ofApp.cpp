#include "ofApp.h"
#include "colorIndex.h"

//--------------------------------------------------------------
void ofApp::setup() {
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

	starShader.load("stars.vert", "stars.frag");
	starShader.printActiveUniforms();
	starShader.printActiveAttributes();

	ofJson result = ofLoadJson("hyg.json");
	double r = 1000.0;
	glm::vec3 origin(0, 0, 0);
	glm::vec3 normal;
	glm::vec3 mirror(-1.0, 1.0, 1.0);
	for (auto star : result) {
		glm::vec3 xyz(star["x"], star["z"], star["y"]); // intentionally swap y/z
		glm::vec3 pos;
		bool b = glm::intersectLineSphere(origin, xyz, origin, r, pos, normal);
		size_t hour = star["hour"];
		ofFloatColor color = colorIndexToRGB(star["ci"]);
		float magnitude = star["magnitude"];
		if (b && hour >= 0 && hour < 24) {
			star_meshes[hour].push(pos * mirror, magnitude, color);
		}
	}
	for (auto &star_mesh : star_meshes) {
		star_mesh.init(starShader);
	}

	ofSetFrameRate(FPS);
	ofSetVerticalSync(true);
	camera.lookAt(glm::vec3(0.0, 0.0, -1.0), celestialPole);
	//cout << "x: " << camera.getXAxis() << ", y: " << camera.getYAxis() << ", z: " << camera.getZAxis() << endl;
	glEnable(GL_POINT_SMOOTH);
	glCullFace(GL_BACK);

	userFrame.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update() {
	reloadShader();
	camera.rotateDeg(0.002, celestialPole);
	nite::UserMap userMap = oni_manager.getUserMap();
	//oni_manager.getUserFrame(&userFrame);

	for (int i = 0; i < STAR_MESH_COUNT; i++) {
		bool in_view = star_meshes[i].isInView(camera);
		star_mesh_in_view[i] = in_view;

		if (oni_manager.usersPresent()) {
			if (in_view) {
				star_meshes[i].updateFocus(camera, userMap);
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(0, 0, 0), ofColor(0, 8, 30), OF_GRADIENT_LINEAR);
	//userFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofSetColor(255);
	glEnable(GL_CULL_FACE);
	camera.begin();
	starShader.begin();
	starShader.setUniform1i("frameNo", ofGetFrameNum());
	for (auto &star_mesh : star_meshes) {
		star_mesh.draw();
	}
	starShader.end();
	camera.end();
	glDisable(GL_CULL_FACE);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case ' ':
		shaderDirty = true;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::reloadShader() {
	if (shaderDirty) {
		ofLogNotice() << "Reloading shader." << endl;
		starShader.load("stars.vert", "stars.frag");
		GLint err = glGetError();
		if (err != GL_NO_ERROR) {
			ofLogNotice() << "Shader failed to compile:" << endl << err << endl;
		}
		shaderDirty = false;
	}
}