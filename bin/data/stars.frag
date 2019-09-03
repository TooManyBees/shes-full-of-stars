#version 150

uniform int frameNo;
in vec4 varyingColor;
in float varyingIntensity;
in float varyingFocus;
in float varyingMagnitude;
out vec4 outputColor;

float intensity() {
    // max(1, varyingMagnitude) clamps Sirius, Betelgeuse, etc
    return pow(10, -0.2 * (max(1, varyingMagnitude) - 6.0));
}

// Standard issue Stackoverflow Perlin noise with frame and star as inputs
float twinkle() {
    vec2 star = vec2(frameNo, gl_PrimitiveID);
    return 1 + 0.3 * fract(sin(dot(star.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float d = distance(vec2(0.5), gl_PointCoord.st) * 40;
    float i = intensity() * varyingFocus;
    vec4 col = varyingColor * i / (d * d * d);
    float a = smoothstep(0, 3, col.r + col.g + col.b);
    // 1.5 multiplier is arbitrary to boost starlight after the d^3 falloff
    // shrinks dim stars to points
    outputColor = vec4(col.rgb, a / max(1, varyingMagnitude)) * 1.5 * twinkle();
}
