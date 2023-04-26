game:game.c
	gcc game.c functions.c -o game -g -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer 
game.o:game.c
	gcc -c game.c -g

functions.o:functions.c
	gcc -c functions.c -g

