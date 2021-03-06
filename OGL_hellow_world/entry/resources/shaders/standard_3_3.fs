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

uniform DirLight sun;

uniform bool flashLightOnOff;
uniform SpotLight flashLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

#define MAXIMUM_TEXTURE_UNITS 16 // guarenteed 16 texture unit support
uniform sampler2D diffuseTextures[MAXIMUM_TEXTURE_UNITS];
uniform sampler2D specularTextures[MAXIMUM_TEXTURE_UNITS];
uniform int diffuseNr; // watch out for off by 1 error
uniform int specularNr;

#define SHININESS 32.0

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
	
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult);	
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult);
							
void main()											
{	
	// combine texture maps
	vec3 diffuseResult = vec3(0.0, 0.0, 0.0);
	vec3 specularResult = vec3(0.0, 0.0, 0.0);
	
	for ( int i = 1; i < diffuseNr; i++ )
	{
		diffuseResult += vec3(texture(diffuseTextures[i], TexCoords));
	}
	for ( int i = 1; i < specularNr; i++ )
	{
		specularResult += vec3(texture(specularTextures[i], TexCoords));
	}

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(FragPos - viewPos);
	vec3 result = vec3(0.0, 0.0, 0.0);

	result = CalcDirLight(sun, norm, viewDir, diffuseResult, specularResult);
	if(flashLightOnOff)
	{
		result += CalcSpotLight(flashLight, norm, viewDir, diffuseResult, specularResult);
	}
	for(int i = 0; i < NR_POINT_LIGHTS; i++ )
	{
		result += CalcPointLight(pointLights[i], norm, viewDir, diffuseResult, specularResult);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseResult, vec3 specularResult)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
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
		float spec = pow(max(dot(reflectDir, viewDir), 0.0), SHININESS);
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
		diffuse *= intensity;
		specular *= intensity;
		return(ambient + diffuse + specular);
	}
}