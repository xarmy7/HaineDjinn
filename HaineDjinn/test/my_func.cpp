
#include <cstdio>

extern "C" __declspec(dllexport) void my_func(void)
{
	printf("From DLL.\n");
}
