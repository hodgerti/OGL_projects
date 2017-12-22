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
#define TEXTURE_FILE_PATH "D:/OGL_hellow_world/entry/resources/textures/"
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
unsigned int		    VBO;
unsigned int			VBO_light;
unsigned int		    VAO;
unsigned int			VAO_light;
int						window_width;
int						window_height;
glm::mat4				trans;

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

Shader *standard_shader = new Shader( DFLT_VS_PATH, DFLT_FS_PATH );
Shader *light_shader = new Shader( LIGHT_VS_PATH, LIGHT_FS_PATH );

/*------------------------------------------
-	Load models
-------------------------------------------*/
Model nanosuit( "D:\OGL_hellow_world\entry\resources\models\nanosuit\nanosuit.obj" );


/*------------------------------------------
-	Setting up VAO for light
-------------------------------------------*/
glBindVertexArray( VAO_light );

glBindBuffer( GL_ARRAY_BUFFER, VBO_light );
glBufferData( GL_ARRAY_BUFFER, sizeof( cube_vertices_ogl ), cube_vertices_ogl, GL_STATIC_DRAW );

glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* )0 );
glEnableVertexAttribArray( 0 );

Buttonpress_eh *buttonpress_eh = new Buttonpress_eh( window );

glEnable( GL_DEPTH_TEST ); 

float light_x_center = 3.0f;
float light_z_center = 3.0f;
float light_radius = 2.5f;
float light_x_pos;
float light_z_pos;
float time;
float prev_time = 0.0f;
float delta_time = 0.0f;
float cur_rot = 0.0f;
bool flashlight_onoff = false;
glm::vec3 light_pos( 0.0f, 1.0f, 0.0f );
glm::vec3 light_color( 1.0f, 1.0f, 1.0f );
glm::vec3 flash_light_color( 1.0f, 1.0f, 1.0f );
glm::vec3 sun_direction( -0.2f, -1.0f, -0.3f );
glm::vec3 sun_pos = scale_vec( -15, sun_direction );

while ( !glfwWindowShouldClose( window ) )
{

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	buttonpress_eh->evnt_process_key_inputs();

	time = glfwGetTime();
	if ( buttonpress_eh->evnt_query_event_list( EH_R_PRESSED ) )
	{
		delta_time = time - prev_time;
		cur_rot += delta_time;
	}
	else if ( buttonpress_eh->evnt_query_event_list( EH_E_PRESSED ) )
	{
		delta_time = time - prev_time;
		cur_rot -= delta_time;
	}
	prev_time = time;
	
	light_x_pos = light_x_center + ( light_radius*cos( cur_rot ) );
	light_z_pos = light_z_center + ( light_radius*sin( cur_rot ) );
	light_pos[0] = light_x_pos;
	light_pos[2] = light_z_pos;

	flash_light_color = calc_color(244, 66, 66);
	glm::vec3 diffuse_color = light_color   * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambient_color = diffuse_color * glm::vec3(0.05f); // low influence

	standard_shader->use();
	standard_shader->set_vec3( "viewPos", camera.get_camera_pos() );
	standard_shader->set_float( "material.shininess", 16.0f );
	
	/*----------------------------------------------
	- point lights
	-----------------------------------------------*/
	for( int i = 0; i < 4; i++ )
	{
		std::string k = std::to_string(i);
		standard_shader->set_vec3( "pointLights[" + k + "].posistion", pointLightPositions[0] );
		standard_shader->set_vec3( "pointLights[" + k + "].ambient", ambient_color );
		standard_shader->set_vec3( "pointLights[" + k + "].diffuse", diffuse_color );
		standard_shader->set_vec3( "pointLights[" + k + "].specular", glm::vec3( 1.0f, 1.0f, 1.0f ) );
		standard_shader->set_float( "pointLights[" + k + "].constant", 1.0f );
		standard_shader->set_float( "pointLights[" + k + "].linear", 0.09f );
		standard_shader->set_float( "pointLights[" + k + "].quadratic", 0.032f );
	}

	/*----------------------------------------------
	- flashlight
	-----------------------------------------------*/
	if ( buttonpress_eh->evnt_query_event_list( EH_F_CLICKED ) )
	{
		flashlight_onoff = !flashlight_onoff;
	}
	if ( flashlight_onoff )
	{
		standard_shader->set_bool( "flashLightOnOff", true );

		diffuse_color = flash_light_color * glm::vec3(0.8f);
		ambient_color = diffuse_color * glm::vec3(0.5f);

		standard_shader->set_vec3( "flashLight.posistion", camera.get_camera_pos() );
		standard_shader->set_vec3( "flashLight.direction", camera.get_camera_front() );
		standard_shader->set_float( "flashLight.innerCutOff", glm::cos(glm::radians(5.0f)));
		standard_shader->set_float( "flashLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		standard_shader->set_vec3( "flashLight.ambient", ambient_color );
		standard_shader->set_vec3( "flashLight.diffuse", diffuse_color );
		standard_shader->set_vec3( "flashLight.specular", glm::vec3( 1.0f, 1.0f, 1.0f ) );
		standard_shader->set_float( "flashLight.constant", 1.0f );
		standard_shader->set_float( "flashLight.linear", 0.09f );
		standard_shader->set_float( "flashLight.quadratic", 0.032f );
	}
	else
	{
		standard_shader->set_bool( "flashLightOnOff", false );
	}

	/*----------------------------------------------
	- sun
	-----------------------------------------------*/
	glm::vec3 sun_direction( -0.2f, -1.0f, -0.3f );
	standard_shader->set_vec3( "sun.direction", sun_direction );
	standard_shader->set_vec3( "sun.ambient", glm::vec3( 0.05f, 0.05f, 0.05f ) );
	standard_shader->set_vec3( "sun.diffuse", glm::vec3( 0.4f, 0.4f, 0.4f ) );
	standard_shader->set_vec3( "sun.specular", glm::vec3( 0.5f, 0.5f, 0.5f ) );


	camera.process_button_presses( buttonpress_eh );
	camera.calc_speed();
	camera.update_camera( window_width, window_height, standard_shader->ID );
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, container_diffuse_map );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, container_specular_map );

	glm::mat4 model;

	glBindVertexArray( VAO );
	for(unsigned int i = 0; i < 10; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, cube_positions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		standard_shader->set_mat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	light_shader->use();
	camera.update_camera( window_width, window_height, light_shader->ID );
	light_shader->set_vec3( "LightColor", light_color );

    glBindVertexArray( VAO_light );
	for(unsigned int i = 0; i < 4; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale( model, glm::vec3(0.2f) );
		light_shader->set_mat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glm::vec3 sun_color(0.95f, 0.808f, 0.259f);
	model = glm::mat4();
	model = glm::translate(model, 15.0f * -sun_direction);
	light_shader->set_mat4("model", model);
	light_shader->set_vec3("LightColor", sun_color);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glfwSwapBuffers( window );

	/* Clean-up		*/
	glBindVertexArray( 0 );

	glfwPollEvents();

}

/*------------------------------------------
-	Delete Array Objects
-------------------------------------------*/
glDeleteVertexArrays( 1, &VAO );
glDeleteVertexArrays( 1, &VAO_light );
glDeleteBuffers( 1, &VBO );
glDeleteBuffers( 1, &VBO_light );

delete buttonpress_eh;
delete standard_shader;

glfwTerminate();

return 0;
}

glm::vec3 calc_color( float x, float y, float z )
{
	return( glm::vec3(x/255, y/255, z/255));
}