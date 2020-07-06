#include "PA6.h"


void display_menu(void) // printing menu
{
	printf("Welcome to Battleship!\n");
	printf("1. Print Game Rules\n");
	printf("2. Start a Game of Battleship\n");
	printf("3. Exit\n");
}

int get_option(void) // prompts for option once
{
	int option = 0;
	printf("Please enter the number corresponding with the desired action: ");
	scanf("%d", &option);
	return option;
}

int evaluate_option(int option) // makes sure option is between 1 and 3, inclusively
{
	if (option < 1 || option > 3)
	{
		printf("That's not a valid option!\n");
		return 0; // not satisfied
	}
	else
	{
		return 1; // satisfied
	}
}

int menu_select(void) // continues to display menu until option requirements are satisfied
{
	int option = 0;
	do {
		display_menu();
		option = get_option();
		system("cls");
	} while (!evaluate_option(option));
	return option;
}


void display_rules(void) // shows battleship rules
{
	printf("Battleship is a two player Navy game. The objective of the game is to sink all ships in your enemy's fleet.\n"
		"The Player to sink his/her enemy's fleet first wins. Both players' fleets consist of 5 ships that are hidden from the enemy.\n"
		"Each ship may be differentiated by its size (besides the Cruiser and Submarine) or number of cells it expands on the game board.\n"
		"The Carrier has 5 cells, Battleship has 4 cells, Cruiser has 3 cells, Submarine has 3 cells, and the Destroyer has 2 cells.\n"
		"Each player has a 10x10 board to place their ships, either vertically or horizontally. The rows are marked alphabetically (A-J),\n"
		"While the columns are marked numerically (0-9). Ships are placed and targeted based on this coordinate system. Once ships are\n"
		"placed, each player takes shots at the other's fleet one at a time. When a player sinks an enemy ship, they are alerted of which\n" 
		"ship was sunk. The game continues until a player's fleet is destroyed. In this version of the game, the user (Player 1) plays\n"
		"against the computer (Player 2).\n\n");
}

void initialize_grids(char player1_grid[10][10], char player2_grid[10][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			player1_grid[i][j] = '-'; // both grids now filled with dashes
			player2_grid[i][j] = '-';
		}
	}
}

void display_grids(char player1_grid[10][10], char player2_grid[10][10])
{
	printf("Enemy Board:\n");  // printing enemy board first
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	display_grid(player2_grid, 2);

	printf("_____________________\n\n"); // line separator

	printf("Your Board:\n");
	printf("  0 1 2 3 4 5 6 7 8 9\n"); // then printing allied board
	display_grid(player1_grid, 1);
}


void display_grid(char player_grid[10][10], int player) // printing just a single board
{
	for (int i = 0; i < 10; i++)
	{
		printf("%c ", i + 65); // showing rows as letters
		for (int j = 0; j < 10; j++)
		{
			if (player_grid[i][j] != '*' && player_grid[i][j] != 'm')
			{
				if (player == 2) // if enemy board
				{
					printf("- "); // disguising ships as '-', but still showing hits and misses
				}
				else // if allied board, prints the real thing
				{
					printf("%c ", player_grid[i][j]);
				}
			}
			else
			{
				printf("%c ", player_grid[i][j]);
			}
		}
		printf("\n");
	}
}

