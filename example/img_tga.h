#ifndef __TGA_H__
#define __TGA_H__

//#pragma comment(lib, "Opengl32.lib")					//Link to OpenGL32.lib so we can use OpenGL stuff
//
//#include <windows.h>									// Standard windows header
//#include <stdio.h>										// Standard I/O header 
//#include <gl\gl.h>										// Header for OpenGL32 library
//
//#include <windows.h>									// Standard windows header
//#include <stdio.h>										// Standard I/O header 
//#include <gl\gl.h>										// Header for OpenGL32 library
//
//typedef	struct									
//{
//	GLubyte	* data;									// Image Data (Up To 32 Bits)
//	unsigned int	bpp;											// Image Color Depth In Bits Per Pixel
//	unsigned int	width;											// Image Width
//	unsigned int	height;											// Image Height
//	unsigned int	texID;											// Texture ID Used To Select A Texture
//	unsigned int	type;											// Image Type (GL_RGB, GL_RGBA)
//} Texture;	
//
//bool loadUncompressedTGA(Texture *, char *, FILE *);	// Load an Uncompressed file
//bool loadCompressedTGA(Texture *, char *, FILE *);		// Load a Compressed file
//bool loadTGA(Texture * texture, char * filename);
//bool saveTGA(Texture * texture, char * filename);

//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
 
#define uchar unsigned char
#define sint short int
 
typedef struct
{
        uchar depth;
        sint w, h;
        uchar* data;
} Texture;
 
Texture* loadTGA(char* fn);

void saveTGA(Texture *tga, char *filename);

#endif






