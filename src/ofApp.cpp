#include "ofApp.h"
#include "colorIndex.h"

void findStar(char* name, glm::vec3 &dst, glm::vec3 &pos, ofFloatColor &color, nlohmann::json &star) {
	if (dst == glm::vec3(0.0) && star["name"] == name) {
		cout << "Found " << name << "! (" << pos << ") (" << color << ") " << star["magnitude"] << endl;
		dst = pos;
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
#ifdef FOCUS
	if (oni_manager.setup(WIDTH, HEIGHT, FPS, true)) {
		cout << "Setup device and streams.\n" << endl;
	}
	else {
		cerr << openni::OpenNI::getExtendedError() << endl;
		return std::exit(1);
	}
#endif

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

	for (size_t hour = 0; hour < 24; hour++) {
		star_slice_indices[hour] = star_mesh.size();
		for (auto star : result) {
			if ((size_t)star["hour"] != hour) continue;

			glm::vec3 xyz(star["x"], star["z"], star["y"]); // intentionally swap y/z
			glm::vec3 pos;
			bool b = glm::intersectLineSphere(origin, xyz, origin, r, pos, normal);
			pos *= mirror;
			size_t hour = star["hour"];
			ofFloatColor color = colorIndexToRGB2(star["ci"]);
			float magnitude = star["magnitude"];
			if (b) {
				findStar("Betelgeuse", betelgeuse, pos, color, star);
				findStar("Rigel", rigel, pos, color, star);
				findStar("Sirius", sirius, pos, color, star);
				star_mesh.push(pos, magnitude, color);
			}
		}
	}
	/*for (int z = 0; z <= 24; z++) {
		cout << star_slice_indices[z] << endl;
	}*/

	//for (auto star : result) {
	//	glm::vec3 xyz(star["x"], star["z"], star["y"]); // intentionally swap y/z
	//	glm::vec3 pos;
	//	bool b = glm::intersectLineSphere(origin, xyz, origin, r, pos, normal);
	//	size_t hour = star["hour"];
	//	ofFloatColor color = colorIndexToRGB2(star["ci"]);
	//	float magnitude = star["magnitude"];
	//	if (b && hour >= 0 && hour < 24) {
	//		if (betelgeuse == glm::vec3(0.0) && star["name"] == "Betelgeuse") {
	//			cout << "Found betelgeuse! " << pos * mirror << endl;
	//			betelgeuse = pos * mirror;
	//			color *= 100.0;
	//		}
	//		//star_meshes[hour].push(pos * mirror, magnitude, color);
	//		star_mesh.push(pos * mirror, magnitude, color);
	//	}
	//}
	//for (auto &star_mesh : star_meshes) {
		star_mesh.init(starShader);
	//}

	ofSetFrameRate(FPS);
	ofSetVerticalSync(true);
#ifndef FREECAM
	//camera.lookAt(glm::vec3(0.0, 0.0, -1.0), celestialPole);
	camera.lookAt(betelgeuse, celestialPole);
#endif
	//cout << "x: " << camera.getXAxis() << ", y: " << camera.getYAxis() << ", z: " << camera.getZAxis() << endl;
	glEnable(GL_POINT_SMOOTH);
	glCullFace(GL_BACK);

	userFrame.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
	ofEnableAlphaBlending();
	glPointSize(25);
}

//--------------------------------------------------------------
void ofApp::update() {
	reloadShader();
#ifndef FREECAM
	//camera.rotateDeg(0.002, celestialPole);
#endif
#ifdef FOCUS
	nite::UserMap userMap = oni_manager.getUserMap();
	//oni_manager.getUserFrame(&userFrame);

	//for (int i = 0; i < STAR_MESH_COUNT; i++) {
	//	bool in_view = star_meshes[i].isInView(camera);
	//	star_mesh_in_view[i] = in_view;

	//	if (oni_manager.usersPresent()) {
	//		if (in_view) {
	//			star_meshes[i].updateFocus(camera, userMap);
	//		}
	//		//star_mesh.updateFocus(camera, userMap);
	//	}
	//}
#endif
}

void drawStar(glm::vec3 &star, char *name, ofCamera &camera) {
	if (star != glm::vec3(0.0)) {
		glm::vec3 s = camera.worldToScreen(star);
		ofSetColor(20);
		ofDrawCircle(s.x, s.y, 5);
		ofSetColor(255);
		ofDrawBitmapString(name, s.x + 10.0, s.y);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(0, 0, 0), ofColor(0, 8, 30), OF_GRADIENT_LINEAR);
#ifdef FOCUS
	//userFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
#endif

	//drawStar(betelgeuse, "Betelgeuse", camera);
	//drawStar(rigel, "Rigel", camera);
	//drawStar(sirius, "Sirius", camera);

	ofSetColor(255);
	glEnable(GL_CULL_FACE);
	camera.begin();
	starShader.begin();
	starShader.setUniform1i("frameNo", ofGetFrameNum());
	//for (auto &star_mesh : star_meshes) {
		star_mesh.draw();
	//}
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
