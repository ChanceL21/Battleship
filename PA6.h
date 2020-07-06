/*
Name: Chance Lewis
Date: 10/24/19
Description: Battleship
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


// struct used to contain the ending statistics of a player based on their in-game performance
typedef struct statistics
{
	int hits;
	int misses;
	int shots;
	double percentage;
	char winner_loser[8];

} Statistics;


/*
Description: prints menu to user
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: void
Preconditions: none
Postconditions: none
*/
void display_menu(void);

/*
Description: prompts user for option correspond to menu selection
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: int
Preconditions: none
Postconditions: none
*/
int get_option(void);

/*
Description: determines if menu option is satisfactory. Returns 1 if so, and 0 otherwise
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int
Return Type: int
Preconditions: none
Postconditions: none
*/
int evaluate_option(int option);

/*
Description: Continuously displays menu until proper option is input
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: int
Preconditions: none
Postconditions: none
*/
int menu_select(void);

/*
Description: Prints Battleship rules to user
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: void
Preconditions: none
Postconditions: none
*/
void display_rules(void);

/*
Description: sets both player's boards as 10x10 of '-'
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, char array
Return Type:
Preconditions: arrays must be 10x10 2 dimensional array
Postconditions: arrays modified
*/
void initialize_grids(char player1_grid[10][10], char player2_grid[10][10]);

/*
Description: shows both boards, computer then user. computer ships are hidden
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, char array
Return Type: void
Preconditions: none
Postconditions: none
*/
void display_grids(char player1_grid[10][10], char player2_grid[10][10]);

/*
Description: displays a single board. computer ships are hidden based on player integer supplied as second parameter (1 or 2)
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, int
Return Type: void
Preconditions: none
Postconditions: none
*/
void display_grid(char player_grid[10][10], int player);

/*
Description: Handles main gameplay of Battleship
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, int
Return Type: void
Preconditions: none
Postconditions: creates/overwrites battleship.log file 
*/
void play_game(void);

/*
Description: returns 0 or 1 randomly
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: int
Preconditions: none
Postconditions: none
*/
int binary_generator(void);

/*
Description: lets user place their ships by coordinates
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, char array, char array, char array, int array
Return Type: void
Preconditions: none
Postconditions: user board edited to contain ships
*/
void manually_place_ships(char player1_grid[10][10], char player2_grid[10][10], char ship_names[][15], char ship_symbols[5], int ship_lengths[5]);

/*
Description: allows computer and player (if they so desire) to randomly place ships on board
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, char array, int array
Return Type: void
Preconditions: none
Postconditions: boards updated
*/
void randomly_place_ships(char player_grid[10][10], char ship_symbols[5], int ship_lengths[5]);

/*
Description: prompts user if they want to manually place ships. returns their input
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: char
Preconditions: none
Postconditions: keyboard input scanned once for character
*/
char random_or_manual(void);

/*
Description: returns 1 if input is either 'y' or 'n' for yes or no. returns 0 otherwise
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char
Return Type: int
Preconditions: none
Postconditions: none
*/
int evaluate_yes_no(char yes_no);

/*
Description: returns random integer from 0 to 9, inclusively
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: int
Preconditions: none
Postconditions: none
*/
int coordinate_generator(void);

/*
Description: converts letter chosen to row index on board. For example, parameter A returns 0. if the character has an ascii above 90, then it is reduced by 32 first, so that lower case a also returns 0 (and so on)
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char
Return Type: int
Preconditions: none
Postconditions: none
*/
int letter_to_number(char letter);

/*
Description: evaluates if the user ship placing coordinates (2 sets total) are satisfactory. Returns 1 if so, and 0 otherwise. Prints out relevant statements to user as well
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int, int, int, int, char array, int
Return Type: int
Preconditions: none
Postconditions: none
*/
int evaluate_coordinates(int row1, int col1, int row2, int col2, char player_grid[10][10], int ship_length);

