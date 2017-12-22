#ifndef MESH_H
#define MESH_H

/*****************************************************
*	filename: mesh.h
*	description: mesh class
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <assimp/Importer.hpp>
#include <string>
#include <vector>
#include <shaders\shader_class.h>
#include <debug\custom_debugging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

/*****************************************************
*	Types
******************************************************/

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture 
{
    unsigned int id;
    string type;
	string path;
};  

struct Shader_Texture 
{
	int id;
	string name;
	string number;
};

class Mesh
{
	public:
		/* Mesh Data	*/
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		/* Functions	*/
		Mesh( vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		void Draw( Shader shader );
	private:
		/* Render Data	*/
		unsigned int VAO, VBO, EBO;
		/* Functions	*/
		void setup_mesh( void );
};
#endif
