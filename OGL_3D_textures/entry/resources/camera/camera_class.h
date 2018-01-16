#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

/*****************************************************
*	filename: camera_class.h
*	description: camera handling
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <event_handlers\eh_buttonpresses.h>
#include <debug_custom\custom_debugging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum 
{
	FACE_RIGHT,
	FACE_LEFT,
	FACE_UP,
	FACE_DOWN,
	FACE_BACK,
	FACE_FRONT
};

class Camera
{
public:
	Camera();
	~Camera();
	glm::vec3 get_camera_front();
	glm::vec3 get_camera_pos();
	glm::mat4 get_camera_view();
	glm::mat4 get_camera_proj();
	void mouse_callback( GLFWwindow* window, double xpos, double ypos );
	void process_button_presses( Buttonpress_eh *buttonpress_eh );
	void calc_speed();
	void update_camera( int window_width, int window_height, unsigned int ID );
	void scroll_callback( GLFWwindow* window, double xoffset, double yoffset );
	void mirror( int dir, glm::vec3 pos, int window_width, int window_height, unsigned int ID );

private:
	glm::vec3 camera_front  = glm::vec3(0.0f, 0.0f,  0.0f);
	glm::vec3 camera_pos    = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 camera_up     = glm::vec3(0.0f, 1.0f,  0.0f);
	bool  firstMouse		= true;
	float yaw				= -90.0f;
	float pitch				=  0.0f;
	float lastX				=  800.0f / 2.0;
	float lastY				=  600.0 / 2.0;
	float fov				=  45.0f;
	float camera_speed		=  0.5f;
	float current_frame		=  0.0f;
	float delta_time		=  0.0f;
	float last_frame		=  0.0f;	
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 view_local;
	glm::mat4 proj_local;
};
#endif