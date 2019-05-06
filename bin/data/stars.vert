#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec4 color;
in vec4 normal;
in vec4 texcoord;

uniform uint frameNo;
in vec3 starCoord;
//in float magnitude;
//in uint lastFocused;

void main(){
    // float focusScale =  max(10.0, frameNo - lastFocused) / 10.0 + 1;
    // gl_Position = modelViewProjectionMatrix * (position * focusScale * 1000.0 + vec4(starCoord, 1.0));
    vec4 movedPosition = position + vec4(starCoord, 1.0);
    //vec4 movedPosition = position;
    //movedPosition.y += gl_InstanceID * 100.0;
    gl_Position = modelViewProjectionMatrix * movedPosition;
}
