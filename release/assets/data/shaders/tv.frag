precision  mediump     float;

uniform    sampler2D   tex0;

varying    vec2        vTexCoord;
uniform    vec2        Color
uniform    float       Time;
uniform    vec2        Resolution;

void  main()
{
    vec2 q = vTexCoord;
    vec2 uv = 0.5 + (q-0.5)*(0.9 + 0.1*sin(0.2*Time));

    vec4 oricol = texture2D(tex0, vTexCoord);
    vec4 col = oricol;

    col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);

    col *= 0.5 + 0.5*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);

    col *= vec4(0.8,1.0,0.7, 1.0);

    col *= 0.9+0.1*sin(10.0*Time+uv.y*1000.0);

    col *= 0.97+0.03*sin(110.0*Time);

    gl_FragColor = col;

	//gl_FragColor  =  texture2D(tex0, vTexCoord) * Color;
}