precision  mediump     float;

uniform    sampler2D   tex0;

uniform    vec2        vTexCoord;
uniform    vec4        Color;

vec4 sepia(vec4 color)
{
	return vec4(dot(Color, vec4(.393, .769, .189, .0)),
				dot(Color, vec4(.349, .686, .168, .0)),
				dot(Color, vec4(.272, .534, .131, .0)),
				Color.a);
}

void main()
{
    gl_FragColor = sepia(texture2D(tex0, vTexCoord) * Color);
}