precision  mediump     float;

uniform    sampler2D   tex0;

varying    vec2        vTexCoord;
varying    vec4        vColor;

void  main()
{
	gl_FragColor  =  texture2D(tex0, vTexCoord) * vColor;
}