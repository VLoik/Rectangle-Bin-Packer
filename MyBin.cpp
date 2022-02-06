#include "MyBin.h"

bool MyBin::tryAddRectToLvls(std::shared_ptr<MyRect> &rect)
{
	//проверка на то, поместится ли прямоугольник с высотой rheight и ширинойr width
	auto checkRectToLvl = [width = width](const RectVector &level, int rwidth, int rheight)
	{
		auto &last_rect = level.back();
		auto &first_rect = level.front();
		//находим последнюю заполненную координату х на уровне
		int last_x = last_rect->getX() + last_rect->getWidth();
		/*
		прямоугольники, конечно, отсортированны, но проверить по высоте тоже нужно
		а все из-за того, что первый прямоугольник на уровне мог быть повернут на 90 градусов, если не влезал по высоте
		и хотя он должен быть "выше" по сортировке, его новая высота - это ширина
		поэтому на этот уровень добавим только если у нового прямоугольника высота не больше чем у первого на уровне
		*/
		if (width - last_x >= rwidth && first_rect->getHeight() >= rheight)
			return true;
		else
			return false;
	};

	//level - последний заполненный уровень, rwidth/rheight - ширина и высота прямоугольника
	auto checkNewLvl = [height = height, width = width](const RectVector &level, int rwidth, int rheight)
	{
		//смотрим на высоту первого прямоугольника последнего уровня
		auto &upper_rect = level.front();
		int last_y = upper_rect->getY() + upper_rect->getHeight();
		if (height - last_y >= rheight && width >= rwidth)
			return true;
		else
			return false;
	};


	//поиск угла, под которым прямоугольник максимально плотно войдет в другой прямоугольник
	auto find_optimal_angle = [](int w_outer, int h_outer, int w_rect, int h_rect)
	{
		//a - большая сторона большего прямоугольника, b - меньшая сторона большего прямоугольника
		//p - большая сторона меньшего прямоугольника, q - меньшая сторона меньшего прямоугольника
		int a{ w_outer > h_outer ? w_outer : h_outer };
		int b{ w_outer > h_outer ? h_outer : w_outer };
		int p{ w_rect > h_rect ? w_rect : h_rect };
		int q{ w_rect > h_rect ? h_rect : w_rect };

		//ищем корни уравнения (p^2 + q^2)t^2 -2apt + (a^2-q^2) = 0

		double c1 = p * p + q * q;
		double c2 = -a * p;
		double c3 = a * a - q * q;

		double best_root = 0;
		if (c2 * c2 - c3 > 0)
		{
			double t1 = (-c2 + sqrt(c2 * c2 - c1 * c3)) / c1;
			if (t1 >= 0 && t1 <= 1)
				best_root = t1;
			double t2 = (-c2 - sqrt(c2 * c2 - c1 * c3)) / c1;
			if (t2 >= 0 && t2 <= 1 && t2 < t1)
				best_root = t2;
		}
		else if (c2 * c2 - c1 * c3 == 0)
		{
			double t = -c2 / c1;
			if (t >= 0 && t <= 1)
				best_root = t;
		}

		//вернет 0, если подходящих корней не нашлось, но такого быть не должно, иначе отбросили бы прямоугольник на предыдущих этапах
		return acos(best_root);
	};

	auto put_rotated_rect_to_lvl = [find_optimal_angle, rect, height = height, width = width](int last_x, int lvl_y, int lvl_h)
	{
		double a = find_optimal_angle(width - last_x, lvl_h, rect->getWidth(), rect->getHeight());
		if (width - last_x < lvl_h)
		{
			rect->setRotatedCoords(last_x + rect->getHeight() * sin(a), lvl_y,
				last_x + rect->getHeight() * sin(a) + rect->getWidth() * cos(a), lvl_y + rect->getWidth() * sin(a),
				last_x + rect->getWidth() * cos(a), lvl_y + rect->getHeight() * cos(a) + rect->getWidth() * sin(a),
				last_x, lvl_y + rect->getHeight() * cos(a));
		}
		else
		{ //если свободное место на уровне ориентированно горизонтально, то координаты надо поменять местами
			rect->setRotatedCoords(last_x + rect->getHeight() * cos(a), lvl_y, 
				last_x + rect->getHeight() * cos(a) + rect->getWidth() * sin(a), lvl_y + rect->getWidth() * cos(a),
				last_x + rect->getWidth() * sin(a), lvl_y + rect->getHeight() * sin(a) + rect->getWidth() * cos(a),
				last_x, lvl_y + rect->getHeight() * sin(a)				
				);
		}

	};

	
	//эта проверка выполняется в вызывающей функции, но на всякий случай и тут проверим
	//мало ли кто и откуда ее дернет
	if (!checkRectToFit(width, height, rect->getWidth(), rect->getHeight()))
		return false;

	//первым делом нужно пройтись по уже заполненным уровням, если они есть
	for (auto &lvl : levels)
	{
		//находим последнюю заполненную координату х на уровне
		int last_x = lvl.back()->getX() + lvl.back()->getWidth();
		//проверяем влезает ли новый прямоугольник по ширине
		if (checkRectToLvl(lvl, rect->getWidth(), rect->getHeight()))
		{
			lvl.push_back(rect);
			addRect(rect, last_x, lvl.front()->getY());
			return true;
		}

		//если не влез так, проверяем влезет ли, если повернуть
		if (checkRectToLvl(lvl, rect->getHeight(), rect->getWidth()))
		{
			rect->rotate90();
			lvl.push_back(rect);
			addRect(rect, last_x, lvl.front()->getY());
			return true;
		}

		//проверим, влезет ли, если повернуть на произвольный угол
		if (checkRectToFit(width - last_x, lvl.front()->getHeight(), rect->getWidth(), rect->getHeight()))
		{
			put_rotated_rect_to_lvl(last_x, lvl.front()->getY(), lvl.front()->getHeight());
			lvl.push_back(rect);
			addRect(rect, last_x, lvl.front()->getY());
			return true;
		}

	}
	//в имеющиеся уже уровни не влез (или их нет), смотрим влезет ли новый уровень

	//Корзина была пустая, просто кладем
	if (levels.empty())
	{
		//Мы знаем, что по одной из сторон он точно влезет (иначе выкинуло бы раньше)
		//Контейнер всегда ориентирован вертикально
		if (rect->getHeight() < rect->getWidth())
			rect->rotate90();
		if (rect->getHeight() <= height)
		{
			//влез, добавляем
			RectVector new_lvl{ rect };
			levels.push_back(new_lvl);
			addRect(rect, 0, 0);
			return true;
		}
		else
		{//поместится точно должен, но нужно покрутить
			double a = find_optimal_angle(width, height, rect->getWidth(), rect->getHeight());
			rect->setRotatedCoords(rect->getHeight()* sin(a), 0,
									rect->getHeight()* sin(a) + rect->getWidth() * cos(a), rect->getWidth()* sin(a),
									rect->getWidth()* cos(a), rect->getHeight()* cos(a) + rect->getWidth() * sin(a),
									0, rect->getHeight()* cos(a));
						
			RectVector new_lvl{ rect };
			levels.push_back(new_lvl);
			addRect(rect, 0, 0);
			return true;
		}
	}
	//смотрим, можем ли добавить прямоугольник на новый уровень

	//последняя занятая координата по Y
	int last_y = levels.back().front()->getY() + levels.back().front()->getHeight();
	if (checkNewLvl(levels.back(), rect->getWidth(), rect->getHeight()))
	{
		//влез, добавляем
		RectVector new_lvl{ rect };
		levels.push_back(new_lvl);
		addRect(rect, 0, last_y);
		return true;
	}
	//просто так новый не влез, а если повернуть?
	if (checkNewLvl(levels.back(), rect->getHeight(), rect->getWidth()))
	{
		//влез, добавляем
		rect->rotate90();
		RectVector new_lvl{ rect };
		levels.push_back(new_lvl);
		addRect(rect, 0, last_y);
		return true;
	}

	//может влезет новый уровень, если повернуть?
	if (checkRectToFit(width, height - last_y, rect->getWidth(), rect->getHeight()))
	{
		RectVector new_lvl{ rect };
		levels.push_back(new_lvl);
		put_rotated_rect_to_lvl(0, last_y, height - last_y);
		addRect(rect, 0, last_y);
		return true;
	}

	//получается никуда не влез, пусть идет в другую корзину
	return false;
}

bool MyBin::checkRectToFit(int w_outer, int h_outer, int w_rect, int h_rect)
{
	//a - большая сторона большего прямоугольника, b - меньшая сторона большего прямоугольника
	//p - большая сторона меньшего прямоугольника, q - меньшая сторона меньшего прямоугольника
	int a{ w_outer > h_outer ? w_outer : h_outer };
	int b{ w_outer > h_outer ? h_outer : w_outer };
	int p{ w_rect > h_rect ? w_rect : h_rect };
	int q{ w_rect > h_rect ? h_rect : w_rect };

	if ((p <= a && q <= b) || (p > a &&
		b >= (2 * p * q * a + (p * p - q * q) * sqrt(p * p + q * q - a * a)) / (p * p + q * q)))
		return true;
	else
		return false;
}

void MyBin::addRect(std::shared_ptr<MyRect> &rect, int x, int y)
{
	rects.push_back(rect);
	rect->setX(x);
	rect->setY(y);
	free_area -= rect->getWidth() * rect->getHeight();
}
