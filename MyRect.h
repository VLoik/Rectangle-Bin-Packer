#pragma once
#include <tuple>
#include <utility>
#include <algorithm>
#include <array>

using points = std::tuple<std::pair<int, int>, std::pair<int, int>, std::pair<int, int>, std::pair<int, int>>;

//прямоугольник, который будем упаковывать
class MyRect
{
private:
	//ширина и высота прямоугольника
	int width;
	int height;

	//координаты верхнего левого угла прямоугольника (или описывающего прямоугольника)
	int coord_x{0};
	int coord_y{0};

	//координаты прямоугольника в том случае, если он повернут (на угол не кратный 90 градусов)
	std::array<std::pair<int, int>, 4> real_coords;
	bool was_rotate{false}; //был поворот, но не на 90 градусов

public:
	MyRect() = delete;
	MyRect(int w, int h) : width(w), height(h) {};

	inline void setX(int x) { coord_x = x; }
	inline void setY(int y) { coord_y = y; }	
	inline void setCoords(int x, int y) { coord_x = x; coord_y = y; }

	//поворот на 90 градусов - просто смена высоты и широты. Поворот на произвольный угол обрабатывается отдельно
	inline void rotate90() { std::swap(width, height); }

	//доступ к свойствам
	inline int getX() { return coord_x; }
	inline int getY() { return coord_y; }

	inline int getWidth() { return width; }
	inline int getHeight() { return height; }

	//начиная с самой нижней по часовой стрелке
	void setRotatedCoords(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

	//получаем все координаты по часовой стрелке, начиная с левого верхнего угла
	points getCoords();

};

