#include "Base.h"

#include <iostream>
#include <chrono>
#include <time.h>
#include <stdarg.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
cv::Mat DEBUG::src;
cv::Mat DEBUG::src_c1;

int main()
{
	auto spCollision = std::make_shared<JPSCollision>();
	if (!spCollision)
		throw std::bad_alloc();

	//====================================================
	// CREATE MAP 
	//====================================================
	Int32 GridWidth = 300, GridHeight = 200;
//	Int32 GridWidth = 300, GridHeight = 300;
	spCollision->Create(GridWidth, GridHeight);

	srand(32);

	// Start, End Position (������, ������)
	Int32 Sx = 0, Sy = 0;
	Int32 Ex = GridWidth - 1, Ey = GridHeight - 1;

    DEBUG::src.create(GridHeight, GridWidth, CV_8UC3);
    DEBUG::src_c1.create(GridHeight, GridWidth, CV_8UC1);

	// Make a Maze (�浹 �̷θ� �����.)
	for (Int32 y = 0; y < GridHeight; ++y)
	{
		for (Int32 x = 0; x < GridWidth; ++x)
		{
			// Don't Mark Collision at Start, End Position (����,�������� �浹ó�� ���� �ʴ´�.)
			if (x == Sx && y == Sy)	continue;
			if (x == Ex && y == Ey) continue;

			// Randomly Mark Collision at Position (�����ϰ� �浹ó���� �Ѵ�.)
            if ((y + 1) % 50 == 0 and x != 100 and x != 99 and x != 98 and x != 199 and x != 200 and x != 201) spCollision->SetAt(x, y);
            if ((y + 1) % 50 == 0 and x != 100 and x != 99 and x != 98 and x != 199 and x != 200 and x != 201) spCollision->SetAt(x, y);
            if ((y + 1) % 50 == 0 and x != 100 and x != 99 and x != 98 and x != 199 and x != 200 and x != 201) {DEBUG::src.ptr<cv::Vec3b>(y)[x] = cv::Vec3b{90,90,90};DEBUG::src_c1.ptr(y)[x] = 1;}
            if ((y + 1) % 50 == 0 and x != 100 and x != 99 and x != 98 and x != 199 and x != 200 and x != 201) {DEBUG::src.ptr<cv::Vec3b>(y)[x] = cv::Vec3b{90,90,90};DEBUG::src_c1.ptr(y)[x] = 1;}
			if (rand() % 150 == 0) {
                spCollision->SetAt(x, y);
                DEBUG::src.ptr<cv::Vec3b>(y)[x] = cv::Vec3b{90,90,90};
                DEBUG::src_c1.ptr(y)[x] = 1;
            }
		}
	}

    namedWindow("src_c1",2);
    imshow("src_c1", DEBUG::src_c1*255);
    waitKey();

	//====================================================
	// Find PATH
	//====================================================
	std::list<JPSCoord> ResultNodes;	// Result for JPS

	JPSPath	jps;
	// SET
	jps.Init(spCollision);
	// SEARCH
    auto start = clock();
	jps.Search(Sx, Sy, Ex, Ey, ResultNodes);
    std::cout<<"time: "<<clock()-start<<std::endl;
	// STRAIGHT PATH 
	//jps.PullingString(ResultNodes);
	//====================================================
	// SAVE RESULT MAP TO FILE FOR DEBUG
	//====================================================
	std::string results(GridHeight * (GridWidth + 1) + 1, ' ');

	// Mark Collision With '@' ('@' �� �浹���� ǥ���մϴ�.)
	for (Int32 y = 0; y < GridHeight; y++)
	{
		for (Int32 x = 0; x < GridWidth; x++)
		{
			results[(GridHeight - 1 - y) * (GridWidth + 1) + x] = !spCollision->IsCollision(x, y) ? ' ' : '@';
		}
		results[(GridHeight - 1 - y) * (GridWidth + 1) + GridWidth] = '\n';
	}

    std::cout<<"ResultNodes.size() "<<ResultNodes.size() <<std::endl;

	// Mark Path Nodes With '#' ('#'�� ã�� ��θ� ǥ���մϴ�.)
	if (ResultNodes.size() > 0)
	{
		auto iterS = ResultNodes.begin();
		auto iterE = ResultNodes.end();
		auto iterP = iterS;	++iterS;
		for (; iterS != iterE; iterP = iterS, ++iterS)
		{
			auto& PreCoord = (*iterP);
			auto& CurCoord = (*iterS);

			Int32 x = PreCoord.m_x;
			Int32 y = PreCoord.m_y;
			Int32 dx = core::clamp<Int32>(CurCoord.m_x - PreCoord.m_x, -1, 1);
			Int32 dy = core::clamp<Int32>(CurCoord.m_y - PreCoord.m_y, -1, 1);

			for (Int32 v = y, u = x; ; v += dy, u += dx)
			{
				results[(GridHeight - 1 - v) * (GridWidth + 1) + u] = '#';
                DEBUG::src.ptr<cv::Vec3b>(y)[x] = {255,0,0};

				if (u == CurCoord.m_x && v == CurCoord.m_y)
					break;

				Int32 deltax = core::clamp<Int32>(CurCoord.m_x - u, -1, 1);
				Int32 deltay = core::clamp<Int32>(CurCoord.m_y - v, -1, 1);
				if (deltax != dx || deltay != dy)
				{
					std::cout << "INVALID NODE\n";
					break;
				}
			}
			results[(GridHeight - 1 - CurCoord.m_y) * (GridWidth + 1) + CurCoord.m_x] = '#';
		}

		// Mark Start & End Position ('S', 'E' �� ������ �������� ǥ���մϴ�.)
		auto	iterStart	= ResultNodes.begin();
		auto	iterEnd		= ResultNodes.rbegin();
		auto&	startCoord	= (*iterStart);
		auto&	endCoord	= (*iterEnd);
		results[(GridHeight - 1 - startCoord.m_y) * (GridWidth + 1) + startCoord.m_x] = 'S';
		results[(GridHeight - 1 - endCoord.m_y) * (GridWidth + 1) + endCoord.m_x]	  = 'E';
	}

    namedWindow("src",2);
    imshow("src", DEBUG::src);
    waitKey();


    //====================================================
	// SAVE FILE
	//====================================================
	if(results.size() > 0)
	{
		FILE* pFile = fopen("result_jps(b).txt", "wt");
		if (pFile != NULL)
		{
			fwrite(results.c_str(), sizeof(char), results.size(), pFile);
			fclose(pFile);
		}
	}
}
