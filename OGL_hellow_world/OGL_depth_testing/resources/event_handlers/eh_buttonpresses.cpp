/*****************************************************
*	filename: eh_buttonpresses.cpp
*	description: handler for GLFW window button presses
*
*
******************************************************/

#pragma once
/*****************************************************
*	Includes
******************************************************/
#include <GLFW/glfw3.h>
#include <event_handlers\eh_buttonpresses.h>

/*****************************************************
*	Procedures
******************************************************/

/*****************************************************
*	function name: evnt_query_event_list
*	description: queries if an event is in an event list
*
******************************************************/
bool Buttonpress_eh::evnt_query_event_list
(
	int event
)
{
for( int idx = 0; idx < len - 1; idx++ )
{
	if( event_list[ idx ] == event )
	{
		return true;
	}
}
return false;
} /* evnt_query_event_list	*/

/*****************************************************
*	function name: process_keys
*	description: does things with button presses
*
******************************************************/
bool Buttonpress_eh::evnt_process_key_inputs
(
	void
)
{
/*****************************************************
*	Local Variables
******************************************************/
int idx;

/*****************************************************
*	Init Local Variables
******************************************************/
idx = 0;

/* Clear event list		*/
for( int i = 0; i < len; i++ )
{
	event_list[ i ] = EH_BUTTONPRESS_NO_HINT;
}

/* ESC key handler	*/
if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, true );
		event_list[ idx++ ] = EH_BUTTONPRESS_WINDOW_CLOSE;
	}

/* G key handler - Could just track release of key, but where is the fun in that?	*/
if( glfwGetKey( window, GLFW_KEY_G ) == GLFW_PRESS && !eh_buttonpress_change_shader )
	{
	eh_buttonpress_change_shader = true;
	event_list[ idx++ ] = EH_BUTTONPRESS_CHANGE_SHADER;
	}
if( glfwGetKey( window, GLFW_KEY_G ) == GLFW_RELEASE && eh_buttonpress_change_shader )
	{
	eh_buttonpress_change_shader = false;
	}

/* If up-arrow key clicked	*/
if( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS && !eh_buttonpress_mix_visability_up )
	{
	eh_buttonpress_mix_visability_up = true;
	event_list[ idx++ ] = EH_UP_CLICK;
	}
if( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_UP_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_RELEASE && eh_buttonpress_mix_visability_up )
	{
	eh_buttonpress_mix_visability_up = false;
	}

/* If down-arrow key clicked	*/
if( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS && !eh_buttonpress_mix_visability_down )
	{
	eh_buttonpress_mix_visability_down = true;
	event_list[ idx++ ] = EH_DOWN_CLICK;
	}
if( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_DOWN_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_RELEASE && eh_buttonpress_mix_visability_down )
	{
	eh_buttonpress_mix_visability_down = false;
	}

/* If right-arrow key pressed	*/
if( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS && !eh_buttonpress_rotate_view_right )
	{
	eh_buttonpress_rotate_view_right = true;
	event_list[ idx++ ] = EH_RIGHT_CLICK;
	}
if( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_RIGHT_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_RELEASE && eh_buttonpress_rotate_view_right )
	{
	eh_buttonpress_rotate_view_right = false;
	}

/* If left-arrow key pressed	*/
if( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS && !eh_buttonpress_rotate_view_left )
	{
	eh_buttonpress_rotate_view_left = true;
	event_list[ idx++ ] = EH_LEFT_CLICK;
	}
if( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_LEFT_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_RELEASE && eh_buttonpress_rotate_view_left )
	{
	eh_buttonpress_rotate_view_left = false;
	}
	
/* button presses	*/
if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_W_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_S_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_D_PRESSED;
	}
if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_A_PRESSED;
	}

// r clicked
if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS && !eh_buttonpress_r_clicked )
	{
	eh_buttonpress_r_clicked = true;
	event_list[ idx++ ] = EH_R_CLICKED;
	}
if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_RELEASE && eh_buttonpress_r_clicked )
	{
	eh_buttonpress_r_clicked = false;
	}
// r unclicked
if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_RELEASE && !eh_buttonpress_r_unclicked )
	{
	eh_buttonpress_r_unclicked = true;
	event_list[ idx++ ] = EH_R_UNCLICKED;
	}
if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS && eh_buttonpress_r_unclicked )
	{
	eh_buttonpress_r_unclicked = false;
	}

// r pressed
if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_R_PRESSED;
	}

// e pressed
if( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_E_PRESSED;
	}

// shift pressed
if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_SHIFT_PRESSED;
	}

// space pressed
if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_SPACE_PRESSED;
	}

// f pressed
if( glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS )
	{
	event_list[ idx++ ] = EH_F_PRESSED;
	}

// f clicked
if( glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS && !eh_buttonpress_f_clicked )
	{
	eh_buttonpress_f_clicked = true;
	event_list[ idx++ ] = EH_F_CLICKED;
	}
if( glfwGetKey( window, GLFW_KEY_F ) == GLFW_RELEASE && eh_buttonpress_f_clicked )
	{
	eh_buttonpress_f_clicked = false;
	}



if( idx < 1 )
{
	return false;
}

return true;

} /* process_keys */


/*****************************************************
*	function name: constructor
*	description: init button eh
*
******************************************************/
Buttonpress_eh::Buttonpress_eh
( 
	GLFWwindow *window_ext
)
{
	window = window_ext;
	event_list = new int[ EH_BUTTONPRESS_HINT_NUM ];
	len = EH_BUTTONPRESS_HINT_NUM;
} /* Buttonpress_eh		*/

/*****************************************************
*	function name: destructor
*	description: boom boom button eh
*
******************************************************/
Buttonpress_eh::~Buttonpress_eh
( 
)
{
	delete [] event_list;
} /* Buttonpress_eh		*/