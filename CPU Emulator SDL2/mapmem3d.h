#pragma once

struct Size3D
{
	int x, y, z;
};

struct Pixeldata
{
	int yes = 1;
	bool reflect;
	char r, g, b;
	


};



struct Submap
{
	int testing = 123456;
	bool filled;
	Pixeldata fill;
	Size3D Size = {100,100,100};
	//Pixeldata* pixels[];
	Pixeldata** pixels = new Pixeldata * [Size.x * Size.y * Size.z];
};


class mapmem3d
{
public:

	Size3D Size = { 100, 100, 100 };
	//Submap* submaps[];
	Submap** submaps = new Submap * [Size.x * Size.y * Size.z];
	void init();
	Pixeldata* getPixel(double x, double y, double z);
	void setPixel(double x, double y, double z, Pixeldata* pixel);

};