#ifndef EH_BUTTONPRESSES_H
#define EH_BUTTONPRESSES_H

/*****************************************************
*	filename: eh_buttonpresses.h
*	description: handler for GLFW window button presses
*
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <GLFW/glfw3.h>

/*****************************************************
*	Enums
******************************************************/
/* Possible hints	*/
enum
{
	EH_BUTTONPRESS_WINDOW_CLOSE,
	EH_BUTTONPRESS_CHANGE_SHADER,
	EH_UP_CLICK,
	EH_DOWN_CLICK,
	EH_RIGHT_CLICK,
	EH_LEFT_CLICK,
	EH_UP_PRESSED,
	EH_DOWN_PRESSED,
	EH_LEFT_PRESSED,
	EH_RIGHT_PRESSED,
	EH_W_PRESSED,
	EH_A_PRESSED,
	EH_S_PRESSED,
	EH_D_PRESSED,
	EH_R_PRESSED,
	EH_R_CLICKED,
	EH_R_UNCLICKED,
	EH_E_PRESSED,
	EH_SHIFT_PRESSED,
	EH_SPACE_PRESSED,
	EH_F_PRESSED,
	EH_F_CLICKED,

	EH_BUTTONPRESS_NO_MATCH,

	EH_BUTTONPRESS_NO_HINT,

	EH_BUTTONPRESS_HINT_NUM
};

class Buttonpress_eh
{
public:
	Buttonpress_eh( GLFWwindow *window_ext );
	~Buttonpress_eh();
	bool evnt_query_event_list( int event );
	bool evnt_process_key_inputs( void );
private:
	int *event_list;
	GLFWwindow *window;
	int len;
	
	/* booleans to handle one time presses */
	bool eh_buttonpress_change_shader		= true;
	bool eh_buttonpress_mix_visability_up	= true;
	bool eh_buttonpress_mix_visability_down	= true;
	bool eh_buttonpress_rotate_view_right	= true;
	bool eh_buttonpress_rotate_view_left	= true;
	bool eh_buttonpress_r_clicked			= true;
	bool eh_buttonpress_r_unclicked			= true;
	bool eh_buttonpress_f_clicked			= true;
};
#endif
