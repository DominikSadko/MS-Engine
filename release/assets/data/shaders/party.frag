precision  mediump     float;

uniform 	sampler2D 	tex0;
uniform	 	vec2		vTexCoord;
uniform   	vec4        Color;
uniform 	float 		Time;

void main()
{
    vec4 col = texture2D(tex0, vTexCoord) * Color;

    float d = Time * 2.0;
    col.x += (1.0 + sin(d))*0.25;
    col.y += (1.0 + sin(d*2.0))*0.25;
    col.z += (1.0 + sin(d*4.0))*0.25;
    gl_FragColor = col;
}
