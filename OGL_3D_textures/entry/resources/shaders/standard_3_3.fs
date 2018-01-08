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

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float constant;
	float linear;
	float quadratic;
};

struct PointLight {
	vec3 posistion;
	vec3 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight {
	vec3 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

// sun
uniform DirLight sun;

// flashlight
uniform bool flashLightOnOff;
uniform SpotLight flashLight;

// point lights
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

// camera posistion
uniform vec3 viewPos;

// model loading
#define MAXIMUM_TEXTURE_UNITS 16 // guarenteed 16 texture unit support
uniform sampler2D diffuseTextures[MAXIMUM_TEXTURE_UNITS];
uniform sampler2D specularTextures[MAXIMUM_TEXTURE_UNITS];
uniform int diffuseNr; // watch out for off by 1 error
uniform int specularNr;

// straight texture mapping
uniform Material material;

// all light sources currently use static shinines
#define SHININESS 32.0

// pipeline
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
	
// lighting helper functions
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 diffuseResult, vec4 specularResult);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec4 diffuseResult, vec4 specularResult);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec4 diffuseResult, vec4 specularResult);
						
void main()											
{		
	vec4 diffuseResult = texture(material.texture_diffuse, TexCoords);
	vec4 specularResult = texture(material.texture_specular, TexCoords);

	if ( diffuseResult.a < 0.01 )
	{
		discard;
	}

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(FragPos - viewPos);

	vec4 result = CalcDirLight(sun, norm, viewDir, diffuseResult, specularResult);
	if(false)
	{
		result += CalcSpotLight(flashLight, norm, viewDir, diffuseResult, specularResult);
	}
	for(int i = 0; i < NR_POINT_LIGHTS; i++ )
	{
		result += CalcPointLight(pointLights[i], norm, viewDir, diffuseResult, specularResult);
	}

	result.a = diffuseResult.a;
	FragColor = result;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 diffuseResult, vec4 specularResult)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//combine results
	vec4 ambient = light.ambient * diffuseResult;
	vec4 diffuse = light.diffuse * diff * diffuseResult;
	vec4 specular = light.specular * spec * specularResult;
	return( ambient + diffuse + specular );
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec4 diffuseResult, vec4 specularResult)
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
	vec4 ambient = light.ambient * diffuseResult;
	vec4 diffuse = light.diffuse * diff * diffuseResult;
	vec4 specular = light.specular * spec * specularResult;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return( ambient + diffuse + specular );
} 

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec4 diffuseResult, vec4 specularResult)
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
		vec4 ambient = light.ambient * diffuseResult;
		vec4 diffuse = light.diffuse * diff * diffuseResult ;
		vec4 specular = light.specular * spec * specularResult;
		ambient *= attenuation * intensity;
		diffuse *= attenuation * intensity;
		specular *= attenuation * intensity;
		return( ambient + diffuse + specular );
	}
}
