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
//For sleep 
#include <chrono>
#include <thread>

enum{MENU_SLOW, MENU_FAST, MENU_ENLARGE, MENU_SMALLER, MENU_ENLARGE_WIDTH, MENU_ENLARGE_HEIGHT, 
MENU_SMALLER_WIDTH, MENU_SMALLER_HEIGHT, MENU_ROTATE, MENU_RAND, MENU_RESET, MENU_QUIT};

typedef struct {
	GLfloat x,y;
} point;

typedef struct {
	GLfloat x,y;
} velocity;

typedef struct {
	GLfloat r, g, b;
} colour;

//the global structure
typedef struct {
	point p1;
    point p2;
    GLfloat screen_width = 1920.0;
    GLfloat screen_height = 1080.0;
    GLfloat square_width = 384;
    GLfloat square_height = 216;
    velocity v;
    float busy_sleep = 10;
    int twoTimes = 10;
    int range = 5;
    colour square_colours[4];
} glob;
glob global;

void rectangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glColor3f(global.square_colours[0].r, global.square_colours[0].g, global.square_colours[0].b);
	glVertex2i(global.p1.x, global.p1.y);
        glColor3f(global.square_colours[1].r, global.square_colours[1].g, global.square_colours[1].b);
	glVertex2i(global.p1.x, global.p2.y);
        glColor3f(global.square_colours[2].r, global.square_colours[2].g, global.square_colours[2].b);
	glVertex2i(global.p2.x, global.p2.y);
        glColor3f(global.square_colours[3].r, global.square_colours[3].g, global.square_colours[3].b);
	glVertex2i(global.p2.x, global.p1.y);
    glEnd();
    glutSwapBuffers();
}

void start_rect(){
    global.p1.x = random()%(int)(global.screen_width-global.square_width);
    global.p1.y = random()%(int)(global.screen_height-global.square_height);
    global.p2.x = global.p1.x+global.square_width;
    global.p2.y = global.p1.y+global.square_height;
    int rand;
    do{
        rand = (random()%(global.twoTimes))-global.range;
    }while(rand < 2 && rand > -2);
    global.v.x = rand;
    do{
        rand = (random()%(global.twoTimes))-global.range;
    }while(rand < 2 && rand > -2);
    global.v.y = rand;
    rectangle();
}

void animate(int value){
    int offset = 0;
    if(global.p1.x + global.v.x >= offset && global.p2.x + global.v.x <= global.screen_width){
        global.p1.x += global.v.x;
        global.p2.x += global.v.x;
    }
    else{
        int rand;
        do{
            rand = (random()%(global.twoTimes))-global.range;
        }while(rand < 2 && rand > -2);
        global.v.x = rand;
    }
    if(global.p1.y + global.v.y >= offset && global.p2.y + global.v.y <= global.screen_height){
        global.p1.y += global.v.y;
        global.p2.y += global.v.y;
    }
    else{
        int rand;
        do{
            rand = (random()%(global.twoTimes))-global.range;
        }while(rand < 2 && rand > -2);
        global.v.y = rand;
    }
    glutPostRedisplay();
    glutTimerFunc(global.busy_sleep, animate, 0);
    //std::this_thread::sleep_for(std::chrono::milliseconds(global.busy_sleep));
}

void random_colour(){
    for(int i=0;i<4;i++){
        global.square_colours[i].r = (rand()%1000)*0.001;
        global.square_colours[i].g = (rand()%1000)*0.001;
        global.square_colours[i].b = (rand()%1000)*0.001;
    }
}

void reset(){
    global.square_width = 384;
    global.square_height = 216;
    global.busy_sleep = 10;
    start_rect();
}

