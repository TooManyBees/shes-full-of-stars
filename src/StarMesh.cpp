#include "StarMesh.h"

void StarMesh::push(glm::vec3 position, float magnitude, ofFloatColor color) {
	positions.push_back(position);
	colors.push_back(color);
	magnitudes.push_back(magnitude);
	lastFocus.push_back(0);
}

void StarMesh::init(ofShader &shader) {
	float size = positions.size();
	probeIndices[0] = (int)(size * 0.1);
	probeIndices[1] = (int)(size * 0.3);
	probeIndices[2] = (int)(size * 0.5);
	probeIndices[3] = (int)(size * 0.7);
	probeIndices[4] = (int)(size * 0.9);
	
	bufPositions.allocate(positions, GL_STATIC_DRAW);
	bufColors.allocate(colors, GL_STATIC_DRAW);
	bufMagnitudes.allocate(magnitudes, GL_STATIC_DRAW);
	bufLastFocus.allocate(lastFocus, GL_DYNAMIC_DRAW);

	ofSpherePrimitive sphere;
	sphere.set(1, 3);
	star = sphere.getMesh();
	ofVbo &starVbo = star.getVbo();

	GLint starCoordLocation = shader.getAttributeLocation("starCoord");
	GLint magnitudeLocation = shader.getAttributeLocation("magnitude");
	lastFocusedLocation = shader.getAttributeLocation("lastFocused");
	starColorLocation = shader.getAttributeLocation("starColor");

	starVbo.setAttributeBuffer(starCoordLocation, bufPositions, 3, 0);
	starVbo.setAttributeDivisor(starCoordLocation, 1);

	starVbo.setAttributeBuffer(magnitudeLocation, bufMagnitudes, 1, 0);
	starVbo.setAttributeDivisor(magnitudeLocation, 1);

	starVbo.setAttributeBuffer(lastFocusedLocation, bufLastFocus, 1, 0);
	starVbo.setAttributeDivisor(lastFocusedLocation, 1);

	starVbo.setAttributeBuffer(starColorLocation, bufColors, 4, 0);
	starVbo.setAttributeDivisor(starColorLocation, 1);
}

bool StarMesh::isInView(ofCamera &camera) {
	for (int i : probeIndices) {
		if (isStarInView(camera, positions[i])) {
			return true;
		}
	}
	return false;
}

void StarMesh::updateFocus(ofCamera &camera, nite::UserMap &userMap) {
	ofVbo &starVbo = star.getVbo();
	uint64_t focusTime = ofGetFrameNum();
	float windowWidth = ofGetWidth();
	float windowHeight = ofGetHeight();
	float mapWidth = userMap.getWidth();
	float mapHeight = userMap.getHeight();
	const nite::UserId* userPixels = userMap.getPixels();

	size_t size = positions.size();
	
	for (int i = 0; i < size; i++) {
		glm::vec3 screenCoordinates = camera.worldToScreen(positions[i]);
		int x = screenCoordinates.x / windowWidth * mapWidth;
		if (x < 0 || x >= mapWidth) continue;
		int y = screenCoordinates.y / windowHeight * mapHeight;
		if (y < 0 || y >= mapHeight) continue;
		if (x >= 0 && y >= 0 && userPixels[y * userMap.getWidth() + x] > 0) {
			lastFocus[i] = (uint32_t) focusTime;
		}
	}

	starVbo.updateAttributeData(lastFocusedLocation, (float*)lastFocus.data(), size);
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
	star.drawInstanced(OF_MESH_FILL, positions.size());
}