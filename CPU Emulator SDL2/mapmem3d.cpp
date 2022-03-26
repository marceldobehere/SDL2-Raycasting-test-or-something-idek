#include "mapmem3d.h"
#define	_ISOC9X_SOURCE	1
#define _ISOC99_SOURCE	1

#include  <math.h>
using namespace std;
#include <iostream>

void mapmem3d::init()
{
	//submaps = new Submap*[100 * 100 * 100];

	for (int i = 0; i < 100 * 100 * 100; i++)
		submaps[i] = 0;


}

Pixeldata* mapmem3d::getPixel(long double x, long double y, long double z)
{
	//x += 0.0002;
	//y += 0.0002;
	//z += 0.0002;
	//printf("TEST5: %d\n", submaps[52 + (50 * 100) + (50 * 10000)]->testing);




	int m_x = int(x);
	int m_y = int(y);
	int m_z = int(z);

	if (m_x < 0 || m_x >= 100)
		return 0;
	if (m_y < 0 || m_y >= 100)
		return 0;
	if (m_z < 0 || m_z >= 100)
		return 0;

	if (submaps[m_x + (m_y * 100) + (m_z * 10000)] == 0)
		return 0;

	//int bruh = submaps[52 + (50 * 100) + (50 * 10000)]->testing;
	//int bruh1 = submaps[m_x + (m_y * 100) + (m_z * 10000)]->testing;

	Submap* submap = submaps[m_x + (m_y * 100) + (m_z * 10000)];


	if (submap->filled)
		return &submap->fill;

	int s_x = int(x * 100) % 100;
	int s_y = int(y * 100) % 100;
	int s_z = int(z * 100) % 100;

	//printf("%d %d %d, %d %d %d, (%d) %d %d\n", m_x, m_y, m_z, s_x, s_y, s_z, submap->testing, submap->filled, submaps[m_x + (m_y * 100) + (m_z * 10000)]);


	if (submap->testing != 123456)
	{
		printf(" - ERROR!\n");
		return 0;
	}

	//else
		//printf(" - OK\n");

	Pixeldata* temp = submap->pixels[s_x + (s_y * 100) + (s_z * 10000)];


	return temp;
}

void mapmem3d::setPixel(long double x, long double y, long double z, Pixeldata* pixel)
{
	//x += 0.0002;
	//y += 0.0002;
	//z += 0.0002;

	int m_x = int(x);
	int m_y = int(y);
	int m_z = int(z);

	if (m_x < 0 || m_x >= 100)
		return;
	if (m_y < 0 || m_y >= 100)
		return;
	if (m_z < 0 || m_z >= 100)
		return;

	if (submaps[m_x + (m_y * 100) + (m_z * 10000)] == 0)
	{
		Submap* testing = new Submap;
		submaps[m_x + (m_y * 100) + (m_z * 10000)] = testing;
		testing->filled = false;

		for (int i = 0; i < 100 * 100 * 100; i++)
			testing->pixels[i] = 0;
	}

	Submap* submap = (Submap*)submaps[m_x + (m_y * 100) + (m_z * 10000)];


	if (submap->filled)
	{
		if (&submap->fill == pixel)
			return;
		else
		{
			submap->filled = false;

			for (int i = 0; i < 100 * 100 * 100; i++)
				submap->pixels[i] = &submap->fill;
		}
	}

	int s_x = int(x * 100) % 100;
	int s_y = int(y * 100) % 100;
	int s_z = int(z * 100) % 100;

	submap->pixels[s_x + (s_y * 100) + (s_z * 10000)] = pixel;
}


void mapmem3d::setPixel(long double x_, long double y_, long double z_, Pixeldata* pixel, Vector3* scale)
{
	for (int y = 0; y < scale->y; y++)
		for (int z = 0; z < scale->z; z++)
			for (int x = 0; x < scale->x; x++)
				mapmem3d::setPixel(x_ + (x / 100.0) + 0.002f, y_ + (y / 100.0) + 0.002f, z_ + (z / 100.0) + 0.002f, pixel);
}















void Lightmapmem3d::init()
{
	//submaps = new Submap*[100 * 100 * 100];

	for (int i = 0; i < 100 * 100 * 100; i++)
		submaps[i] = 0;


}





Lightdata* Lightmapmem3d::getLightPixel(long double x, long double y, long double z)
{
	x += 0.0002;
	y += 0.0002;
	z += 0.0002;

	//printf("TEST5: %d\n", submaps[52 + (50 * 100) + (50 * 10000)]->testing);

	int m_x = int(x);
	int m_y = int(y);
	int m_z = int(z);

	if (m_x < 0 || m_x >= 100)
		return 0;
	if (m_y < 0 || m_y >= 100)
		return 0;
	if (m_z < 0 || m_z >= 100)
		return 0;

	if (submaps[m_x + (m_y * 100) + (m_z * 10000)] == 0)
	{
		LightSubmap* testing = new LightSubmap;
		submaps[m_x + (m_y * 100) + (m_z * 10000)] = testing;

		for (int i = 0; i < 100 * 100 * 100; i++)
			testing->pixels[i] = 0;
	}

	//int bruh = submaps[52 + (50 * 100) + (50 * 10000)]->testing;
	//int bruh1 = submaps[m_x + (m_y * 100) + (m_z * 10000)]->testing;

	LightSubmap* submap = submaps[m_x + (m_y * 100) + (m_z * 10000)];


	int s_x = int(x * 100) % 100;
	int s_y = int(y * 100) % 100;
	int s_z = int(z * 100) % 100;

	//printf("%d %d %d, %d %d %d, (%d) %d %d\n", m_x, m_y, m_z, s_x, s_y, s_z, submap->testing, submap->filled, submaps[m_x + (m_y * 100) + (m_z * 10000)]);


	//else
		//printf(" - OK\n");

	Lightdata* temp = submap->pixels[s_x + (s_y * 100) + (s_z * 10000)];

	if (temp == 0)
	{
		temp = new Lightdata;
		submap->pixels[s_x + (s_y * 100) + (s_z * 10000)] = temp;
	}

	return temp;
}






void Lightmapmem3d::setLightPixel(long double x, long double y, long double z, Lightdata* pixel)
{
	x += 0.0002;
	y += 0.0002;
	z += 0.0002;


	int m_x = int(x);
	int m_y = int(y);
	int m_z = int(z);

	if (m_x < 0 || m_x >= 100)
		return;
	if (m_y < 0 || m_y >= 100)
		return;
	if (m_z < 0 || m_z >= 100)
		return;

	if (submaps[m_x + (m_y * 100) + (m_z * 10000)] == 0)
	{
		LightSubmap* testing = new LightSubmap;
		submaps[m_x + (m_y * 100) + (m_z * 10000)] = testing;

		for (int i = 0; i < 100 * 100 * 100; i++)
			testing->pixels[i] = 0;
	}

	LightSubmap* submap = (LightSubmap*)submaps[m_x + (m_y * 100) + (m_z * 10000)];


	int s_x = int(x * 100) % 100;
	int s_y = int(y * 100) % 100;
	int s_z = int(z * 100) % 100;

	submap->pixels[s_x + (s_y * 100) + (s_z * 10000)] = pixel;
}

