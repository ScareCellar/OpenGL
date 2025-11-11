layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out vec2 v_texcoord;
out vec3 v_position;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out VS_OUT{
	vec2 v_texcoord;
	vec3 v_position;
	vec3 v_normal;
} vs_out;

uniform struct Material
{
	vec3 baseColor;
	vec3 emissiveColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
} u_material;

void main()
{
	vs_out.v_texcoord = a_texcoord * u_material.tiling + u_material.offset;
	
	mat4 model_view = u_view * u_model;
	vs_out.v_position = vec3(model_view * vec4(a_position, 1));
	vs_out.v_normal = mat3(model_view) * a_normal;
	
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}