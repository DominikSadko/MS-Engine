uniform    mat3  u_projection;
uniform    mat3  u_translation;
uniform    vec2  u_Resolution;

attribute  vec2  a_position;
attribute  vec2  a_texCoord;
 
varying    vec2  vTexCoord;

void main()
{
    gl_Position  =  vec4(u_projection * u_translation * vec3(a_position.xy, 1.0), 1.0);
    vTexCoord    =  a_texCoord;
}