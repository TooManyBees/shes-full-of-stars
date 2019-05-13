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
