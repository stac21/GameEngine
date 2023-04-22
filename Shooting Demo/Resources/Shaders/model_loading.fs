#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} fs_in;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool is_on;
};

struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	bool is_on;
};

struct SpotLight {
	vec3 pos;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	bool is_on;
};

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

uniform int num_point_lights;
uniform int num_spot_lights;

uniform vec3 viewPos;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_diffuse6;
uniform sampler2D texture_diffuse7;
uniform sampler2D texture_diffuse8;
uniform sampler2D texture_diffuse9;
uniform sampler2D texture_diffuse10;
uniform sampler2D texture_diffuse11;
uniform sampler2D texture_diffuse12;
uniform sampler2D texture_diffuse13;
uniform sampler2D texture_diffuse14;
uniform sampler2D texture_diffuse15;
uniform sampler2D texture_diffuse16;

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_specular5;
uniform sampler2D texture_specular6;
uniform sampler2D texture_specular7;
uniform sampler2D texture_specular8;
uniform sampler2D texture_specular9;
uniform sampler2D texture_specular10;
uniform sampler2D texture_specular11;
uniform sampler2D texture_specular12;
uniform sampler2D texture_specular13;
uniform sampler2D texture_specular14;
uniform sampler2D texture_specular15;
uniform sampler2D texture_specular16;

void main() {
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);

	// phase 1: directional lighting
	vec3 result = calcDirLight(dirLight, fs_in.normal, viewDir);

	//phase 2: point lighting
	for (int i = 0; i < num_point_lights; i++) {
		if (pointLights[i].is_on) {
			result += calcPointLight(pointLights[i], fs_in.normal, fs_in.fragPos, viewDir);
		}
	}

	// phase 3: spotlight
	for (int i = 0; i < num_spot_lights; i++) {
		if (spotLights[i].is_on) {
			result += calcSpotLight(spotLights[i], fs_in.normal, fs_in.fragPos, viewDir);
		}
	}

	FragColor = vec4(result, 1.0f);

	// check whether fragment is higher than threshold, if so output as brightness color
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	if (brightness > 1.0) {
		BrightColor = vec4(FragColor.rgb, 1.0);
	} else {
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}

vec3 calcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
	vec3 lightDir = normalize(-dirLight.direction);
	float shininess = 32.0f;

	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);

	// specular shading (need to figure out what to replace shininess with)
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), shininess);

	// combine results
	vec3 ambient = dirLight.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;
	vec3 diffuse = dirLight.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;
	vec3 specular = dirLight.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

	return ambient + diffuse + specular;
}


vec3 calcPointLight(PointLight pointLight, vec3 norm, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(pointLight.pos - fragPos);
	float shininess = 32.0f;

	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);

	// specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), shininess);

	// attenuation
	float distance = length(pointLight.pos - fragPos);
	//float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	float attenuation = 1.0f / (pointLight.quadratic * distance * distance);

	// combine the results
	vec3 ambient = pointLight.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;
	vec3 diffuse = pointLight.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;
	vec3 specular = pointLight.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight(SpotLight spotLight, vec3 norm, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(spotLight.pos - fragPos);
	float shininess = 32.0f;

	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);

	// specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), shininess);

	// attenuation
	float distance = length(spotLight.pos - fragPos);
	float attenuation = 1.0f / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * distance * distance);

	// intensity
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);

	// combine the results
	vec3 ambient = spotLight.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;
	vec3 diffuse = spotLight.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;
	vec3 specular = spotLight.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

	return (ambient + diffuse + specular) * attenuation * intensity;
}