void play_game(void)
{
	// initializing many starting values for later use
	Statistics player1_stats = { 0, 0, 0, 0.0, "Loser" };
	Statistics player2_stats = { 0, 0, 0, 0.0, "Loser" };

	FILE* outfile = NULL;

	outfile = fopen("battleship.log", "w");

	int user_victory = 0;

	int shots1 = 0, shots2 = 0, hits1 = 0, hits2 = 0, misses1 = 0, misses2 = 0;

	char player1_grid[10][10], player2_grid[10][10];

	// the following are set up in parallel arrays
	char ship_names[][15] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	char ship_symbols[5] = { 'c', 'b', 'r', 's', 'd' };
	int ship_lengths[5] = { 5, 4, 3, 3, 2 };


	int goes_first = 0;

	char manual = '\0';


	initialize_grids(player1_grid, player2_grid); // make grids all dashes

	display_grids(player1_grid, player2_grid); // dispaly them


	manual = random_or_manual(); //check if player play wants to manually or randomly set up their board


	if (manual == 'y')
	{
		manually_place_ships(player1_grid, player2_grid, ship_names, ship_symbols, ship_lengths);
	}
	else
	{
		randomly_place_ships(player1_grid, ship_symbols, ship_lengths);
	}
	randomly_place_ships(player2_grid, ship_symbols, ship_lengths); // computer sets up randomly regardless

	system("cls"); // always clearing screen before displaying board from here on out
	display_grids(player1_grid, player2_grid);

	goes_first = binary_generator();

	if (goes_first) // code for if the user goes first
	{
		while (1)
		{
			printf("It's your turn!\n");

			player_turn(player1_grid, player2_grid, outfile, &hits1, &misses1); // player takes their turn

			if (ships_destroyed(player2_grid)) // game ends and they win if computer's ships are destroyed
			{
				printf("Victory!\n\n");
				user_victory = 1;
				break;
			}

			computer_turn(player1_grid, player2_grid, outfile, &hits2, &misses2); // computer takes their turn


			if (ships_destroyed(player1_grid)) // checking to see if computer has won
			{
				printf("Defeat. Better luck next time!\n\n");
				break; // ending game if so
			}
		}
	}
	else // code for if the user goes second
	{
		while (1)
		{
			computer_turn(player1_grid, player2_grid, outfile, &hits2, &misses2);

			if (ships_destroyed(player1_grid))
			{
				printf("Defeat. Better luck next time!\n\n");
				break;
			}

			printf("It's your turn!\n");

			player_turn(player1_grid, player2_grid, outfile, &hits1, &misses1);

			if (ships_destroyed(player2_grid))
			{
				printf("Victory!\n\n");
				user_victory = 1;
				break;
			}
		}
	}
	
	if (user_victory) // both of these were intialized as Loser originally, so only need to update if they win
	{
		strcpy(player1_stats.winner_loser, "Winner");
	}
	else
	{
		strcpy(player2_stats.winner_loser, "Winner");
	}

	shots1 = hits1 + misses1;
	shots2 = hits2 + misses2;

	//updating stats

	player1_stats.hits = hits1;
	player1_stats.misses = misses1;
	player1_stats.shots = shots1;
	player1_stats.percentage = (hits1 / (double)shots1) * 100;

	player2_stats.hits = hits2;
	player2_stats.misses = misses2;
	player2_stats.shots = shots2;
	player2_stats.percentage = (hits2 / (double)shots2) * 100;
	
	//printing out stats to battleship.log

	fprintf(outfile, "\nPlayer 1 Statistics:\n");
	print_stats_to_file(outfile, player1_stats);
	fprintf(outfile, "\nPlayer 2 Statistics:\n");
	print_stats_to_file(outfile, player2_stats);

	fclose(outfile);

}


int binary_generator(void) //returns 0 or 1
{
	return rand() % 2;
}

void manually_place_ships(char player1_grid[10][10], char player2_grid[10][10], char ship_names[][15], char ship_symbols[5], int ship_lengths[5])
{

	char letter1 = '\0', letter2 = '\0', dash = '\0';
	int col1 = -1, col2 = -1, row1 = -1, row2 = -1, temp = 0; // setting up initializations at impossible values first

	for (int i = 0; i < 5; i++) // cycling through ship names, ship symbols, and ship lengths, which are set up in a parallel fashion.
	{
		system("cls");
		display_grids(player1_grid, player2_grid);

		do {
			printf("Please enter the first and last coordinates of the %s, which occupies %d cells, separated by a dash. Ex: B4-B%d or A1-%c1: ", ship_names[i], ship_lengths[i], ship_lengths[i] + 3, ship_lengths[i] + 65 - 1);
			scanf(" %c", &letter1);
			scanf("%d", &col1);
			scanf(" %c", &dash); //scanned and tossed
			scanf(" %c", &letter2);
			scanf("%d", &col2);
			row1 = letter_to_number(letter1);
			row2 = letter_to_number(letter2);

			if (row1 > row2) // swapping row1 and row2 if row1 is greater to make later coding easier
			{
				temp = row1;
				row1 = row2;
				row2 = temp;
			}

			if (col1 > col2) // same for columns
			{
				temp = col1;
				col1 = col2;
				col2 = temp;
			}

		} while (!evaluate_coordinates(row1, col1, row2, col2, player1_grid, ship_lengths[i])); // redoing the while loop if the coordinates are not satisfactory

		place_ship(row1, col1, row2, col2, player1_grid, ship_symbols[i]); // placing ships after above confirmation


	}


}

