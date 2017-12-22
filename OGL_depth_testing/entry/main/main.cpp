/*****************************************************
*	filename: main.cpp
*	description: main loop and window creation
*
******************************************************/


/*****************************************************
*	Includes
******************************************************/
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <math.h>
#include <event_handlers\eh_mainwindow.h>
#include <event_handlers\eh_buttonpresses.h>
#include <vertices\raw_vertices.h>
#include <debug\custom_debugging.h>
#include <textures\STB\stb_image.h>
#include <camera\camera_class.h>
#include <shaders\shader_class.h>
#include <mesh_model\mesh.h>
#include <mesh_model\model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mesh_model/mesh.h>
#include "./illuminati_triangle.h"

/*****************************************************
*	Literal Constants
******************************************************/
#define TEXTURE_FILE_PATH "D:/OGL_projects/OGL_depth_testing/entry/resources/textures/"
#define TEXTURE_METALBOX_PNG TEXTURE_FILE_PATH"metalbox_holes.png"
#define TEXTURE_METALBOX_SPECULAR_PNG TEXTURE_FILE_PATH"metalbox_holes_specular.png"
#define TEXTURE_SURFACE_PNG TEXTURE_FILE_PATH"mossy.png"
#define TEXTURE_SURFACE_SPECULAR_PNG TEXTURE_FILE_PATH"mossy_specular.png"

#define PI 3.14159265358979323846

/*****************************************************
*	Macros
******************************************************/
#define deg_to_rad( val )			\
		 ( val * ( 2 * PI ) / 360.0f )
#define rad_to_deg( val )			\
		 ( val * 360.0f / ( 2 * PI ) )


glm::vec3 scale_vec( float k, glm::vec3 vec )
{
	glm::vec3 res;
	res.x = vec.x * k;
	res.y = vec.y * k;
	res.z = vec.z * k;
	return res;
}

glm::vec3 find_normal( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3 )
{
	glm::vec3 line1 = v1 - v2;
	glm::vec3 line2 = v1 - v3;
	glm::vec3 normal = glm::cross( line1, line2 );
	printf( "\nX:%f\nY:%f\nZ:%f\n", normal[0], normal[1], normal[2] );
	return normal;
}

unsigned int loadTexture(char const *path);

glm::vec3 calc_color( float x, float y, float z );

/*****************************************************
*	Global event handlers
******************************************************/
Mainwindow_eh mainwindow_eh;
Camera camera;
void mouse_callback_wrapper( GLFWwindow* window, double xpos, double ypos )
{
	camera.mouse_callback( window, xpos, ypos );
}
void evnt_framebuffer_size_callback_wrapper( GLFWwindow* window, int width, int height )
{
	mainwindow_eh.evnt_framebuffer_size_callback( window, width, height );
}
void scroll_callback_wrapper( GLFWwindow* window, double xoffset, double yoffset )
{
	camera.scroll_callback( window, xoffset, yoffset );
}

/*****************************************************
*	Main function
******************************************************/
int main()
{
/*------------------------------------------
-	Local variables
-------------------------------------------*/
int						window_width;
int						window_height;
glm::mat4				trans;
glm::mat4				model;

/*------------------------------------------
-	Init local variables
-------------------------------------------*/	
window_width = 800;
window_height = 600;

/*------------------------------------------
-	Create GLFW window and contexts
-------------------------------------------*/
glfwInit();
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#ifdef __APPLE__
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

GLFWwindow* window = glfwCreateWindow( window_width, window_height, "hello_world_OGL", NULL, NULL );
// disable mouse cursor
glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
if ( window == NULL )
{
	printf( "Failed to create GLFW window" );
	glfwTerminate();
	assert_always();
	return -1;
}
glfwMakeContextCurrent( window );
glfwSetFramebufferSizeCallback( window, evnt_framebuffer_size_callback_wrapper );
glfwSetCursorPosCallback( window, mouse_callback_wrapper );
glfwSetScrollCallback( window, scroll_callback_wrapper );

/*------------------------------------------
-	Load GLAD
-------------------------------------------*/
if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
{
	printf( "Failed to initialize GLAD" );
	assert_always();
	return -1;
}

/*------------------------------------------
-	Set up OGL window
-------------------------------------------*/
glViewport(0, 0, 800, 600);

Shader standard_shader( DFLT_VS_PATH, DFLT_FS_PATH );
Shader light_shader( LIGHT_VS_PATH, LIGHT_FS_PATH );

/*------------------------------------------
-	Create VAO's
-------------------------------------------*/
// cube VAO
unsigned int VAO_cube, VBO_cube;
glGenVertexArrays( 1, &VAO_cube );
glGenBuffers( 1, &VBO_cube );
glBindVertexArray( VAO_cube );
glBindBuffer( GL_ARRAY_BUFFER, VBO_cube );
glBufferData( GL_ARRAY_BUFFER, sizeof(cube_vertices_ogl), &cube_vertices_ogl, GL_STATIC_DRAW);
glEnableVertexAttribArray( 0 );
glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );
glEnableVertexAttribArray( 1 );
glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 3 * sizeof(float) ) );
glEnableVertexAttribArray( 2 );
glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 6 * sizeof(float) ) );
glBindVertexArray( 0 );

