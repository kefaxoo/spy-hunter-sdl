#define _USE_MATH_DEFINES
#include "SpyHunter.h"
#define SECONDS_BETWEEN_REFRESH 0.5
#define REFRESH_RATE 1/SECONDS_BETWEEN_REFRESH

/*
ПЛАН НА 10.01
- пересмотреть функции, убрать все ненужное
- доделать сохранение игры в файл (считывание)

БАГИ:


Изменить:


Недочеты:
- при паузе пуля продолжает лететь и может убить авто


Проверить:
- пофиксить спавн PowerUp'а
- при сбитии вражеского авто, которое уперлось (снизу) в другое авто, оно отодвигается не в ту сторону
- когда 2 атакующих авто прилегают друг к другу, они не атакуют
*/


int main(int argc, char** argv) {
	SDL sdl = { NULL };
	Game game = { NULL };
	CarInfo cars[5];
	char savedGames[10][20];
	for (int i = 0; i < 10; i++)
		savedGames[i][0] = '\0';
	if (initGame(&sdl)) return 1;
	// initialize the game
	NewGame(&game, cars);
	for (int i = 0; i < ENEMIES; i++) {
		cars[i].car = sdl.cars[0];
		cars[i].colorIndex = 0;
	}
	int timeStart = game.time.startGame, timeEnd, quit = 0, frames = 0, roadMarkingPos = SCREEN_HEIGHT - 100;
	double fpsTimer = 0, fps = 0;

	while (!quit) {
		// Fill window with green color;
		if (!game.pause) SDL_FillRect(sdl.screen, NULL, SDL_MapRGB(sdl.screen->format, 107, 142, 35));
		DrawDest(&game, &sdl, &roadMarkingPos);
		// info text
		
		timeEnd = SDL_GetTicks();
		game.time.delta = (timeEnd - timeStart) * 0.001;
		timeStart = timeEnd;
		if (!game.pause) {
			game.time.total += game.time.delta;

			if (game.time.scoreFreeze > 0) game.time.scoreFreeze -= game.time.delta;
			if (game.time.scoreFreeze < 0) game.time.scoreFreeze = 0;
			if (game.time.killMessage > 0) game.time.killMessage -= game.time.delta;
			if (game.time.deadMessage > 0) game.time.deadMessage -= game.time.delta;
			if (game.player.power.time > 0) game.player.power.time -= game.time.delta;

			game.totalDistance += game.time.delta - game.player.speed * game.time.delta;
			// WARN - моэно поменять; дефолтно скорость 0, ибо авто не едет
			if (!game.time.scoreFreeze) game.score += modul(game.player.speed) * game.totalDistance * game.time.delta / 2;

			game.player.coord.y += (game.player.speed > 0 ? 400 : 100) * game.time.delta * game.player.speed;
			fixCoordY(&game.player.coord.y);
			game.player.coord.x += game.player.turn * game.time.delta * 300;
			fixCoordX(&game.player.coord.x);
		}
			DrawBullet(cars, &game, &sdl);
			DrawRandomPower(cars, &game, &sdl);
			DrawRandomCar(cars, &game, &sdl);
			// draw player
			if (DrawPlayer(&game, &sdl)) {
				NewGame(&game, cars);
				game.time.deadMessage = 2;
			}
		//DrawSurface(sdl.screen, game.player.player, game.player.coord.x, game.player.coord.y);
		DrawHeader(sdl.screen, game, sdl, fps);

		fpsTimer += game.time.delta;
		if (fpsTimer > SECONDS_BETWEEN_REFRESH) {
			fps = frames * REFRESH_RATE;
			frames = 0;
			fpsTimer -= SECONDS_BETWEEN_REFRESH;
		};
		
		RenderSurfaces(&sdl);
		
		// handling of events (if there were any)
		while (SDL_PollEvent(&sdl.event)) {
			switch (sdl.event.type) {
			case SDL_KEYDOWN:
				if (sdl.event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (sdl.event.key.keysym.sym == SDLK_n)
					NewGame(&game, cars);
				else if (sdl.event.key.keysym.sym == SDLK_p) {
					game.pause = !game.pause;
					PauseGame(&game, cars, &sdl);
					timeStart = SDL_GetTicks();
				}
				else if (sdl.event.key.keysym.sym == SDLK_s) SaveGame(&game, cars, &sdl, savedGames);
				else if (sdl.event.key.keysym.sym == SDLK_l) {
					ShowSavedGames(&game, cars, &sdl, savedGames);
					timeStart = SDL_GetTicks();
				}
				else if (sdl.event.key.keysym.sym == SDLK_f) {
					game.pause = !game.pause;
					system("cls");
					printf("Finish game\n");
					printf("Total time: ");
					printf("%f", game.time.total);
					printf("\nScore: ");
					printf("%f", game.score);
					quit = 1;
				}
				else if (sdl.event.key.keysym.sym == SDLK_UP) game.player.speed = -1;
				else if (sdl.event.key.keysym.sym == SDLK_DOWN) game.player.speed = 1;
				else if (sdl.event.key.keysym.sym == SDLK_LEFT) game.player.turn = -1;
				else if (sdl.event.key.keysym.sym == SDLK_RIGHT) game.player.turn = 1;
				else if (sdl.event.key.keysym.sym == SDLK_SPACE) if (!game.pause) addBullet(&game);
				break;
			case SDL_KEYUP:
				if (sdl.event.key.keysym.sym == SDLK_UP) game.player.speed = 0;
				else if (sdl.event.key.keysym.sym == SDLK_DOWN) game.player.speed = 0;
				else if (sdl.event.key.keysym.sym == SDLK_LEFT) game.player.turn = 0;
				else if (sdl.event.key.keysym.sym == SDLK_RIGHT) game.player.turn = 0;
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		frames++;
	};

	// freeing all surfaces
	FreeSurfaces(sdl);
	return 0;
};
