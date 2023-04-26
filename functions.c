#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include "game.h"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define SPRITE_SIZE 32


void initializeSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
}

void createWindow(SDL_Surface** screen)
{
    *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
}

void initializeCharacter(Character* character) {
    character->image[0] = IMG_Load("jump1.png");
    character->image[1] = IMG_Load("jump2.png");
    character->image[2] = IMG_Load("jump3.png");
    character->image[3] = IMG_Load("left1.png");
    character->image[4] = IMG_Load("left2.png");
    character->image[5] = IMG_Load("left3.png");
    character->image[6] = IMG_Load("right1.png");
    character->image[7] = IMG_Load("right2.png");
    character->image[8] = IMG_Load("right3.png");
    character->image[9] = IMG_Load("attack1.png");
    character->image[10] = IMG_Load("attack2.png");
    character->rect.x = SCREEN_WIDTH / 2 - SPRITE_SIZE / 2;
    character->rect.y = SCREEN_HEIGHT - SPRITE_SIZE * 3;
    character->velocity_x = 0;
    character->acceleration_x = 0.1;
    character->velocity_y = 0;
    character->acceleration_y = 0.2;
    character->jump_velocity = -5;
    character->is_jumping = false;
    character->sprint_multiplier = 2.0f;
    character->frame = 6;
    character->animation_ticks = 0;
    character->health = 3;
    character->score = 0; 
}

void handleEvents(Character* character,Character* character1, SDL_Event event) {
  

    while (SDL_PollEvent(&event)) {
	damagePlayer(character1, event);
       damagePlayer(character, event);
        if (event.type == SDL_QUIT) {
            SDL_FreeSurface(character->image[0]);
            SDL_FreeSurface(character->image[1]);
            SDL_FreeSurface(character->image[2]);
            SDL_FreeSurface(character->image[3]);
            SDL_FreeSurface(character->image[4]);
            SDL_FreeSurface(character->image[5]);
            SDL_FreeSurface(character->image[6]);
            SDL_FreeSurface(character->image[7]);
            SDL_FreeSurface(character->image[8]);
	    SDL_FreeSurface(character->image[9]);
	    SDL_FreeSurface(character->image[10]);
            
            IMG_Quit();
            SDL_Quit();
            exit(0);
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                character->velocity_x = -2 * (event.key.keysym.mod & KMOD_SHIFT ? character->sprint_multiplier : 1.2f);
                
                break;
            case SDLK_RIGHT:
                character->velocity_x = 2 * (event.key.keysym.mod & KMOD_SHIFT ? character->sprint_multiplier : 1.2f);
            
                break;
            case SDLK_a:
            	character1->velocity_x = -2 * (event.key.keysym.mod & KMOD_RALT ? character->sprint_multiplier : 1.2f);
                
                break;
             case SDLK_x:
                        if (character->attack !=1) {
                            character->attack = 1;
                        }
                        break;
            case SDLK_d:
                character1->velocity_x = 2 * (event.key.keysym.mod & KMOD_RALT ? character->sprint_multiplier : 1.2f);
            
                break;
            	
            case SDLK_SPACE:
                if (!character->is_jumping) {
                    character->velocity_y = character->jump_velocity;
                    character->is_jumping = true;
                    character->animation_ticks = 0;

                }
                break;
            case SDLK_z:
                if (!character1->is_jumping) {
                    character1->velocity_y = character1->jump_velocity;
                    character1->is_jumping = true;
                    character1->animation_ticks = 0;

                }
                break;
            case SDLK_f:
                character->score +=1;
                break;
            case SDLK_n:
                character1->score +=1;
                break;
            case SDLK_ESCAPE:
                SDL_FreeSurface(character->image[0]);
                SDL_FreeSurface(character->image[1]);
                SDL_FreeSurface(character->image[2]);
                SDL_FreeSurface(character->image[3]);
                SDL_FreeSurface(character->image[4]);
                SDL_FreeSurface(character->image[5]);
                SDL_FreeSurface(character->image[6]);
                SDL_FreeSurface(character->image[7]);
                SDL_FreeSurface(character->image[8]);
                SDL_FreeSurface(character->image[9]);
                
	    SDL_FreeSurface(character->image[10]);
                SDL_FreeSurface(character1->image[0]);
                SDL_FreeSurface(character1->image[1]);
                SDL_FreeSurface(character1->image[2]);
                SDL_FreeSurface(character1->image[3]);
                SDL_FreeSurface(character1->image[4]);
                SDL_FreeSurface(character1->image[5]);
                SDL_FreeSurface(character1->image[6]);
                SDL_FreeSurface(character1->image[7]);
                SDL_FreeSurface(character1->image[8]);
                SDL_FreeSurface(character1->image[9]);
                 SDL_FreeSurface(character1->image[10]);
                
                IMG_Quit();
                SDL_Quit();
                exit(0);
                break;
            }
        }
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT:
         
                if (!SDL_GetKeyState(NULL)[SDLK_LEFT] && !SDL_GetKeyState(NULL)[SDLK_RIGHT]) {
                    character->velocity_x = 0;
                }
                break;
          case SDLK_a:
            case SDLK_d:
            
                if (!SDL_GetKeyState(NULL)[SDLK_a] && !SDL_GetKeyState(NULL)[SDLK_d]) {
                    character1->velocity_x = 0;
                }
                break;
            }
        }
        else {
      
            if (!SDL_GetKeyState(NULL)[SDLK_LEFT] && !SDL_GetKeyState(NULL)[SDLK_RIGHT]) {
                character->velocity_x = 0;
            }
            if (!SDL_GetKeyState(NULL)[SDLK_a] && !SDL_GetKeyState(NULL)[SDLK_d]) {
                character1->velocity_x = 0;
            }
        }

        
    }
}






