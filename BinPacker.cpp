#include "BinPacker.h"

BinPacker::BinPacker(int bwidth, int bheight)
{
	if (bwidth > bheight)
	{
		bin_height = bwidth;
		bin_width = bheight;
		was_rotated = true;
	}
	else
	{
		bin_height = bheight;
		bin_width = bwidth;
	}

	bins.push_back(MyBin(bin_width, bin_height));
}

void BinPacker::addRect(MyRect &rect)
{
	if (rect.getHeight() < rect.getWidth()) rect.rotate90();
	unplaced_rects.push_back(std::make_shared<MyRect>(rect));
}

void BinPacker::addRect(int w, int h)
{
	if (h < w) 
		unplaced_rects.push_back(std::make_shared<MyRect>(h,w));
	else
		unplaced_rects.push_back(std::make_shared<MyRect>(w, h));
}

int BinPacker::calc()
{
	int unpacked{ 0 };
	//начнем с сортировки прямоугольников по убыванию высоты, а затем ширины
	auto greaterH =  [](const std::shared_ptr<MyRect> &a, const std::shared_ptr<MyRect> &b)
	{
		if (a->getHeight() == b->getHeight())
			return a->getWidth() > b->getWidth();
		else
			return a->getHeight() > b->getHeight();
	};

	std::sort(unplaced_rects.begin(), unplaced_rects.end(), greaterH);

	auto minFreeArea =  [](const MyBin &a, const MyBin &b)
	{
		return a.getFreeArea() < b.getFreeArea();
	};

	for (auto rect : unplaced_rects)
	{
		//проверим, а влезет ли вообще
		if (!MyBin::checkRectToFit(bin_width, bin_height, rect->getWidth(), rect->getHeight()))
		{
			unpacked++;
			continue;
		}

		//будем пытаться сначала до конца заполнить контейнер в котором меньше всего места
		//поэтому сначала отсортируем
		std::sort(bins.begin(), bins.end(), minFreeArea);
		
		//пытаемся вставить в уже имеющиеся контейнеры
		bool succ{ false };
		auto it = bins.begin();
		while (!succ && it != bins.end())
		{
			succ = it->tryAddRectToLvls(rect);
			it++;
		}
		//если ни в одну не влезло, делаем новую
		if (!succ)
		{
			MyBin new_bin{ bin_width, bin_height };

			//в новую то точно должно влезть, но вдруг
			if (new_bin.tryAddRectToLvls(rect))
				bins.push_back(new_bin);
			else
				unpacked++;
		}
			

	}

	return unpacked;
}