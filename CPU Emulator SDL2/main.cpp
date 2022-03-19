using namespace std;


// TODO
// ADD Distorting angle depending on volume/density whatever
// ADD RENDERING OPTIMIZATIONS
// PROFIT




#include <SDL.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
//#include "CPU.h"
#include <string>
//#include "IO.h"
#include <math.h>
#include "mapmem3d.h"
#include "other/vec3.h"
#include "old/IO.h"
#include <tuple>
#define PI 3.14159265358979323846
#define pi_a 0.017453292519943295

bool debug_out = true;



struct Point
{
	Vector3 pos, rot;
};



double ray_step_divider = 500;


int rgbtoint(int r, int g, int b)
{
	return ((r % 256) * 256 * 256) + ((g % 256) * 256) + (b % 256);
}

int* rgbinttointarray(int rgb)
{
	int intarray[3] = {};
	intarray[2] = rgb % 256;
	intarray[1] = (rgb / 256) % 256;
	intarray[0] = ((rgb / 256) / 256) % 256;

	return intarray;
}

void log_debug(const char a[], char sign)
{
	if (debug_out)
	{
		printf("%c", sign);
		printf(" ");
		printf(a);
		printf("\n");
	}
}
void log(const char a[], char sign)
{
	printf("%c", sign);
	printf(" ");
	printf(a);
	printf("\n");
}






void getRotatedVec(Point* point, bool normalize, bool mult)
{
	//long double ray_move_x = 0;
	//long double ray_move_y = 0;
	//long double ray_move_z = 0;


	//ray_move_x += sin(point->rot.x * pi_a);
	//ray_move_y += sin(point->rot.y * pi_a);
	//ray_move_z += sin(point->rot.z * pi_a);

	//ray_move_z += cos(point->rot.x * pi_a);
	//ray_move_z += cos(point->rot.y * pi_a);
	//ray_move_y += cos(point->rot.z * pi_a);

	long double rot_x = point->rot.x * pi_a;
	long double rot_y = point->rot.y * pi_a;


	long double ray_move_x = -cos(rot_x) * sin(rot_y);
	long double ray_move_y = sin(rot_x);
	long double ray_move_z = -cos(rot_x) * cos(rot_y);


	//ray_move_x += sin(point->rot.x * pi_a);
	//ray_move_z += cos(point->rot.x * pi_a);

	//ray_move_x += sin(point->rot.y * pi_a);
	//ray_move_y += cos(point->rot.y * pi_a);

	//ray_move_y += sin(point->rot.z * pi_a);
	//ray_move_z += cos(point->rot.z * pi_a);

	//ray_move_y += (cos(point->rot.x * pi_a) - sin(point->rot.x * pi_a));
	//ray_move_z += (cos(point->rot.x * pi_a) + sin(point->rot.x * pi_a));

	//ray_move_z += (cos(point->rot.y * pi_a) - sin(point->rot.y * pi_a));
	//ray_move_x += (cos(point->rot.y * pi_a) + sin(point->rot.y * pi_a));

	//ray_move_x += (cos(point->rot.z * pi_a) - sin(point->rot.z * pi_a));
	//ray_move_y += (cos(point->rot.z * pi_a) + sin(point->rot.z * pi_a));

	


	if (normalize)
	{
		long double mod = 0.0;

		mod += ray_move_x * ray_move_x;
		mod += ray_move_y * ray_move_y;
		mod += ray_move_z * ray_move_z;

		if (mod != 0) 
		{
			long double mag = std::sqrt(mod);
			ray_move_x /= mag;
			ray_move_y /= mag;
			ray_move_z /= mag;
		}
	}

	if (mult)
	{
		point->pos.x *= ray_move_x;
		point->pos.y *= ray_move_y;
		point->pos.z *= ray_move_z;
	}
	else
	{
		point->pos.x = ray_move_x;
		point->pos.y = ray_move_y;
		point->pos.z = ray_move_z;
	}

}





