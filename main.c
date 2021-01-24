#ifdef _WIN32
	#include <direct.h>
	#define getcwd _getcwd
#else
	#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

int main() {
	int num = 14;
	printf("hello %d\n", num);
	return 0;
}
