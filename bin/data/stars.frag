#version 150

uniform vec4 globalColor;
in vec3 varyingColor;
in float varyingMagnitude;
out vec4 outputColor;

float intensity() {
    return pow(10, -0.4 * (varyingMagnitude - 5.0));
}

void main() {
    // outputColor = vec4(varyingColor.rgb * intensity(), varyingColor.a);

    // outputColor = vec4(varyingColor, 1.0);
    outputColor = vec4(varyingColor * intensity(), 1.0);
}
