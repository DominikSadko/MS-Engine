uniform    sampler2D   tex0;

varying    vec2     vTexCoord;
uniform    vec4    	Color;
uniform    float 	Time;
uniform    vec2     Resolution;

void  main()
{
	gl_FragColor  =  texture2D(tex0, vTexCoord) * Color;
}