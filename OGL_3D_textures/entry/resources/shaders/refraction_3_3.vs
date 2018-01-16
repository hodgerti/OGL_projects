#version 330 core		

// navigating meshes								
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// space transforming
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
		
// pipeline	
out vec3 Normal;
out vec3 Position;
							
void main()											
{	
	Normal = mat3(transpose(inverse(model))) * aNormal;										
	Position = vec3(model * vec4(aPos, 1.0));
	gl_Position = proj * view * model * vec4(aPos, 1.0);
}