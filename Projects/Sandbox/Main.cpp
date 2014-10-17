#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <thread>
#include "miniz.c"

//#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
//#include "C:/Program Files (x86)/Visual Leak Detector/include/vld.h"

#include "NGineFixedSizeAllocator.h"

using namespace NGine;



class TestObject : public TPoolAllocator<TestObject>
{
public:

	char padding[64];
};

int main()
{
	TestObject* obj = new TestObject();
	delete obj;

	system("pause");
	return 0;
}