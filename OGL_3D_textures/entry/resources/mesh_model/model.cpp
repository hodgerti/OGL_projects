/*****************************************************
*	filename: model.cpp
*	description: model classes
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <mesh_model\model.h>

/*****************************************************
*	Procedures
******************************************************/

Model::Model
(
	char *path
)
{
	load_model( path );
}

void Model::Draw
(
	Shader shader,
	unsigned int pass_thru_tex
)
{
	for( unsigned int i = 0; i < meshes.size(); i++ )
	{
		meshes[i].Draw( shader, pass_thru_tex );
	}
}

void Model::load_model
( 
	string path 
)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );

	if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		printf( "ERROR::ASSIMP::%s\n", import.GetErrorString() );
		assert_always();
		return;
	}
	directory = path.substr( 0, path.find_last_of( '/' ) );

	process_node( scene->mRootNode, scene );

}

void Model::process_node
( 
	aiNode *node, 
	const aiScene *scene
)
{
	// process all the node's meshes (if any)
	for( unsigned int i = 0; i < node->mNumMeshes; i++ )
	{
		aiMesh *mesh = scene->mMeshes[ node->mMeshes[i] ];
		meshes.push_back( process_mesh( mesh, scene ) );
	}
	// recursively process all children nodes and thier children
	for( unsigned int i = 0; i < node->mNumChildren; i++ )
	{
		process_node( node->mChildren[i], scene );
	}
}

Mesh Model::process_mesh
( 
	aiMesh *mesh_in, 
	const aiScene *scene
)
{
	/* Local variable */
	glm::vec3 vertice_vector;

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for( unsigned int i = 0; i < mesh_in->mNumVertices; i++ )
	{
		Vertex vertex;
		/*-------------------------------------------
		- process vertex pos's, norm's, and tex's coords 
		---------------------------------------------*/
		// vertices
		vertice_vector.x = mesh_in->mVertices[i].x;
		vertice_vector.y = mesh_in->mVertices[i].y;
		vertice_vector.z = mesh_in->mVertices[i].z;
		vertex.Position = vertice_vector;
		// normals
		vertice_vector.x = mesh_in->mNormals[i].x;
		vertice_vector.y = mesh_in->mNormals[i].y;
		vertice_vector.z = mesh_in->mNormals[i].z;
		vertex.Normal = vertice_vector;
		// texcoords
		if( mesh_in->mTextureCoords[0] )
		{
			vertice_vector.x = mesh_in->mTextureCoords[0][i].x;
			vertice_vector.y = mesh_in->mTextureCoords[0][i].y;
			vertex.TexCoords = glm::vec2( vertice_vector.x, vertice_vector.y );
		}
		else
		{
			vertex.TexCoords = glm::vec2( 0.0f, 0.0f );
		}
		// save vertice
		vertices.push_back( vertex );
	}

	/*-------------------------------------------
	- process indices
	---------------------------------------------*/
	for( unsigned int i = 0; i < mesh_in->mNumFaces; i++ )
	{
		aiFace face = mesh_in->mFaces[i];
		for( unsigned int j = 0; j < face.mNumIndices; j++ )
		{
			indices.push_back( face.mIndices[j] );
		}
	}

	/*-------------------------------------------
	- process material (textures)
	---------------------------------------------*/
	if( mesh_in->mMaterialIndex >= 0 )
	{
		/* grab material from scene		*/
		aiMaterial *material = scene->mMaterials[ mesh_in->mMaterialIndex ];
		/* load diffuse texture map		*/
		vector<Texture> diffuse_maps = load_material_textures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
		textures.insert( textures.end(), diffuse_maps.begin(), diffuse_maps.end() );
		/* load specular texture map		*/
		vector<Texture> specular_maps = load_material_textures( material, aiTextureType_SPECULAR, "texture_specular" );
		textures.insert( textures.end(), specular_maps.begin(), specular_maps.end() );
		/* load REFLECTIVE texture map		*/
		vector<Texture> reflective_maps = load_material_textures( material, aiTextureType_AMBIENT, "texture_reflective" );
		textures.insert( textures.end(), reflective_maps.begin(), reflective_maps.end() );
	}

	return Mesh( vertices, indices, textures );
}

vector<Texture> Model::load_material_textures
( 
	aiMaterial *mat,
	aiTextureType type,
	string type_name 
)
{
	vector<Texture> textures;
	bool skip;
	for( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ )
	{
		aiString str;
		mat->GetTexture( type, i, &str );
		skip = false;
		for( unsigned int j = 0; j < textures_loaded.size(); j++ )
		{
			if( std::strcmp( textures_loaded[j].path.data(), str.C_Str() ) == 0 )
			{
				textures.push_back( textures_loaded[j] );
				skip = true;
				break;
			}
		}
		if( !skip )
		{
			Texture texture;
			texture.id = texture_from_file( str.C_Str(), directory );
			texture.type = type_name;
			texture.path = str.C_Str();	

			textures.push_back( texture );
			textures_loaded.push_back( texture );
		}
	}
	return textures;
};

/*--------------------------------------------------------
- Utility function for loading a 2D texture from file
--------------------------------------------------------*/
unsigned int Model::texture_from_file
(
	const char *path,
	const string directory,
	bool gamma
)
{
	/* get full path */
	string filename = string( path );
	filename = directory + '/' + filename;

	/* gen a texture id	*/
	unsigned int texture_ID;
	glGenTextures( 1, &texture_ID );

	/* load raw texture	*/
	int width, height, nr_components;
	unsigned char *data = stbi_load( filename.c_str(), &width, &height, &nr_components, 0 );
	/* process raw texture	*/
	if( data )
	{
		// check render format
		GLenum format;
		if( nr_components == 1 )
			format = GL_RED;
		else if( nr_components == 3 )
			format = GL_RGB;
		else if( nr_components == 4 )
			format = GL_RGBA;

		/* openGL:	*/
		// load raw data into video memory
		glBindTexture( GL_TEXTURE_2D, texture_ID );
		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );

		// set presentation of texture settings
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	}
	else
	{
		printf( "Texture failed to load at path: %s/n", path );	
	}

	// free image data
	stbi_image_free( data );

	return texture_ID;
}


