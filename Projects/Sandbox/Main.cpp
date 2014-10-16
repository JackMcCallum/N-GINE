#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <thread>
#include "miniz.c"

//#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
//#include "C:/Program Files (x86)/Visual Leak Detector/include/vld.h"

size_t threadId;
unsigned int a = 0;

void threadMain()
{
	for (int i = 0; i < 100000; i++)
	{
		if (4 != threadId)
			a++;
	}
}

int main()
{
	threadId = std::this_thread::get_id().hash();

	int b = clock();

	std::thread thread(&threadMain);
	thread.join();

	std::cout << clock() - b << std::endl;


	system("pause");
	return 0;
}