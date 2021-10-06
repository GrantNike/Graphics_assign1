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

enum{MENU_SLOW,MENU_RESET,MENU_QUIT};

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
    GLuint square_width = 100;
    GLuint square_height = 100;
    velocity v;
    int busy_sleep = 10000;
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
    glutSwapBuffers();
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
    int rand;
    do{
        rand = (random()%(10))-5;
    }while(rand == 0);
    global.v.x = rand;
    do{
        rand = (random()%(10))-5;
    }while(rand == 0);
    global.v.y = rand;
    square();
}

void animate(){
    int offset = 5;
    if(global.p1.x + global.v.x >= offset && global.p2.x + global.v.x <= 799){
        global.p1.x += global.v.x;
        global.p2.x += global.v.x;
    }
    else{
        int rand;
        do{
            rand = (random()%(10))-5;
        }while(rand == 0);
        global.v.x = rand;
    }
    if(global.p1.y + global.v.y >= offset && global.p2.y + global.v.y <= 799){
        global.p1.y += global.v.y;
        global.p2.y += global.v.y;
    }
    else{
        int rand;
        do{
            rand = (random()%(10))-5;
        }while(rand == 0);
        global.v.y = rand;
    }
    square();
    sleep(global.busy_sleep);
}

//Defines what each menu function does
void menu_func(int value){
    switch(value){
        case MENU_SLOW:
            global.busy_sleep = global.busy_sleep < 160000? global.busy_sleep*2 : global.busy_sleep;
            break;
        case MENU_QUIT:
            exit(0);
            break;
        case MENU_RESET:
            global.busy_sleep = 10000;
            start_square();
            break;
    }
}

//Creates menu for user input
void create_menu(){
    int main_menu = glutCreateMenu(&menu_func);
    glutAddMenuEntry("Reset", MENU_RESET);
    glutAddMenuEntry("Slow Time", MENU_SLOW);
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
            global.busy_sleep = 10000;
            start_square();
            break;
        case 's':
            global.busy_sleep = global.busy_sleep < 160000? global.busy_sleep*2 : global.busy_sleep;
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