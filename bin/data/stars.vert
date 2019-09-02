#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
in vec4 position;
in vec4 color;

uniform int frameNo;
in vec3 starCoord;
in float magnitude;
in int lastFocused;

out vec4 varyingColor;
out float varyingFocus;
out float varyingMagnitude;

const float focusDecay = 10.0;
const float focusMultiplier = 2.0;

float focusSize() {
    float focusTime = clamp(float(frameNo - lastFocused), 0.0, focusDecay);
    float scale = 1.0 + focusMultiplier * (focusDecay - focusTime) / focusDecay;
    return scale;
}

float magnitudeSize(float magnitude) {
    return (-1.0 * magnitude) + 7.0;
}

float magnitudeBrightness(float magnitude) {
    return 1.0;
}

void main() {
    varyingColor = color;
    varyingFocus = focusSize();
    varyingMagnitude = magnitude;

    // vec3 scaledPosition = position.xyz * focusSize() * magnitudeSize(magnitude);
    // vec4 movedPosition = vec4(scaledPosition, position.w) + vec4(starCoord, 1.0);
    gl_Position = modelViewProjectionMatrix * position;
}
