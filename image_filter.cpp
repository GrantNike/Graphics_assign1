/*
Grant Nike
Oct 8th
6349302
COSC 3P98 Assignment #1
Image Processing Application
*/
#define FILENAME "img.tif"

#include <stdlib.h>

#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <GL/glut.h>
#include <FreeImage.h>

//the pixel structure
typedef struct {
	GLubyte r, g, b;
} pixel;

//the global structure
typedef struct {
	pixel *work_buff;
	pixel *temp_buff;
	pixel *save_buff;
	int w, h;
	int x1,y1;
	int x2,y2;
} glob;
glob global;

enum {MENU_GREY, MENU_MONO, MENU_NTSC, MENU_SWAP, MENU_PURE_RED, MENU_PURE_GREEN, MENU_PURE_BLUE, MENU_SAVE, MENU_RESET, MENU_QUIT};

//read image
pixel *read_img(char *name, int *width, int *height) {
	FIBITMAP *image;
	int i, j, pnum;
	RGBQUAD aPixel;
	pixel *data;

	if ((image = FreeImage_Load(FIF_TIFF, name, 0)) == NULL) {
		return NULL;
	}
	*width = FreeImage_GetWidth(image);
	*height = FreeImage_GetHeight(image);

	data = (pixel *)malloc((*height)*(*width)*sizeof(pixel *));
	pnum = 0;
	for (i = 0; i < (*height); i++) {
		for (j = 0; j < (*width); j++) {
			FreeImage_GetPixelColor(image, j, i, &aPixel);
			data[pnum].r = (aPixel.rgbRed);
			data[pnum].g = (aPixel.rgbGreen);
			data[pnum++].b = (aPixel.rgbBlue);
		}
	}
	FreeImage_Unload(image);
	return data;
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
void greyscale_filter(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
	int i, j;
	//Go through each pixel in image, take the average of the rgb values of each, and set the rgb values to the average
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
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

void monochrome_filter(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
	int i, j;
	//Go through each pixel in image...
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
			int rgb_sum = work_buff[i*myIm_Width + j].r + work_buff[i*myIm_Width + j].g + work_buff[i*myIm_Width + j].b;
			if(rgb_sum/3 > (0.5*255)){
				temp_buff[i*myIm_Width + j].r = 255;
				temp_buff[i*myIm_Width + j].g = 255;
				temp_buff[i*myIm_Width + j].b = 255;
			}
			else{
				temp_buff[i*myIm_Width + j].r = 0;
				temp_buff[i*myIm_Width + j].g = 0;
				temp_buff[i*myIm_Width + j].b = 0;
			}
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

void ntsc_filter(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
int i, j;
	//Go through each pixel in image...
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
			int rgb_sum = (int)((0.299)*work_buff[i*myIm_Width + j].r) + (int)((0.587)*work_buff[i*myIm_Width + j].g) + (int)((0.114)*work_buff[i*myIm_Width + j].b);
			
			temp_buff[i*myIm_Width + j].r = rgb_sum;
			temp_buff[i*myIm_Width + j].g = rgb_sum;
			temp_buff[i*myIm_Width + j].b = rgb_sum;
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

void swap_channel(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
int i, j;
	//Go through each pixel in image...
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
			int temp = temp_buff[i*myIm_Width + j].r;
			temp_buff[i*myIm_Width + j].r = temp_buff[i*myIm_Width + j].g;
			temp_buff[i*myIm_Width + j].g = temp_buff[i*myIm_Width + j].b;
			temp_buff[i*myIm_Width + j].b = temp;
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

void pure_red_filter(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
int i, j;
	//Go through each pixel in image...
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
			temp_buff[i*myIm_Width + j].g = 0;
			temp_buff[i*myIm_Width + j].b = 0;
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

void pure_green_filter(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
int i, j;
	//Go through each pixel in image...
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
			temp_buff[i*myIm_Width + j].r = 0;
			temp_buff[i*myIm_Width + j].b = 0;
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

void pure_blue_filter(pixel* work_buff, pixel* temp_buff, int x1, int y1, int myIm_Width, int myIm_Height){
int i, j;
	//Go through each pixel in image...
	for (i = y1; i < myIm_Height; i++) {
		for (j = x1; j < myIm_Width; j++) {
			temp_buff[i*myIm_Width + j].r = 0;
			temp_buff[i*myIm_Width + j].g = 0;
		}
	}
	//Write finalized changes to display buffer
	global.work_buff = deep_copy(global.temp_buff);
	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}

//Resets image to the unfiltered original
void reset_image(){
	global.work_buff = deep_copy(global.save_buff);
	global.temp_buff = deep_copy(global.save_buff);
	glutPostRedisplay();
}
//Sets x1,y1 coordinates to left mouse click coordinates
void set_coordinates(int button, int state, int x, int y){
	int count = 0;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(count == 0){
			global.x1 = x;
			global.y1 = global.h-y-35;
			count++;
		}
		else if(count == 1){
			global.x2 = x;
			global.y2 = global.h-y;
			count++;
		}
		if (global.x1 > global.x2){
			int temp_x = global.x1;
			global.x1 = global.x2;
			global.x2 = temp_x;
		}
		if (global.y1 > global.y2){
			int temp_y = global.y1;
			global.y1 = global.y2;
			global.y2 = temp_y;
		}
	}
}
//Sets x2,y2 coordinates to left mouse click coordinates
void set_coordinates2(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		global.x2 = x;
		global.y2 = global.h-y;
	}
}

void area_select(){
	glutMouseFunc(set_coordinates);
	if (global.x1 > global.x2){
		int temp_x = global.x1;
		global.x1 = global.x2;
		global.x2 = temp_x;
	}
	if (global.y1 > global.y2){
		int temp_y = global.y1;
		global.y1 = global.y2;
		global.y2 = temp_y;
	}
}

//Resets rectangular filter area to being the entire image
void reset_filter_area(){
	global.x1 = 0;
	global.y1 = 0;
	global.x2 = global.w;
	global.y2 = global.h;
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
	case 'g':
	case 'G':
		greyscale_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
		break;
	case 'm':
	case 'M':
		monochrome_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
		break;
	case 'n':
	case 'N':
		ntsc_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
		break;
	case 'c':
	case 'C':
		swap_channel(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
		break;
	case 'p':
	case 'P':
		pure_red_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
		break;
	case 'a':
	case 'A':
		area_select();
		break;
	case 'k':
	case 'K':
		reset_filter_area();
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
		case MENU_GREY:
			greyscale_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
			break;
		case MENU_MONO:
			monochrome_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
			break;
		case MENU_NTSC:
			ntsc_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
			break;
		case MENU_SWAP:
			swap_channel(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
			break;
		case MENU_PURE_RED:
			pure_red_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
			break;
		case MENU_PURE_GREEN:
			pure_green_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
			break;
		case MENU_PURE_BLUE:
			pure_blue_filter(global.work_buff, global.temp_buff, global.x1, global.y1, global.x2, global.y2);
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
	int pure_filter = glutCreateMenu(&menuFunc);
	glutAddMenuEntry("Pure Red Filter", MENU_PURE_RED);
	glutAddMenuEntry("Pure Green Filter", MENU_PURE_GREEN);
	glutAddMenuEntry("Pure Blue Filter", MENU_PURE_BLUE);

	int sub_menu = glutCreateMenu(&menuFunc);
	glutAddMenuEntry("Greyscale Filter", MENU_GREY);
	glutAddMenuEntry("Monochrome Filter", MENU_MONO);
	glutAddMenuEntry("NTSC Filter", MENU_NTSC);
	glutAddMenuEntry("Channel Swap Filter", MENU_SWAP);
	glutAddSubMenu("Pure RGB Filter", pure_filter);

	int main_menu = glutCreateMenu(&menuFunc);
	glutAddSubMenu("Filter", sub_menu);
	glutAddMenuEntry("Reset", MENU_RESET);
	glutAddMenuEntry("Save", MENU_SAVE);
	glutAddMenuEntry("Quit", MENU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{
	global.save_buff = read_img(FILENAME, &global.w, &global.h);
	global.work_buff = read_img(FILENAME, &global.w, &global.h);
	global.temp_buff = global.work_buff;
	//Initialize rectangle corners
	reset_filter_area();
	if (global.save_buff == NULL)
	{
		printf("Error loading image file %s\n", FILENAME);
		return 1;
	}

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