void randomly_place_ships(char player_grid[10][10], char ship_symbols[5], int ship_lengths[5])
{
	int row1 = 0, col1 = 0, row2 = 0, col2 = 0, ship_direction = 0;


	for (int i = 0; i < 5; i++)
	{
		do {

			row1 = coordinate_generator(); // gives number between 0 and 9, inclusively
			col1 = coordinate_generator();
			row2 = row1;
			col2 = col1;

			ship_direction = binary_generator();

			// row1 and row2 are equal, and col1 and col2 are equal. the ship length is then randomly added to either row2 or col2

			if (ship_direction)
			{
				col2 += ship_lengths[i] - 1;
			}
			else
			{
				row2 += ship_lengths[i] - 1;
			}

		} while (!evaluate_coordinates_no_print(row1, col1, row2, col2, player_grid)); // making sure new ship is on grid still (after ship addition) and that ships dont cross

		place_ship(row1, col1, row2, col2, player_grid, ship_symbols[i]); // placing once validated

	}

}

char random_or_manual(void) // input validation loop prompting user for manual or random
{
	char manual = '\0';
	do {
		printf("Would you like to manually place your ships? Enter 'y' for yes or 'n' for no: ");
		scanf(" %c", &manual);
	} while (!evaluate_yes_no(manual));

	return manual;
}

int evaluate_yes_no(char yes_no)
{
	if (yes_no == 'y' || yes_no == 'n')
	{
		return 1;
	}
	else
	{
		printf("That's not a valid input!\n");
		return 0;
	}
}

int coordinate_generator(void)
{
	return rand() % 10;
}

int letter_to_number(char letter) //converts row letter to row index
{
	if (letter > 90)
	{
		letter -= 32; // this makes it so that uppercase and lower case can be used interchangeably (B1 vs b1)
	}

	return letter - 65; // For example, A would return 0


}

int evaluate_coordinates(int row1, int col1, int row2, int col2, char player_grid[10][10], int ship_length) //making sure manual ship placement is satisfactory. printing out relevant statements
{
	int a[4] = { row1, col1, row2, col2 };

	for (int i = 0; i < 4; i++)
	{
		if (a[i] < 0 || a[i] > 9)
		{
			printf("That is not on the board!\n");
			return 0;
		}
	}

	if (row1 != row2 && col1 != col2)
	{
		printf("You can't place slanted ships!\n");
		return 0;
	}
	
	if (row1 == row2 && col1 == col2)
	{
		printf("A single coordinate can't be both the start and end!\n");
		return 0;
	}


	if (row2 == row1)
	{
		if (col2 - col1 != ship_length - 1)
		{
			printf("That ship is not the correct length!\n");
			return 0;
		}
		else
		{
			for (int i = col1; i <= col2; i++)
			{
				if (player_grid[row1][i] != '-')
				{
					printf("You can't lay a ship across another one!\n");
					return 0;
				}
			}
		}
	}

	if (col2 == col1)
	{
		if (row2 - row1 != ship_length - 1)
		{
			printf("That ship is not the correct length!\n");
			return 0;
		}
		else
		{
			for (int i = row1; i <= row2; i++)
			{
				if (player_grid[i][col1] != '-')
				{
					printf("You can't lay a ship across another one!\n");
					return 0;
				}
			}
		}
	}

	return 1; // if all above conditions are met, 1 is returned instead of 0, so good to go
}