void LoadObj(const char* filename, mapmem3d* MEM_MAP, Vector3* pos, Vector3* scale)
{
	tuple<int, char*> testing = ReadFile(filename);

	int size = get<0>(testing);
	char* data = get<1>(testing);

	cout << "OBJ FILE: " << data[0] << data[1] << data[2] << data[3] << "\n";
	cout << "OBJ DATA: " << data[4] << data[5] << data[6] << data[7] << "\n";

	int pixelsize = ConvertCharPointerToInt(&data[12]);
	int uniquepixelcount = ConvertCharPointerToInt(&data[8]);

	cout << "OBJ PIXEL SIZE: " << pixelsize << "\n";
	cout << "OBJ UNIQUE PIXEL COUNT: " << uniquepixelcount << "\n";

	Pixeldata** pixels = new Pixeldata*[uniquepixelcount];
	int addr = 16;
	for (int i = 0; i < uniquepixelcount; i ++)
	{
		pixels[i] = new Pixeldata;
		pixels[i]->r = ConvertCharPointerToShort(&data[addr + 0]);
		pixels[i]->g = ConvertCharPointerToShort(&data[addr + 2]);
		pixels[i]->b = ConvertCharPointerToShort(&data[addr + 4]);
		pixels[i]->a = ConvertCharPointerToShort(&data[addr + 6]);

		//cout << " - TEMP: " << ConvertCharPointerToShort(&data[addr + 8]) << "\n";

		if (ConvertCharPointerToShort(&data[addr + 8]) == 1)
			pixels[i]->reflect = true;
		else
			pixels[i]->reflect = false;

		if (ConvertCharPointerToShort(&data[addr + 10]) == 1)
			pixels[i]->def_normal = true;
		else
			pixels[i]->def_normal = false;

		if (ConvertCharPointerToShort(&data[addr + 12]) == 1)
			pixels[i]->light = true;
		else
			pixels[i]->light = false;

		if (ConvertCharPointerToShort(&data[addr + 14]) == 1)
			pixels[i]->transparent = true;
		else
			pixels[i]->transparent = false;

		
		pixels[i]->normal_x = ConvertCharPointerToDouble(&data[addr + 16 + (0 * 8)]);
		pixels[i]->normal_y = ConvertCharPointerToDouble(&data[addr + 16 + (1 * 8)]);
		pixels[i]->normal_z = ConvertCharPointerToDouble(&data[addr + 16 + (2 * 8)]);
		pixels[i]->distortion = ConvertCharPointerToDouble(&data[addr + 16 + (3 * 8)]);

		//cout << "NORMS: " << pixels[i]->normal_x << ", " << pixels[i]->normal_y << ", " << pixels[i]->normal_z << ".\n";


		addr += pixelsize * 2;
	}


	int size_x = ConvertCharPointerToInt(&data[addr + 0]);
	int size_y = ConvertCharPointerToInt(&data[addr + 4]);
	int size_z = ConvertCharPointerToInt(&data[addr + 8]);

	addr += 4 * 3;

	cout << "Dimensions: " << size_x << "x" << size_y << "x" << size_z << "\n";

	pixels[0] = 0;



	for (int y = 0; y < size_y; y++)
	{
		for (int z = 0; z < size_z; z++)
		{
			for (int x = 0; x < size_x; x++)
			{
				if (data[4] == 'I')
				{
					Pixeldata* temp_pixl = pixels[ConvertCharPointerToInt(&data[addr])];
					if (temp_pixl != 0)
						MEM_MAP->setPixel((pos->x + (x / 100.0 * scale->x)), (pos->y + (y / 100.0 * scale->y)), (pos->z + (z / 100.0 * scale->z)), temp_pixl, scale);

					addr += 4;
				}
				else
				{
					Pixeldata* temp_pixl = pixels[ConvertCharPointerToShort(&data[addr])];
					if (temp_pixl != 0)
						MEM_MAP->setPixel((pos->x + (x / 100.0 * scale->x)), (pos->y + (y / 100.0 * scale->y)), (pos->z + (z / 100.0 * scale->z)), temp_pixl, scale);

					addr += 2;
				}
			}
		}
	}


}


struct obj
{
	Vector3 pos, scale;
	string filename;
};



void LoadScene(const char* filename, mapmem3d* MEM_MAP)
{
	cout << "Loading File: \"" << filename << "\".\n";
	ifstream input(filename);

	for (string line; getline(input, line); )
	{
		if (line == "#")
		{
			obj a;

			getline(input, line);
			a.pos.x = stold(line);
			getline(input, line);
			a.pos.y = stold(line);
			getline(input, line);
			a.pos.z = stold(line);

			getline(input, line);

			getline(input, line);
			a.scale.x = stold(line);
			getline(input, line);
			a.scale.y = stold(line);
			getline(input, line);
			a.scale.z = stold(line);

			getline(input, line);

			getline(input, line);
			a.filename = line;

			LoadObj(a.filename.c_str(), MEM_MAP, &a.pos, &a.scale);

		}
	}
}





