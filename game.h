#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define SPRITE_SIZE 32

typedef struct {
    SDL_Surface *image[11];
    SDL_Rect rect;
    float velocity_x;
    float acceleration_x;
    float velocity_y;
    float acceleration_y;
    float jump_velocity;
    bool is_jumping;
    float sprint_multiplier;
    int frame;
    int animation_ticks;
    int animation_type;
    int health;
    int score;
    int numplayer;
    int attack;
    int exframe;
   
} Character;

void initializeCharacter(Character* character);
void initializeSDL();
void createWindow(SDL_Surface** screen);

void handleEvents(Character* character,Character* character1, SDL_Event event);

void applyGravity(Character* character);

void updatePosition(Character* character);

void keepOnScreen(Character* character);

void clearScreen(SDL_Surface *screen);

void drawCharacter(Character *character, SDL_Surface *screen);

void updateScreen(SDL_Surface *screen);

void animateJump(Character* character);

void animateLeft(Character* character);

void animateRight(Character* character);

void cleanupSDL(Character *character);
void updateAnimation(Character* character);



void score(Character* character,Character * character1, SDL_Surface * screen, TTF_Font* font);
void damagePlayer(Character* player, SDL_Event event);





