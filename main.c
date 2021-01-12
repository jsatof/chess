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
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
		printf("error: SDL_Init failure\n%s\n", SDL_GetError());
		return 1;
	}
	
	if(TTF_Init() == -1) {
		printf("error: TTF_Init failure\n%s\n", TTF_GetError());
		SDL_Quit();
		return 1;
	}

	char *current_path = getcwd(NULL, 0);
	char font_path[256];
	strcpy(font_path, current_path);
	free(current_path);
	
	strcat(font_path, "/Hack-Regular.ttf");
	
 
	TTF_Font *font_type = TTF_OpenFont(font_path, 24);
	if(!font_type) {
		printf("error: TTF_OpenFont failure\n%s\n", TTF_GetError());
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	char *message = "hello";
	SDL_Color font_color = { 255, 255, 255 };
	SDL_Surface *message_surface = TTF_RenderText_Solid(font_type, message, font_color);
	if(message_surface == NULL) {
		printf("error: TTF_RenderText_Solid failure\n%s\n", SDL_GetError());
		TTF_CloseFont(font_type);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Event event;
	const int window_width = 900;
	const int window_height = 600;

	Uint32 window_flags = 0;

	if(SDL_CreateWindowAndRenderer(window_width, window_height, window_flags, &window, &renderer)) {
		printf("error: SDL_CreateWindowAndRenderer failure\n%s\n", SDL_GetError());
		TTF_CloseFont(font_type);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Texture *message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);
	if(message_texture == NULL) {
		printf("error: SDL_CreateTextureFromSurface failure\n%s\n", SDL_GetError());
		SDL_FreeSurface(message_surface);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		TTF_CloseFont(font_type);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_FreeSurface(message_surface);
	SDL_Rect message_rect = { .x = 0, .y = 0, .w = 100, .h = 32 };

	int close = 0;
	while(!close) {
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT) {
			close = 1;
		}

		SDL_RenderClear(renderer);
		
		// Draw items here
		SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);
		

		SDL_RenderPresent(renderer);
		SDL_Delay(1000/30);
	}


	SDL_DestroyTexture(message_texture);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font_type);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
