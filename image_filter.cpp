/*
Grant Nike
Oct 8th
6349302
COSC 3P98 Assignment #1
Image Processing Application
*/
#define FILENAME "img.tif"

#include <stdlib.h>

#include <stdio.h>
#include <malloc.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <vector>

//the pixel structure
typedef struct {
	GLubyte r, g, b;
} pixel;

//the global structure
typedef struct {
	std::vector<std::vector<pixel>> working_buff;
	std::vector<std::vector<pixel>> temporary_buff;
	std::vector<std::vector<pixel>> saving_buff;
	pixel *work_buff;
	pixel *temp_buff;
	pixel *save_buff;
	int w, h;
} glob;
glob global;

enum {MENU_FILTER, MENU_SAVE, MENU_TRIANGLE, MENU_RESET, MENU_QUIT};

//read image
std::vector<std::vector<pixel>> read_img(char *name, int *width, int *height) {
	FIBITMAP *image;
	int i, j, pnum;
	RGBQUAD aPixel;
	pixel *data;

	if ((image = FreeImage_Load(FIF_TIFF, name, 0)) == NULL) {
		return NULL;
	}

	*width = FreeImage_GetWidth(image);
	*height = FreeImage_GetHeight(image);
	std::vector<std::vector<pixel>> data1;
	data = (pixel *)malloc((*height)*(*width)*sizeof(pixel *));
	pnum = 0;
	for(int x=0;x<(*width);x++){
		for(int y=0;y<(*height);y++){
			FreeImage_GetPixelColor(image, x, y, &aPixel);
			data1[i][j].r = (aPixel.rgbRed);
			data1[i][j].g = (aPixel.rgbGreen);
			data1[i][j].b = (aPixel.rgbBlue);
		}
	}
	for (i = 0; i < (*height); i++) {
		for (j = 0; j < (*width); j++) {
			FreeImage_GetPixelColor(image, j, i, &aPixel);
			data[pnum].r = (aPixel.rgbRed);
			data[pnum].g = (aPixel.rgbGreen);
			data[pnum++].b = (aPixel.rgbBlue);
		}
	}
	FreeImage_Unload(image);
	return data1;
}//read_img

//write_img
void write_img(char *name, pixel *data, int width, int height) {
	FIBITMAP *image;
	RGBQUAD aPixel;
	int i, j;

	image = FreeImage_Allocate(width, height, 24, 0, 0, 0);
	if (!image) {
		perror("FreeImage_Allocate");
		return;
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			aPixel.rgbRed = data[i*width + j].r;
			aPixel.rgbGreen = data[i*width + j].g;
			aPixel.rgbBlue = data[i*width + j].b;

			FreeImage_SetPixelColor(image, j, i, &aPixel);
		}
	}
	if (!FreeImage_Save(FIF_TIFF, image, name, 0)) {
		perror("FreeImage_Save");
	}
	FreeImage_Unload(image);
}//write_img


/*draw the image - it is already in the format openGL requires for glDrawPixels*/
void display_image(void)
{
	glDrawPixels(global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.work_buff);
	glFlush();
}//display_image()

// Read the screen image back to the data buffer after drawing to it
void draw_triangle(void)
{
	glDrawPixels(global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.working_buff.data());
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0, 0);
	glVertex2i(rand() % global.w, rand() % global.h);
	glColor3f(0, 1.0, 0);
	glVertex2i(rand() % global.w, rand() % global.h);
	glColor3f(0, 0, 1.0);
	glVertex2i(rand() % global.w, rand() % global.h);
	glEnd();
	glFlush();
	glReadPixels(0, 0, global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.working_buff.data());
}

//Creates a deep copy of a pixel struct array
pixel* deep_copy(pixel* original){

	//Note: watch for memory leaks!***********************************************************************************************************************************
	
	//allocate space for new pixel struct
	pixel* copy = (pixel *)malloc((*(&global.h))*(*(&global.w))*sizeof(pixel *));
	//read rgb values from original image and copy them to the copy of the image
	for (int i = 0; i < global.h; i++) {
		for (int j = 0; j < global.w; j++) {
			copy[i*global.w + j].r = original[i*global.w + j].r;
			copy[i*global.w + j].g = original[i*global.w + j].g;
			copy[i*global.w + j].b = original[i*global.w + j].b;
		}
	}
	return copy;
}

// A greyscale filter.
void greyscale_filter(pixel* work_buff, pixel* temp_buff, int myIm_Width, int myIm_Height){
	int i, j;
	//Go through each pixel in image, take the average of the rgb values of each, and set the rgb values to the average
	for (i = 0; i < myIm_Height; i++) {
		for (j = 0; j < myIm_Width; j++) {
			int rgb_sum = work_buff[i*myIm_Width + j].r + work_buff[i*myIm_Width + j].g + work_buff[i*myIm_Width + j].b;
			temp_buff[i*myIm_Width + j].r = rgb_sum/3;
			temp_buff[i*myIm_Width + j].g = rgb_sum/3;
			temp_buff[i*myIm_Width + j].b = rgb_sum/3;
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

//Resets image to the unfiltered original
void reset_image(){
	global.work_buff = deep_copy(global.save_buff);
	glutPostRedisplay();
}


/*glut keyboard function*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 0x1B:
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'r':
	case 'R':
		reset_image();
		break;
	case 's':
	case 'S':
		printf("SAVING IMAGE: backup.tif\n");
		write_img("backup.tif", global.work_buff, global.w, global.h);
		break;
	case 't':
	case 'T':
		draw_triangle();
		break;
	case 'f':
	case 'F':
		greyscale_filter(global.work_buff, global.temp_buff, global.w, global.h);
		break;
	}
}//keyboard


//Glut menu callback function
void menuFunc(int value)
{
	switch (value) {
		case MENU_QUIT:
			exit(0);
			break;
		case MENU_SAVE:
			printf("SAVING IMAGE: backup.tif\n");
			write_img("backup.tif", global.work_buff, global.w, global.h);
			break;
		case MENU_RESET:
			reset_image();
			break;
		case MENU_TRIANGLE:
			draw_triangle();
			break;
		case MENU_FILTER:
			greyscale_filter(global.work_buff, global.temp_buff, global.w, global.h);
			break;
	}
}//menuFunc


void show_keys()
{
	printf("Q:quit\nR:reset\nF:filter\nT:triangle\nS:save\n");
}

//Glut menu set up
void init_menu()
{
	int sub_menu = glutCreateMenu(&menuFunc);
	glutAddMenuEntry("Triangle", MENU_TRIANGLE);
	glutAddMenuEntry("Filter", MENU_FILTER);

	int main_menu = glutCreateMenu(&menuFunc);
	glutAddSubMenu("Modify", sub_menu);
	glutAddMenuEntry("Reset", MENU_RESET);
	glutAddMenuEntry("Save", MENU_SAVE);
	glutAddMenuEntry("Quit", MENU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{
	global.saving_buff = read_img(FILENAME, &global.w, &global.h);
	global.working_buff = read_img(FILENAME, &global.w, &global.h);
	global.temporary_buff = global.working_buff;
	/*if (global.save_buff == NULL)
	{
		printf("Error loading image file %s\n", FILENAME);
		return 1;
	}*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(global.w, global.h);
	glutCreateWindow("SIMPLE DISPLAY");
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(display_image);
	glutKeyboardFunc(keyboard);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, global.w, 0, global.h, 0, 1);

	init_menu();
	show_keys();

	glutMainLoop();

	return 0;
}