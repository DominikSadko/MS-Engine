uniform    mat3  u_projection;
uniform    mat3  u_translation;
uniform    vec4  u_color;

attribute  vec2  a_position;
attribute  vec2  a_texCoord;
 
varying    vec2  vTexCoord;
varying    vec4  vColor;

void main()
{
    gl_Position  =  vec4(u_projection * u_translation * vec3(a_position.xy, 1.0), 1.0);
    vTexCoord    =  a_texCoord;
    vColor       =  u_color;
}