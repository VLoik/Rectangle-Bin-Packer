#pragma once
#include "MyRect.h"
#include "MyBin.h"

#include <vector>
#include <memory>

//решатель задачи размещения
class BinPacker
{
private:
	//ширина и высота каждого контейнера
	int bin_width;
	int bin_height;

	//количество контейнеров
	int bin_count{1};

	//пул еще не размещенных прямоугольников
	std::vector<std::shared_ptr<MyRect>> unplaced_rects;
	//контейнеры, в которые размещаем прямоугольники
	std::vector<MyBin> bins;

	bool was_rotated{ false };

public:
	BinPacker() = delete;
	BinPacker(int bwidth, int bheight);

	//добавление прямоугольника в пул неразмещенных прямоугольников
	//перед добавлением поворачивает прямоугольники для вертикальной ориентации
	void addRect(MyRect &rect); 
	void addRect(int w, int h);

	//запуск расчета размещения прямоугольников по контейнерам
	//возвращает количество прямоугольников, которые никуда не влезли
	int calc();

	inline  const std::vector<MyBin>& getBins() { return bins; };

	//если мы вращали контейнеры при размещении прямоугольников, для правильного отображения в html нужно повернуть все обратно при выводе
	inline bool rotateBeforeOutput() { return was_rotated; }
};