// plane VAO
// normal generation
const unsigned int plane_normals_size =  3 * sizeof(plane_vertices) / ( 5 * sizeof(float) );
float plane_normals[ plane_normals_size ];
int i = 0;
int j = 0;
while ( i < plane_normals_size )
{
	int k = i / 3;
	glm::vec3 v1( plane_vertices[ k ], plane_vertices[ k + 1 ], plane_vertices[ k + 2 ] );
	glm::vec3 v2( plane_vertices[ k +  5 ], plane_vertices[ k + 6 ], plane_vertices[ k + 7 ] );
	glm::vec3 v3( plane_vertices[ k +  10 ], plane_vertices[ k + 11 ], plane_vertices[ k + 12 ] );
	glm::vec3 normal = find_normal( v1, v2, v3 );
	plane_normals[ i + j ] = normal.x; 
	plane_normals[ i + j + 1 ] = normal.y; 
	plane_normals[ i + j + 2 ] = normal.z; 
	j += 3;
	if ( j == 9 )
	{
		i += 9;
		j = 0;
	}
}
// add normals to plane vertices
const unsigned int combined_total = plane_normals_size + ( sizeof(plane_vertices) / sizeof(float) );
float plane_vertices_w_normals[ combined_total ];
i = 0;
j = 0;
int five_count = 4;
for ( int f = 0; f < combined_total; f++ )
{
	if ( five_count == 1 )
	{
		for ( int h = 0; h < 3; h++ )
		{
			plane_vertices_w_normals[f] = plane_normals[j];
			j++; f++;
		}
	}
	plane_vertices_w_normals[f] = plane_vertices[i];
	i++;
	five_count--;
	if ( five_count < 0 )
	{
		five_count = 4;
	}
}
unsigned int VAO_plane, VBO_plane;
glGenVertexArrays( 1, &VAO_plane );
glGenBuffers( 1, &VBO_plane );
glBindVertexArray( VAO_plane );
glBindBuffer( GL_ARRAY_BUFFER, VBO_plane );
glBufferData( GL_ARRAY_BUFFER, sizeof(plane_vertices_w_normals), &plane_vertices_w_normals, GL_STATIC_DRAW );
glEnableVertexAttribArray( 0 );
glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );
glEnableVertexAttribArray( 1 );
glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 3 * sizeof(float) ) );
glEnableVertexAttribArray( 2 );
glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 6 * sizeof(float) ) );
glBindVertexArray( 0 );

/*------------------------------------------
-	Load Textures
-------------------------------------------*/
unsigned int cube_texture = loadTexture( TEXTURE_METALBOX_PNG );
unsigned int cube_specular_texture = loadTexture( TEXTURE_METALBOX_SPECULAR_PNG );
unsigned int surface_texture = loadTexture( TEXTURE_SURFACE_PNG );
unsigned int surface_specular_texture = loadTexture( TEXTURE_SURFACE_SPECULAR_PNG );

standard_shader.use();
standard_shader.set_int( "material.texture_diffuse", 0 );
standard_shader.set_int( "material.texture_specular", 1 );
standard_shader.set_float( "material.shininess", 16.0f );

Buttonpress_eh *buttonpress_eh = new Buttonpress_eh( window );

/*----------------------------------------------
- depth testing setup
-----------------------------------------------*/
glEnable( GL_DEPTH_TEST ); 
glDepthFunc( GL_LESS );


bool flashlight_on_off = false;

