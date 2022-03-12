#include "mapmem3d.h"
#include <math.h>
using namespace std;
#include <iostream>

void mapmem3d::init()
{
	//submaps = new Submap*[100 * 100 * 100];

	


}


Pixeldata* mapmem3d::getPixel(double x, double y, double z)
{

	//printf("TEST5: %d\n", submaps[52 + (50 * 100) + (50 * 10000)]->testing);

	int m_x = floor(x);
	int m_y = floor(y);
	int m_z = floor(z);

	if (m_x < 0 || m_x >= 100)
		return 0;
	if (m_y < 0 || m_y >= 100)
		return 0;
	if (m_z < 0 || m_z >= 100)
		return 0;

	if (submaps[m_x + (m_y * 100) + (m_z * 10000)] == 0)
		return 0;

	int bruh = submaps[52 + (50 * 100) + (50 * 10000)]->testing;
	int bruh1 = submaps[m_x + (m_y * 100) + (m_z * 10000)]->testing;

	Submap* submap = submaps[m_x + (m_y * 100) + (m_z * 10000)];


	if (submap->filled)
		return &submap->fill;

	int s_x = (int)round(x * 100) % 100;
	int s_y = (int)round(y * 100) % 100;
	int s_z = (int)round(z * 100) % 100;

	

	//printf("%d %d %d, %d %d %d, 123 (%d) %d %d, (%d) %d %d\n", m_x, m_y, m_z, s_x, s_y, s_z, 123, bruh, bruh1, submap->testing, submap->filled, submaps[m_x + (m_y * 100) + (m_z * 10000)]);
	

	return (Pixeldata*)submap->pixels[s_x + (s_y * 100) + (s_z * 10000)];
}

void mapmem3d::setPixel(double x, double y, double z, Pixeldata* pixel)
{
	int m_x = floor(x);
	int m_y = floor(y);
	int m_z = floor(z);

	int s_x = (int)round(x * 100) % 100;
	int s_y = (int)round(y * 100) % 100;
	int s_z = (int)round(z * 100) % 100;

	if (m_x < 0 || m_x >= 100)
		return;
	if (m_y < 0 || m_y >= 100)
		return;
	if (m_z < 0 || m_z >= 100)
		return;

	if (submaps[m_x + (m_y * 100) + (m_z * 10000)] == 0)
		return;

	Submap* submap = (Submap*)submaps[m_x + (m_y * 100) + (m_z * 10000)];


	if (submap->filled)
	{
		if (&submap->fill == pixel)
			return;
		else
		{
			submap->filled = false;

			for (int i = 0; i < 100*100*100; i++)
				submap->pixels[i] = &submap->fill;
		}
	}

	submap->pixels[s_x + (s_y * 100) + (s_z * 10000)] = pixel;
}