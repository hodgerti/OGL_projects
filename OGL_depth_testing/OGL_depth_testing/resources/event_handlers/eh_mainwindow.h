#ifndef WINDOW_EH_H
#define WINDOW_EH_H

/*****************************************************
*	filename: window_eh.h
*	description: custom handler for GLFW window context
*
*
******************************************************/

#pragma once
/*****************************************************
*	Includes
******************************************************/
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mainwindow_eh
{
public:
	Mainwindow_eh();
	~Mainwindow_eh();
	void evnt_framebuffer_size_callback( GLFWwindow *window, int width,int height );
};


#endif