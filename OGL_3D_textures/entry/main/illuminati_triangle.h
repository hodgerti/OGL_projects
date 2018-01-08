#ifndef ILLUMINATI_TRIANGLE_H
#define ILLUMINATI_TRIANGLE_H

/*****************************************************
*	filename: illuminati_triangle.h
*	description: functions to made a factored triangle
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include <debug_custom\custom_debugging.h>

/*------------------------------------------
Types
-------------------------------------------*/
struct cust_vec3
{
	float x;
	float y;
	float z;
};

/*------------------------------------------
Memory constants
-------------------------------------------*/
#define ILLUMINATI_VERTICE_SIZE	5


/*------------------------------------------
Procedures
-------------------------------------------*/

/* mid point between 2 points in space */
cust_vec3 mid_point
(
cust_vec3 a,
cust_vec3 b
);


int int_pow(int base, int exp);

/* helps calculates fractal vertice size	*/
int calc_increasing_fractal
(
int val,
int depth
);


/* calculates fractal vertice size	*/
int calc_illuminati_size
(
int depth
);


int recursive_function
(
cust_vec3 a,
cust_vec3 b,
cust_vec3 c,
int depth,
float *vertices,
int vert_idx
);

#endif