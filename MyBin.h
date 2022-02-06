#pragma once
#include "MyRect.h"
#include <vector>
#include <memory>
#include <cmath>

//модель контейнера, в который будут загружаться прямоугольники

using RectVector = std::vector<std::shared_ptr<MyRect>>;

class MyBin
{
private:
	int width;
	int height;
	int free_area;

	RectVector rects;
	std::vector<RectVector> levels;
public:
	MyBin() = delete;
	MyBin(int w, int h) : width(w), height(h), free_area(w*h) {};
	
	inline int getFreeArea() const { return free_area; }

	//пытаемся добавить прямоугольник в корзину
	//Вернет true, если место нашлось, вернет false, если прямоугольник не поместился
	//!!! Предполагается, что прямоугольники уже отсортированы по убыванию высоты перед добавлением
	bool tryAddRectToLvls(std::shared_ptr<MyRect> &rect);

	inline const std::vector<std::shared_ptr<MyRect>> & getAllRects() { return rects; }

	//проверка на то, существует ли угол, при котором прямоугольник [w_rect, h_rect] можно уместить в [w_outer, h_outer]
	static bool checkRectToFit(int w_outer, int h_outer, int w_rect, int h_rect);

private:
	//добавляем прямоугольник в список, указываем координаты, пересчитываем все необходимое
	void addRect(std::shared_ptr<MyRect> &rect, int x, int y);
};

