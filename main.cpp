#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

// Функция изменения размеров окна с сохранением пропорций
void changeSize(int w, int h)
{
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

void renderScene(void)
{
    // очистить буфер цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // обнулить трансформацию
    glLoadIdentity();

    // Здесь основной алгоритм рисования

    // Обновить изображение
    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
    // Выход по Esc
    if (key == 27) {
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