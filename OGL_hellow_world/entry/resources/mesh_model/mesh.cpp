/*****************************************************
*	filename: mesh.cpp
*	description: mesh classes
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <mesh_model\mesh.h>

/*****************************************************
*	Procedures
******************************************************/

Mesh::Mesh
( 
	vector<Vertex> vertices, 
	vector<unsigned int> indices, 
	vector<Texture> textures
)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setup_mesh();
}

void Mesh::Draw
( 
	Shader shader 
)
{
	unsigned int diffuse_nr = 1;
	unsigned int specular_nr = 1;

	for( unsigned int i = 0; i < textures.size(); i++ )
	{
		// retrieve texture number ( diffuse_texture(N) )
		string number;
		string name = textures[i].type;
		if( name == "texture_diffuse" )
		{
			number = std::to_string( diffuse_nr++ );
			shader.set_int( ( "diffuseTextures[" + number + "]" ).c_str(), i );
		}
		else if( name == "texture_specular" )
		{
			number = std::to_string( specular_nr++ );
			shader.set_int( ( "specularTextures[" + number + "]" ).c_str(), i );
		}
		else
		{
			cout << "ERROR: Unknown texture type: " << name << endl;
			assert_always();
		}
		// activate proper texture unit
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, textures[i].id );
	}
	shader.set_int( "diffuseNr", diffuse_nr );
	shader.set_int( "specularNr", specular_nr );
	glActiveTexture( GL_TEXTURE0 );

	// draw mesh
	glBindVertexArray( VAO );
	glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );

}

void Mesh::setup_mesh
(
	void
)
{
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &EBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW );

	// vertex positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0 );
	// vertex normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof( Vertex, Normal ) );
	// texture coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof( Vertex, TexCoords ) );

	glBindVertexArray( 0 );
}