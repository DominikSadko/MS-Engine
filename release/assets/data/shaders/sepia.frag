precision  mediump     float;

uniform    sampler2D   tex0;

varying    vec2        vTexCoord;
varying    vec4        vColor;

vec4 sepia(vec4 color)
{
	return vec4(dot(color, vec4(.393, .769, .189, .0)),
				dot(color, vec4(.349, .686, .168, .0)),
				dot(color, vec4(.272, .534, .131, .0)),
				color.a);
}

void main()
{
    gl_FragColor = sepia(texture2D(tex0, vTexCoord) * vColor);
}