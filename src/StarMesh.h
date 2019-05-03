#pragma once

class StarMesh {
public:
	StarMesh() {
		mesh.setMode(OF_PRIMITIVE_POINTS);
		randomColor = ofFloatColor::fromHsb(ofRandom(1.0), 1.0, 1.0);
		white = ofFloatColor(1.0, 1.0, 1.0);
		red = ofFloatColor(1.0, 0.0, 0.0);
	}
	ofMesh mesh;
	vector<glm::vec3> positions;
	vector<ofFloatColor> colors;
	int probeIndices[5];
	ofFloatColor randomColor;
	ofFloatColor white;
	ofFloatColor red;

	void push(glm::vec3 position) {
		positions.push_back(position);
		colors.push_back(white);
	}

	void init() {
		float size = positions.size();
		probeIndices[0] = (int)(size * 0.1);
		probeIndices[1] = (int)(size * 0.3);
		probeIndices[2] = (int)(size * 0.5);
		probeIndices[3] = (int)(size * 0.7);
		probeIndices[4] = (int)(size * 0.9);

		mesh.addVertices(positions);
		mesh.addColors(colors);
		/*for (auto i : probeIndices) {
			mesh.setColor(i, randomColor);
		}*/
		mesh.setMode(OF_PRIMITIVE_POINTS);
	}

	bool isInView(ofCamera &camera) {
		for (int i : probeIndices) {
			if (isStarInView(camera, positions[i])) {
				return true;
			}
		}
		return false;
	}

	void updateFocus(ofCamera &camera, nite::UserMap &userMap) {
		float windowWidth = ofGetWidth();
		float windowHeight = ofGetHeight();
		float mapWidth = userMap.getWidth();
		float mapHeight = userMap.getHeight();
		const nite::UserId* userPixels = userMap.getPixels();
		for (int i = 0; i < positions.size(); i++) {
			glm::vec3 screenCoordinates = camera.worldToScreen(positions[i]);
			int x = screenCoordinates.x / windowWidth * mapWidth;
			if (x < 0 || x >= mapWidth) continue;
			int y = screenCoordinates.y / windowHeight * mapHeight;
			if (y < 0 || y >= mapHeight) continue;
			if (x >= 0 && y >= 0 && userPixels[y * userMap.getWidth() + x] > 0) {
				mesh.setColor(i, red);
			}
		}
	}

	void draw() {
		mesh.draw();
	}

private:
	bool isStarInView(ofCamera &camera, glm::vec3 &star) {
		glm::vec3 screenCoordinates = camera.worldToScreen(star);
		return (
			screenCoordinates.x > 0 && screenCoordinates.x < ofGetWidth() &&
			screenCoordinates.y > 0 && screenCoordinates.y < ofGetHeight() &&
			screenCoordinates.z > 0
		);
	}
};