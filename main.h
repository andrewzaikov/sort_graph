#include <stdio.h>
#pragma once

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
    Position start;
    Position stop;

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
    Position pos;
    Color clr;
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