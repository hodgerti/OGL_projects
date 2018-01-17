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
#include <debug_custom\custom_debugging.h>
#include <textures\STB\stb_image.h>
#include <camera\camera_class.h>
#include <shaders\shader_class.h>
#include <mesh_model\mesh.h>
#include <mesh_model\model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mesh_model/mesh.h>
#include <map>
#include "./illuminati_triangle.h"

/*****************************************************
*	Literal Constants
******************************************************/
#ifndef LAPTOP
#define TEXTURE_FILE_PATH "D:/OGL_projects/OGL_3D_textures/entry/resources/textures/"
#else
#define TEXTURE_FILE_PATH "C:/..."
#endif
#define TEXTURE_METALBOX_PNG TEXTURE_FILE_PATH"metalbox_holes.png"
#define TEXTURE_METALBOX_SPECULAR_PNG TEXTURE_FILE_PATH"metalbox_holes_specular.png"
#define TEXTURE_SURFACE_PNG TEXTURE_FILE_PATH"tile_diffuse.png"
#define TEXTURE_SURFACE_SPECULAR_PNG TEXTURE_FILE_PATH"tile_specular.png"
#define WINDOW_PNG TEXTURE_FILE_PATH"blending_transparent_window.png"

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
	return normal;
}

glm::vec4 calc_color( float x, float y, float z, float alpha = 1.0f );

void lighting_standard_handler( Shader shader, Buttonpress_eh *buttonpress_eh );

unsigned int load_cubemap( vector<string> faces, bool fill );
unsigned int load_texture( char const *path );

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
Shader mirror_shader( MIRROR_VS_PATH, MIRROR_FS_PATH );
Shader skybox_shader( SKYBOX_VS_PATH, SKYBOX_FS_PATH );

/*------------------------------------------
-	Create VAO's
-------------------------------------------*/
// cube VAO
unsigned int VAO_cubeprop, VBO_cubeprop;
glGenVertexArrays( 1, &VAO_cubeprop );
glGenBuffers( 1, &VBO_cubeprop );
glBindVertexArray( VAO_cubeprop );
glBindBuffer( GL_ARRAY_BUFFER, VBO_cubeprop );
glBufferData( GL_ARRAY_BUFFER, sizeof(cube_vertices_ogl), &cube_vertices_ogl, GL_STATIC_DRAW);
glEnableVertexAttribArray( 0 );
glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );
glEnableVertexAttribArray( 1 );
glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 3 * sizeof(float) ) );
glEnableVertexAttribArray( 2 );
glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 6 * sizeof(float) ) );
glBindVertexArray( 0 );

// cube VAO
unsigned int VAO_cube, VBO_cube;
glGenVertexArrays( 1, &VAO_cube );
glGenBuffers( 1, &VBO_cube );
glBindVertexArray( VAO_cube );
glBindBuffer( GL_ARRAY_BUFFER, VBO_cube );
glBufferData( GL_ARRAY_BUFFER, sizeof(mirror_box), &mirror_box, GL_STATIC_DRAW );
glEnableVertexAttribArray( 0 );
glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0 );
glEnableVertexAttribArray( 1 );
glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)( 3 * sizeof(float) ) );
glBindVertexArray( 0 );

// skybox VAO
unsigned int VAO_skybox, VBO_skybox;
glGenVertexArrays( 1, &VAO_skybox );
glGenBuffers( 1, &VBO_skybox );
glBindVertexArray( VAO_skybox );
glBindBuffer( GL_ARRAY_BUFFER, VBO_skybox );
glBufferData( GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW );
glEnableVertexAttribArray( 0 );
glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
glBindVertexArray( 0 );

/*----------------------------------------------
-  FBO
-----------------------------------------------*/
// create framebuffer and bind it
unsigned int FBO_mirror;
glGenFramebuffers( 1, &FBO_mirror );
glBindFramebuffer( GL_FRAMEBUFFER, FBO_mirror );
// generate a depth/stencil render buffer attachment
unsigned int rbo;
glGenRenderbuffers( 1, &rbo );
glBindRenderbuffer( GL_RENDERBUFFER, rbo );
glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height );
glBindRenderbuffer( GL_RENDERBUFFER, 0 );
glBindFramebuffer( GL_FRAMEBUFFER, 0 );

