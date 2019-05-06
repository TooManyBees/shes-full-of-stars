#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

uniform int frameNo;
in vec3 starCoord;
in vec4 starColor;
//in float magnitude;
in int lastFocused;

out vec4 varyingColor;

const float maxFocusScale = 10.0;

void main(){
    varyingColor = starColor;

    float focusTime = clamp(float(frameNo - lastFocused), 0.0, maxFocusScale);
    float focusScale = 1.0 + (maxFocusScale - focusTime) / maxFocusScale;

    vec4 scaledPosition = position * focusScale;

    vec4 movedPosition = scaledPosition + vec4(starCoord, 1.0);
    gl_Position = modelViewProjectionMatrix * movedPosition;
}