vec3 getNormal(Pixeldata* currentPixel, Vector3 pos, Vector3 mov)
{
	vec3 point_a = vec3(pos.x - 2 * mov.x, pos.y - 2 * mov.y, pos.z - 2 * mov.z);
	vec3 point_center = vec3(
		((int)(pos.x * 100)) / 100.0 + 0.005,
		((int)(pos.y * 100)) / 100.0 + 0.005,
		((int)(pos.z * 100)) / 100.0 + 0.005
	);
	vec3 temp = point_center - point_a;


	if (currentPixel->def_normal)
	{
		//cout << "TEST 1: " << currentPixel->reflect << ".\n";
		//cout << "TEST 2: " << qaaa->reflect << ".\n";

		//pixl = qaaa;
		//break;


		Vector3 temp_1;

		{
			long double abs_x = abs(temp.x());
			long double abs_y = abs(temp.y());
			long double abs_z = abs(temp.z());

			long double maximum = max(abs_x, max(abs_y, abs_z));
			if (abs_x == maximum)
			{
				//cout << "<x>\n";
				temp_1.x = copysign(1, temp.x());
				temp_1.y = 0;
				temp_1.z = 0;
			}
			else if (abs_y == maximum)
			{
				//cout << "<y>\n";
				temp_1.x = 0;
				temp_1.y = copysign(1, temp.y());
				temp_1.z = 0;
			}
			else
			{
				//cout << "<z>\n";
				temp_1.x = 0;
				temp_1.y = 0;
				temp_1.z = copysign(1, temp.z());
			}
		}


		return vec3(temp_1.x, temp_1.y, temp_1.z);
	}

	
	return vec3(currentPixel->normal_x, currentPixel->normal_y, currentPixel->normal_z);
}


void refract_2(Pixeldata* pixel, vec3* tempvec3, Vector3 pos, Vector3 mov, double n1, double n2, bool flip)
{
	if (n1 != n2)
	{
		vec3 normal = getNormal(pixel, pos, mov);
		normal *= -1;
		*tempvec3 = refract(unit_vector(*tempvec3), unit_vector(normal), n2 / n1);
	}
}


void calcLightPixel(mapmem3d* MEM_MAP, Lightmapmem3d* Light_MEM_MAP, Pixeldata* data, int index, int subindex, int* lightcount)
{

	(*lightcount)++;
	cout << " - Light " << *lightcount << "   \r";
	Size3D size = MEM_MAP->Size;

	Size3D size2 = MEM_MAP->submaps[index]->Size;

	Vector3 Position;
	Position.x = ((index) % size.x); 
	Position.y = ((index / size.x) % size.y);
	Position.z = ((index / (size.x * size.y)) % size.z);

	Position.x += ((subindex) % size2.x) / 100.0;
	Position.y += ((subindex / size2.x) % size2.y) / 100.0;
	Position.z += ((subindex / (size2.x * size2.y)) % size2.z) / 100.0;


	{
		Lightdata* temp_l = new Lightdata;
		Light_MEM_MAP->setLightPixel(Position.x, Position.y, Position.z, temp_l);
	}



	int hits = 0;

	long double res = 2;
	vec3 tempvec3;
	Point ray_;
	for (long double w1 = 0; w1 < 360; w1 += res)
	{
		for (long double w2 = -90; w2 < 90; w2 += res)
		{
			Vector3 light = Vector3(data->r / 255.0, data->g / 255.0, data->b / 255.0);
			//light = Vector3(1, 1, 1);
			{
				Point mov_;
				mov_.rot.x = w2;
				mov_.rot.y = w1;
				mov_.rot.z = 0;
				getRotatedVec(&mov_, false, false);

				tempvec3 = vec3(mov_.pos.x, mov_.pos.y, mov_.pos.z);
			}

			ray_.pos.x = Position.x;
			ray_.pos.y = Position.y;
			ray_.pos.z = Position.z;

			Vector3 mov, pos;
			pos = Vector3(ray_.pos.x, ray_.pos.y, ray_.pos.z);
			mov = Vector3(tempvec3.x() / ray_step_divider, tempvec3.y() / ray_step_divider, tempvec3.z() / ray_step_divider);


			Pixeldata* currentPixel = data;
			Pixeldata* oldPixel = 0;
			double distortion = data->distortion;

			int dis = 3000;
			while (dis > 0 && (light.x + light.y + light.z) > 0.05)
			{
				pos.x += mov.x;
				pos.y += mov.y;
				pos.z += mov.z;
				//cout << " - Light: " << *lightcount << " POS: " << pos.x << ", " << pos.y << ", " << pos.z << "DIS:  " << (600-dis) << " HITS:" << hits << "                 \r";

				oldPixel = currentPixel;
				currentPixel = MEM_MAP->getPixel(pos.x, pos.y, pos.z);


				if (currentPixel != 0)
				{
					if (!currentPixel->light)
					{
						if (currentPixel->transparent)
						{
							if (distortion != currentPixel->distortion)
							{
								//vec3 normal = getNormal(currentPixel, pos, mov);
								//tempvec3 = refract_2(tempvec3, normal, distortion, currentPixel->distortion);
								//
								refract_2(currentPixel, &tempvec3, pos, mov, distortion, currentPixel->distortion, false);
								distortion = currentPixel->distortion;


								mov.x = tempvec3.x() / ray_step_divider;
								mov.y = tempvec3.y() / ray_step_divider;
								mov.z = tempvec3.z() / ray_step_divider;
							}
							

							long double rgb_a = (currentPixel->a / 255.0);
							light.x *= (currentPixel->r * rgb_a / 255.0);
							light.y *= (currentPixel->g * rgb_a / 255.0);
							light.z *= (currentPixel->b * rgb_a / 255.0);
						}
						else
						{
							hits++;

							{
								vec3 normal = getNormal(currentPixel, pos, mov);
								tempvec3 = reflect(tempvec3, normal);
								mov = Vector3(tempvec3.x() / ray_step_divider, tempvec3.y() / ray_step_divider, tempvec3.z() / ray_step_divider);
							}



							//long double temp_a = currentPixel->a / 255.0;

							//light.x *= (currentPixel->r / 255.0) * temp_a;
							//light.y *= (currentPixel->g / 255.0) * temp_a;
							//light.z *= (currentPixel->b / 255.0) * temp_a;




							{
								Lightdata* temp_light = Light_MEM_MAP->getLightPixel(pos.x, pos.y, pos.z);

								temp_light->r += light.x;
								temp_light->g += light.y;
								temp_light->b += light.z;
								temp_light->amount++;
							}


							long double rgb_a = (currentPixel->a / 255.0);
							light.x *= (currentPixel->r * rgb_a / 255.0);
							light.y *= (currentPixel->g * rgb_a / 255.0);
							light.z *= (currentPixel->b * rgb_a / 255.0);

							light.x *= 0.95;
							light.y *= 0.95;
							light.z *= 0.95;

							/*if (currentPixel->reflect)
							{

							}
							else
							{
								light.x *= (currentPixel->light_level.x / 255.0);
								light.y *= (currentPixel->light_level.y / 255.0);
								light.z *= (currentPixel->light_level.z / 255.0);
							}*/




							//currentPixel->r = 255;



							//break;
						}
						
					}
					else
					{
						//return;
					}


				}
				else
				{
					if (distortion != 1)
					{
						refract_2(oldPixel, &tempvec3, pos, mov, distortion, 1, true);
						distortion = 1;

						mov.x = tempvec3.x() / ray_step_divider;
						mov.y = tempvec3.y() / ray_step_divider;
						mov.z = tempvec3.z() / ray_step_divider;
					}
				}

				//pos.x += mov.x;
				//pos.y += mov.y;
				//pos.z += mov.z;

				light.x *= 0.999;
				light.y *= 0.999;
				light.z *= 0.999;

				dis--;
			}


		}
	}

}


