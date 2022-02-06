#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "BinPacker.h"
#include <chrono> 

int main()
{
	auto first = std::chrono::high_resolution_clock::now();

	//cчитываем размер контейнеров
	std::string dim_input;
	std::getline(std::cin, dim_input);
	char separator;
	std::stringstream ss(dim_input);
	int w{ 0 };
	int h{ 0 };
	ss >> w >> separator >> h;
	//сразу инициализируем класс-решатель
	BinPacker packer(w,h);

	//считываем контейнеры
	for (std::string rect_string; std::getline(std::cin, rect_string) && !rect_string.empty();)
	{
		ss = std::stringstream(rect_string);
		int rw{ 0 };
		int rh{0};
		ss >> rw >> separator>> rh;

		packer.addRect(rw, rh);
	}

	//расчет упаковки
	int unplaced = packer.calc();

	//вывод в нужном формате
	//для расчета контейнеры поворачиваются вертикально, поэтому для вывода в том виде,
	//в котором ширина и высота контейнера задавалась, перед выводом нужно поменять местами координаты прямоугольников
	//укладка будет отзеркалена
	auto bins = packer.getBins();
	std::cout << bins.size() << "," << w << "," << h << std::endl;
	int bin_num{ 0 };
	for (auto bin : bins)
	{
		for (auto rect : bin.getAllRects())
		{
			auto[p1, p2, p3, p4] = rect->getCoords();
			if(!packer.rotateBeforeOutput())
				std::cout << bin_num << "," << p1.first << "," << p1.second << "," << p2.first << "," << p2.second << "," <<
				p3.first << "," << p3.second << "," << p4.first << "," << p4.second << std::endl;
			else
				std::cout << bin_num << "," << p1.second << "," << p1.first << "," << p2.second << "," << p2.first << "," <<
				p3.second << "," << p3.first << "," << p4.second << "," << p4.first << std::endl;
		}
		bin_num++;
	}
	auto second = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = second - first;

	//уже после выполнения основной программы залоггируем все
	std::ofstream file{ "log.txt" };
	if (file)
	{
		file << "Number of bin: " << bins.size() << std::endl;
		file << "Number of unplaced rectangles: " << unplaced<< std::endl;
		file << "Execution time: " << elapsed.count() << std::endl;
	}


    
}

