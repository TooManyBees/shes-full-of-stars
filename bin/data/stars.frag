#version 150

uniform int frameNo;
in vec4 varyingColor;
in float varyingFocus;
in float varyingMagnitude;
out vec4 outputColor;

float invertedMagnitude = -1 * varyingMagnitude + 7;
const float falloff = 45;
float intensity = pow(10, 0.2 * invertedMagnitude);
float fadeIntensity = pow(10, 0.05 * min(4, invertedMagnitude));

vec4 star(vec4 color, float intensity, float focus, float dist) {
    vec4 point = color * intensity * (1 + focus) / (dist * dist * dist);
    vec4 fade = color * fadeIntensity * (1 + focus * 3) / pow(dist, 0.9);
    return point + fade;
}

// Standard issue Stackoverflow Perlin noise with frame and star as inputs
float twinkle() {
    vec2 star = vec2(frameNo, gl_PrimitiveID);
    return 1 + 0.2 * fract(sin(dot(star.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float d = distance(vec2(0.5), gl_PointCoord.st) * falloff;
    float i = intensity;
    vec4 col = star(varyingColor, i, varyingFocus, d);
    float a = smoothstep(0, 6, col.r + col.g + col.b);
    a = a / max(1, varyingMagnitude) * (1 + varyingFocus / 2);
    outputColor = vec4(col.rgb, a) * twinkle();
}
