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
#include <stdbool.h>
#include <SDL_ttf.h>

//Creating a button
typedef struct {
    SDL_Rect draw_rect;    // dimensions of button
		struct {
        	Uint8 r, g, b, a;
    	} colour;

    bool pressed;
} button_t;

static void button_process_event(button_t *btn, const SDL_Event *ev) {
// react on mouse click within button rectangle by setting 'pressed'
	if(ev->type == SDL_MOUSEBUTTONDOWN) {
		if(ev->button.button == SDL_BUTTON_LEFT &&
    		ev->button.x >= btn->draw_rect.x &&
        	ev->button.x <= (btn->draw_rect.x + btn->draw_rect.w) &&
	        ev->button.y >= btn->draw_rect.y &&
	        ev->button.y <= (btn->draw_rect.y + btn->draw_rect.h)) {
    		btn->pressed = true;
    	}
    }
}

static bool button(SDL_Renderer *r, button_t *btn) {
// draw button
	SDL_SetRenderDrawColor(r, btn->colour.r, btn->colour.g, btn->colour.b, btn->colour.a);
	SDL_RenderFillRect(r, &btn->draw_rect);

	// if button press detected - reset it so it wouldn't trigger twice
	if(btn->pressed) {
	    btn->pressed = false;
	    return true;
	}
	return false;
}


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

	const int window_width = 900;
	const int window_height = 600;

	// button state - colour and rectangle
    button_t start_button = {
        .colour = { .r = 0, .g = 255, .b = 255, .a = 255, },
        .draw_rect = { .x = 100, .y = 100, .w = 10, .h = 10 },
    };

	enum {
        STATE_IN_MENU,
        STATE_IN_GAME,
    } state = 0;

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

	char *title = "Chess";
	char *play = "Play Game";
	char *exit = "Exit";
	SDL_Color font_color = { 255, 255, 255 };
	SDL_Surface *title_surface = TTF_RenderText_Solid(font_type, title, font_color);
	SDL_Surface *play_surface = TTF_RenderText_Solid(font_type, play, font_color);
	SDL_Surface *exit_surface = TTF_RenderText_Solid(font_type, exit, font_color);
	if(title_surface == NULL || play_surface == NULL || exit_surface == NULL) {
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

	Uint32 window_flags = 0;

	if(SDL_CreateWindowAndRenderer(window_width, window_height, window_flags, &window, &renderer)) {
		printf("error: SDL_CreateWindowAndRenderer failure\n%s\n", SDL_GetError());
		TTF_CloseFont(font_type);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
	SDL_Texture *play_texture = SDL_CreateTextureFromSurface(renderer, play_surface);
	SDL_Texture *exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surface);
	if(title_texture == NULL || play_texture == NULL || exit_texture == NULL) {
		printf("error: SDL_CreateTextureFromSurface failure\n%s\n", SDL_GetError());
		SDL_FreeSurface(title_surface);
		SDL_FreeSurface(play_surface);
		SDL_FreeSurface(exit_surface);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		TTF_CloseFont(font_type);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_FreeSurface(title_surface);
	SDL_Rect title_rect = { .x = window_width/2-150, .y = window_height/2-180, .w = 300, .h = 96 };

	SDL_FreeSurface(play_surface);
	SDL_Rect play_rect = { .x = window_width/2-100, .y = window_height/2-40, .w = 200, .h = 32 };

	SDL_FreeSurface(exit_surface);
	SDL_Rect exit_rect = { .x = window_width/2-40, .y = window_height/2+20, .w = 80, .h = 32 };

	int close = 0;
	while(!close) 
	{
		SDL_PollEvent(&event);
		
		button_process_event(&start_button, &event);
		
		if(state == STATE_IN_MENU) {
            if(button(renderer, &start_button)) {
                state = STATE_IN_GAME;   // state change - button will not be drawn anymore
            }
        } else if(state == STATE_IN_GAME) {
            /* your game logic */
        }
		
		if(event.type == SDL_QUIT) {
			close = 1;
		}

		SDL_RenderClear(renderer);
		
		// Draw items here
		SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
		SDL_RenderCopy(renderer, play_texture, NULL, &play_rect);
		SDL_RenderCopy(renderer, exit_texture, NULL, &exit_rect);
		

		SDL_RenderPresent(renderer);
		SDL_Delay(1000/30);
	}


	SDL_DestroyTexture(title_texture);
	SDL_DestroyTexture(play_texture);
	SDL_DestroyTexture(exit_texture);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font_type);
	TTF_Quit();
	SDL_Quit();
	return 0;
}