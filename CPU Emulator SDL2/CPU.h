//#include <tuple>
//#include "IO.h"
//
//static bool debug_out = false;
//static bool stop = false;
//static int RAM[250000000] = {};
//static int REG[100] = {};
//static const Uint8* KEYS;
////int vramoffset = 2000000; // reg 3
//
//int CPU_TICK_COUNTER = 0;
//int CPU_TICKS_PER_FRAME = 1500000;
//
////int CPUPointer;
//
//
//void log_debug(const char a[], char sign)
//{
//	if (debug_out)
//	{
//		printf("%c", sign);
//		printf(" ");
//		printf(a);
//		printf("\n");
//	}
//}
//void log(const char a[], char sign)
//{
//	printf("%c", sign);
//	printf(" ");
//	printf(a);
//	printf("\n");
//}
//
//int rgbtoint(int r, int g, int b)
//{
//	return ((r % 256) * 256 * 256) + ((g % 256) * 256) + (b % 256);
//}
//
//int* rgbinttointarray(int rgb)
//{
//	int intarray[3] = {};
//	intarray[2] = rgb % 256;
//	intarray[1] = (rgb / 256) % 256;
//	intarray[0] = ((rgb / 256) / 256) % 256;
//
//	return intarray;
//}
//
//void V_CPU_LOAD_ROM_INTO_RAM()
//{
//	std::tuple<int, int*> rom_ = ReadFile("ROM.mcpu");
//	int length = std::get<0>(rom_);
//	int* rom = std::get<1>(rom_);
//	cout << "ROM:\n";
//	for (int i = 0; i < length; i++)
//	{
//		RAM[i] = rom[i];
//		cout << rom[i] << " ";
//	}
//	cout << "\n";
//}
//
//
//void V_CPU_INIT()
//{
//	KEYS = SDL_GetKeyboardState(NULL);
//	//cout << rgbtoint(100, 200, 100) << "\n";
//	//cout << (-18 / 4) << " lol\n";
//
//	debug_out = true;
//	stop = false;
//
//
//	log_debug("Preparing Registers...", '>');
//	REG[250000000] = {};
//	for (int i = 0; i < 100; i++)
//	{
//		REG[i] = 0;
//	}
//	REG[0] = 0; // instruction pointer
//	REG[1] = 0; // regoffset
//	REG[2] = 0; // ramoffset
//	REG[3]  = 101000000; // vramoffset
//	REG[42] = 100000000; // keyboardoffset
//	        
//	log_debug("Done.", '<');
//
//
//
//	log_debug("Preparing RAM...", '>');
//	RAM[250000000] = {};
//	for (int i = 0; i < 250000000; i++)
//	{
//		RAM[i] = 0;
//	}
//	log_debug("Done.", '<');
//
//
//	log_debug("Preparing VRAM...", '>');
//	{
//		//int testrgb = rgbtoint(100, 200, 100);
//		int endvram = REG[3] + (1280 * 720);
//		for (int i = REG[3]; i < endvram; i++)
//		{
//			RAM[i] = 0;
//		}
//	}
//	log_debug("Done.", '<');
//
//	REG[0] = 0;
//
//
//	log_debug("Loading ROM into RAM...", '>');
//	V_CPU_LOAD_ROM_INTO_RAM();
//	log_debug("Done.", '<');
//}
//
//
//
//
//
//
//
//
//void V_CPU_TICK()
//{
//	CPU_TICK_COUNTER++;
//
//	int command[4] = { RAM[REG[0] + 0],RAM[REG[0] + 1],RAM[REG[0] + 2],RAM[REG[0] + 3] };
//
//	//cout << "current instruction at (" << REG[0] << "): " << command[0] << " " << command[1] << " " << command[2] << " " << command[3] << " " << "\n";
//
//
//
//	if (command[0] == 17)
//	{
//		//cout << "> Register " << command[2] << " (" << REG[command[2]] << ") set to " << command[1] << ".\n";
//		REG[command[2]] = command[1];
//		//cout << "< Register " << command[2] << " (" << REG[command[2]] << ") set to " << command[1] << ".\n";
//	}
//
//	else if (command[0] == 18)
//	{
//		//cout << "> Register " << command[2] << " (" << REG[command[2]] << ") set to " << command[1] << ".\n";
//		RAM[command[2]] = command[1];
//		//cout << "< Register " << command[2] << " (" << REG[command[2]] << ") set to " << command[1] << ".\n";
//	}
//
//	else if (command[0] == 1)
//		REG[command[2]] = REG[command[1]];
//
//	else if (command[0] == 2)
//	{
//		//cout << "> Register " << command[2] << " (" << REG[command[2]] << ") set to " << command[1] << ".\n";
//		REG[REG[7]] = REG[REG[6]];
//		//cout << "< Register " << command[2] << " (" << REG[command[2]] << ") set to " << command[1] << ".\n";
//	}
//
//	else if (command[0] == 3)
//		RAM[command[2]] = REG[command[1]];
//
//	else if (command[0] == 4)
//		REG[command[2]] = RAM[command[1]];
//
//	else if (command[0] == 5)
//	{
//		//cout << "> copying " << REG[REG[6]] << "(" << REG[6]  << ") to " << RAM[REG[7]] << "(" << REG[7] << ") "<<".\n";
//		RAM[REG[7]] = REG[REG[6]];
//		//cout << "< copying " << REG[REG[6]] << "(" << REG[6] << ") to " << RAM[REG[7]] << "(" << REG[7] << ") " << ".\n";
//	}
//		
//
//	else if (command[0] == 6)
//		REG[REG[7]] = RAM[REG[6]];
//
//	else if (command[0] == 7)
//	{
//		int resulta = 0;
//		int resultb = 0;
//
//		if (command[2] == 0)
//			resulta = command[1] + command[3];
//		else if (command[2] == 1)
//			resulta = command[1] - command[3];
//		else if (command[2] == 2)
//			resulta = command[1] * command[3];
//		else if (command[2] == 3)
//		{
//			resulta = command[1] / command[3];
//			resultb = command[1] % command[3];
//		}
//
//		REG[4] = resulta;
//		REG[5] = resultb;
//	}
//
//	else if (command[0] == 8)
//	{
//		int resulta = 0;
//		int resultb = 0;
//
//		if (command[2] == 0)
//			resulta = REG[command[1]] + command[3];
//		else if (command[2] == 1)
//			resulta = REG[command[1]] - command[3];
//		else if (command[2] == 2)
//			resulta = REG[command[1]] * command[3];
//		else if (command[2] == 3)
//		{
//			resulta = REG[command[1]] / command[3];
//			resultb = REG[command[1]] % command[3];
//		}
//
//		REG[4] = resulta;
//		REG[5] = resultb;
//	}
//
//	else if (command[0] == 9)
//	{
//		int resulta = 0;
//		int resultb = 0;
//
//		if (command[2] == 0)
//			resulta = REG[command[1]] + REG[command[3]];
//		else if (command[2] == 1)
//			resulta = REG[command[1]] - REG[command[3]];
//		else if (command[2] == 2)
//			resulta = REG[command[1]] * REG[command[3]];
//		else if (command[2] == 3)
//		{
//			resulta = REG[command[1]] / REG[command[3]];
//			resultb = REG[command[1]] % REG[command[3]];
//		}
//
//		REG[4] = resulta;
//		REG[5] = resultb;
//	}
//
//
//	else if (command[0] == 10)
//	{
//		int resulta = 0;
//		int resultb = 0;
//
//		if (REG[command[2]] == 0)
//			resulta = REG[command[1]] + REG[command[3]];
//		else if (REG[command[2]] == 1)
//			resulta = REG[command[1]] - REG[command[3]];
//		else if (REG[command[2]] == 2)
//			resulta = REG[command[1]] * REG[command[3]];
//		else if (REG[command[2]] == 3)
//		{
//			resulta = REG[command[1]] / REG[command[3]];
//			resultb = REG[command[1]] % REG[command[3]];
//		}
//
//		REG[4] = resulta;
//		REG[5] = resultb;
//	}
//
//	else if (command[0] == 11)
//		REG[0] = command[1] - 4;
//
//	else if (command[0] == 12)
//	{
//		if (REG[command[2]] != 0)
//			REG[0] = command[1] - 4;
//	}
//
//	else if (command[0] == 13)
//	{
//		if (REG[command[2]] != 0)
//			REG[0] = REG[command[1]] - 4;
//	}
//
//
//	else if (command[0] == 14)
//	{
//		int result = 0;
//		bool a = command[1] == 1;
//		bool b = command[2] == 1;
//
//		if (command[3] == 0)
//			result = a && b;
//		else if (command[3] == 1)
//			result = a || b;
//		else if (command[3] == 2)
//			result = !a;
//		else if (command[3] == 3)
//			result = !(a && b);
//		else if (command[3] == 4)
//			result = !(a || b);
//		else if (command[3] == 5)
//			result = a ^ b;
//
//		REG[4] = result;
//	}
//
//	else if (command[0] == 15)
//	{
//		int result = 0;
//		bool a = REG[6] == 1;
//		bool b = REG[7] == 1;
//
//		if (REG[8] == 0)
//			result = a && b;
//		else if (REG[8] == 1)
//			result = a || b;
//		else if (REG[8] == 2)
//			result = !a;
//		else if (REG[8] == 3)
//			result = !(a && b);
//		else if (REG[8] == 4)
//			result = !(a || b);
//		else if (REG[8] == 5)
//			result = a ^ b;
//
//		REG[4] = result;
//	}
//
//	else if (command[0] == 16)
//	stop = true;
//
//
//
//	REG[0] += 4;
//
//	//int temp = CPU_TICK_COUNTER;
//
//	//int testrgb = (temp%(256 * 256 * 256));
//	//int endvram = vramoffset + (1280 * 720);
//	//for (int i = vramoffset; i < endvram; i++)
//	//{
//	//	RAM[i] = testrgb;
//	//}
//
//	/*
//	for (int y = 0; y < 200; y++)
//	{
//		for (int x = 0; x < 400; x++)
//		{
//			RAM[vramoffset + ((y * 1280) + x)] = (CPU_TICK_COUNTER % 1000);
//		}
//	}*/
//
//
//
//
//
//
//}