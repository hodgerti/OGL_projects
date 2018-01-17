#ifndef MODEL_H
#define MODEL_H

/*****************************************************
*	filename: model.h
*	description: model class
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <shaders\shader_class.h>
#include <mesh_model\mesh.h>
#include <textures\STB\stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

/*****************************************************
*	Types
******************************************************/

class Model
{
	public:
		/*	Functions	*/
		Model( char *path );
		void Draw( Shader shader, unsigned int pass_thru_tex = 0 );
	private:
		/*	Model Data	*/
		vector<Mesh> meshes;
		string directory;
		vector<Texture> textures_loaded;
		/*	Functions	*/
		void load_model( string path );
		void process_node( aiNode *node, const aiScene *scene );
		Mesh process_mesh( aiMesh *mesh_in, const aiScene *scene );
		vector<Texture> load_material_textures( aiMaterial *mat, aiTextureType type, string type_name );
		unsigned int Model::texture_from_file( const char *path, const string directory, bool gamma = false );
};

#endif
