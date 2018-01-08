/*****************************************************
*	filename: illuminati_triangle.cpp
*	description: functions to made a factored triangle
*
******************************************************/

/*****************************************************
*	Includes
******************************************************/
#include "./illuminati_triangle.h"


/*------------------------------------------
Procedures
-------------------------------------------*/
/* mid point between 2 points in space */
cust_vec3 mid_point
(
cust_vec3 a,
cust_vec3 b
)
{
cust_vec3 vec3 =
{
( a.x + b.x ) / 2,
( a.y + b.y ) / 2,
( a.z + b.z ) / 2
};

return vec3;

}


int int_pow(int base, int exp)
{
int result = 1;
while (exp)
{
    if (exp & 1)
        result *= base;
    exp /= 2;
    base *= base;
}
return result;
}

/* helps calculates fractal vertice size	*/
int calc_increasing_fractal
(
int val,
int depth
)
{
if ( depth == 0 )
{
 return val;
}
calc_increasing_fractal( ( val += int_pow( 3, depth + 1 ) ), --depth);
}


/* calculates fractal vertice size	*/
int calc_illuminati_size
(
int depth
)
{
if ( depth < 0 )
{
	assert_always();
}
else if ( depth == 0 )
{
	return 3;// * ILLUMINATI_VERTICE_SIZE;
}
else
{
	return calc_increasing_fractal( 3, depth );
}
}


int recursive_function
(
cust_vec3 a,
cust_vec3 b,
cust_vec3 c,
int depth,
float *vertices,
int vert_idx
)
{
if( depth < 0 )
{
	return vert_idx;
}

/* create vertices */
int place_idx = 0;
// pos a
vertices[ vert_idx + place_idx++ ]	= a.x;  printf( "ST( vert_idx -  %d ):\n%f   ", vert_idx, vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= a.y;	printf( "%f   ", vertices[ vert_idx + place_idx - 1] );
vertices[ vert_idx + place_idx++ ]	= a.z;	printf( "%f\n", vertices[ vert_idx + place_idx - 1 ] );
																						  
//texture coords a																		  
vertices[ vert_idx + place_idx++ ]	= 0.5f;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= 1.0f;	printf( "%f\n", vertices[ vert_idx + place_idx - 1 ] );
																						 
// pos b																				 
vertices[ vert_idx + place_idx++ ]	= b.x;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= b.y;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= b.z;	printf( "%f\n", vertices[ vert_idx + place_idx - 1 ] );
																						  
//texture coords b																		  
vertices[ vert_idx + place_idx++ ]	= 0;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= 0;	printf( "%f\n", vertices[ vert_idx + place_idx - 1 ] );
																						  
// pos c																				  
vertices[ vert_idx + place_idx++ ]	= c.x;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= c.y;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= c.z;	printf( "%f\n", vertices[ vert_idx + place_idx - 1 ] );	
																						 
//texture coords c																		 
vertices[ vert_idx + place_idx++ ]	= 1.0f;	printf( "%f   ", vertices[ vert_idx + place_idx - 1 ] );
vertices[ vert_idx + place_idx++ ]	= 0;	printf( "%f\n:END\n\n\n", vertices[ vert_idx + place_idx - 1 ] );

cust_vec3 x = mid_point( a, b );
cust_vec3 y = mid_point( a, c );
cust_vec3 z = mid_point( b, c );

depth--;
vert_idx += place_idx;
vert_idx = recursive_function( a, x, y, depth, vertices, vert_idx );
vert_idx = recursive_function( x, b, z, depth, vertices, vert_idx );
recursive_function( y, z, c, depth, vertices, vert_idx );

}

void main_thing ()
{
/*------------------------------------------
-	Setting up VAO for illuminati
-------------------------------------------*/
//int depth = 2;
//int size = calc_illuminati_size( depth );
//size *= ILLUMINATI_VERTICE_SIZE;
//float *illum_vertices = new float[ size ];
//cust_vec3 a = { 0.0f,  0.5f, 0.0f	};
//cust_vec3 b = { -0.5f, -0.5f, 0.0f	};
//cust_vec3 c = { 0.5f, -0.5f, 0.0f	};
//recursive_function( a, b, c, depth, illum_vertices,  0 );
//
//glBindVertexArray( VAO_illuminati );
//
//glBindBuffer( GL_ARRAY_BUFFER, VBO_illuminati );
//glBufferData( GL_ARRAY_BUFFER, sizeof( illum_vertices ), illum_vertices, GL_STATIC_DRAW );
//
//glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )0 );
//glEnableVertexAttribArray( 0 );
//glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )( 3 * sizeof( float ) ) );
//glEnableVertexAttribArray( 2 );
}