#version 150

uniform mat4 modelViewProjectionMatrix;
uniform int frameNo;

in vec4 position;
in vec4 color;
in float magnitude;
in int lastFocused;

out vec4 varyingColor;
out float varyingFocus;
out float varyingMagnitude;

const float focusDecay = 15.0;

float focus() {
    float focusTime = clamp(float(frameNo - lastFocused), 0.0, focusDecay);
    return (focusDecay - focusTime) / focusDecay;
}

void main() {
    varyingColor = color;
    varyingFocus = focus();
    varyingMagnitude = magnitude;
    gl_Position = modelViewProjectionMatrix * position;
}
