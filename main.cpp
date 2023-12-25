#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

// идентификаторы меню
int menu;

// угол поворота камеры
float angle=0.0;
// координаты вектора направления движения камеры
float lx=0.0f, lz=-1.0f;
// XZ позиция камеры
float x=0.0f, z=5.0f;
// Движение камеры. Переменные инициализируются нулевыми значениями когда клавиши не нажаты
float deltaAngle = 0.0f;
float deltaMove = 0;

int width, height;

char statusText[100] = {'\0'};

// Для удобства задания позиции
struct Position {
    double x;
    double y;
    double z;

    Position(){}

    Position(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

// Для удобства задания цвета
struct Color {
    double r;
    double g;
    double b;

    Color(){}

    Color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

// Шарик
struct Circle {
    Position pos = Position();
    Color clr = Color();
    double r;
    char name[5];

    Circle(){}

    Circle(Position pos, double r, Color clr, int num) {
        this->pos = pos;
        this->clr = clr;
        this->r = r;
        sprintf(this->name, "%d", num);
    }
};


// Цветовые константы
const Color BLACK = Color(0, 0, 0);
const Color RED = Color(1.0f, 0, 0);
const Color GREEN = Color(0, 1.0f, 0);
const Color BLUE = Color(0, 0, 1.0f);
const Color WHITE = Color(1.0f, 1.0f, 1.0f);
const Color GREY = Color(0.7f, 0.7f, 0.7f);
const Color DARK = Color(0.4f, 0.4f, 0.4f);
const Color YELLOW = Color(1, 1, 0);
const Color PINK = Color(1, 0, 1);
const Color SKY = Color(0, 1, 1);
const Color ORANGE = Color(1, 0.75f, 0);

// массив с цветами
const Color colors[] = {RED, ORANGE, YELLOW, GREEN, SKY, BLUE, PINK, WHITE, GREY, DARK};
Circle circles[10];

// Нарисовать надпись
void renderStrokeFontString(Position pos, void *font, char *string) {  
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (char *c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

void renderCircleAt(Position pos, double rad, Color clr) {
    glPushMatrix(); // сохраняем текущие координаты

    glTranslated(pos.x, pos.y, pos.z);
    glColor3d(clr.r, clr.g, clr.b);
    glutSolidSphere(rad, 32, 32);

    glPopMatrix();
}

void renderCircle(Circle circle) {
    renderCircleAt(circle.pos, circle.r, circle.clr);
    Position capPos = Position(circle.pos.x-0.06*strlen(circle.name), circle.pos.y+circle.r+0.02f, circle.pos.z);
    renderStrokeFontString(capPos, GLUT_STROKE_ROMAN, circle.name);
}

void renderCircles()
{

    for (int i=0; i<10; i++) {
        circles[i] = Circle(Position(-8.0f+1.5f*i, 1.0f, -5.0f), 0.1f+0.05f*i, colors[i], i);
        renderCircle(circles[i]);
    }
}

// Функция изменения размеров окна с сохранением пропорций
void changeSize(int w, int h)
{
    width = w;
    height = h;

    // предотвращение деления на ноль
    if (h == 0)
    {
        h = 1;
    }
    float ratio = w * 1.0 / h;

    // используем матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // обнуляем матрицу
    glLoadIdentity();

    // установить параметры вьюпорта
    glViewport(0, 0, w, h);

    // установить корректную перспективу
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // вернуться к матрице проекции
    glMatrixMode(GL_MODELVIEW);
}

// Перейти в ортогональную проекцию
void setOrthographicProjection() {
	// переключения режима проецирования
	glMatrixMode(GL_PROJECTION);

	// Сохраняем предыдущую матрицу, которая содержит параметры перспективной проекции
	glPushMatrix();

	// обнуляем матрицу
	glLoadIdentity();

	// устанавливаем 2D ортогональную проекцию
	gluOrtho2D(0, width, height, 0);

	// возврата в режим обзора модели
	glMatrixMode(GL_MODELVIEW);
}

// Вернуться в перспективную проекцию
void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);

	// восстановить предыдущую матрицу проекции
	glPopMatrix();

	// вернуться в режим модели
	glMatrixMode(GL_MODELVIEW);
}

// Вывести надпись 
void renderBitmapString(Position pos, void *font, char *string) {
	glRasterPos3f(pos.x, pos.y, pos.z);

	for (char *c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void computePos(float deltaMove) 
{
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}
 
void computeDir(float deltaAngle)
{
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

// Функция создания изображения
void renderScene()
{
	if (deltaMove) {
		computePos(deltaMove);
    }
	if (deltaAngle) {
		computeDir(deltaAngle);
    }

    // очистить буфер цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // обнулить трансформацию
    glLoadIdentity();

    // камера
 	gluLookAt(	   x,	1.0f,	  z,
				x+lx,	1.0f,  z+lz,
				0.0f,   1.0f,  0.0f		);

    // поверхность
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
        glVertex3f(-50.0f, -15.0f, -50.0f);
        glVertex3f(-50.0f, -15.0f,  50.0f);
        glVertex3f( 50.0f, -15.0f,  50.0f);
        glVertex3f( 50.0f, -15.0f, -50.0f);
    glEnd();    

    // Здесь основной алгоритм рисования
    renderCircles();

    // Нарисовать текст (русские буквы не работают)
    glColor3f(0.8f, 0.8f, 0.8f);
    setOrthographicProjection();
	    glPushMatrix();
	        glLoadIdentity();
	        renderBitmapString(Position(20, 30, 0), GLUT_BITMAP_HELVETICA_18, statusText);
	    glPopMatrix();
	restorePerspectiveProjection();

    // Обновить изображение
    glutSwapBuffers();
}

// Функция обработки нажатия клавиш
void processNormalKeys(unsigned char key, int x, int y)
{
    // Выход по Esc
    if (key == 27)
    {
        glutDestroyMenu(menu);
        exit(0);
    }
}

void pressKey(int key, int x, int y) {
 
	switch (key) {
		case GLUT_KEY_LEFT:
			deltaAngle = -0.01f;
			break;
		case GLUT_KEY_RIGHT:
			deltaAngle = 0.01f;
			break;
		case GLUT_KEY_UP:
			deltaMove = 0.5f;
			break;
		case GLUT_KEY_DOWN:
			deltaMove = -0.5f;
			break;
	}
}
 
void releaseKey(int key, int x, int y) {
 
	switch (key) {
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: 
			deltaAngle = 0.0f;
			break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN: 
			deltaMove = 0.0f;
			break;
	}
}

void processMenu(int option) {
 
	switch (option) {
		case 1: // randomize
            strcpy(statusText, "Randomize array");
			break;
		case 2: // insertion
            strcpy(statusText, "Insertion sort");
			break;
		case 3: // bubble
            strcpy(statusText, "Bubble sort");
			break;
	}
}

void createPopupMenus() {
 
	menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Randomize array",1 );
	glutAddMenuEntry("Insertion sort",2 );
	glutAddMenuEntry("Bubble sort",3 );

	// прикрепить меню к правой кнопке
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
    // Инициализация GLUT и создание окна
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Визуализация сортировки");

    // Функции рисования
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // Функции работы с клавиатурой
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(processNormalKeys);
	glutIdleFunc(renderScene);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

    // Создать меню
    createPopupMenus();

    // Главный цикл
    glutMainLoop();

    return 0;
}