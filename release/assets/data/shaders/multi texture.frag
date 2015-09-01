precision  mediump     float;

uniform    sampler2D   tex0;
uniform    sampler2D   tex1;

varying    vec2        vTexCoord;
varying    vec4        vColor;

void  main()
{
	vec4 t1  =  texture2D(tex0, vTexCoord);
	vec4 t2  =  texture2D(tex1, vTexCoord);

	gl_FragColor  =  t1 * t2 * vColor;
}