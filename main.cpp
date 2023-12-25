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

// Для движения и поворота камеры
//  угол поворота камеры
float angle = 0.0;
//  координаты вектора направления движения камеры
float lx = 0.0f, lz = -1.0f;
//  XZ позиция камеры
float x = 0.0f, z = 5.0f;
//  Движение камеры. Переменные инициализируются нулевыми значениями когда клавиши не нажаты
float deltaAngle = 0.0f;
float deltaMove = 0;

// Отладочная надпись - текст
char statusText[100] = {'\0'};
// Размеры окна
int width, height;

// Массив шариков заполнен?
bool arrayFilled = false;

// Структура (класс) - положение
struct Position
{
    double x;
    double y;
    double z;

    Position() {}

    Position(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

// Структура (класс) - движение
struct Move
{
    int item;
    Position start = Position();
    Position stop = Position();

    Move() {}

    Move(int item, Position start, Position stop)
    {
        this->item = item;
        this->start = start;
        this->stop = stop;
    }
};

// Структура (класс) - цвет
struct Color
{
    double r;
    double g;
    double b;

    Color() {}

    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

// Структура (класс) - Шарик
struct Circle
{
    Position pos = Position();
    Color clr = Color();
    double r;
    char name[5];

    Circle() {}

    Circle(Position pos, double r, Color clr, int num)
    {
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

// массив с шариками
Circle circles[10];

// массив с перемещениями
Move moves[100];

// количество перемещений
int moveCount = -1;

// Дельта по высоте
double dH = 2.0f;

// Число шагов для анимации
double nSteps = 50;

// перемещение по X, Y
double deltaX = 0.0f, deltaY = 0.0f;

// Нарисовать подпись
void renderStrokeFontString(Position pos, void *font, char *string)
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(0.002f, 0.002f, 0.002f);
    for (char *c = string; *c != '\0'; c++)
    {
        glutStrokeCharacter(font, *c);
    }
    glPopMatrix();
}

// Нарисовать шарик без подписи
void renderCircleAt(Position pos, double rad, Color clr)
{
    glPushMatrix(); // сохраняем текущие координаты

    glTranslated(pos.x, pos.y, pos.z);
    glColor3d(clr.r, clr.g, clr.b);
    glutSolidSphere(rad, 32, 32);

    glPopMatrix();
}

// Нарисовать шарик с подписью
void renderCircle(Circle circle)
{
    renderCircleAt(circle.pos, circle.r, circle.clr);
    Position capPos = Position(circle.pos.x - 0.06 * strlen(circle.name), circle.pos.y + circle.r + 0.02f, circle.pos.z);
    renderStrokeFontString(capPos, GLUT_STROKE_ROMAN, circle.name);
}

// Нарисовать 10 шариков
void renderCircles()
{
    for (int i = 0; i < 10; i++)
    {
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

// Отладочная надпись - перейти в ортогональную проекцию
void setOrthographicProjection()
{
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

// Отладочная надпись - вернуться в перспективную проекцию
void restorePerspectiveProjection()
{
    glMatrixMode(GL_PROJECTION);

    // восстановить предыдущую матрицу проекции
    glPopMatrix();

    // вернуться в режим модели
    glMatrixMode(GL_MODELVIEW);
}

// Отладочная надпись - вывести надпись
void renderBitmapString(Position pos, void *font, char *string)
{
    glRasterPos3f(pos.x, pos.y, pos.z);

    for (char *c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

// для движения камеры - перемещение
void computePos(float deltaMove)
{
    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

// для движения камеры - поворот
void computeDir(float deltaAngle)
{
    angle += deltaAngle;
    lx = sin(angle);
    lz = -cos(angle);
}

// Заполнить массив с шариками случайными значениями
void randomizeArray()
{
    int k;

    for (int i = 0; i < 10; i++)
    {
        k = random() % 10;
        circles[i] = Circle(Position(-8.0f + 1.5f * i, 1.0f, -5.0f), 0.1f + 0.05f * k, colors[i], 10 * k + i);
    }
}

// Для теста: анимация, изменить 0 и 3 элементы
void swapElements(int l, int r)
{
    // записать в обратном порядке
    Position lpos = circles[l].pos;
    Position rpos = circles[r].pos;

    // левую на место правой
    moves[++moveCount] = Move(l, Position((lpos.x + rpos.x) / 2.0f, lpos.y - dH, lpos.z), rpos);
    // правую на место левой
    moves[++moveCount] = Move(r, Position((lpos.x + rpos.x) / 2.0f, rpos.y + dH, rpos.z), lpos);
    // левую на середину внизу
    moves[++moveCount] = Move(l, lpos, Position((lpos.x + rpos.x) / 2.0f, lpos.y - dH, lpos.z));
    // правую на середину вверху
    moves[++moveCount] = Move(r, rpos, Position((lpos.x + rpos.x) / 2.0f, rpos.y + dH, rpos.z));
}

// Рассчитать следующее движение
void computeNextMove()
{
    if (moveCount < 0)
    {
        return;
    }

    Move move = moves[moveCount];
    printf("move.item = %d, moveCount = %d\n", move.item, moveCount);
    if (deltaX == 0 && deltaY == 0)
    {
        deltaX = (move.stop.x - move.start.x) / nSteps;
        deltaY = (move.stop.y - move.start.y) / nSteps;

        printf("circles[move.item].pos.x = %f, circles[move.item].pos.y = %f\n", circles[move.item].pos.x, circles[move.item].pos.y);
        printf("move.stop.x = %f, move.start.x = %f\n", move.stop.x, move.start.x);
        printf("move.stop.y = %f, move.start.y = %f\n", move.stop.y, move.start.y);
        printf("deltaX = %f, deltaY = %f\n", deltaX, deltaY);
    }

    Position pos = circles[move.item].pos;
    printf("current pos.x = %f, pos.y = %f, deltaX = %f, deltaY = %f\n", pos.x, pos.y, deltaX, deltaY);

    pos.x += deltaX;
    if (abs(pos.x - move.stop.x) <= abs(deltaX))
    {
        pos.x = move.stop.x;
        deltaX = 0;
    }

    pos.y += deltaY;
    if (abs(pos.y - move.stop.y) <= abs(deltaY))
    {
        pos.y = move.stop.y;
        deltaY = 0;
    }
    printf("new pos.x = %f, pos.y = %f, dx = %f, dy = %f\n", pos.x, pos.y, abs(pos.x - move.stop.x), abs(pos.y - move.stop.y));
    circles[move.item].pos = pos;

    if (deltaX == 0 && deltaY == 0)
    {
        moveCount--;
    }
}

// Функция создания изображения
void renderScene()
{
    if (deltaMove)
    {
        computePos(deltaMove);
    }
    if (deltaAngle)
    {
        computeDir(deltaAngle);
    }

    if (!arrayFilled)
    {
        arrayFilled = true;
        srand(glutGet(GLUT_ELAPSED_TIME));
        randomizeArray();
    }

    computeNextMove();

    // очистить буфер цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // обнулить трансформацию
    glLoadIdentity();

    // камера
    gluLookAt(x, 1.0f, z,
              x + lx, 1.0f, z + lz,
              0.0f, 1.0f, 0.0f);

    // поверхность
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, -15.0f, -50.0f);
    glVertex3f(-50.0f, -15.0f, 50.0f);
    glVertex3f(50.0f, -15.0f, 50.0f);
    glVertex3f(50.0f, -15.0f, -50.0f);
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

// для движения камеры - нажатие кнопки
void pressKey(int key, int x, int y)
{

    switch (key)
    {
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

// для движения камеры - отпустить кнопку
void releaseKey(int key, int x, int y)
{

    switch (key)
    {
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

void processMenu(int option)
{

    switch (option)
    {
    case 1: // randomize
        strcpy(statusText, "Randomize array");
        randomizeArray();
        break;
    case 2: // insertion
        strcpy(statusText, "Insertion sort");
        break;
    case 3: // bubble
        strcpy(statusText, "Bubble sort");
        break;
    case 4: // Swap 0 and 3
        strcpy(statusText, "Swap 0 and 3");
        swapElements(0, 3);
        break;
    }
}

void createPopupMenus()
{

    menu = glutCreateMenu(processMenu);
    glutAddMenuEntry("Randomize array", 1);
    glutAddMenuEntry("Insertion sort", 2);
    glutAddMenuEntry("Bubble sort", 3);
    glutAddMenuEntry("Move 0 and 3", 4);

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

    // для движения камеры
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);

    // Создать меню
    createPopupMenus();

    // Главный цикл
    glutMainLoop();

    return 0;
}