void place_ship(int row1, int col1, int row2, int col2, char player_grid[10][10], char symbol) //places ship, already validated
{
	if (row2 == row1)
	{
		for (int i = col1; i <= col2; i++)
		{
			player_grid[row1][i] = symbol;
		}
	}

	if (col2 == col1)
	{
		for (int i = row1; i <= row2; i++)
		{
			player_grid[i][col1] = symbol;
		}
	}
}

int evaluate_coordinates_no_print(int row1, int col1, int row2, int col2, char player_grid[10][10]) // simplified version of evaluate_coordinates, for computer instead of user
{
	int a[4] = { row1, col1, row2, col2 };

	for (int i = 0; i < 4; i++)
	{
		if (a[i] < 0 || a[i] > 9)
		{
			return 0;
		}
	}

	if (row2 == row1)
	{
		for (int i = col1; i <= col2; i++)
		{
			if (player_grid[row1][i] != '-')
			{
				return 0;
			}
		}

	}

	if (col2 == col1)
	{
		for (int i = row1; i <= row2; i++)
		{
			if (player_grid[i][col1] != '-')
			{
				return 0;
			}
		}

	}

	return 1;
}

void computer_turn(char player1_grid[10][10], char player2_grid[10][10], FILE * outfile, int* hits2, int* misses2)
{
	input_to_progress();

	int row = 0, col = 0;

	do {
		row = coordinate_generator(); // random firing
		col = coordinate_generator();

	} while (!evaluate_coordinate_no_print(row, col, player1_grid, player2_grid, outfile, hits2, misses2)); // chooses new target if already targete before
}

void player_turn(char player1_grid[10][10], char player2_grid[10][10], FILE * outfile, int* hits1, int* misses1)
{
	char letter = '\0';
	int row = -1, col = -1;

	do {
		printf("Please enter the target cell. Ex. A4 or D7: "); // lets player choose target
		scanf(" %c", &letter);
		scanf("%d", &col);
		row = letter_to_number(letter);

	} while (!evaluate_coordinate(row, col, player1_grid, player2_grid, outfile, hits1, misses1)); // makes them rechoose if off board or already targeted


}

int ships_destroyed(char player_grid[10][10]) // checks if any ships remain
{
	int ships_afloat = 0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (player_grid[i][j] == 'c' || player_grid[i][j] == 'b' || player_grid[i][j] == 'r' || player_grid[i][j] == 's' || player_grid[i][j] == 'd')
			{
				ships_afloat = 1;
			}
		}
	}

	return !ships_afloat; // returns 1 if at least one afloat, otherwise 0
}