/*------------------------------------------
-	Load Models
-------------------------------------------*/
Model nanosuit( "D:/OGL_projects/OGL_3D_textures/entry/resources/models/nansuit_reflection/nanosuit.obj" );

/*------------------------------------------
-	Load Textures
-------------------------------------------*/
#ifndef LAPTOP
#define CUBE_TEXTURE "D:/OGL_projects/OGL_3D_textures/entry/resources/textures/3D_textures/ely_nevada/"
#else
	#define CUBE_TEXTURE  "C:/..."
#endif

//vector<string> texture_faces
//{
//	CUBE_TEXTURE"shadowpeak_rt.tga",
//	CUBE_TEXTURE"shadowpeak_lf.tga",
//	CUBE_TEXTURE"shadowpeak_up.tga",
//	CUBE_TEXTURE"shadowpeak_dn.tga",
//	CUBE_TEXTURE"shadowpeak_bk.tga",
//	CUBE_TEXTURE"shadowpeak_ft.tga"
//};
vector<string> texture_faces
{
	CUBE_TEXTURE"nevada_rt.tga",
	CUBE_TEXTURE"nevada_lf.tga",
	CUBE_TEXTURE"nevada_up.tga",
	CUBE_TEXTURE"nevada_dn.tga",
	CUBE_TEXTURE"nevada_bk.tga",
	CUBE_TEXTURE"nevada_ft.tga"
};

#undef CUBE_TEXTURE
unsigned int texture_skybox = load_cubemap( texture_faces, true );
unsigned int mirror_box = load_cubemap( texture_faces, true ); 

/*------------------------------------------
-	Load Textures
-------------------------------------------*/
unsigned int cube_texture = load_texture( TEXTURE_METALBOX_PNG );
unsigned int cube_specular_texture = load_texture( TEXTURE_METALBOX_SPECULAR_PNG );

/*------------------------------------------
-	Assign Textures
-------------------------------------------*/
mirror_shader.use();
mirror_shader.set_int( "skybox", 3 );

skybox_shader.use();
skybox_shader.set_int( "skybox", 3 );

standard_shader.use();
standard_shader.set_int( "material.texture_diffuse", 3 );
standard_shader.set_int( "material.texture_specular", 4 );
standard_shader.set_float( "material.shininess", 16.0f );


Buttonpress_eh *buttonpress_eh = new Buttonpress_eh( window );

/*----------------------------------------------
- depth testing setup
-----------------------------------------------*/
glEnable( GL_DEPTH_TEST );

bool flashlight_on_off = false; 

