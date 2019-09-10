#version 150

uniform int frameNo;
in vec4 varyingColor;
in float varyingFocus;
in float varyingMagnitude;
out vec4 outputColor;

const float focusMultiplier = 2.0;

float intensity() {
    // max(1, varyingMagnitude) clamps Sirius, Betelgeuse, etc
    return pow(10, -0.2 * (max(1, varyingMagnitude) - 6.0));
}

vec4 star(vec4 color, float intensity, float dist) {
    vec4 point = color * intensity / (dist * dist * dist);
    vec4 fade = color / dist;
    return point + fade;
}

vec4 star2(vec4 color, float intensity, float dist) {
    return color * pow(intensity, 1) / pow(dist, 2);
}

vec4 star3(vec4 color, float intensity, float dist) {
    vec4 point = color * intensity / (dist * dist);
    vec4 fade = color / pow(dist, 1.5);
    return point + fade;
}

// Standard issue Stackoverflow Perlin noise with frame and star as inputs
float twinkle() {
    vec2 star = vec2(frameNo, gl_PrimitiveID);
    return 1 + 0.2 * fract(sin(dot(star.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float d = distance(vec2(0.5), gl_PointCoord.st) * 50;
    float i = intensity() * 1 + focusMultiplier * varyingFocus;
    vec4 col = star3(varyingColor, i, d);
    float a = smoothstep(0, 3, col.r + col.g + col.b);
    outputColor = vec4(col.rgb, a / max(1, varyingMagnitude)) * twinkle();
}
