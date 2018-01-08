#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

/*****************************************************
*	filename: fragment_class.h
*	description: shader class
*
******************************************************/  
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef LAPTOP
#define SHADER_START_LOC "D:/OGL_projects/OGL_3D_textures/entry/resources/shaders/"
#else
#define SHADER_START_LOC "C:...."
#endif
#define DFLT_VS_PATH SHADER_START_LOC"standard_3_3.vs"
#define DFLT_FS_PATH SHADER_START_LOC"standard_3_3.fs"
#define SINGLE_COLOR_VS_PATH SHADER_START_LOC"standard_3_3.vs"
#define SINGLE_COLOR_FS_PATH SHADER_START_LOC"single_color_3_3.fs"
#define SCREEN_VS_PATH SHADER_START_LOC"screen_3_3.vs"
#define SCREEN_FS_PATH SHADER_START_LOC"screen_3_3.fs"

class Shader
{
public:
	// program ID
	unsigned int ID;

	// constuctor reads and builds the shader
	Shader( const GLchar *vertex_path, const GLchar *fragment_path );
	// use/activate the shader
	void use( void );
	// utility uniform functions
	void set_bool( const std::string &name, bool value ) const;
	void set_int( const std::string &name, int value ) const;
	void set_float( const std::string &name, float value ) const;
	void set_vec3( const std::string &name, glm::vec3 value ) const;
	void set_vec4(const std::string &name, glm::vec4 value ) const;
	void set_mat4( const std::string &name, glm::mat4 value ) const;
};

#endif