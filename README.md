# Pong_C

This program implements a single player Pong game in C.

![cpong](https://i.imgur.com/NTXM94B.jpg)

The program uses the ncurses library for graphical output. The ncurses library allows the game to run directly in the console window by providing functionality to print characters at specific coordinates on the console window. The ball and paddles are drawn using color-inverted space characters.

The left paddle is controlled by the user using the 'w' and 's' keys. The right paddle is controlled by the computer and continuously follows the ball so that the computer never loses. The movement of the right paddle is handled by a separate thread.

When the user fails to return the ball, the game ends and the user's score is stored in a text file.
