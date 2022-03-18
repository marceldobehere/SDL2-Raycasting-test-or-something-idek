#pragma once
using namespace std;
#include <iostream>
#include <fstream>
#include <tuple>

std::tuple<int, char*> ReadFile(const char* filename)
{
	streampos begin, end;
	ifstream myfile(filename, ios::binary);
	begin = myfile.tellg();
	myfile.seekg(0, ios::end);
	end = myfile.tellg();
	myfile.close();

	if ((end - begin) == 0)
	{
		cout << "! No Data found!" << "\nX Exiting...\n";
		exit(0);
	}
	int size = (end - begin);
	cout << "I Data size: " << size << " bytes.\n";


	char* filedata = new char[size];
	myfile.open(filename, ios::binary);
	myfile.read(filedata, size);
	myfile.close();

	return std::tuple<int, char*>(size, filedata);
}

int ConvertCharPointerToInt(char* pointer)
{
	bool target[32];
	for (int byteshift = 0; byteshift < 32; byteshift += 8)
		for (int bitshift = 0; bitshift < 8; bitshift++)
			target[bitshift + byteshift] = ((pointer[byteshift / 8] >> bitshift) & 1) == 1;

	int test2 = 0;
	int base = 1;
	for (int i = 0; i < 32; i++)
	{
		test2 += target[i] * base;
		base *= 2;
	}
	return test2;
}

int ConvertCharPointerToShort(char* pointer)
{
	bool target[16];
	for (int byteshift = 0; byteshift < 16; byteshift += 8)
		for (int bitshift = 0; bitshift < 8; bitshift++)
			target[bitshift + byteshift] = ((pointer[byteshift / 8] >> bitshift) & 1) == 1;

	int test2 = 0;
	int base = 1;
	for (int i = 0; i < 16; i++)
	{
		test2 += target[i] * base;
		base *= 2;
	}
	return test2;
}


double ConvertCharPointerToDouble(char* pointer)
{
	double d = 0;
	memcpy(&d, pointer, sizeof d);
	return d;
}




std::tuple<int, int*> ReadFile_old(const char* filename)
{
	streampos begin, end;
	ifstream myfile(filename, ios::binary);
	begin = myfile.tellg();
	myfile.seekg(0, ios::end);
	end = myfile.tellg();
	myfile.close();

	if ((end - begin) == 0)
	{
		cout << "! No Data found!" << "\nX Exiting...\n";
		exit(0);
	}
	cout << "I Data size: " << (end - begin) << " bytes.\n";


	char* test = new char[4];
	int iend = ((end - begin) / 4);
	int* filedata = new int[iend];
	myfile.open(filename, ios::binary);


	for (int currentinteger = 0; currentinteger < iend; currentinteger++)
	{
		myfile.read(test, 4);

		bool target[32];
		for (int byteshift = 0; byteshift < 32; byteshift += 8)
			for (int bitshift = 0; bitshift < 8; bitshift++)
				target[bitshift + byteshift] = ((test[byteshift / 8] >> bitshift) & 1) == 1;

		int test2 = 0;
		int base = 1;
		for (int i = 0; i < 32; i++)
		{
			test2 += target[i] * base;
			base *= 2;
		}

		filedata[currentinteger] = test2;
	}
	myfile.close();

	return  std::tuple<int, int*>(iend, filedata);
}



void WriteFile(const char* filename, std::tuple<int, int*> filedata_)
{
	ofstream myfile(filename, ios::binary);

	int datasize = std::get<0>(filedata_);
	int* filedata = std::get<1>(filedata_);

	char* data = new char[datasize * 4];

	for (int currentint = 0; currentint < datasize; currentint++)
	{
		int index = currentint * 4;

		bool* target = new bool[32];
		for (int bitshift = 0; bitshift < 32; bitshift++)
			target[bitshift] = ((filedata[currentint] >> bitshift) & 1) == 1;

		int minidata[4];
		for (int i = 0; i < 4; i++)
		{
			int base = 1;
			int tempint = 0;
			for (int i2 = 0; i2 < 8; i2++)
			{
				tempint += target[(i * 8) + i2] * base;
				base *= 2;
			}
			minidata[i] = tempint;
		}

		for (int i = 0; i < 4; i++)
			data[index + i] = minidata[i];
	}

	myfile.write(data, datasize * 4);

	myfile.close();
}