int evaluate_coordinate(int row, int col, char player1_grid[10][10], char player2_grid[10][10], FILE* outfile, int* hits1, int* misses1) //makes sure user target is good
{
	if (row < 0 || row > 9 || col < 0 || col > 9)
	{
		printf("That is not on the board!\n");
		return 0;
	}

	if (player2_grid[row][col] == 'm' || player2_grid[row][col] == '*')
	{
		printf("You have already targeted that cell!\n");
		return 0;
	}

	fprintf(outfile, "Player 1: %c%d - ", row + 65, col); //adding to game transcript


	if (player2_grid[row][col] == '-')
	{
		player2_grid[row][col] = 'm';

		system("cls");
		display_grids(player1_grid, player2_grid);

		printf("Miss!\n");

		fprintf(outfile, "Miss\n");

		*misses1 += 1;

		return 1;
	}

	fprintf(outfile, "Hit\n");
	*hits1 += 1;

	//checking if each ship is hit, then if it is destroyed

	if (player2_grid[row][col] == 'c')
	{
		ship_hit_and_destroyed(row, col, 'c', "Carrier", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player2_grid[row][col] == 'b')
	{
		ship_hit_and_destroyed(row, col, 'b', "Battleship", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player2_grid[row][col] == 'r')
	{
		ship_hit_and_destroyed(row, col, 'r', "Cruiser", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player2_grid[row][col] == 's')
	{
		ship_hit_and_destroyed(row, col, 's', "Submarine", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player2_grid[row][col] == 'd')
	{
		ship_hit_and_destroyed(row, col, 'd', "Destroyer", player1_grid, player2_grid, outfile);
		return 1;
	}

}

int evaluate_coordinate_no_print(int row, int col, char player1_grid[10][10], char player2_grid[10][10], FILE* outfile, int* hits2, int* misses2) //same as above but for computer
{
	if (player1_grid[row][col] == 'm' || player1_grid[row][col] == '*')
	{
		return 0;
	}

	fprintf(outfile, "Player 2: %c%d - ", row + 65, col);

	if (player1_grid[row][col] == '-')
	{
		player1_grid[row][col] = 'm';

		system("cls");
		display_grids(player1_grid, player2_grid);

		printf("The computer chose %c%d.\n", row + 65, col);
		printf("Miss!\n");

		fprintf(outfile, "Miss\n");

		*misses2 += 1;

		return 1;
	}

	fprintf(outfile, "Hit\n");
	*hits2 += 1;

	if (player1_grid[row][col] == 'c')
	{
		ship_hit_and_destroyed_no_print(row, col, 'c', "Carrier", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player1_grid[row][col] == 'b')
	{
		ship_hit_and_destroyed_no_print(row, col, 'b', "Battleship", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player1_grid[row][col] == 'r')
	{
		ship_hit_and_destroyed_no_print(row, col, 'r', "Cruiser", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player1_grid[row][col] == 's')
	{
		ship_hit_and_destroyed_no_print(row, col, 's', "Submarine", player1_grid, player2_grid, outfile);
		return 1;
	}

	if (player1_grid[row][col] == 'd')
	{
		ship_hit_and_destroyed_no_print(row, col, 'd', "Destroyer", player1_grid, player2_grid, outfile);
		return 1;
	}
}

void ship_hit_and_destroyed(int row, int col, char symbol, char ship_name[], char player1_grid[10][10], char player2_grid[10][10], FILE* outfile)
{
	int ship_afloat = 0;

	player2_grid[row][col] = '*';

	system("cls");
	display_grids(player1_grid, player2_grid);

	printf("Hit!\n");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (player2_grid[i][j] == symbol)
			{
				ship_afloat = 1; // changes from 0 to 1 if there is still another symbol after the current one is changed to * after hit
			}
		}
	}

	if (!ship_afloat)
	{
		printf("Enemy %s has been sunk!\n", ship_name); // prints that the ship is sunk. Since this function is only called after this specific ship is hit, the sunk statement is only printed out right when it actually happens.
		fprintf(outfile, "Player 2's %s has been sunk\n", ship_name);
	}
}

void ship_hit_and_destroyed_no_print(int row, int col, char symbol, char ship_name[], char player1_grid[10][10], char player2_grid[10][10], FILE* outfile) // same as above but for computer. Print statments are generally kept in for user interaction.
{
	int ship_afloat = 0;

	player1_grid[row][col] = '*';

	system("cls");
	display_grids(player1_grid, player2_grid);

	printf("The computer chose %c%d.\n", row + 65, col);

	printf("Hit!\n");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (player1_grid[i][j] == symbol)
			{
				ship_afloat = 1;
			}
		}
	}

	if (!ship_afloat)
	{
		printf("Allied %s has been sunk!\n", ship_name);
		fprintf(outfile, "Player 1's %s has been sunk\n", ship_name);
	}
}


void input_to_progress(void) // makes user input character to continue
{
	char input = '\0';
	printf("Enter any non-whitespace character to let the computer take its turn: ");
	scanf(" %c", &input);
}

void print_stats_to_file(FILE* outfile, Statistics player_stats) // prints all the stats for each player at the end of the game
{
	fprintf(outfile, "Total hits: %d\n", player_stats.hits);
	fprintf(outfile, "Total misses: %d\n", player_stats.misses);
	fprintf(outfile, "Total shots: %d\n", player_stats.shots);
	fprintf(outfile, "Hit Percentage: %.2lf%%\n", player_stats.percentage);
	fprintf(outfile, "Game Result: %s\n", player_stats.winner_loser);
}