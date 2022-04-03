/*
*	Documentación del programa 'autos.c'
*
*	Descripción:	Programa que muestra 4 autos que se mueven de forma horizontal
*					manejados por hilos.
*
*	Modo de compilación: gcc -Wall autos.c -o autos  `sdl-config --cflags --libs` -lpthread
*
*	Modo de ejecución:	./autos 
*
*	Elaborado por:	
*		Ayala Ruíz Mario Antonio
*		Elorza Velásquez Margarita
*		García González Axel Isaac
*
*	Licencia: CC BY-NC-SA 4.0
*/

#include <SDL/SDL.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

SDL_Surface *screen;
SDL_Surface *fondo;
SDL_Surface *carro1;
SDL_Surface *carro2;
SDL_Rect destino;
void pantalla();
void *funcion1(void *valor);
void *funcion2(void *valor);
void *funcion3(void *valor);
void *funcion4(void *valor);
pthread_mutex_t EM;

int main(int argc, char *argv[]){

	pthread_t hilo1, hilo2, hilo3, hilo4;
	pthread_attr_t attr;

	/* Inicializar la biblioteca SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("No se pudo inicializar SDL: %s\n", SDL_GetError());
		exit(1);
	}

	fondo = SDL_LoadBMP("carretera.bmp");
	carro1 = SDL_LoadBMP("carro.bmp");
	carro2 = SDL_LoadBMP("carro_r.bmp");
	SDL_SetColorKey(carro1, SDL_SRCCOLORKEY, SDL_MapRGB(carro1->format, 0, 0, 0));
	SDL_SetColorKey(carro2, SDL_SRCCOLORKEY, SDL_MapRGB(carro2->format, 0, 0, 0));
	
	screen = SDL_SetVideoMode(1024, 720, 16, SDL_HWSURFACE);
	if (screen == NULL)
	{
		printf("Error al entrar a modo grafico: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	
	pantalla();
	SDL_Flip(screen);
	
	// Coloca atributo predeterminados o coloque NULL
	pthread_attr_init(&attr);
	// Crear hilos
	pthread_create(&hilo1, &attr, funcion1, NULL);
	pthread_create(&hilo2, &attr, funcion2, NULL);
	pthread_create(&hilo3, &attr, funcion3, NULL);
	pthread_create(&hilo4, &attr, funcion4, NULL);
	// Espera hilos
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	pthread_join(hilo3, NULL);
	pthread_join(hilo4, NULL);
	SDL_Quit();
	return EXIT_SUCCESS;
}

/*
*	Renderiza el fondo en la ventana
*/
void pantalla()
{
	/* Titulo de la ventana */
	SDL_WM_SetCaption("Carros", "Carros");
	destino.x = 0;
	destino.y = 0;
	SDL_BlitSurface(fondo, NULL, screen, &destino);
}

/*
*	Función del hilo que renderiza al auto 1
*	que se mueve de derecha a izquierda
*/
void *funcion1(void *valor)
{
	int j = 0;

	while (1)
	{
		// Enciende Semáforo (INICIO Exclusión) para indicar que entrara a SC
		pthread_mutex_lock(&EM);
		// Inicio de Sección Crítica
		pantalla();
		destino.x = 1024 - (30 * j++);
		destino.y = 65;
		if (j == 36)
			j = 0;
		SDL_BlitSurface(carro1, NULL, screen, &destino);
		/* Actualizamos la pantalla */
		SDL_Flip(screen);
		SDL_Delay(50);
		// Fin de Sección Crítica
		// Apaga Semáforo (FIN Exclusión) para indicar que sale de SC
		pthread_mutex_unlock(&EM);
	}
	pthread_exit(0);
}

/*
*	Función del hilo que renderiza al auto 2
*	que se mueve de derecha a izquierda
*/
void *funcion2(void *valor)
{
	int j = 0;
	while (1)
	{
		// Enciende Semáforo (INICIO Exclusión) para indicar que entrara a SC
		pthread_mutex_lock(&EM);
		// Inicio de Sección Crítica
		pantalla();
		destino.x = 1024 - (30 * j++);
		destino.y = 180;
		if (j == 36)
			j = 0;
		SDL_BlitSurface(carro1, NULL, screen, &destino);
		SDL_Flip(screen);
		SDL_Delay(50);
		// Fin de Sección Crítica
		// Apaga Semáforo (FIN Exclusión) para indicar que sale de SC
		pthread_mutex_unlock(&EM);
	}
	pthread_exit(0);
}

/*
*	Función del hilo que renderiza al auto 3
*	que se mueve de izquierda a derecha
*/
void *funcion3(void *valor)
{
	int j = 0;
	while (1)
	{
		// Enciende Semáforo (INICIO Exclusión) para indicar que entrara a SC
		pthread_mutex_lock(&EM);
		// Inicio de Sección Crítica
		pantalla();
		destino.x = 0 + (30 * j++);
		destino.y = 320;
		if (j == 36)
			j = 0;
		SDL_BlitSurface(carro2, NULL, screen, &destino);
		SDL_Flip(screen);
		SDL_Delay(50);
		// Fin de Sección Crítica
		// Apaga Semáforo (FIN Exclusión) para indicar que sale de SC
		pthread_mutex_unlock(&EM);
	}
	pthread_exit(0);
}

/*
*	Función del hilo que renderiza al auto 4
*	que se mueve de izquierda a derecha
*/
void *funcion4(void *valor)
{
	int j = 0;
	while (1)
	{
		// Enciende Semáforo (INICIO Exclusión) para indicar que entrara a SC
		pthread_mutex_lock(&EM);
		// Inicio de Sección Crítica
		pantalla();
		destino.x = 0 + (30 * j++);
		destino.y = 510;
		if (j == 36)
			j = 0;
		SDL_BlitSurface(carro2, NULL, screen, &destino);
		SDL_Flip(screen);
		SDL_Delay(50);
		// Fin de Sección Crítica
		// Apaga Semáforo (FIN Exclusión) para indicar que sale de SC
		pthread_mutex_unlock(&EM);
	}
	pthread_exit(0);
}