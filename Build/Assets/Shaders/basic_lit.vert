#version 460 core

#define MAX_LIGHTS 5
#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT{
	vec2 v_texcoord;
	vec3 v_position;
	vec3 v_normal;
	vec3 v_color;
} vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;


uniform vec3 u_ambientLight;

uniform struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	
	float innerSpotAngle;
	float outerSpotAngle;
};

uniform struct Material
{
	sampler2D texture;
	float shininess;
	vec2 tiling;
	vec2 offset;
};

uniform int u_numLights = 1;
uniform Light u_lights[MAX_LIGHTS];
uniform Material u_material;


float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = max(0.0, (1.0 - (light_distance/range)));
	return pow(attenuation, 2.0);
}

vec3 calculateLight(in Light light, in vec3 position, in vec3 normal)
{
float attenuation = 1.0f;
float light_distance;
vec3 light_dir;
	switch(light.type){
	case POINT:
	light_dir = normalize(light.position - position);
	light_distance = length(light.position - position);
	attenuation = calculateAttenuation(light_distance, light.range);
	break;

	case DIRECTIONAL:
		light_dir = normalize(light.direction);
		attenuation = 1.0;
	break;

	case SPOT:
	light_dir = normalize(light.position-position);

	light_distance = length(light.position - position);
	attenuation = calculateAttenuation(light_distance, light.range);

	float angle = acos(dot(light_dir, light.direction));
	if(angle > light.outerSpotAngle) attenuation = 0.0;
	else {
		float spotAttenuation = smoothstep(light.outerSpotAngle, light.innerSpotAngle, angle);
		attenuation *= spotAttenuation;
	}
	break;
	}
	//diffuse
	light_dir = normalize(light.position - position);
	float intensity = max(dot(light_dir, normal), 0);
	vec3 diffuse = light.color * intensity;

	//specular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);

	// blinn-phong
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal, halfway_dir), 0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = vec3(NdotH);

	/*intensity = max(dot(reflection, view_dir),0);
	intensity = pow(intensity, u_material.shininess);
	vec3 specular = vec3(intensity);*/

	return u_ambientLight + diffuse + specular;

	//when this is fixed, make sure to put it in basic_phong.frag
}

void main()
{
	vs_out.v_texcoord = a_texcoord * u_material.tiling + u_material.offset;
	
	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1));
	vec3 normal = mat3(model_view) * a_normal;
	vs_out.v_color = u_ambientLight;
	for(int i = 0; i <u_numLights; i++){
		vs_out.v_color += calculateLight(u_lights[i], position, normal);
	}
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
