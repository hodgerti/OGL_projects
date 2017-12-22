/*****************************************************
*	filename: window_eh.c
*	description: custom handler for GLFW window context
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <GLFW/glfw3.h>
#include <event_handlers/eh_mainwindow.h>

/*****************************************************
*	Procedures
******************************************************/

/*****************************************************
*	function name: framebuffer_size_callback
*	description: callback to resize the OGL window afer a GLFW resize
*
******************************************************/
void Mainwindow_eh::evnt_framebuffer_size_callback
	( 
	GLFWwindow *window,
	int width, 
	int height 
	)
{
	glViewport( 0, 0, width, height );

} /* framebuffer_size_callback */


Mainwindow_eh::Mainwindow_eh()
{

}

Mainwindow_eh::~Mainwindow_eh()
{

}