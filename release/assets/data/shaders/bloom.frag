uniform sampler2D tex0;
varying vec2 vTexCoord;

uniform vec4 Color;

void main()
{
    vec4 color = texture2D(tex0, vTexCoord);
    int j;
    int i;

    for(i = -4 ;i <= 4; i++)
        for(j = -4; j <= 4; j++)
            color += texture2D(tex0, vTexCoord + vec2(i, j)*0.003) * 0.008;

    gl_FragColor = color * Color;
}
