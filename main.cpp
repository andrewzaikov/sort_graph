#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

double red = 0.0f;
double green = 0.0f;
double blue = 0.0f;
double step = 0.05f;

int width, height;

// Для удобства задания позиции
struct Position {
    double x;
    double y;
    double z;

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

    Color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

// Цветовые константы
const Color RED = Color(1.0f, 0, 0);
const Color BLACK = Color(0, 0, 0);
const Color GREEN = Color(0, 1.0f, 0);
const Color BLUE = Color(0, 0, 1.0f);
const Color WHITE = Color(1.0f, 1.0f, 1.0f);
const Color GREY = Color(0.7f, 0.7f, 0.7f);


void shiftColors()
{
}

void renderCircleAt(Position pos, double rad, Color clr) {
    glPushMatrix(); // сохраняем текущие координаты

    glTranslated(pos.x, pos.y, pos.z);
    glColor3d(clr.r, clr.g, clr.b);
    glutSolidSphere(rad, 32, 32);

    glPopMatrix();
}

void renderCircles()
{
    shiftColors();

    renderCircleAt(Position(-1.0f, 1.0f, -5.0f), 0.2f, RED);
    renderCircleAt(Position(1.0f, 1.0f, -5.0f), 0.2f, GREEN);
    renderCircleAt(Position(-1.0f, -1.0f, -5.0f), 0.2f, BLUE);
    renderCircleAt(Position(1.0f, -1.0f, -5.0f), 0.2f, WHITE);
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
	//переключения режима проецирования
	glMatrixMode(GL_PROJECTION);
	//Сохраняем предыдущую матрицу, которая содержит
        //параметры перспективной проекции
	glPushMatrix();
	//обнуляем матрицу
	glLoadIdentity();
	//устанавливаем 2D ортогональную проекцию
	gluOrtho2D(0, width, height, 0);
	// возврата в режим обзора модели
	glMatrixMode(GL_MODELVIEW);
}

// Вернуться в перспективную проекцию
void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	//восстановить предыдущую матрицу проекции
	glPopMatrix();
	//вернуться в режим модели
	glMatrixMode(GL_MODELVIEW);
}

// Вывести надпись 
void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {
 
	char *c;
	glRasterPos3f(x, y, z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}


// Функция создания изображения
void renderScene(void)
{
    // очистить буфер цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // обнулить трансформацию
    glLoadIdentity();

    // камера
    gluLookAt(0, 0, 3.0f,
              0, 0, 0,
              0, 1.0f, 0);

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
    setOrthographicProjection();
	    glPushMatrix();
	        glLoadIdentity();
	        renderBitmapString(20, 30, 0, GLUT_BITMAP_HELVETICA_18, "Some text");
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
        exit(0);
    }
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

    // Главный цикл
    glutMainLoop();

    return 0;
}