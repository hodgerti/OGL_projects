/*****************************************************
*	filename: camera_class.cpp
*	description: camera handling
*
*
******************************************************/

#include <camera\camera_class.h>

glm::vec3 Camera::get_camera_front()
{
	return camera_front;
}

glm::vec3 Camera::get_camera_pos()
{
	return camera_pos;
}

glm::mat4 Camera::get_camera_view()
{
	return view;
}

glm::mat4 Camera::get_camera_proj()
{
	return proj;
}

void Camera::set_camera_speed_mod
( 
float val 
)
{
	camera_speed_mod = val;
}

float Camera::get_camera_speed_mod
(
)
{
	return camera_speed_mod;
}
/*****************************************************
*	function name: mouse_callback
*	description: callback to resize the OGL window afer a GLFW resize
*
******************************************************/
void Camera::mouse_callback
	(
	GLFWwindow* window, 
	double xpos, 
	double ypos
	)
{
    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);
}

void Camera::process_button_presses
( 
Buttonpress_eh *buttonpress_eh 
)
{
/* Save some time by not processing if an event isn't triggered	*/
if ( buttonpress_eh->evnt_process_key_inputs() )
{
	if ( buttonpress_eh->evnt_query_event_list( EH_W_PRESSED ) )
	{
		camera_pos += camera_speed * camera_front;
	}
	if ( buttonpress_eh->evnt_query_event_list( EH_S_PRESSED ) )
	{
		camera_pos -= camera_speed * camera_front;
	}
	if ( buttonpress_eh->evnt_query_event_list( EH_D_PRESSED ) )
	{
		camera_pos += glm::normalize( glm::cross( camera_front, camera_up ) ) * camera_speed;
	}
	if ( buttonpress_eh->evnt_query_event_list( EH_A_PRESSED ) )
	{
		camera_pos -= glm::normalize( glm::cross( camera_front, camera_up ) ) * camera_speed;		
	}
	if ( buttonpress_eh->evnt_query_event_list( EH_SHIFT_PRESSED ) )
	{
		camera_pos += glm::vec3( 0.0f, 0.65f, 0.0f ) * camera_speed;
	}
	if ( buttonpress_eh->evnt_query_event_list( EH_SPACE_PRESSED ) )
	{
		camera_pos -= glm::vec3( 0.0f, 0.65f, 0.0f ) * camera_speed;
	}
}
}
void Camera::calc_speed
(
)
{
	// Calculate new camera_speed
	current_frame = (float)glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	camera_speed = camera_speed_mod * delta_time;
}

/***************************************************************
* NOTE: Model Translation needs to be done seperately
***************************************************************/
void Camera::update_camera
(
int window_width,
int window_height,
unsigned int ID
)
{
	view = glm::mat4();
	proj = glm::mat4();
	view = glm::lookAt(	camera_pos,					// posistion
  						camera_pos + camera_front,	// target, translates target looking at with pos
  						camera_up );				// up
	proj = glm::perspective( glm::radians(fov), (float)window_width/(float)window_height, 0.1f, 100.0f );

	glUniformMatrix4fv( glGetUniformLocation(ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv( glGetUniformLocation(ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
}

void Camera::scroll_callback
( 
GLFWwindow* window, 
double xoffset, 
double yoffset 
)
{
	if ( fov >= 1.0f && fov <= 45.0f )
		fov -= yoffset;
	if ( fov < 1.0f )
		fov = 1.0f;
	if ( fov > 45.0f )
		fov = 45.0f;
}

Camera::Camera()
{

}

Camera::~Camera()
{

}