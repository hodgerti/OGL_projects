/*****************************************************
*	filename: fragment_class.cpp
*	description: shader class
*
******************************************************/

#include <shaders\shader_class.h>
#include <debug\custom_debugging.h>

/*--------------------------------------------------------------
Constructor. Loads shader programs into string object.
---------------------------------------------------------------*/
Shader::Shader 
( 
const GLchar *vertex_path,
const GLchar *fragment_path 
)
{
/*--------------------------------------------------------------
Local Variables
---------------------------------------------------------------*/
std::string				vertex_code;
std::string				fragment_code;
std::ifstream			v_shader_file;
std::ifstream			f_shader_file;
std::stringstream		v_shader_stream;
std::stringstream		f_shader_stream;
const char			   *v_shader_code;
const char			   *f_shader_code;
unsigned int			vertex_shader;
unsigned int			fragment_shader;
int						success;
char					info_log[512];

/*--------------------------------------------------------------
 Retrieve shader file code
---------------------------------------------------------------*/
/* Grab them source codes */
v_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
f_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
try
{
	// open files
	v_shader_file.open( vertex_path );
	f_shader_file.open( fragment_path );
	// read file's buffer contents into streams
	v_shader_stream << v_shader_file.rdbuf();
	f_shader_stream << f_shader_file.rdbuf();
	// close file handles
	v_shader_file.close();
	f_shader_file.close();
	// convert stream into string
	vertex_code = v_shader_stream.str();
	fragment_code = f_shader_stream.str();
}
catch( std::ifstream::failure e )
{
	printf( "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" );
	assert_always();
}
v_shader_code = vertex_code.c_str();
f_shader_code = fragment_code.c_str();

/*--------------------------------------------------------------
 Compile shader code
---------------------------------------------------------------*/

// Setup vertex shader
vertex_shader = glCreateShader( GL_VERTEX_SHADER );
glShaderSource( vertex_shader, 1, &v_shader_code, NULL );
glCompileShader( vertex_shader );
glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &success );

// Setup fragment shader
fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
glShaderSource( fragment_shader, 1, &f_shader_code, NULL );
glCompileShader( fragment_shader );
glGetShaderiv( fragment_shader, GL_COMPILE_STATUS, &success );

// Checking shader load success
if( !success )
{
	printf( "ERROR: SHADER COMPILE FAIL:\n" );
	glGetShaderInfoLog( vertex_shader, 512, NULL, info_log );
	printf( "VERTEX SHADER:\n%s\n", info_log );
	glGetShaderInfoLog( fragment_shader, 512, NULL, info_log );
	printf( "FRAGMENT SHADER:\n%s\n", info_log );
	assert_always();
}

//Creating shader program
ID = glCreateProgram();
glAttachShader( ID, vertex_shader );
glAttachShader( ID, fragment_shader );
glLinkProgram( ID );
glGetProgramiv( ID, GL_LINK_STATUS, &success);
if( !success ) 
{
    glGetProgramInfoLog( ID, 512, NULL, info_log );
    printf( "ERROR: SHADER PROGRAM CREATION FAIL:\n%s\n", info_log );
	assert_always();
}

// Delete shader handles
glDeleteShader( vertex_shader );
glDeleteShader( fragment_shader );  

} /* Shader */

/*--------------------------------------------------------------
Wrapper to use shader program.
---------------------------------------------------------------*/
void Shader::use
(
	void
)
{
	glUseProgram( ID );
} /* use */

void Shader::set_bool(const std::string &name, bool value) const
{         
    glUniform1i( glGetUniformLocation( ID, name.c_str()) , (int)value ); 
}
void Shader::set_int(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::set_float(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 
void Shader::set_mat4(const std::string &name, glm::mat4 value) const
{ 
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value) ); 
} 
void Shader::set_vec3(const std::string &name, glm::vec3 value ) const
{ 
    glUniform3f( glGetUniformLocation( ID, name.c_str() ), value[0], value[1], value[2] );
}
void Shader::set_vec4(const std::string &name, glm::vec4 value ) const
{ 
    glUniform4f( glGetUniformLocation( ID, name.c_str() ), value[0], value[1], value[2], value[3] );
}