void calcLight(mapmem3d* MEM_MAP, Lightmapmem3d* Light_MEM_MAP)
{
	cout << "Calculating Lights...\n";

	int size = MEM_MAP->Size.x * MEM_MAP->Size.y * MEM_MAP->Size.z;

	Submap** submap = MEM_MAP->submaps;

	int count = 0;

	for (int i = 0; i < size; i++)
	{
		if (submap[i] == 0)
			continue;
		if (submap[i]->filled)
		{
			if (submap[i]->fill.light)
			{
				Pixeldata* pixl = &submap[i]->fill;
				for (int i2 = 0; i2 < size; i2++)
					calcLightPixel(MEM_MAP, Light_MEM_MAP, pixl, i, i2, &count);
			}
		}
		else
		{
			Pixeldata** pixels = submap[i]->pixels;
			for (int i2 = 0; i2 < size; i2++)
			{
				if (pixels[i2] == 0)
					continue;

				Pixeldata* pixl = pixels[i2];

				if (pixl->light)
					calcLightPixel(MEM_MAP, Light_MEM_MAP, pixl, i, i2, &count);
			}
		}
	}














	cout << "\nCalculated Lights!\n";
}








int main(int argc, char** argv)
{

	



	const int width = 1280, height = 720;

	//const int width = 512, height = 288;

	log_debug("Opening Window...", '>');

	//------------------------------------------------------------------------------------------------------------------------------------------------

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow
	(
		"Masls Raytracer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN
	);

	SDL_Renderer* renderer = SDL_CreateRenderer
	(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);
	//std::cout << "Renderer name: " << info.name << std::endl;
	//std::cout << "Texture formats: " << std::endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		//std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	}
	 
	const unsigned int texWidth = width;
	const unsigned int texHeight = height;
	SDL_Texture* texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		texWidth, texHeight
	);

	std::vector< unsigned char > pixels(texWidth * texHeight * 4, 0);

	SDL_Event event;
	bool running = true;
	bool useLocktexture = false;

	unsigned int frames = 0;
	Uint64 start = SDL_GetPerformanceCounter();

 

	log_debug("Done.", '<'); 


	//main_test();


	Point playerPosition;
	playerPosition.pos.x = 52;
	playerPosition.pos.y = 52.5;
	playerPosition.pos.z = 49;
	playerPosition.rot.x = 0;
	playerPosition.rot.y = -160;
	playerPosition.rot.z = 5;

	long double fov = 90;
	long double fov_2 = 2;
	int mode = 0;

	mapmem3d MEM_MAP;
	MEM_MAP.init();
	Lightmapmem3d Light_MEM_MAP;
	Light_MEM_MAP.init();



	/*for (int i = 0; i < 100 * 100 * 100; i++)
		MEM_MAP.submaps[i] = 0;*/



	//cout << "TEST3: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->testing << "\n";

	MEM_MAP.getPixel(0, 0, 0);

	//V_CPU_INIT();




	//MEM_MAP.getPixel(0, 0, 0);


	
		Pixeldata* aaa = (new Pixeldata);
		aaa->r = 255;
		aaa->g = 5;
		aaa->b = 10;
		aaa->a = 200;
		aaa->reflect = false;
		aaa->def_normal = true;

		Submap* xaa = (new Submap);
		xaa->filled = true;
		xaa->fill = *aaa;

		cout << "TEST1: " << xaa->testing << "\n";

		MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)] = xaa;

		cout << "TEST2: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->testing << "\n";

		Pixeldata* qaaa = (new Pixeldata);
		qaaa->r = 10;
		qaaa->g = 15;
		qaaa->b = 244;
		qaaa->a = 200;
		qaaa->reflect = false;
		qaaa->def_normal = true;

		Submap ax = *(new Submap);
		ax.filled = false;
		ax.fill = *qaaa;

		for (int i = 0; i < 10000; i += 1)
		{
			ax.pixels[i + 0] = qaaa;
		}

		for (int i = 10000; i < 100000; i += 2)
		{
			ax.pixels[i + 0] = qaaa;
			ax.pixels[i + 1] = aaa;
		}

		for (int i = 100000; i < 1000000; i += 3)
		{
			ax.pixels[i + 2] = 0;
		}



		MEM_MAP.submaps[52 + (51 * 100) + (50 * 10000)] = &ax;
	



	cout << "TEST A: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->fill.r << "\n";
	cout << "TEST B: " << MEM_MAP.submaps[52 + (51 * 100) + (50 * 10000)]->fill.r << "\n";





	{
		for (int movz = 0; movz < 10; movz++)
		{
			for (int movy = 0; movy < 256; movy++)
			{
				for (int movx = 0; movx < 256; movx++)
				{
					Pixeldata* temp_pixl = (new Pixeldata);
					temp_pixl->r = 255;// movx;
					temp_pixl->g = 255;//movy;
					temp_pixl->b = 255;//movz;
					temp_pixl->a = 200;
					temp_pixl->reflect = false;
					temp_pixl->def_normal = true;
					temp_pixl->reflect = true;

					MEM_MAP.setPixel(50 + (movx * 0.01), 50 + (movy * 0.01), 51 + (movz * 0.01), temp_pixl);

				}
			}
		}


		for (int movz = -50; movz < 51; movz++)
		{
			for (int movy = -50; movy < 51; movy++)
			{
				for (int movx = -50; movx < 51; movx++)
				{
					Pixeldata* temp_pixl = (new Pixeldata);
					temp_pixl->r = (movx + 50) * 2;
					temp_pixl->g = (movy + 50) * 2;
					temp_pixl->b = (movz + 50) * 2;
					temp_pixl->a = 200;
					temp_pixl->reflect = false;
					temp_pixl->def_normal = true;

					int oof = ((movx * movx) + (movy * movy) + (movz * movz));
					if (oof < 50 * 50 && oof > 46 * 46)
						MEM_MAP.setPixel(51 + (movx * 0.01), 54 + (movy * 0.01), 51 + (movz * 0.01), temp_pixl);

				}
			}
		}


	}



	//Vector3 scale;
	//scale.x = 4;
	//scale.y = 4;
	//scale.z = 4;
	//Vector3 pos_test;
	//pos_test.x = 48;
	//pos_test.y = 52;
	//pos_test.z = 50;
	//LoadObj("objs\\mountain.png.mrof", &MEM_MAP, &pos_test, &scale);

	//pos_test.x = 49;
	//pos_test.y = 52;
	//pos_test.z = 50;
	//scale.x = 1;
	//scale.y = 1;
	//scale.z = 1;
	//LoadObj("objs\\testing.png.mrof", &MEM_MAP, &pos_test, &scale);

	////slices.png.mrof

	//pos_test.x = 52;
	//pos_test.y = 53;
	//pos_test.z = 50;
	//scale.x = 5;
	//scale.y = 8;
	//scale.z = 5;
	//LoadObj("objs\\out.png.mrof", &MEM_MAP, &pos_test, &scale);

	{
		string bruh;
		bruh = "Masls Raytracer - Res: ";
		bruh += to_string((int)(width / fov_2));
		bruh += "x";
		bruh += to_string((int)(height / fov_2));
		SDL_SetWindowTitle(window, bruh.c_str());
	}


	LoadScene("scenes\\test-scene.mrsf", &MEM_MAP);



	calcLight(&MEM_MAP, &Light_MEM_MAP);






	Point* temp_move = new Point;
	Point* temp_move_2 = new Point;




	bool stop = false;
	int CPU_TICK_COUNTER = 0;
	static const Uint8* KEYS;
	while (running && !stop)
	{
		CPU_TICK_COUNTER++;
		//V_CPU_TICK();

		SDL_PumpEvents();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		temp_move->pos.x = 0;
		temp_move->pos.y = 0;
		temp_move->pos.z = 0;

		while (SDL_PollEvent(&event))
		{
			if ((SDL_QUIT == event.type) || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
			{
				running = false;
				break;
			}
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_L == event.key.keysym.scancode)
			{
				useLocktexture = !useLocktexture;
				std::cout << "Using " << (useLocktexture ? "SDL_LockTexture() + memcpy()" : "SDL_UpdateTexture()") << std::endl;
			}

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_W == event.key.keysym.scancode)
				temp_move->pos.z += 0.25;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_S == event.key.keysym.scancode)
				temp_move->pos.z -= 0.25;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_A == event.key.keysym.scancode)
				temp_move->pos.x += 0.25;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_D == event.key.keysym.scancode)
				temp_move->pos.x -= 0.25;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_Q == event.key.keysym.scancode)
				temp_move->pos.y -= 0.25;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_E == event.key.keysym.scancode)
				temp_move->pos.y += 0.25;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_DOWN == event.key.keysym.scancode)
				playerPosition.rot.x -= 5;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_UP == event.key.keysym.scancode)
				playerPosition.rot.x += 5;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
				playerPosition.rot.y += 5;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
				playerPosition.rot.y -= 5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_R == event.key.keysym.scancode)
				playerPosition.rot.z += 2.5;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_F == event.key.keysym.scancode)
				playerPosition.rot.z -= 2.5;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_U == event.key.keysym.scancode)
			{
				MEM_MAP.submaps[52 + (51 * 100) + (50 * 10000)] = 0;
				MEM_MAP.submaps[53 + (51 * 100) + (50 * 10000)] = &ax;
			}

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_I == event.key.keysym.scancode)
			{
				MEM_MAP.submaps[52 + (51 * 100) + (50 * 10000)] = &ax;
				MEM_MAP.submaps[53 + (51 * 100) + (50 * 10000)] = 0;
			}
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_O == event.key.keysym.scancode)
				qaaa->g = (qaaa->g + 10) % 256;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_T == event.key.keysym.scancode)
				fov += 10;
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_G == event.key.keysym.scancode)
				fov -= 10;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_B == event.key.keysym.scancode)
			{
				fov_2 += 1;
				string bruh;
				bruh = "Masls Raytracer - Res: ";
				bruh += to_string((int)(width / fov_2));
				bruh += "x";
				bruh += to_string((int)(height / fov_2));
				SDL_SetWindowTitle(window, bruh.c_str());
			}
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_N == event.key.keysym.scancode)
			{
				if (fov_2 > 1)
					fov_2 -= 1;
				string bruh;
				bruh = "Masls Raytracer - Res: ";
				bruh += to_string((int)(width / fov_2));
				bruh += "x";
				bruh += to_string((int)(height / fov_2));
				SDL_SetWindowTitle(window, bruh.c_str());
			}
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_M == event.key.keysym.scancode)
				mode = (mode + 1) % 8;
		}


		if (temp_move->pos.x != 0 || temp_move->pos.y != 0 || temp_move->pos.z != 0)
		{
			temp_move_2->rot.x = playerPosition.rot.x;
			temp_move_2->rot.y = playerPosition.rot.y;
			temp_move_2->rot.z = playerPosition.rot.z;

			getRotatedVec(temp_move_2, false, false);


			playerPosition.pos.x += temp_move_2->pos.x * temp_move->pos.z;
			playerPosition.pos.y += temp_move_2->pos.y * temp_move->pos.z;
			playerPosition.pos.z += temp_move_2->pos.z * temp_move->pos.z;


			temp_move_2->rot.x += 90;
			getRotatedVec(temp_move_2, false, false);

			playerPosition.pos.x += temp_move_2->pos.x * temp_move->pos.y;
			playerPosition.pos.y += temp_move_2->pos.y * temp_move->pos.y;
			playerPosition.pos.z += temp_move_2->pos.z * temp_move->pos.y;


			temp_move_2->rot.x -= 90;
			//temp_move_2->rot.z += 90;
			temp_move_2->rot.y += 90;
			getRotatedVec(temp_move_2, false, false);

			playerPosition.pos.x += temp_move_2->pos.x * temp_move->pos.x;
			playerPosition.pos.y += temp_move_2->pos.y * temp_move->pos.x;
			playerPosition.pos.z += temp_move_2->pos.z * temp_move->pos.x;
		}


		
		{
			const auto aspect_ratio = (double)width / height;
			vec3 vup(0, 1, 0);
			long double dist_to_focus = 0.5;


			point3 lookfrom(
				playerPosition.pos.x, 
				playerPosition.pos.y, 
				playerPosition.pos.z
			);

			Point look;
			look.rot.x = playerPosition.rot.x;
			look.rot.y = playerPosition.rot.y;
			look.rot.z = playerPosition.rot.z;

			getRotatedVec(&look, false, false);

			point3 lookat(
				playerPosition.pos.x + look.pos.x,
				playerPosition.pos.y + look.pos.y ,
				playerPosition.pos.z + look.pos.z
			);


			vec3 horizontal;
			vec3 vertical ;
			vec3 lower_left_corner;
			{
				auto theta = fov*pi_a;
				auto h = tan(theta / 2);
				auto viewport_height = 2.0 * h;
				auto viewport_width = aspect_ratio * viewport_height;

				auto w = unit_vector(lookfrom - lookat);
				auto u = unit_vector(cross(vup, w));
				auto v = cross(w, u);

				horizontal = dist_to_focus * viewport_width * u;
				vertical = dist_to_focus * viewport_height * v;
				lower_left_corner = lookfrom - horizontal / 2 - vertical / 2 - dist_to_focus * w;
			}

			Vector3 pos, mov;
			for (int j = height - 1; j >= 0; j -= fov_2)
			{
				auto v = (long double)j / (height - 1);
				for (int i = 0; i < width; i += fov_2)
				{
					auto u = (long double)i / (width - 1);

					
					pos.x = playerPosition.pos.x;
					pos.y = playerPosition.pos.y;
					pos.z = playerPosition.pos.z;


					vec3 tempvec3 = unit_vector(lower_left_corner + u * horizontal + v * vertical - lookfrom);

					mov.x = tempvec3.x() / ray_step_divider;
					mov.y = tempvec3.y() / ray_step_divider;
					mov.z = tempvec3.z() / ray_step_divider;


					if (mode != 0)
					{
						int intarray[3] = { 0,0,0 };

						intarray[0] = (int)(((mov.x + 1) / 2) * fov_2 * 255) * ((mode / 1) % 2);
						intarray[1] = (int)(((mov.y + 1) / 2) * fov_2 * 255) * ((mode / 2) % 2);
						intarray[2] = (int)(((mov.z + 1) / 2) * fov_2 * 255) * ((mode / 4) % 2);

						int* temprgb = intarray;
						const unsigned int offset = (texWidth * 4 * ((height - 1) - j)) + i * 4;
						pixels[offset + 0] = temprgb[2];       // b
						pixels[offset + 1] = temprgb[1];       // g
						pixels[offset + 2] = temprgb[0];       // r
						pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a

						continue;
					}

					int dis = 1800;
					long double r = 1, g = 1, b = 1;
					Pixeldata* currentPixel = 0;
					Pixeldata* oldPixel = 0;
					
					double distortion = 1;

					while (dis > 0)
					{
						oldPixel = currentPixel;
						currentPixel = MEM_MAP.getPixel(pos.x, pos.y, pos.z);
						bool move = (currentPixel == 0);

						if (currentPixel != 0)
						{
							if (currentPixel->light)
							{
								break;
							}

							if (currentPixel->reflect)
							{
								{
									vec3 normal = getNormal(currentPixel, pos, mov);
									tempvec3 = reflect(tempvec3, normal);

									mov.x = tempvec3.x() / ray_step_divider;
									mov.y = tempvec3.y() / ray_step_divider;
									mov.z = tempvec3.z() / ray_step_divider;
								}
								

								long double temp_a = currentPixel->a / 255.0;

								r *= (currentPixel->r / 255.0) * temp_a;
								g *= (currentPixel->g / 255.0) * temp_a;
								b *= (currentPixel->b / 255.0) * temp_a;
								move = true;
							}

							if (currentPixel->transparent)
							{
								if (distortion != currentPixel->distortion)
								{
									refract_2(currentPixel, &tempvec3, pos, mov, distortion, currentPixel->distortion, false);
									distortion = currentPixel->distortion;

									mov.x = tempvec3.x() / ray_step_divider;
									mov.y = tempvec3.y() / ray_step_divider;
									mov.z = tempvec3.z() / ray_step_divider;
								}

								//long double temp_a = currentPixel->a / 255.0;
								//r *= (currentPixel->r / 255.0) * temp_a;
								//g *= (currentPixel->g / 255.0) * temp_a;
								//b *= (currentPixel->b / 255.0) * temp_a;
								move = true;
							}
						}
						else
						{
							if (distortion != 1)
							{
								refract_2(oldPixel, &tempvec3, pos, mov, distortion, 1, true);
								distortion = 1;

								mov.x = tempvec3.x() / ray_step_divider;
								mov.y = tempvec3.y() / ray_step_divider;
								mov.z = tempvec3.z() / ray_step_divider;
							}
						}

						if (move)
						{
							pos.x += mov.x;
							pos.y += mov.y;
							pos.z += mov.z;
							dis--;
							continue;
						}

						//if (currentPixel->testing != 123)
						//{
						//	cout << "ERROR!\n";
						//	pos.x += mov.x;
						//	pos.y += mov.y;
						//	pos.z += mov.z;
						//	dis--;
						//	continue;
						//}


						{
							long double temp_a = currentPixel->a / 255.0;

							r *= (currentPixel->r / 255.0) * temp_a;
							g *= (currentPixel->g / 255.0) * temp_a;
							b *= (currentPixel->b / 255.0) * temp_a;
						}
						break;
					}

					if (dis == 0)
					{
						r = 0;
						g = 0;
						b = 0;
					}
					else
					{
						if (!currentPixel->light)
						{
							Lightdata* temp_light = Light_MEM_MAP.getLightPixel(pos.x, pos.y, pos.z);
							r *= (1 + 9 * temp_light->r / temp_light->amount) / 10;
							g *= (1 + 9 * temp_light->g / temp_light->amount) / 10;
							b *= (1 + 9 * temp_light->b / temp_light->amount) / 10;

							//r = (1 + 9 * temp_light->light_level.x) / 10;
							//g = (1 + 9 * temp_light->light_level.y) / 10;
							//b = (1 + 9 * temp_light->light_level.z) / 10;
						}
						else
						{
							r = currentPixel->r / 255.0;
							g = currentPixel->g / 255.0;
							b = currentPixel->b / 255.0;
						}
					}

					int intarray[3] = 
					{ 
						r * 255,
						g * 255,
						b * 255
					};
										
					int* temprgb = intarray;

					int t_fov = (int)fov_2;

					for (int off_x = 0; off_x < t_fov && ((i + off_x) < width); off_x++)
					{
						for (int off_y = 0; off_y < t_fov && ((j - off_y) >= 0); off_y++)
						{
							const unsigned int offset = (texWidth * 4 * ((height - 1) - (j-off_y))) + (i+off_x) * 4;
							pixels[offset + 0] = temprgb[2];       // b
							pixels[offset + 1] = temprgb[1];       // g
							pixels[offset + 2] = temprgb[0];       // r
							pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a
						}
					}
				}
			}
		}



		if (useLocktexture)
		{
			unsigned char* lockedPixels = nullptr;
			int pitch = 0;
			SDL_LockTexture
			(
				texture,
				NULL,
				reinterpret_cast<void**>(&lockedPixels),
				&pitch
			);
			std::memcpy(lockedPixels, pixels.data(), pixels.size());
			SDL_UnlockTexture(texture);
		}
		else
		{
			SDL_UpdateTexture
			(
				texture,
				NULL,
				pixels.data(),
				texWidth * 4
			);
		}

		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		frames++;
		const Uint64 end = SDL_GetPerformanceCounter();
		const static Uint64 freq = SDL_GetPerformanceFrequency();
		const long double seconds = (end - start) / static_cast<double>(freq);
		if (seconds > 2.0)
		{
			std::cout
				<< frames << " frames in "
				<< std::setprecision(1) << std::fixed << seconds << " seconds = "
				<< std::setprecision(1) << std::fixed << frames / seconds << " FPS ("
				<< std::setprecision(3) << std::fixed << (seconds * 1000.0) / frames << " ms/frame)"
				<< std::endl;


			start = end;
			frames = 0;
		}


	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	log("Screen Closed!", 'I');

	log("End.", 'X');

	string t;

	//std::getline(std::cin, t);

	return 0;
}



