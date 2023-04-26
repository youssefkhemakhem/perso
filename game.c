#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include "game.h"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define SPRITE_SIZE 32
/**  
* @file main.c  
* @brief Testing Program.  
* @author C Team  
* @version 0.1  
* @date Apr 01, 2015  
*  
* Testing program for movements  *  
*/
bool is_attacking;
int main(int argc, char *argv[]) {
    initializeSDL();
    TTF_Font* font = TTF_OpenFont("fonty.ttf", 25);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    SDL_Surface *screen;
    createWindow(&screen);
    SDL_Event event;
    bool is_two_player_mode = false; // Set to false for one player mode

    SDL_Surface* player1_screen = NULL;
    SDL_Surface* player2_screen = NULL;
    Character player1, player2;
    initializeCharacter(&player1);
    if (is_two_player_mode) {
        player1_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH / 2, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
        player2_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH / 2, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
        player1.numplayer = 1;
        player2.numplayer = 2;
        initializeCharacter(&player2);
        player2.rect.x = SCREEN_WIDTH / 2 + SPRITE_SIZE / 2;
    } else {
        player1_screen = screen;
        player1.numplayer = 1;
        player2.numplayer = 0;
    }

    while (1) {
        SDL_Delay(5);

        handleEvents(&player1, &player2, event);
        damagePlayer(&player1, event);
        if (is_two_player_mode) {
            damagePlayer(&player2, event);
        }

        applyGravity(&player1);
        updatePosition(&player1);
        keepOnScreen(&player1);
        
        updateAnimation(&player1);
        

        if (is_two_player_mode) {
            applyGravity(&player2);
            updatePosition(&player2);
            keepOnScreen(&player2);
            updateAnimation(&player2);
        }

        clearScreen(screen);

        // Draw player 1 onto the first surface
        SDL_FillRect(player1_screen, NULL, SDL_MapRGB(player1_screen->format, 0, 0, 0));
        drawCharacter(&player1, player1_screen);

        if (is_two_player_mode) {
            // Draw player 2 onto the second surface
            SDL_FillRect(player2_screen, NULL, SDL_MapRGB(player2_screen->format, 0, 0, 0));
            drawCharacter(&player2, player2_screen);

            // Blit the two surfaces onto the main screen
            SDL_Rect player1_rect = {0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
            SDL_Rect player2_rect = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
            SDL_BlitSurface(player1_screen, NULL, screen, &player1_rect);
            SDL_BlitSurface(player2_screen, NULL, screen, &player2_rect);
        } else {
            // Draw player 1 onto the main screen
            drawCharacter(&player1, screen);
        }

        // Draw score onto the main screen
        score(&player1, &player2, screen, font);
	player1.exframe = player1.frame;
	player2.exframe = player2.frame;
        // Update the screen
        SDL_Flip(screen);
    }

    // Free surfaces
    SDL_FreeSurface(player1_screen);
    SDL_FreeSurface(player2_screen);

    cleanupSDL(&player1);
    TTF_CloseFont(font);
    return 0;
}
