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
#define TEXTURE_FILE_PATH "C:/Users/id181/Documents/OGL_projects/OGL_hellow_world/OGL_depth_testing/resources/"
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

/*------------------------------------------
-	Init local variables
-------------------------------------------*/	
window_width = 1920;
window_height = 1080;

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
glViewport(0, 0, window_width, window_height);

Shader standard_shader( DFLT_VS_PATH, DFLT_FS_PATH );
Shader light_shader( LIGHT_VS_PATH, LIGHT_FS_PATH );

/*------------------------------------------
-	Load models
-------------------------------------------*/
Model nanosuit_model( "C:/Users/id181/Documents/OGL_projects/OGL_hellow_world/OGL_depth_testing/resources/models/nanosuit/nanosuit.obj" );

Buttonpress_eh *buttonpress_eh = new Buttonpress_eh( window );

glEnable( GL_DEPTH_TEST ); 

/*
glDepthMask( GL_FALSE );
glDepthFunc( GL_LESS );
*/


bool flashlight_on_off = false;

while ( !glfwWindowShouldClose( window ) )
{

	glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* check key inputs for this frame*/
	buttonpress_eh->evnt_process_key_inputs();

	/* set shader*/
	standard_shader.use();
	standard_shader.set_vec3( "viewPos", camera.get_camera_pos() );
	standard_shader.set_float( "material.shininess", 16.0f );

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
		glm::vec3 diffuse_color = light_color * glm::vec3(0.8f);
		glm::vec3 ambient_color = diffuse_color * glm::vec3(0.5f);

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

	/* update camera*/
	camera.process_button_presses( buttonpress_eh );
	camera.calc_speed();
	camera.update_camera( window_width, window_height, standard_shader.ID );

	/* render nanosuit model*/
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    standard_shader.set_mat4( "model", model );
	nanosuit_model.Draw( standard_shader );
	

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