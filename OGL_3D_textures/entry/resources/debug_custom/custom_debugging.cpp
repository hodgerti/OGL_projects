/*****************************************************
*	filename: custom_debugging.cpp
*	description: contains custom debugging functions
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include "debug_custom\custom_debugging.h"

/*****************************************************
*	Procedures
******************************************************/

/*****************************************************
*	function name: assert_always
*	description: slap a break point in here so I don't have
*	to see one in projects
******************************************************/
void assert_always
(
	void
)
{
	printf( "----ASSERT----\n" );
} /* assert_always()	*/