/*
Description: Places ship designated by either user or computer since already validated
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type:
Return Type:
Preconditions: none
Postconditions: none
*/
void place_ship(int row1, int col1, int row2, int col2, char player_grid[10][10], char symbol);

/*
Description: Same as evaluate_coordinates except does not check if out of bounds (since not possible to be generated) and does not print out anything. also does not need ship length
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int, int, int, int, char array, int
Return Type: 
Preconditions: none
Postconditions: none
*/
int evaluate_coordinates_no_print(int row1, int col1, int row2, int col2, char player_grid[10][10]);

/*
Description: Lets computer take turn after user input. Firing is random. Records game transcript and statistics
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, char array, FILE*, int*, int*
Return Type: void
Preconditions: none
Postconditions: user's board is updated with hit or miss, and battleship.log is updated
*/
void computer_turn(char player1_grid[10][10], char player2_grid[10][10], FILE * outfile, int* hits2, int* misses2);

/*
Description: lets player take their turn and chose target cell. Records game transcript and statistics
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array, char array, FILE*, int*, int*
Return Type: void
Preconditions: none
Postconditions: computer's board is updated with hit or miss, and battleship.log is updated
*/
void player_turn(char player1_grid[10][10], char player2_grid[10][10], FILE* outfile, int* hits1, int* misses1);

/*
Description: checks if the ships in a board are destroyed. returns 1 if all are destroyed, and 0 otherwise
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: char array
Return Type: int
Preconditions: none
Postconditions: none
*/
int ships_destroyed(char player_grid[10][10]);

/*
Description: determines if user target cell is a proper strike. returns 1 if so, and 0 otherwise. prints out relevant information
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int, int, char array, char array, FILE*, int*, int*
Return Type:
Preconditions: none
Postconditions: battleship.log updated if target is satisfactory
*/
int evaluate_coordinate(int row, int col, char player1_grid[10][10], char player2_grid[10][10], FILE* outfile, int* hits1, int* misses1);

/*
Description: computer version of evaluate_coordinate. requires less validation. returns 1 if its a proper strike, and 0 otherwise. no printf
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int, int, char array, char array, FILE*, int*, int*
Return Type:
Preconditions: none
Postconditions: battleship.log updated if target is satisfactory
*/
int evaluate_coordinate_no_print(int row, int col, char player1_grid[10][10], char player2_grid[10][10], FILE* outfile, int* hits2, int* misses2);

/*
Description: Checks if enemy ship is hit. If so, it checks if there are anymore of that ship type on enemy board. If not, then it prints that the ship is sunk.
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int, int, char, char array, char array, char array, FILE*
Return Type: void
Preconditions: none
Postconditions: prints to battleship.log that the ship was sunk
*/
void ship_hit_and_destroyed(int row, int col, char symbol, char ship_name[], char player1_grid[10][10], char player2_grid[10][10], FILE* outfile);

/*
Description: Prompts user to enter a non-whitespace character to continue. Makes computer gameplay more interactive.
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: void
Return Type: void
Preconditions: none
Postconditions: 1 keyboard character scanned
*/
void input_to_progress(void);

/*
Description: computer version of ship_hit_and_destroyed. Prints out corresponding information
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: int, int, char, char array, char array, char array, FILE*
Return Type: void
Preconditions: none
Postconditions: prints to battleship.log that the ship was sunk
*/
void ship_hit_and_destroyed_no_print(int row, int col, char symbol, char ship_name[], char player1_grid[10][10], char player2_grid[10][10], FILE* outfile);

/*
Description: prints the parameter statistics entered to the outfile parameter
Date Created: 10/23/2019
Date Modified: 10/25/2019
Parameter Type: FILE*, Statistics (int, int, double, char array)
Return Type: void
Preconditions: none
Postconditions: fprintf stats to outfile
*/
void print_stats_to_file(FILE* outfile, Statistics player_stats);