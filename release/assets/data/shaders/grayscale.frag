precision  mediump     float;

uniform    sampler2D   tex0;

varying    vec2        vTexCoord;
uniform    vec4        Color;

vec4 grayscale(vec4 color)
{
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    return vec4(gray, gray, gray, color.a);
}

void  main()
{
	gl_FragColor = grayscale(texture2D(tex0, vTexCoord) * Color);
}