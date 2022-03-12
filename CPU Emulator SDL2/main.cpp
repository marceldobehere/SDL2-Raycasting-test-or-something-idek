using namespace std;

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
#define PI 3.14159265

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

struct Point
{
	double x, y, z, rot_x, rot_y, rot_z;
};



int main(int argc, char** argv)
{
	const int width = 1280, height = 720;



	log_debug("Opening Window...", '>');

	//------------------------------------------------------------------------------------------------------------------------------------------------

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow
	(
		"VM - Screen",
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







	Point playerPosition;
	playerPosition.x = 50;
	playerPosition.y = 50;
	playerPosition.z = 50;
	playerPosition.rot_x = 0;
	playerPosition.rot_y = 0;
	playerPosition.rot_z = 0;

	double fov = 90;

	mapmem3d MEM_MAP = *(new mapmem3d());
	MEM_MAP.init();



	for (int i = 0; i < 100 * 100 * 100; i++)
		MEM_MAP.submaps[i] = 0;


	{
		Pixeldata aaa= *(new Pixeldata());
		aaa.r = 255;
		aaa.g = 5;
		aaa.b = 0;

		Submap x = *(new Submap());
		x.filled = true;
		x.fill = aaa;

		cout << "TEST1: " << x.testing << "\n";

		MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)] = &x;

		cout << "TEST2: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->testing << "\n";
	}

	//cout << "TEST3: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->testing << "\n";

	MEM_MAP.getPixel(0, 0, 0);

	//V_CPU_INIT();




	//MEM_MAP.getPixel(0, 0, 0);


	{
		Pixeldata qaaa = *(new Pixeldata());
		qaaa.r = 10;
		qaaa.g = 15;
		qaaa.b = 244;

		Submap ax = *(new Submap());
		ax.filled = true;
		ax.fill = qaaa;

		MEM_MAP.submaps[52 + (51 * 100) + (50 * 10000)] = &ax;
	}



	cout << "TEST A: " << MEM_MAP.submaps[52 + (50 * 100) + (50 * 10000)]->fill.r << "\n";
	cout << "TEST B: " << MEM_MAP.submaps[52 + (51 * 100) + (50 * 10000)]->fill.r << "\n";



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
				playerPosition.z += 0.5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_S == event.key.keysym.scancode)
				playerPosition.z -= 0.5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_A == event.key.keysym.scancode)
				playerPosition.x += 0.5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_D == event.key.keysym.scancode)
				playerPosition.x -= 0.5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_Q == event.key.keysym.scancode)
				playerPosition.y += 0.5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_E == event.key.keysym.scancode)
				playerPosition.y -= 0.5;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
				playerPosition.rot_x -= 10;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
				playerPosition.rot_x += 10;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_UP == event.key.keysym.scancode)
				playerPosition.rot_y -= 10;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_DOWN == event.key.keysym.scancode)
				playerPosition.rot_y += 10;
			
			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_R == event.key.keysym.scancode)
				playerPosition.rot_z += 10;

			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_F == event.key.keysym.scancode)
				playerPosition.rot_z -= 10;
		}



		for (unsigned int y = 0; y < height; y += 3)
		{
			for (unsigned int x = 0; x < width; x += 3)
			{
				Point ray;
				ray.x = playerPosition.x;
				ray.y = playerPosition.y;
				ray.z = playerPosition.z;
				ray.rot_x = playerPosition.rot_x;
				ray.rot_y = playerPosition.rot_y;
				ray.rot_z = playerPosition.rot_z;

				ray.rot_y += (((double)y - ((height - 1) / 2.0f)) / (height - 1)) * fov;
				ray.rot_x += (((double)x - ((width-1) / 2.0f)) / (width - 1)) * fov;
				
				double step = 0.1;
				int dis = 70;

				Pixeldata* pixl = 0;

				{
					double ray_move_x = 0;
					double ray_move_y = 0;
					double ray_move_z = 0;

					ray_move_x += sin(ray.rot_x * PI / 180) * step;
					ray_move_y += cos(ray.rot_x * PI / 180) * step;
					ray_move_y += sin(ray.rot_y * PI / 180) * step;
					ray_move_z += cos(ray.rot_y * PI / 180) * step;
					ray_move_z += sin(ray.rot_z * PI / 180) * step;
					ray_move_x += cos(ray.rot_z * PI / 180) * step;

					//ray_move_x += cos(ray.rot_x * PI / 180) * step;
					//ray_move_y += cos(ray.rot_y * PI / 180) * step;
					//ray_move_z += cos(ray.rot_z * PI / 180) * step;

					while (dis > 0)
					{
						Pixeldata* currentPixel = MEM_MAP.getPixel(ray.x, ray.y, ray.z);

						if (currentPixel != 0)
						{
							pixl = currentPixel;
							break;
						}

						ray.x += ray_move_x;
						ray.y += ray_move_y;
						ray.z += ray_move_z;
						dis--;
					}


				}
				int* temprgb;
				
				if (pixl == 0)
				{
					int intarray[3] = {0,0,0};
					temprgb = intarray; 
				}
				else
				{
					//temprgb = rgbinttointarray(rgbtoint(pixl->r, pixl->g, pixl->b));
					int intarray[3] = {pixl->r,pixl->g,pixl->b};
					//intarray[0] = (int)((float)intarray[0] * ((3 + cos(ray.rot_x * PI / 180))) / 4.0f);
					//intarray[1] = (int)((float)intarray[1] * ((3 + cos(ray.rot_x * PI / 180))) / 4.0f);
					//intarray[2] = (int)((float)intarray[2] * ((3 + cos(ray.rot_x * PI / 180))) / 4.0f);
					//intarray[2] = (dis * 10);
					//if (intarray[2] > 255)
					//	intarray[2] = 255;
					temprgb = intarray;
				}
				
				
				
				//int*  temprgb = rgbinttointarray(rgbtoint(x % 256, y % 256, CPU_TICK_COUNTER % 256));



				const unsigned int offset = (texWidth * 4 * y) + x * 4;
				pixels[offset + 0] = temprgb[2];       // b
				pixels[offset + 1] = temprgb[1];       // g
				pixels[offset + 2] = temprgb[0];       // r
				pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a
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

	std::getline(std::cin, t);

	return 0;
}



























void mainold()
{
	printf("test!");
	debug_out = true;
	//int * screenbuffer;
	//screenbuffer = malloc(250000000);
	static int screenbuffer[250000000] = {};
	log_debug("Preparing RAM...", '>');

	for (int i = 0; i < 250000000; i++)
	{
		screenbuffer[i] = 0;
	}
	log_debug("Done.", '<');

	log_debug("Opening Window", '>');
	//openglmakewindow();
	log_debug("Done.", '<');

	log_debug("Drawing Pixels...", '>');
	//opengldrawframe(screenbuffer);
	log_debug("Done...", '<');

	int tempcol = rgbtoint(100, 200, 100);
	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			//opengldrawdot(screenbuffer, x, y, tempcol);
			//screenbuffer[x + (y * 1280)] = rgbtoint(temp, 200, 100);
		}
	}

	//openglupdate();

	log_debug("Drawing Pixels...", '>');
	//opengldrawframe(screenbuffer);
	log_debug("Done...", '<');

	//char test[10];

	//log_debug("> Waiting for user input");

	//cin.getline(test, 10);


	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;
	int temp = 0;
	while (true)
	{
		if (temp % 10 == 0)
			current_ticks = clock();
		//log_debug("> Drawing demo pixels...");
		for (int y = 0; y < 500; y++)
		{
			for (int x = 0; x < 400; x++)
			{
				//opengldrawdot(screenbuffer, x, y, rgbtoint(((temp / 256) / 256) % 256, (temp / 256) % 256, temp % 256));
				//screenbuffer[x + (y * 1280)] = rgbtoint(((temp / 256) / 256) % 256, (temp / 256) % 256, temp % 256);
			}
		}

		//opengldrawframe(screenbuffer);
		//openglupdate();
		temp++;


		if (temp % 10 == 9)
		{
			delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
			if (delta_ticks > 0)
				fps = CLOCKS_PER_SEC / (delta_ticks / 10);
		}

		if (temp % 30 == 0)
			cout << fps << endl;
	}



	//openglclosewindow();
	log_debug("Exit", 'X');
}