while ( !glfwWindowShouldClose( window ) )
{
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	/* check key inputs for this frame*/
	buttonpress_eh->evnt_process_key_inputs();

	standard_shader.use();

	/*----------------------------------------------
	- flashlight
	-----------------------------------------------*/
	if ( buttonpress_eh->evnt_query_event_list( EH_F_CLICKED ) )
	{
		flashlight_on_off = !flashlight_on_off;
	}
	standard_shader.set_bool( "flashLightOnOff", flashlight_on_off );
	if ( flashlight_on_off == true )
	{
		glm::vec3 flash_light_color = calc_color(255, 255, 255);
		glm::vec3 diffuse_color = flash_light_color * glm::vec3(0.8f);
		glm::vec3 ambient_color = diffuse_color * glm::vec3(0.5f);

		standard_shader.set_vec3( "flashLight.posistion", camera.get_camera_pos() );
		standard_shader.set_vec3( "flashLight.direction", camera.get_camera_front() );
		standard_shader.set_float( "flashLight.innerCutoff", glm::cos(glm::radians(12.5f) ) );
		standard_shader.set_float( "flashLight.outerCutOff", glm::cos(glm::radians(17.5f) ) );
		standard_shader.set_vec3( "flashLight.ambient", ambient_color );
		standard_shader.set_vec3( "flashLight.diffuse", diffuse_color );
		standard_shader.set_vec3( "flashLight.specular", glm::vec3( 1.0f, 1.0f, 1.0f ) );
		standard_shader.set_float( "flashLight.constant", 1.0f );
		standard_shader.set_float( "flashLight.linear", 0.09f );
		standard_shader.set_float( "flashLight.quadratic", 0.032f );
	}

	/*----------------------------------------------
	- point lights
	-----------------------------------------------*/
	for( int i = 0; i < 4; i++ )
	{
		glm::vec3 light_color = calc_color(255, 255, 255);
		glm::vec3 diffuse_color = light_color * glm::vec3(0.3f);
		glm::vec3 ambient_color = diffuse_color * glm::vec3(0.09f);

		std::string k = std::to_string(i);
		standard_shader.set_vec3( "pointLights[" + k + "].posistion", pointLightPositions[0] );		 
		standard_shader.set_vec3( "pointLights[" + k + "].ambient", ambient_color );
		standard_shader.set_vec3( "pointLights[" + k + "].diffuse", diffuse_color );
		standard_shader.set_vec3( "pointLights[" + k + "].specular", glm::vec3( 1.0f, 1.0f, 1.0f ) );	 
		standard_shader.set_float( "pointLights[" + k + "].constant", 1.0f );
		standard_shader.set_float( "pointLights[" + k + "].linear", 0.09f );
		standard_shader.set_float( "pointLights[" + k + "].quadratic", 0.032f );
	}

	/*----------------------------------------------
	- sun
	-----------------------------------------------*/
	standard_shader.set_vec3( "sun.direction", glm::vec3( -0.2f, -1.0f, -0.3f ) );
	standard_shader.set_vec3( "sun.ambient", glm::vec3( 0.05f, 0.05f, 0.05f ) );
	standard_shader.set_vec3( "sun.diffuse", glm::vec3( 0.4f, 0.4f, 0.4f ) );
	standard_shader.set_vec3( "sun.specular", glm::vec3( 0.5f, 0.5f, 0.5f ) );

	/*----------------------------------------------
	- update camera
	-----------------------------------------------*/
	camera.calc_speed();
	camera.process_button_presses( buttonpress_eh );
	camera.update_camera( window_width, window_height, standard_shader.ID );

	/*----------------------------------------------
	- render
	-----------------------------------------------*/

	// cubes
	glBindVertexArray( VAO_cube );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, cube_texture );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, cube_specular_texture );
    model = glm::translate(model, glm::vec3( -1.0f, 0.0f, -1.0f ) );
    standard_shader.set_mat4( "model", model );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    standard_shader.set_mat4( "model", model );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
	
	// plane
	glBindVertexArray( VAO_plane );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, surface_texture );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, surface_specular_texture );
	standard_shader.set_mat4( "model", glm::mat4() );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );

	// swap buffers and prepare for next render
	glfwSwapBuffers( window );
	glfwPollEvents();

}

glfwTerminate();

return 0;
}

glm::vec3 calc_color( float x, float y, float z )
{
	return( glm::vec3(x/255, y/255, z/255));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}