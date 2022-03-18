#pragma once

struct Size3D
{
	int x = 0, y = 0, z = 0;
};


struct Vector3
{
	double x = 0, y = 0, z = 0;
	Vector3(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
};

struct Pixeldata
{
	int testing = 123;
	int yes = 1;
	bool reflect = false;
	bool def_normal = true;
	bool light = false;
	int r = 0, g = 0, b = 0, a = 0;
	double normal_x = 0, normal_y = 1, normal_z = 0;
};



struct Submap
{
	int testing = 123456;
	bool filled = false;
	Pixeldata fill;
	Size3D Size = { 100,100,100 };
	//Pixeldata* pixels[];
	Pixeldata** pixels = new Pixeldata * [Size.x * Size.y * Size.z]();
};


class mapmem3d
{
public:

	Size3D Size = { 100, 100, 100 };
	//Submap* submaps[];
	Submap** submaps = new Submap * [Size.x * Size.y * Size.z]();
	void init();
	Pixeldata* getPixel(double x, double y, double z);
	void setPixel(double x, double y, double z, Pixeldata* pixel);
	void setPixel(double x, double y, double z, Pixeldata* pixel, Vector3* scale);
};




struct Lightdata
{
	double r = 0, g = 0, b = 0;
	int amount = 0;
};



struct LightSubmap
{
	Size3D Size = { 100,100,100 };
	//Pixeldata* pixels[];
	Lightdata** pixels = new Lightdata * [Size.x * Size.y * Size.z]();
};


class Lightmapmem3d
{
public:

	Size3D Size = { 100, 100, 100 };
	//Submap* submaps[];
	LightSubmap** submaps = new LightSubmap * [Size.x * Size.y * Size.z]();
	void init();
	Lightdata* getLightPixel(double x, double y, double z);
	void setLightPixel(double x, double y, double z, Lightdata* pixel);
};