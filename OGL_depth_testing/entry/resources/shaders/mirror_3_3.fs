#version 330 core
	
out vec4 FragColor;		
				
struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    float shininess;
}; 

struct SpotLight {
	vec3 posistion;
	vec3 direction;

	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct PointLight {
	vec3 posistion;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// all light sources currently use static shinines
#define SHININESS 32.0

// camera posistion
uniform vec3 viewPos;

// pipeline
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
	
// lighting helper functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult);
						
void main()											
{		
	vec3 viewDir = normalize(FragPos - viewPos);
	vec3 reflectDir = reflect(-viewDir, normal );

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//combine results
	vec3 ambient = light.ambient * diffuseResult;
	vec3 diffuse = light.diffuse * diff * diffuseResult;
	vec3 specular = light.specular * spec * specularResult;
	return(ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult)
{
	vec3 lightDir = normalize(light.posistion - FragPos);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//attenuation
	float distance = length(light.posistion - FragPos);
	float attenuation = 1.0 / ( light.constant + ( light.linear * distance ) + ( light.quadratic * distance * distance ) );
	//combine results
	vec3 ambient = light.ambient * diffuseResult;
	vec3 diffuse = light.diffuse * diff * diffuseResult;
	vec3 specular = light.specular * spec * specularResult;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return(ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult)
{
	vec3 lightDir = normalize( light.posistion - FragPos );
	float theta = dot(lightDir,  normalize(-light.direction));
	if(theta > light.outerCutOff)
	{
		//diffuse
		float diff = max(dot(normal, lightDir), 0.0);
		//specular
		vec3 reflectDir = reflect(-lightDir, normal );
		float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
		//attenuation
		float distance = length(light.posistion - FragPos);
		float attenuation = 1.0 / ( light.constant + ( light.linear * distance ) + ( light.quadratic * distance * distance ) );
		//intensity
		float epsilon = light.innerCutOff - light.outerCutOff; 
		float intensity = clamp( ( theta - light.outerCutOff ) / ( epsilon ), 0.0, 1.0 );
		//combine results
		vec3 ambient = light.ambient * diffuseResult;
		vec3 diffuse = light.diffuse * diff * diffuseResult;
		vec3 specular = light.specular * spec * specularResult;
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		ambient *= intensity;
		diffuse *= intensity;
		specular *= intensity;
		return(ambient + diffuse + specular);
	}
}