while ( !glfwWindowShouldClose( window ) )
{
	/* check key inputs for this frame*/
	buttonpress_eh->evnt_process_key_inputs();

	/*----------------------------------------------
	- update camera
	-----------------------------------------------*/
	camera.calc_speed();
	camera.process_button_presses( buttonpress_eh );

	/*----------------------------------------------
	- render
	-----------------------------------------------*/
	glBindFramebuffer( GL_FRAMEBUFFER, FBO_mirror );
	glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glm::vec3 translate_vector_cube ( -1.5f, 0.0f, 0.0f );
	for( unsigned int face_itr = 0; face_itr < 6; ++face_itr )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + face_itr, mirror_box, 0 );

		standard_shader.use();
		camera.mirror( FACE_RIGHT + face_itr, translate_vector_cube, window_width, window_height, standard_shader.ID );
		standard_shader.set_vec3( "viewPos", camera.get_camera_pos() );
		model = glm::mat4();
		model = glm::translate( model, translate_vector_cube );
		standard_shader.set_mat4( "model", model );
		lighting_standard_handler( standard_shader, buttonpress_eh );
		glBindVertexArray( VAO_cubeprop );
		standard_shader.set_int( "diffuseNr", 0 );
		standard_shader.set_int( "specularNr", 0 );
		glActiveTexture( GL_TEXTURE3 );
		glBindTexture( GL_TEXTURE_2D, cube_texture );
		glActiveTexture( GL_TEXTURE4 );
		glBindTexture( GL_TEXTURE_2D, cube_specular_texture );
		glDrawArrays( GL_TRIANGLES, 0, 36 );

		/* render nanosuit model*/
		standard_shader.use();
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		standard_shader.set_mat4( "model", model );
		nanosuit.Draw( standard_shader );

		// skybox
		skybox_shader.use();
		camera.mirror( FACE_RIGHT + face_itr, translate_vector_cube, window_width, window_height, skybox_shader.ID );
		glDepthFunc( GL_LEQUAL );
		glBindVertexArray( VAO_skybox );
		glActiveTexture( GL_TEXTURE3 );
		glBindTexture( GL_TEXTURE_CUBE_MAP, texture_skybox );
		glDrawArrays( GL_TRIANGLES, 0, 36 );
		glBindVertexArray( 0 );
		glDepthFunc( GL_LESS );
	}
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// cube
	mirror_shader.use();
	camera.update_camera( window_width, window_height, mirror_shader.ID );
	model = glm::mat4();
	mirror_shader.set_mat4( "model", model );
	mirror_shader.set_vec3( "cameraPos", camera.get_camera_pos() );
	glBindVertexArray( VAO_cube );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, mirror_box );
	glDrawArrays( GL_TRIANGLES, 0, 36 );
	glBindVertexArray(0);

	standard_shader.use();
	camera.update_camera( window_width, window_height, standard_shader.ID );
	standard_shader.set_vec3( "viewPos", camera.get_camera_pos() );
	model = glm::mat4();
	model = glm::translate( model, translate_vector_cube );
	standard_shader.set_mat4( "model", model );
	lighting_standard_handler( standard_shader, buttonpress_eh );
	glBindVertexArray( VAO_cubeprop );
	standard_shader.set_int( "diffuseNr", 0 );
	standard_shader.set_int( "specularNr", 0 );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, cube_texture );
	glActiveTexture( GL_TEXTURE4 );
	glBindTexture( GL_TEXTURE_2D, cube_specular_texture );
	glDrawArrays( GL_TRIANGLES, 0, 36 );

	/* render nanosuit model*/
	standard_shader.use();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	standard_shader.set_mat4( "model", model );
	nanosuit.Draw( standard_shader, texture_skybox );

	// skybox
	skybox_shader.use();
	camera.update_camera( window_width, window_height, skybox_shader.ID );
	skybox_shader.set_mat4( "view", glm::mat4(glm::mat3(camera.get_camera_view())) );
	glDepthFunc( GL_LEQUAL );
	glBindVertexArray( VAO_skybox );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, texture_skybox );
	glDrawArrays( GL_TRIANGLES, 0, 36 );
	glBindVertexArray( 0 );
	glDepthFunc( GL_LESS );


	// swap buffers and prepare for next render
	glfwSwapBuffers( window );
	glfwPollEvents();

}

glDeleteVertexArrays( 1, &VAO_cube );
glDeleteVertexArrays( 1, &VAO_skybox );
glDeleteFramebuffers( 1, &FBO_mirror );
glDeleteBuffers( 1, &VBO_cube );
glDeleteBuffers( 1, &VBO_skybox );
glDeleteTextures( 1, &mirror_box );
glDeleteRenderbuffers( 1, &rbo );
glDeleteTextures( 1, &texture_skybox );

glfwTerminate();

return 0;
}

glm::vec4 calc_color( float x, float y, float z, float alpha )
{
	return( glm::vec4(x/255, y/255, z/255, alpha));
}

