#pragma once

#include "ofMain.h"
#include "NiTE.h"

class StarMesh {
public:
	StarMesh() {
		randomColor = ofFloatColor::fromHsb(ofRandom(1.0), 1.0, 1.0);
	}
	void push(glm::vec3 position, float magnitude, ofFloatColor color);
	void init(ofShader &shader);
	void updateFocus(ofCamera &camera, nite::UserMap &userMap);
	void draw();
	size_t size() {
		return positions.size();
	}

private:
	vector<glm::vec3> positions;
	vector<ofFloatColor> colors;
	vector<float> magnitudes;
	vector<uint32_t> lastFocus;

	GLint lastFocusedLocation;
	ofBufferObject bufMagnitudes;
	ofBufferObject bufLastFocus;

	ofFloatColor randomColor;
	static const size_t NUM_PROBES = 5;
	glm::vec3 probes[NUM_PROBES];
	ofVboMesh star;

	bool isInView(ofCamera &camera);
	bool isStarInView(ofCamera &camera, glm::vec3 &star);
};