#pragma once

#include "ofMain.h"
#include "NiTE.h"

class StarMesh {
public:
	StarMesh() {
		randomColor = ofFloatColor::fromHsb(ofRandom(1.0), 1.0, 1.0);
		white = ofFloatColor(1.0, 1.0, 1.0);
		red = ofFloatColor(1.0, 0.0, 0.0);
	}
	void push(glm::vec3 position, float magnitude, ofFloatColor color);
	void init(ofShader &shader);
	bool isInView(ofCamera &camera);
	void updateFocus(ofCamera &camera, nite::UserMap &userMap);
	void draw();
	size_t size();

private:
	vector<glm::vec3> positions;
	vector<ofFloatColor> colors;
	vector<float> magnitudes;
	vector<uint32_t> lastFocus;

	GLint lastFocusedLocation;
	ofBufferObject bufMagnitudes;
	ofBufferObject bufLastFocus;

	int probeIndices[5];
	ofFloatColor randomColor;
	ofFloatColor white;
	ofFloatColor red;
	ofVboMesh star;

	bool isStarInView(ofCamera &camera, glm::vec3 &star);
};