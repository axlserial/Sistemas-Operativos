// compilación: gcc -Wall sdl1.c -o sdl1  `sdl-config --cflags --libs`
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR en el inicializado de SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE) == NULL)
	{
		printf("Error al entrar a modo grafico: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	SDL_WM_SetCaption("Hola mundo!", "Hola");

	SDL_Delay(5000);
	SDL_Quit();
	return EXIT_SUCCESS;
}