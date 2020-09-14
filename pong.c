/*
 * Pong Game in C
 * Author: Abhi Ardeshana
 * Purpose: A single player Pong game that records scores in a text file.
 */

// Libraries
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Function declaration
void startgame();
void *moverightpaddle();
void endgame();

// Variable declaration
int screenX;
int screenY;
int ballX = 15;
int ballY = 5;
int ballSpeedX = 2;
int ballSpeedY = 1;
int paddleL;
int paddleR;
int paddleLength = 10;
int score = 0;
char keypress;
FILE * fileptr;
pthread_t paddlemover;

// Main function
int main() {
	
	// Calls the startgame() function to initialize the game window
	startgame();
	
	// Game loop
	while (1) {
		
		// Clears the screen
		erase();
		
		// Obtains the user input
		keypress = getch();
		// Moves the paddle down if the user is pressing the 's' key
		if (keypress == 's' && paddleL < screenY - paddleLength) {
			paddleL++;
		}
		// Moves the paddle up if the user is pressing the 'w' key
		if (keypress == 'w' && paddleL > 0) {
			paddleL--;
		}
		
		// Draws both paddles onto the screen
		for (int i = 0; i < paddleLength; i++) {
			mvprintw(paddleL + i, 5, "  ");
			mvprintw(paddleR + i, screenX - 7, "  ");
		}
		
		// Checks for collision between the ball and the left paddle
		if (ballX == 7 && ballY >= paddleL - 1 && ballY <= paddleL + paddleLength) {
			// Bounces the ball back and increments the score
			ballSpeedX = 2;
			score++;
		}
		// Ends the loop if the user fails to return the ball
		if (ballX < 0) {
			break;
		}
		// Bounces the ball off of the right paddle
		if (ballX == screenX - 9) {
			ballSpeedX = -2;
		}
		// Bounces the ball off of the top of the screen
		if (ballY == 0) {
			ballSpeedY = 1;
		}
		// Bounces the ball off of the bottom of the screen
		if (ballY == screenY - 1) {
			ballSpeedY = -1;
		}
		
		// Moves the ball
		ballX = ballX + ballSpeedX;
		ballY = ballY + ballSpeedY;
		// Draws the ball onto the screen
		mvprintw(ballY, ballX, "  ");
		
		// Refreshes the screen and waits before the next game cycle
		refresh();
		usleep(30000);
		
	}
	
	// Calls the endgame() function to save the score and close the game window
	endgame();
	
	return 0;
}

// This function runs at the start of the program and initializes the game
void startgame() {
	// Sets up the curses window
	initscr();
	timeout(0);
	cbreak();
	noecho();
	curs_set(0);
	// Provides instructions to the user and waits for a keypress before proceeding
	mvprintw(1, 2, "Welcome to Pong!");
	mvprintw(2, 2, "Use the W and S keys to control your paddle!");
	mvprintw(3, 2, "Press any key to begin!");
	refresh();
	getchar();
	// Turns on the reverse attribute and obtains the screen size
	attron(A_REVERSE);
	getmaxyx(stdscr, screenY, screenX);
	// Positions the paddles based on the screen size and the ball's positioning
	paddleL = screenY / 2 - (paddleLength / 2);
	paddleR = ballY - (paddleLength / 2);
	// Initializes the file and starts the thread
	fileptr = fopen("Score List.txt", "a");
	pthread_create(&paddlemover, NULL, moverightpaddle, NULL);
}

// This thread controls the right paddle in the background
// It follows the ball so that the computer never loses
// Thus, the user is essentially playing against themselves
void *moverightpaddle() {
	while (1) {
		// Moves the paddle down if the ball is below the paddle
		if (paddleR + (paddleLength / 2) < ballY && paddleR < screenY - paddleLength) {
			paddleR++;
		}
		// Moves the paddle up if the ball is above the paddle
		if (paddleR + (paddleLength / 2) > ballY && paddleR > 0) {
			paddleR--;
		}
		usleep(1000);
	}
}

// This function runs at the end of the program
// It saves the user's score and closes the window
void endgame() {
	// Turns off the reverse attribute and clears the screen
	attroff(A_REVERSE);
	erase();
	// Prints the user's score to the screen
	mvprintw(1, 2, "Oops! You lost!");
	mvprintw(2, 2, "Your score was %d.", score);
	mvprintw(3, 2, "The application will exit in 5 seconds.");
	refresh();
	// Saves the user's score to the text file
	fprintf(fileptr, "%d\r\n", score);
	fflush(fileptr);
	fclose(fileptr);
	// Waits five seconds and then closes the window
	sleep(5);
	endwin();
}