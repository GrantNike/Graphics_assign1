/*
Grant Nike
Oct 8th
6349302
COSC 3P98 Assignment #1
Screen Saver Application
*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
//For Random
#include <cstdlib> 
//Used as parameter to random seed
#include <ctime> 

enum{MENU_QUIT};

typedef struct {
	GLuint x,y;
} point;

typedef struct {
	GLuint x,y;
} velocity;

//the global structure
typedef struct {
	point p1;
    point p2;
    int square_width = 100;
    int square_height = 100;
    velocity v;
} glob;
glob global;

void square() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);
	glVertex2i(global.p1.x, global.p1.y);
        glColor3f(0.0, 1.0, 0.0);
	glVertex2i(global.p1.x, global.p2.y);
        glColor3f(0.0, 0.0, 1.0);
	glVertex2i(global.p2.x, global.p2.y);
        glColor3f(1.0, 1.0, 1.0);
	glVertex2i(global.p2.x, global.p1.y);
    glEnd();
    glFlush();
}

void sleep(int ms) {
	clock_t target = clock() + ms;
	while (clock() < target) { }
}

void start_square(){
    global.p1.x = random()%(799-global.square_width);
    global.p1.y = random()%(799-global.square_height);
    global.p2.x = global.p1.x+global.square_width;
    global.p2.y = global.p1.y+global.square_height;
    global.v.x = (random()%(10))-5;
    global.v.y = (random()%(10))-5;
    square();
}

void animate(){
    if(global.p1.x + global.v.x >= 0){
        if(global.p2.x + global.v.x <= 799){
            global.p1.x += global.v.x;
            global.p2.x += global.v.x;
        }
    }
    else{
        global.v.x = (random()%(10))-5;
    }
    if(global.p1.y + global.v.y >= 0){
        if(global.p2.y + global.v.y <= 799){
            global.p1.y += global.v.y;
            global.p2.y += global.v.y;
        }
    }
    else{
        global.v.y = (random()%(10))-5;
    }
    sleep(10000);
    square();
}

//Defines what each menu function does
void menu_func(int value){
    switch(value){
        case MENU_QUIT:
            exit(0);
            break;
    }
}

//Creates menu for user input
void create_menu(){
    int main_menu = glutCreateMenu(&menu_func);
    glutAddMenuEntry("Quit", MENU_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//Glut keyboard function to handle keyboard inputs
void keyboard(unsigned char key, int x, int y){
    switch (key){
        case 0x1B:
        case 'q':
        case 'Q':
            exit(0);
            break;
        case 'r':
            start_square();
            break;
    }
}

void show_keys(){
	std::cout<<(random()%2)-1;
}

main(int argc, char **argv){
    //Create random seed for random number generation
	srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Screen Saver");
    glutFullScreen();

    start_square();
    glutDisplayFunc(animate);
    glutIdleFunc(animate);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 799.0, 0.0, 799.0); 
    glutKeyboardFunc(keyboard);
    create_menu();
    show_keys();

    glutMainLoop();
    return(0);
}