#version 150

uniform sampler2DRect tex0;
in vec2 texCoordVarying;
out vec4 outputColor;

const float blurAmnt = 10.0;

void main()
{
    // vec4 color = texture(tex0, texCoordVarying);
    // color.rgb = normalize(color.rgb);
    // outputColor = color;
    // return;

    vec4 additive = vec4(0.);
    for (float n = -1.0 * blurAmnt; n <= blurAmnt; n += 1.0) {
        vec2 samplePoint = texCoordVarying + vec2(n, 0.0);
        float clipScreen = step(0., samplePoint.x) * step(samplePoint.x, 1024.); // don't count the samples off screen
        vec4 sample = texture(tex0, samplePoint); // contains non-normalized rgb and normalized alpha. hmm.
        sample = vec4(normalize(sample.rgb), sample.a);
        // float d = abs(n) / (blurAmnt);
        // additive += clipScreen * sample / (1.0 - d * d * d);
        additive += clipScreen * (sample / (blurAmnt * .5));
        // additive += clipScreen * (sample * (1.0 - abs(n) / blurAmnt ));
    }
    // float a = step(vec3(1.), additive.rgb).r + additive.a;

    // additive = step(gl_FragCoord.x, 100.) * vec3(1.0, 0., 0.) + step(100., gl_FragCoord.x) * additive;

    outputColor = vec4(additive);
}
