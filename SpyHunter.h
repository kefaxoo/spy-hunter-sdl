#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include <time.h>
#include<string.h>
#include <stdlib.h>
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"

#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	720
#define GOD_MODE_TIME	15
#define ENEMIES			5
#define CAR_SPEED		250
#define DATE_FORMAT		"%x-%I:%M%p"

struct Game {
	struct {
		SDL_Surface* sprite;
		int colorIndex;
		
		struct Coord {
			int x;
			int y;
		} coord;
		
		struct {
			// power котопую подобрал
			SDL_Surface* sprite;
			double time;
		} power;
		
		int speed;
		int turn;
		int lives;
	} player;
	
	struct {
		SDL_Surface* sprite;
		struct {
			int x;
			int y;
			int y2;
		} coord;
		int speed;
		double distance;
	} bullet;

	struct {
		double startGame;
		double total;
		double delta;
		double scoreFreeze;
		double killMessage;
		double deadMessage;
	} time;

	// power, что можно подобрать
	struct {
		SDL_Surface* sprite;
		struct {
			int x;
			int y;
		} coord;
		double time;
	} power;
	
	double totalDistance;
	double score;
	bool pause;
	int coeff = 2;
};


struct CarInfo {
	SDL_Surface* car;
	struct Coord {
		int x;
		int y;
	} coord;
	
	double speed;
	bool isEnemy;
	int colorIndex;
};


struct SDL {
	SDL_Event event;
	SDL_Surface* screen;
	SDL_Surface* charset;
	SDL_Surface* cars[6];
	SDL_Surface* playerCars[6];
	SDL_Surface* powerup[2];
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
};


int initGame(SDL* sdl);


// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);


// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);


// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor);


// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset);


// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);


void DrawDest(Game* game, SDL* sdl, int* roadMarkingPos);


void DrawRoadRectangle(SDL_Surface* screen, int y);


void DrawHeader(SDL_Surface* screen, Game game, SDL sdl, double fps);


int DrawPlayer(Game* game, SDL* sdl);


void DrawBullet(CarInfo* cars, Game* game, SDL* sdl);


void DrawRandomCar(CarInfo* cars, Game* game, SDL* sdl);


void DrawRandomPower(CarInfo* cars, Game* game, SDL* sdl);


void NewGame(Game* game, CarInfo* cars);


void SaveGame(Game* game, CarInfo* cars, SDL* sdl, char savedGames[10][20]);

void LoadGame(Game* game, CarInfo* cars, SDL* sdl, char fileName[20]);

void ShowSavedGames(Game* game, CarInfo* cars, SDL* sdl, char savedGames[10][20]);

void GetFileName(Game* game, CarInfo* cars, SDL* sdl, char savedGames[10][20]);


void SpawnPlayer(Game* game, CarInfo* cars);


// check if X and Y coordinates are inside a screen
bool inArray(int x, int y);


// Check ifhorizontal position is out of screen and do it in the range.
void fixCoordX(int* horizontal, Game *game);


// Check if vertical position is out of screen and do it in the range.
void fixCoordY(int* vertical);


// x,y - координата на периметре машины; x2, y2 - центр объекта
bool numbersInArray(int x, int y, CarInfo* object);


// Проверка для пикселей по периметру машины, находится ли позиция в другом объекте.
bool touchObject(Game* game, CarInfo* object, const double deltaTime, CarInfo *cars, SDL* sdl);


bool isDestroyed(CarInfo* car, SDL* sdl);



bool isFreePlace(CarInfo* car, CarInfo* cars, int turn);


double canAttack(CarInfo* car, Game* game, CarInfo* cars);


bool canRide(CarInfo* car, CarInfo* cars);


bool freeSpace(CarInfo* car, CarInfo* cars);


bool inFault(int num1, int num2, int fault);


int modul(int num);


bool canGo(CarInfo* car, CarInfo* cars, int direction);


bool canSpawn(Game* game, CarInfo* cars);


void addBullet(Game* game);


int carIsKilled(Game* game, CarInfo* cars, SDL* sdl, int y);


void RenderSurfaces(SDL* sdl);


void FreeSurfaces(SDL sdl);

void PauseGame(Game* game, CarInfo* cars, SDL* sdl);

void FinishGame(Game* game, SDL* sdl);

void changeCoeff(bool isDecrement, Game *game);