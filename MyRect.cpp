#include "MyRect.h"

void MyRect::setRotatedCoords(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	real_coords = { std::pair{ round(x1),round(y1) },std::pair{ round(x2),round(y2) }, std::pair{ round(x3),round(y3) }, std::pair{ round(x4),round(y4) } };
	coord_x = real_coords[3].first;
	coord_y = real_coords[0].second;
	width = real_coords[1].first - real_coords[3].first;
	height = real_coords[2].second - real_coords[0].second;
	was_rotate = true;
}

points MyRect::getCoords()
{
	if (!was_rotate)
	{
		std::pair p1{ coord_x, coord_y };
		std::pair p2{ coord_x + width, coord_y };
		std::pair p3{ coord_x + width, coord_y + height };
		std::pair p4{ coord_x, coord_y + height };
		return std::make_tuple(p1, p2, p3, p4);
	}
	else 
		return std::make_tuple(real_coords[0], real_coords[1], real_coords[2], real_coords[3]);
}
