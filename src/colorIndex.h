#pragma once

float colorIndexToTemp(float bv) {
	return (
		4600.0 / (0.92 * bv + 1.7) + 4600.0 / (0.92 * bv + 0.62)
		);
}

ofFloatColor colorTempToRgb(float temp) {
	temp = ofClamp(temp, 1000.0, 40000.0) / 100.0;

	float r = 255;
	if (temp > 66) {
		float calc = temp - 60.0;
		calc = 329.698727446 * pow(calc, -0.1332047592);
		r = ofClamp(calc, 0.0, 255.0);
	}

	float g;
	if (temp <= 66) {
		float calc = 99.4708025861 * log(temp) - 161.1195681661;
		g = ofClamp(calc, 0.0, 255.0);
	} else {
		float calc = temp - 60.0;
		calc = 288.1221695283 * pow(calc, -0.0755148492);
		g = ofClamp(calc, 0.0, 255.0);
	}

	float b;
	if (temp >= 66) {
		b = 255.0;
	} else if (temp <= 19) {
		b = 0;
	} else {
		float calc = temp - 10.0;
		calc = 138.5177312231 * log(calc) - 305.0447927307;
		b = ofClamp(calc, 0.0, 255.0);
	}

	return ofFloatColor(
		ofNormalize(r, 0, 255),
		ofNormalize(g, 0, 255),
		ofNormalize(b, 0, 255)
	);
}

ofFloatColor colorIndexToRGB(float bv) {
	float temp = colorIndexToTemp(bv);
	return colorTempToRgb(temp);
}

// https://www.shadertoy.com/view/XdsGWs

float f(float x) {
	return (6. + x * (6. + x * (3. + x))) * exp(-x);
}

float intPlanck(float t, float lambda1, float lambda0) {
	const float A = 1.1;
	const float B = 1. / 1.05;
	float C0 = 0.014387770;
	float C = C0 / (B * t);
	t = 1.;
	return 100.* A / B * pow(100. * t / C0, 4.) * (f(C / lambda1) - f(C / lambda0));
}

glm::vec3 planck(float t) {
	return glm::vec3(
		intPlanck(t, 0.7e-6, 0.55e-6),
		intPlanck(t, 0.55e-6, 0.49e-6),
		intPlanck(t, 0.49e-6, 0.4e-6)
	)*1e-14;
}

ofFloatColor colorIndexToRGB2(float bv) {
	float temp = colorIndexToTemp(bv);
	glm::vec3 rgb = planck(temp);
	return ofFloatColor(rgb.x, rgb.y, rgb.z);
}

// https://www.seedofandromeda.com/blogs/51-procedural-star-rendering

glm::vec3 getTempColorShift(float temperature) {
	return glm::vec3(temperature * (0.0534 / 255.0) - (43.0 / 255.0),
		temperature * (0.0628 / 255.0) - (77.0 / 255.0),
		temperature * (0.0735 / 255.0) - (115.0 / 255.0));
}

ofFloatColor colorIndexToRGB3(float bv) {
	float temp = colorIndexToTemp(bv);
	glm::vec3 rgb = getTempColorShift(temp);
	return ofFloatColor(rgb.x, rgb.y, rgb.z);
}
