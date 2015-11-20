/*
 *  Conway's game of life, implemented in C
 *  Output rendered in BearlibTerminal.
 *  v0.1 David Bergeron
 *  sceptredata@gmail.com
 */
#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "BearLibTerminal.h"

#define CELL(X, Y) (arr[size * (X)+(Y)])

#define GAME_SIZE 20

int init_grid[GAME_SIZE * GAME_SIZE] = {0};
int init_t_grid[GAME_SIZE * GAME_SIZE] = {0};

int count_alive( int *arr, int i, int j, int size );
void evolve_grid( int *arr, int *tmp_arr, int size );
void display_grid( int *arr, int size );

int main( int argc, char *argv[] )
{
        FreeConsole();

        int i, j, mx, my, gens;
        int *grid, *ticked_grid, *tmp;
        char buffer[4] = {0};

        if (argc > 1) {
                gens = atoi(argv[1]);
        } else {
                gens = 10; //Replace with Bearlibterminal function.
        }

        grid = init_grid;
        ticked_grid = init_t_grid;

        terminal_open();
        terminal_set("window: size=50x25, cellsize=auto, title='Game of Life'; font: default");
        
        mx = -1, my = -1;
        
        int in_grid = 0;
        int running = 1;
        while (running) {
                // Set up strings.
                terminal_color(color_from_name("white"));
                terminal_print( 1, 1, "Conway's Life");
                terminal_printf( 1, 3, "Mouse X: [color=orange]%d[/color] ", mx);
                terminal_printf( 1, 4, "Mouse Y: [color=orange]%d[/color] ", my);
                terminal_print(1, 6, "([color=yellow]N[/color])ew game");
                terminal_printf(1, 7, "([color=yellow]G[/color])enerations: %d ", gens);
                terminal_print(1, 9, "([color=yellow]R[/color])un game");
                terminal_print(1, 10, "([color=yellow]S[/color])ingle step");
                terminal_print(1, 15, "([color=red]Q[/color])uit");

                // Draw Grid
                display_grid( grid, GAME_SIZE );

                mx = terminal_state(TK_MOUSE_X);
                my = terminal_state(TK_MOUSE_Y);
                
                if ( (mx >= 20) && (mx <= 40) && (my > 0) && (my <= 21) ) {
                        mx = mx - 20;
                        my = my - 1;
                        in_grid = 1;
                } else {
                        mx = -1;
                        my = -1;
                        in_grid = 0;
                }

                terminal_refresh();

                do {
                        int code = terminal_read();

                        if (code == TK_ESCAPE || code == TK_CLOSE || code == TK_Q) {
                                 running = 0;
                        }
                        else if ( code == TK_MOUSE_LEFT ) {
                                if (in_grid) { 
                                        grid[GAME_SIZE * mx + my] = !grid[GAME_SIZE * mx + my];
                                }
                        }
                        else if ( code == TK_N ) {
                                for (i = 0; i < GAME_SIZE * GAME_SIZE; i++) {
                                        grid[i] = 0;
                                }
                        }
                        else if ( code == TK_G ) {
                                terminal_clear_area(15, 7, 4, 1);
                                terminal_read_str(16, 7, buffer, 3);
                                gens = atoi(buffer);
                        }
                        else if ( code == TK_R ) {
                                for ( i = 0; i < gens; i++) {
                                        evolve_grid( grid, ticked_grid, GAME_SIZE);
                                        grid = ticked_grid;
                                        display_grid( grid, GAME_SIZE );
                                        terminal_refresh();
                                        terminal_delay( 100 );
                                }
                        }
                        else if ( code == TK_S ) {
                                evolve_grid( grid, ticked_grid, GAME_SIZE );
                                grid = ticked_grid;
                        }
                } while (running && terminal_has_input());

        }
        return 0;
}

/* count_alive() steps: Nested for loop to move through array
 * if center square, continue.
 * Check if square in grid range.
 * if square is alive, add to counter.
 */
int count_alive( int *arr, int i, int j, int size )
{
        int x, y, num_alive = 0;
        for ( y = j - 1; y <= j + 1; y++) {
                for ( x = i - 1; x <= i + 1; x++ ) {
                        if ( ( x == i ) && ( y == j ) ) continue;
                        if ( ( x < size ) && ( y < size ) && ( x >= 0 ) && ( y >= 0 ) ) {
                                if (CELL(x, y) == 1) {
                                        num_alive += 1;
                                }
                        }
                }
        }
        return num_alive;
}

void evolve_grid( int *arr, int *tmp_arr, int size )
{
        int x, y, num_alive;

        for ( y = 0; y < size; y++) {
                for ( x = 0; x < size; x++ ) {
                        num_alive = count_alive( arr, x, y, size );
                        if ( CELL( x, y ) == 1 ) { // If cell already alive
                                if ( (num_alive > 3) || (num_alive < 2) ) {
                                        tmp_arr[size * x + y] = 0; // Dead Cell
                                } else {
                                        tmp_arr[size * x + y] = 1; // Live Cell
                                }
                        } else { // If cell is already dead.
                                if (num_alive == 3) {
                                        tmp_arr[size * x + y] = 1; // Live Cell
                                } else {
                                        tmp_arr[size * x + y] = 0; // Dead Cell
                                }
                        }
                }
        }
}

void display_grid( int *arr, int size)
{
        int i, j;
        for (j = 0; j < size; j++) {
                        for (i = 0; i < size; i++) {
                            if ( arr[size * i + j] == 1 ) {
                                    terminal_color(color_from_name("orange"));
                                    terminal_put( i + 20, j + 1 , 0x2588 );
                            } else {
                                    terminal_color(color_from_name("white"));
                                    terminal_put( i + 20, j + 1 , 0x2588);
                            }
                        }
                }
}

