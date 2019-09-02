#version 150

uniform vec4 globalColor;
in vec4 varyingColor;
in float varyingIntensity;
in float varyingFocus;
in float varyingMagnitude;
out vec4 outputColor;

float intensity() {
    return pow(10, -0.3 * (varyingMagnitude - 7.0));
}

float magnitudeSize() {
    return (-1.0 * varyingMagnitude) + 7.0;
}

void main() {
    float d = distance(vec2(0.5), gl_PointCoord.st) * 100;
    vec4 col = varyingColor * intensity() / (d * d * d);
    float a = smoothstep(0, 3, col.r + col.g + col.b);
    outputColor = vec4(col.rgb, a / max(1, varyingMagnitude));
    // outputColor = col * step(varyingMagnitude, 0.5);
    // outputColor = col * intensity();
}