void applyGravity(Character* character) {
    character->velocity_y += character->acceleration_y;
    
}
void updatePosition(Character* character) {
    character->rect.x += (int)character->velocity_x;
    character->rect.y += (int)character->velocity_y;
}
void keepOnScreen(Character* character) {
    if (character->rect.x < 0) {
        character->rect.x = 0;
    } else if (character->rect.x > SCREEN_WIDTH - SPRITE_SIZE) {
        character->rect.x = SCREEN_WIDTH - SPRITE_SIZE;
    }
    if (character->rect.y > SCREEN_HEIGHT - SPRITE_SIZE*3) {
        character->rect.y = SCREEN_HEIGHT - SPRITE_SIZE*3;
        character->velocity_y = 0;
        character->is_jumping = false;
         
    }
}
void clearScreen(SDL_Surface *screen) {
    SDL_FillRect(screen, NULL, 0);
}

void drawCharacter(Character* character, SDL_Surface* screen) {

    SDL_Rect healthRect = { 10, 10, 0, 0 };
    
 
    for (int i = 0; i < character->health; i++) {
        SDL_Surface* heart = IMG_Load("heart.png");
        SDL_BlitSurface(heart, NULL, screen, &healthRect);
        healthRect.x += 32;
        SDL_FreeSurface(heart);
    }

   
    SDL_BlitSurface(character->image[character->frame], NULL, screen, &character->rect);
}

void updateScreen(SDL_Surface *screen) {
    SDL_Flip(screen);
}
void cleanupSDL(Character *character) {
    SDL_FreeSurface(character->image[0]);
    SDL_FreeSurface(character->image[1]);
    SDL_FreeSurface(character->image[2]);
    SDL_FreeSurface(character->image[3]);
    SDL_FreeSurface(character->image[4]);
    SDL_FreeSurface(character->image[5]);
    SDL_FreeSurface(character->image[6]);
    SDL_FreeSurface(character->image[7]);
    SDL_FreeSurface(character->image[8]);
    SDL_FreeSurface(character->image[9]);
    SDL_FreeSurface(character->image[10]);

   
    IMG_Quit();
    SDL_Quit();
}

void animateJump(Character* character) {
    character->animation_ticks++;

    if (character->animation_ticks < 5) {
        character->frame = 0;
    } else if (character->animation_ticks < 10) {
        character->frame = 1;
    } else {
        character->frame = 2;
    }

    if (character->animation_ticks == 15) {
        character->animation_ticks = 0;
    }

    if (character->velocity_y > 0) {
        character->frame = 2;
    } else if (character->velocity_y < 0) {
        character->frame = 0;
    }
  
}


void animateLeft(Character* character) {
    character->animation_ticks++;

    if (character->animation_ticks < 5) {
        character->frame = 3;
    } else if (character->animation_ticks < 10) {
        character->frame = 4;
    } else {
        character->frame = 5;
    }

    if (character->animation_ticks == 15) {
        character->animation_ticks = 0;
    }
}


void animateRight(Character* character) {
    character->animation_ticks++;

    if (character->animation_ticks < 5) {
        character->frame = 6;
    } else if (character->animation_ticks < 10) {
        character->frame = 7;
    } else {
        character->frame = 8;
    }

    if (character->animation_ticks == 15) {
        character->animation_ticks = 0;
    }
}

void attack(Character* character) {
    character->animation_ticks++;

   
    if (character->animation_ticks < 20) {
    	if ((character->frame == 6 )|| (character->frame == 7) || (character->frame == 8)){
        character->frame = 9;}
        else {character->frame = 10;}
        SDL_Delay(2);
    } else {
        
        character->attack =0;
	character->animation_ticks = 0;
    }

    
        
        
	
	    
}

void updateAnimation(Character* character) {
    if(character->attack == 1){attack(character);}
    else if(character->is_jumping) {
        animateJump(character);
    } else if (character->velocity_x < 0) {
        animateLeft(character);
    } else if (character->velocity_x > 0) {
        animateRight(character);
    } else {
       if ((character->frame == 6 )|| (character->frame == 7) || (character->frame == 8)|| (character->frame == 10)){
        character->frame = 6;}
        else if ((character->frame == 3 )|| (character->frame == 4) || (character->frame == 5) || (character->frame == 9)) {character->frame = 3;}
        else{character->frame = 6;}
    }
}


void score(Character* character,Character * character1, SDL_Surface * screen, TTF_Font* font){
	SDL_Rect scorep1;
	SDL_Rect scorep2;
	char str[15];
        sprintf(str, "%d", character->score);
        char strs[15];
        sprintf(strs, "%d", character1->score);
	SDL_Color foregroundColor = { 125, 0, 0 };
	SDL_Surface* textSurface= TTF_RenderText_Solid(font, str, foregroundColor);
	SDL_Surface* textSurface1= TTF_RenderText_Solid(font, strs, foregroundColor);
	scorep1.x = 10;
	scorep1.y = 50;
	scorep2.x = 950;
	scorep2.y = 50;
	SDL_BlitSurface(textSurface, NULL, screen, &scorep1);
	SDL_BlitSurface(textSurface1, NULL, screen, &scorep2);
	
	
}
void damagePlayer(Character* character, SDL_Event event) {
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_l) {
        character->health--;
        printf("Player %d damaged, health: %d\n", character->numplayer, character->health);
    }
}