//Defines what each menu function does
void menu_func(int value){
    switch(value){
        case MENU_SLOW:
            global.busy_sleep = global.busy_sleep < 160? global.busy_sleep*2 : global.busy_sleep;
            break;
        case MENU_FAST:
            global.busy_sleep = global.busy_sleep > 1 ? global.busy_sleep/2 : 1;
            break;
        case MENU_ENLARGE:
            if(global.square_width < global.screen_width/2 && global.square_height < global.screen_height/2){
                global.square_width = global.square_width*1.5;
                global.square_height = global.square_height*1.5;
                start_rect();
            }
            break;
        case MENU_SMALLER:
            if(global.square_width > 30 && global.square_height > 30){
                global.square_width = global.square_width*(1/1.5);
                global.square_height = global.square_height*(1/1.5);
                start_rect();
            }
            break;
        case MENU_ENLARGE_WIDTH:
            if(global.square_width < global.screen_width/2) global.square_width = global.square_width*1.5;
            start_rect();
            break;
        case MENU_ENLARGE_HEIGHT:
            if(global.square_height < global.screen_height/2) global.square_height = global.square_height*1.5;
            start_rect();
            break;
        case MENU_SMALLER_WIDTH:
            if(global.square_width > 30) global.square_width = global.square_width*(1/1.5);
            start_rect();
            break;
        case MENU_SMALLER_HEIGHT:
            if(global.square_height > 30) global.square_height = global.square_height*(1/1.5);
            start_rect();
            break;
        case MENU_ROTATE:
            {
                int temp = global.square_width;
                global.square_width = global.square_height;
                global.square_height = temp;
                start_rect();
            }
            break;
        case MENU_RAND:
            random_colour();
            start_rect();
            break;
        case MENU_QUIT:
            exit(0);
            break;
        case MENU_RESET:
            reset();
            break;
    }
}

//Creates menu for user input
void create_menu(){
    int change_size = glutCreateMenu(&menu_func);
    glutAddMenuEntry("Enlarge Shape", MENU_ENLARGE);
    glutAddMenuEntry("Smaller Shape", MENU_SMALLER);
    glutAddMenuEntry("Enlarge Width", MENU_ENLARGE_WIDTH);
    glutAddMenuEntry("Enlarge Height", MENU_ENLARGE_HEIGHT);
    glutAddMenuEntry("Smaller Width", MENU_SMALLER_WIDTH);
    glutAddMenuEntry("Smaller Height", MENU_SMALLER_HEIGHT);

    int main_menu = glutCreateMenu(&menu_func);
    glutAddMenuEntry("Reset", MENU_RESET);
    glutAddMenuEntry("Slow Time", MENU_SLOW);
    glutAddMenuEntry("Speed Up Time", MENU_FAST);
    glutAddMenuEntry("Random Colour", MENU_RAND);
    glutAddSubMenu("Change Size", change_size);
    glutAddMenuEntry("Rotate", MENU_ROTATE);
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
        case 'R':
            reset();
            break;
        case 's':
        case 'S':
            global.busy_sleep = global.busy_sleep < 160? global.busy_sleep*2 : global.busy_sleep;
            break;
        case 'f':
        case 'F':
            global.busy_sleep = global.busy_sleep > 1 ? global.busy_sleep/2 : 1;
            break;
        case 'c':
        case 'C':
            random_colour();
            start_rect();
            break;
    }
}

void show_keys(){
	printf("r = Reset\n");
    printf("s = Slow Time\n");
    printf("f = Speed Up Time\n");
    printf("c = Random Colour\n");
    printf("q = Quit\n");
}

void init_colour(){
    global.square_colours[0].r = 1.0;
    global.square_colours[0].g = 0.0;
    global.square_colours[0].b = 0.0;

    global.square_colours[1].r = 0.0;
    global.square_colours[1].g = 1.0;
    global.square_colours[1].b = 0.0;

    global.square_colours[2].r = 0.0;
    global.square_colours[2].g = 0.0;
    global.square_colours[2].b = 1.0;

    global.square_colours[3].r = 1.0;
    global.square_colours[3].g = 1.0;
    global.square_colours[3].b = 1.0;
}

main(int argc, char **argv){
    random_colour();
    show_keys();
    //Create random seed for random number generation
	srand(time(NULL));
    glutInit(&argc, argv);
    glutInitWindowSize(1920/2,1080/2);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Screen Saver");
    //glutFullScreen();

    start_rect();
    glutDisplayFunc(rectangle);
    //glutIdleFunc(animate);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, global.screen_width, 0.0, global.screen_height); 
    glutKeyboardFunc(keyboard);
    create_menu();
    glutTimerFunc(global.busy_sleep, animate, 0);

    glutMainLoop();
    return(0);
}