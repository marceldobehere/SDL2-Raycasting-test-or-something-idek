using namespace std;


// TODO
// MAKE PNG SLICE TO RAYCAST OBJECT FILE CONVERTER IN C# (GET PNG SLICES FROM GOXEL OR SOMETHING) 
// MAKE A SCENE FILE WHICH HAS/LINKS TO THOSE OBJECT FILES (AND THEIR POSITIONS) WHICH GET LOADED INTO THE 3D MAP AT START OF THE PROGRAM
// POSSIBLY OPTIMIZE THAT LOADING
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
#define PI 3.14159265358979323846
#define pi_a 0.017453292519943295

#include "other/main.cc"

bool debug_out = true;

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


struct Vector3
{
	double x, y, z;
};





struct Point
{
	Vector3 pos, rot;
};






void getRotatedVec(Point* point, bool normalize, bool mult)
{
	//double ray_move_x = 0;
	//double ray_move_y = 0;
	//double ray_move_z = 0;


	//ray_move_x += sin(point->rot.x * pi_a);
	//ray_move_y += sin(point->rot.y * pi_a);
	//ray_move_z += sin(point->rot.z * pi_a);

	//ray_move_z += cos(point->rot.x * pi_a);
	//ray_move_z += cos(point->rot.y * pi_a);
	//ray_move_y += cos(point->rot.z * pi_a);


	double ray_move_x = -cos(point->rot.x * pi_a) * sin(point->rot.y * pi_a);
	double ray_move_y = sin(point->rot.x * pi_a);
	double ray_move_z = -cos(point->rot.x * pi_a) * cos(point->rot.y * pi_a);


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
		double mod = 0.0;

		mod += ray_move_x * ray_move_x;
		mod += ray_move_y * ray_move_y;
		mod += ray_move_z * ray_move_z;

		double mag = std::sqrt(mod);

		if (mag != 0) 
		{
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


int main(int argc, char** argv)
{
	const int width = 1280, height = 720;

	log_debug("Opening Window...", '>');

	//------------------------------------------------------------------------------------------------------------------------------------------------

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow
	(
		"Raycasting test",
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

	double fov = 90;
	double fov_2 = 1;
	int mode = 0;

	mapmem3d MEM_MAP = *(new mapmem3d);
	MEM_MAP.init();



	for (int i = 0; i < 100 * 100 * 100; i++)
		MEM_MAP.submaps[i] = 0;



	//cout << "TEST3: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->testing << "\n";

	MEM_MAP.getPixel(0, 0, 0);

	//V_CPU_INIT();




	//MEM_MAP.getPixel(0, 0, 0);


	
		Pixeldata* aaa = (new Pixeldata);
		aaa->r = 255;
		aaa->g = 5;
		aaa->b = 10;

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
					temp_pixl->r = movx;
					temp_pixl->g = movy;
					temp_pixl->b = movz;
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
					temp_pixl->reflect = true;

					int oof = ((movx * movx) + (movy * movy) + (movz * movz));
					if (oof < 50 * 50 && oof > 46 * 46)
						MEM_MAP.setPixel(51 + (movx * 0.01), 54 + (movy * 0.01), 51 + (movz * 0.01), temp_pixl);

				}
			}
		}


	}








	if (false)
	{

		const auto aspect_ratio = 16.0 / 9.0;
		const int image_width = 480;
		const int image_height = static_cast<int>(image_width / aspect_ratio);
		const int samples_per_pixel = 5;
		const int max_depth = 50;

		// World

		auto world = random_scene();

		// Camera

		point3 lookfrom(13, 2, 3);
		point3 lookat(0, 0, 0);
		vec3 vup(0, 1, 0);
		auto dist_to_focus = 10.0;
		auto aperture = 0.1;

		camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

		for (int j = image_height - 1; j >= 0; --j) 
		{
			std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i)
			{
				color pixel_color(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (i + random_double()) / (image_width - 1);
					auto v = (j + random_double()) / (image_height - 1);
					ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
				COL temp = get_color(pixel_color, samples_per_pixel);


				int intarray[3] = { temp.r,temp.g,temp.b };
				int* temprgb = intarray;

				const unsigned int offset = (texWidth * 4 * ((image_height - 1) - j)) + i * 4;
				pixels[offset + 0] = temprgb[2];       // b
				pixels[offset + 1] = temprgb[1];       // g
				pixels[offset + 2] = temprgb[0];       // r
				pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a

			}

			




		}

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
		const double seconds = (end - start) / static_cast<double>(freq);
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


		std::cout << "\nDone.\n";
		string t;
		std::getline(std::cin, t);







	}









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
				ax.fill.g = (ax.fill.g + 10) % 256;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_T == event.key.keysym.scancode)
				fov += 10;
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_G == event.key.keysym.scancode)
				fov -= 10;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_B == event.key.keysym.scancode)
				fov_2 += 1;
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_N == event.key.keysym.scancode)
				fov_2 -= 1;
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
			playerPosition.pos.y -= temp_move_2->pos.y * temp_move->pos.x;
			playerPosition.pos.z += temp_move_2->pos.z * temp_move->pos.x;
		}






		{
			const auto aspect_ratio = (double)width / height;
			const int samples_per_pixel = 1;
			const int max_depth = 50;




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


			//point3 lookat(
			//	playerPosition.pos.x + playerPosition.rot.x,
			//	playerPosition.pos.y + playerPosition.rot.y,
			//	playerPosition.pos.z + playerPosition.rot.z 
			//);

			vec3 vup(0, 1, 0);
			double dist_to_focus = 1;
			double aperture = 1;

			camera cam = *new camera(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);


			for (int j = height - 1; j >= 0; j -= 1)
			{
				//std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
				for (int i = 0; i < width; i += 1)
				{
					//color pixel_color(0, 0, 0); 
					for (int s = 0; s < samples_per_pixel; ++s) {
						//auto u = (i + random_double()) / (width - 1);
						//auto v = (j + random_double()) / (height - 1);
						auto u = (double)i / (width - 1);
						auto v = (double)j / (height - 1);
						ray r = cam.get_ray(u, v);
						//pixel_color += ray_color(r, world, max_depth);

						Vector3 pos, mov;
						pos.x = r.orig.x();
						pos.y = r.orig.y();
						pos.z = r.orig.z();
						
						mov.x = r.dir.x() / 50;
						mov.y = r.dir.y() / 50;
						mov.z = r.dir.z() / 50;

						



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

						int dis = 200;
						Pixeldata* pixl = 0;
						Pixeldata* currentPixel = 0;
						while (dis > 0)
						{
							currentPixel = MEM_MAP.getPixel(pos.x, pos.y, pos.z);

							if (currentPixel == 0)
							{
								pos.x += mov.x;
								pos.y += mov.y;
								pos.z += mov.z;
								dis--;
								continue;
							}


							pixl = currentPixel;
							break;
						}


						//COL temp = get_color(pixel_color, samples_per_pixel);


						//int intarray[3] = { temp.r,temp.g,temp.b };
						int intarray[3] = { 0,0,0 };
						if (pixl != 0)
						{
							intarray[0] = pixl->r;
							intarray[1] = pixl->g;
							intarray[2] = pixl->b;
							//cout << "DIR: " << 20 * mov.x << ", " << 20 * mov.y << ", " << 20 * mov.z << ".\n";
						}
						int* temprgb = intarray;

						const unsigned int offset = (texWidth * 4 * ((height - 1) - j)) + i * 4;
						pixels[offset + 0] = temprgb[2];       // b
						pixels[offset + 1] = temprgb[1];       // g
						pixels[offset + 2] = temprgb[0];       // r
						pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a


					}


				}


			}
			/*
			//Point ray;
			//Point dir;
			for (unsigned int y = 0; y < height; y += 2)
			{
				for (unsigned int x = 0; x < width; x += 2)
				{






					
						ray.pos.x = playerPosition.pos.x;
						ray.pos.y = playerPosition.pos.y;
						ray.pos.z = playerPosition.pos.z;
						ray.rot.x = playerPosition.rot.x;
						ray.rot.y = playerPosition.rot.y;
						ray.rot.z = playerPosition.rot.z;

						ray.rot.y += (((double)y - ((height - 1) / 2.0f)) / (height - 1)) * fov;
						ray.rot.x += (((double)x - ((width - 1) / 2.0f)) / (width - 1)) * fov;

						//ray.rot_y += (asin((((double)y - ((height - 1) / 2.0f)) / (height - 1))) / pi_a) / fov;
						//ray.rot_x += (asin((((double)x - ((width - 1) / 2.0f)) / (width - 1))) / pi_a) / fov;

						//ray.rot_y += (asin(((double)y / (height - 1))) / pi_a) / fov;
						//ray.rot_x += (asin(((double)x / (width - 1))) / pi_a) / fov;

						double step = 0.05;
						int dis = 70;

						Pixeldata* pixl = 0;

						{
							double ray_move_x = 0;
							double ray_move_y = 0;
							double ray_move_z = 0;


							//ray_move_x += sin(ray.rot_x * pi_a) * step;
							//ray_move_y += cos(ray.rot_x * pi_a) * step;
							//ray_move_y += sin(ray.rot_y * pi_a) * step;
							//ray_move_z += cos(ray.rot_y * pi_a) * step;
							//ray_move_z += sin(ray.rot_z * pi_a) * step;
							//ray_move_x += cos(ray.rot_z * pi_a) * step;

							ray_move_y += (cos(ray.rot.x * pi_a) - sin(ray.rot.x * pi_a));
							ray_move_z += (cos(ray.rot.x * pi_a) + sin(ray.rot.x * pi_a));

							ray_move_z += (cos(ray.rot.y * pi_a) - sin(ray.rot.y * pi_a));
							ray_move_x += (cos(ray.rot.y * pi_a) + sin(ray.rot.y * pi_a));

							ray_move_x += (cos(ray.rot.z * pi_a) - sin(ray.rot.z * pi_a));
							ray_move_y += (cos(ray.rot.z * pi_a) + sin(ray.rot.z * pi_a));


							//{
							//	double mod = 0.0;

							//	mod += ray_move_x * ray_move_x;
							//	mod += ray_move_y * ray_move_y;
							//	mod += ray_move_z * ray_move_z;

							//	double mag = std::sqrt(mod);

							//	if (mag != 0) 
							//	{
							//		ray_move_x /= mag;
							//		ray_move_y /= mag;
							//		ray_move_z /= mag;
							//	}
							//}


							ray_move_x *= step;
							ray_move_y *= step;
							ray_move_z *= step;




							if (mode != 0)
							{

								int intarray[3] = { 0,0,0 };

								intarray[0] = (int)(((ray_move_x + 1) / 2) * fov_2 * 255) * ((mode / 1) % 2);
								intarray[1] = (int)(((ray_move_y + 1) / 2) * fov_2 * 255) * ((mode / 2) % 2);
								intarray[2] = (int)(((ray_move_z + 1) / 2) * fov_2 * 255) * ((mode / 4) % 2);

							


								int* temprgb = intarray;
								const unsigned int offset = (texWidth * 4 * y) + x * 4;
								pixels[offset + 0] = temprgb[2];       // b
								pixels[offset + 1] = temprgb[1];       // g
								pixels[offset + 2] = temprgb[0];       // r
								pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a

								continue;
							}






							Pixeldata* currentPixel = 0;
							while (dis > 0)
							{
								currentPixel = MEM_MAP.getPixel(ray.pos.x, ray.pos.y, ray.pos.z);

								if (currentPixel == 0)
								{
									ray.pos.x += ray_move_x;
									ray.pos.y += ray_move_y;
									ray.pos.z += ray_move_z;
									dis--;
									continue;
								}


								pixl = currentPixel;
								break;
							}


						}
						int intarray[3] = { 0,0,0 };





						if (pixl != 0)
						{
							intarray[0] = { pixl->r,};
							intarray[1] = { pixl->g };
							intarray[2] = { pixl->b };
						}
						else
						{
							//temprgb = rgbinttointarray(rgbtoint(pixl->r, pixl->g, pixl->b));
								//int intarray[3] = { pixl->r,pixl->g,pixl->b };
							//intarray[0] = (int)((float)intarray[0] * ((3 + cos(ray.rot_x * PI / 180))) / 4.0f);
							//intarray[1] = (int)((float)intarray[1] * ((3 + cos(ray.rot_x * PI / 180))) / 4.0f);
							//intarray[2] = (int)((float)intarray[2] * ((3 + cos(ray.rot_x * PI / 180))) / 4.0f);
							//intarray[2] = (dis * 10);
							//if (intarray[2] > 255)
							//	intarray[2] = 255;
								//temprgb = intarray;
						}



						//int*  temprgb = rgbinttointarray(rgbtoint(x % 256, y % 256, CPU_TICK_COUNTER % 256));



						const unsigned int offset = (texWidth * 4 * y) + x * 4;
						pixels[offset + 0] = intarray[2];       // b
						pixels[offset + 1] = intarray[1];       // g
						pixels[offset + 2] = intarray[0];       // r
						pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a
					
				}
			}
			*/
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
		const double seconds = (end - start) / static_cast<double>(freq);
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