// rt, lf, up, dn, bk, ft
unsigned int load_cubemap( vector<string> faces, bool fill )
{
	int width_texture, height_texture, nr_channels;
	unsigned int texture_ID;

	glGenTextures( 1, &texture_ID );
	glBindTexture( GL_TEXTURE_CUBE_MAP, texture_ID );
	for( GLuint i = 0; i < 6 && !fill; i++ )
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	}
	for( GLuint i = 0; i < faces.size() && fill; i++ )
	{
		unsigned char *data = stbi_load( faces[i].c_str(), &width_texture, &height_texture, &nr_channels, 0 );
		if ( data )
		{
			glTexImage2D
			( 
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, data 
			);
			stbi_image_free( data );
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free( data );
			assert_always();
		}
	}
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE ); 

	return texture_ID;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int load_texture(char const *path)
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

float * gen_normals
(
	float *plane_vertices
)
{
	// plane VAO
	// normal generation
	const unsigned int plane_normals_size =  3 * sizeof(plane_vertices) / ( 5 * sizeof(float) );
	float plane_normals[ 1 ]; // should be plane_normals_size
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
	float return_val[1];
	return_val[0] = 1;
	return return_val;
}

void lighting_standard_handler
(
	Shader shader,
	Buttonpress_eh *buttonpress_eh
)
{
	/*----------------------------------------------
	- flashlight
	-----------------------------------------------*/
	if ( buttonpress_eh->evnt_query_event_list( EH_F_CLICKED ) )
	{
		shader.flashlight_on_off = !shader.flashlight_on_off;
	}
	shader.set_bool( "flashLightOnOff", shader.flashlight_on_off );
	if ( shader.flashlight_on_off == true )
	{
		glm::vec4 flash_light_color = calc_color(255, 255, 255);
		glm::vec4 diffuse_color = flash_light_color * glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		glm::vec4 ambient_color = diffuse_color * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

		shader.set_vec3( "flashLight.posistion", camera.get_camera_pos() );
		shader.set_vec3( "flashLight.direction", camera.get_camera_front() );
		shader.set_float( "flashLight.innerCutoff", glm::cos(glm::radians(12.5f) ) );
		shader.set_float( "flashLight.outerCutOff", glm::cos(glm::radians(17.5f) ) );
		shader.set_vec4( "flashLight.ambient", ambient_color );
		shader.set_vec4( "flashLight.diffuse", diffuse_color );
		shader.set_vec4( "flashLight.specular", glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		shader.set_float( "flashLight.constant", 1.0f );
		shader.set_float( "flashLight.linear", 0.09f );
		shader.set_float( "flashLight.quadratic", 0.032f );
	}

	/*----------------------------------------------
	- point lights
	-----------------------------------------------*/
	for( int i = 0; i < 4; i++ )
	{
		glm::vec4 light_color = calc_color(255, 255, 255);
		glm::vec4 diffuse_color = light_color * glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
		glm::vec4 ambient_color = diffuse_color * glm::vec4(0.09f, 0.09f, 0.09f, 1.0f);

		std::string k = std::to_string(i);
		shader.set_vec3( "pointLights[" + k + "].posistion", pointLightPositions[0] );		 
		shader.set_vec4( "pointLights[" + k + "].ambient", ambient_color );
		shader.set_vec4( "pointLights[" + k + "].diffuse", diffuse_color );
		shader.set_vec4( "pointLights[" + k + "].specular", glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );	 
		shader.set_float( "pointLights[" + k + "].constant", 1.0f );
		shader.set_float( "pointLights[" + k + "].linear", 0.09f );
		shader.set_float( "pointLights[" + k + "].quadratic", 0.032f );
	}

	/*----------------------------------------------
	- sun
	-----------------------------------------------*/
	shader.set_vec4( "sun.direction", glm::vec4( -0.2f, -1.0f, -0.3f, 1.0f ) );
	shader.set_vec4( "sun.ambient", glm::vec4( 0.05f, 0.05f, 0.05f, 1.0f ) );
	shader.set_vec4( "sun.diffuse", glm::vec4( 0.4f, 0.4f, 0.4f, 1.0f ) );
	shader.set_vec4( "sun.specular", glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
}