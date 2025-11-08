#version 460 core

in VS_OUT{
	vec2 v_texcoord;
	vec3 v_position;
	vec3 v_normal;
	vec3 v_color;
} fs_in;

out vec4 f_color;

//uniform sampler2D u_texture;

uniform struct Material
{
	sampler2D texture;
	float shininess;
	vec2 tiling;
	vec2 offset;
} u_material;

uniform sampler2D u_texture;

void main()
{
	f_color = texture(u_texture, fs_in.v_texcoord) * vec4(fs_in.v_color, 1);
	//f_color = vec4(v_color,1);
}