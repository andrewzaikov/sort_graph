#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <glut.h>
#include "graphics.h"
#endif

double red = 0.0f;
double green = 0.0f;
double blue = 0.0f;
double step = 0.05f;

void shiftColors() {
    
}

void renderBalls() {

    shiftColors();

    glPushMatrix();                 // сохраняем текущие координаты 

    glTranslated(0, 0.7f, 0);       // сдвигаемся по оси Y на 0.7
    glColor3d(1.0f, 0, 0); 
    glutSolidSphere(0.4, 16, 16);   // рисуем сферу в (1.4,0,0) в абсолютных координатах 
    
    glPopMatrix();  
}