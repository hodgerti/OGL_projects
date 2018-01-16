#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
	float ratio = 1.00 / 1.52;
	vec3 P = normalize(Position - cameraPos);
	vec3 Q = refract(P, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, Q).rgb, 1.0);
}