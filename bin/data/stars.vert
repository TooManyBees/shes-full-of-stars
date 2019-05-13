#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

uniform int frameNo;
in vec3 starCoord;
in vec4 starColor;
in float magnitude;
in int lastFocused;

out vec4 varyingColor;

const float focusDecay = 10.0;
const float focusMultiplier = 2.0;

vec4 focusSize(vec4 star) {
    float focusTime = clamp(float(frameNo - lastFocused), 0.0, focusDecay);
    float scale = 1.0 + focusMultiplier * (focusDecay - focusTime) / focusDecay;
    return star * scale;
}

float magnitudeSize(float magnitude) {
    return (-1.0 * magnitude) + 7.0;
}

float magnitudeBrightness(float magnitude) {
    return 1.0;
}

void main(){
    varyingColor = starColor;

    vec4 scaledPosition = focusSize(position) * magnitudeSize(magnitude);

    vec4 movedPosition = scaledPosition + vec4(starCoord, 1.0);
    gl_Position = modelViewProjectionMatrix * movedPosition;
}