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
	void pushHour(size_t h, vector<glm::vec3> &positions, vector<float> &magnitudes, vector<ofFloatColor> &colors);
	void init(ofShader &shader);
	void updateFocus(ofCamera &camera, nite::UserMap &userMap);
	void draw();

private:
	vector<glm::vec3> positions;
	vector<ofFloatColor> colors;
	vector<float> magnitudes;
	vector<uint32_t> lastFocus;

	GLint lastFocusedLocation;
	ofBufferObject bufMagnitudes;
	ofBufferObject bufLastFocus;

	static const size_t HOURS = 24;
	size_t hourIndices[HOURS];
	ofFloatColor randomColor;
	ofFloatColor white;
	ofFloatColor red;
	ofVboMesh star;

	bool isStarInView(ofCamera &camera, glm::vec3 &star);
};