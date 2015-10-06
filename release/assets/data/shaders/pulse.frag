precision  mediump     float;

uniform    sampler2D   tex0;

varying    vec2        vTexCoord;
uniform    vec4        Color;
uniform    float       Time;
uniform    vec2        Resolution;

void  main()
{
    vec2 halfres = Resolution.xy/2.0;
    vec2 cPos = vTexCoord.xy * Resolution;

    cPos.x -= 0.5*halfres.x*sin(Time/200.0)+0.3*halfres.x*cos(Time / 200)+halfres.x;
    cPos.y -= 0.4*halfres.y*sin(Time/500.0)+0.3*halfres.y*cos(Time / 200)+halfres.y;
    float cLength = length(cPos);

    vec2 uv = vTexCoord.xy+ ((cPos/cLength)*sin(cLength/30.0-Time*100.0)/25.0)*0.15;
    gl_FragColor = texture2D(tex0,uv) * Color * 250.0/cLength;

	//gl_FragColor  =  texture2D(tex0, vTexCoord) * Color;
}