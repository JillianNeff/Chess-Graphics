//
// Created by Jillian Neff on 11/15/19.
//

#include "graphics.h"
#include "Board.h"
#include "Button.h"
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;
GLdouble width, height;
int wd;
Board board;
screen phase;
Button startBtn({0.8,0.8,1.0},{500,500},100,50,"START");
Button resetBtn({.3569,.2471,.3373},{900,200},100,50,"Reset Game");
Button challenge1({.8666,.6274,.8666},{900,300},100,50," Challenge 1");
Button challenge2 ({1,.8941,1},{900,400},100,50,"Challenge 2");
Button challenge3 ({.9019,.9019,.9804},{900,500},100,50,"Challenge 3");
Button endGame({.8863,.8236,.8588},{900,600},100,50, "End Game");

void init() {
    width = 1000;
    height = 800;
    srand(time(0));
    phase = start;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.4f, 0.5137f, 0.4863f, 1.0f); // Black and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(phase == start){
        string label = "Virtual Chess Game";
        glColor3f(1, 1, 1);
        glRasterPos2i(430,300);
        for(const char &letter : label) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
        }
        startBtn.draw();
        glFlush();
    }
    if(phase == game){
        board.draw();
        resetBtn.draw();
        challenge1.draw();
        challenge2.draw();
        challenge3.draw();
        endGame.draw();
        if(board.gameOver()){
            string team;
            if(board.getWhiteTurn()) {
                team = "Black Team";
            }
            else {
                team = "White Team";
            }
            string label = "Game Over! ";
            glColor3f(1, 1, 1);
            glRasterPos2i(860,100);
            for(const char &letter : label) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            label = team + " Won!";
            glRasterPos2i(840,120);
            for(const char &letter : label) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
        }
        else {
            string label = "Press C to Concede";
            glColor3f(1, 1, 1);
            glRasterPos2i(825,100);
            for(const char &letter : label) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

        }

        glFlush();
    }
    if(phase == ending){
        string label = "Virtual Chess Game";
        glColor3f(1, 1, 1);
        glRasterPos2i(434,300);
        for(const char &letter : label) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
        }
        label = "Would You Like to Play Again?";
        glColor3f(1, 1, 1);
        glRasterPos2i(390,400);
        for(const char &letter : label) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
        }
        startBtn.draw();
        glFlush();
    }

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    if (key == 'c') {
        board.setGameOver(true);
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:

            break;
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_UP:

            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {
    if(startBtn.isOverlapping(x,y)){
        startBtn.hover();
    }
    else{
        startBtn.release();
    }

    if(resetBtn.isOverlapping(x,y)){
        resetBtn.hover();
    }
    else{
        resetBtn.release();
    }
    if(challenge1.isOverlapping(x,y)){
        challenge1.hover();
    }
    else{
        challenge1.release();
    }

    if(challenge2.isOverlapping(x,y)){
        challenge2.hover();
    }
    else{
        challenge2.release();
    }

    if(challenge3.isOverlapping(x,y)){
        challenge3.hover();
    }
    else{
        challenge3.release();
    }

    if(endGame.isOverlapping(x,y)){
        endGame.hover();
    }
    else{
        endGame.release();
    }

    glutPostRedisplay();

}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && startBtn.isOverlapping(x,y)){
        startBtn.pressDown();
    }
    else{
        startBtn.release();
    }
    // If the left button is up and the cursor is overlapping with the Button, start the game.
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && startBtn.isOverlapping(x,y)){
        board.resetBoard();
        board.fileToBoard("../boards/default.csv");
        board.setWhiteTurn(true);
        phase = game;
    }

    // Code for reset button
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && resetBtn.isOverlapping(x,y)){
        resetBtn.pressDown();
    }
    else{
        resetBtn.release();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && resetBtn.isOverlapping(x,y)){
        board.resetBoard();
        board.fileToBoard("../boards/default.csv");
        board.setWhiteTurn(true);
    }

    // Code for Challenge 1 Board
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && challenge1.isOverlapping(x,y)){
        challenge1.pressDown();
    }
    else{
        challenge1.release();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && challenge1.isOverlapping(x,y)){
        board.resetBoard();
        board.fileToBoard("../boards/Challenge1.csv");
        board.setWhiteTurn(true);
    }

    // Code for Challenge 2 Board
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && challenge2.isOverlapping(x,y)){
        challenge2.pressDown();
    }
    else{
        challenge2.release();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && challenge2.isOverlapping(x,y)){
        board.resetBoard();
        board.fileToBoard("../boards/Challenge2.csv");
        board.setWhiteTurn(true);
    }

    // Code for Challenge 3 Board
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && challenge3.isOverlapping(x,y)){
        challenge3.pressDown();
    }
    else{
        challenge3.release();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && challenge3.isOverlapping(x,y)){
        board.resetBoard();
        board.fileToBoard("../boards/Challenge3.csv");
        board.setWhiteTurn(true);
    }

    // Code for end game button
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && endGame.isOverlapping(x,y)){
        endGame.pressDown();
    }
    else{
        endGame.release();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && endGame.isOverlapping(x,y)){
        phase = ending;
    }

    // Functionality for showing any possible moves
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && phase == game && !board.gameOver()) {
        if (board.getPiece((x / 100), (y / 100), WHITE) != nullptr && board.getWhiteTurn()) { // button clicked on
            board.getSquares()->at((y / 100) * 8 + (x / 100)).pressDown();
            for (int i : board.cleanValidMoves(x / 100, y / 100, WHITE)) {
                board.getSquares()->at(i).setPressed(true);
            }
        } else if (board.getPiece((x / 100), (y / 100), BLACK) != nullptr && !board.getWhiteTurn()) {
            board.getSquares()->at((y / 100) * 8 + (x / 100)).pressDown();
            for (int i : board.cleanValidMoves(x / 100, y / 100, BLACK)) {
                board.getSquares()->at(i).setPressed(true);
            }
        }
    }

    // Functionality for moving pieces
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && phase == game && !board.gameOver()) {
        int tempX, tempY;
        bool replaced = false;
        if (board.getSquares()->at((y / 100) * 8 + (x / 100)).isPressed()) {
            for (Button &b : *board.getSquares()) {
                tempX = b.getLeftX() / 100;
                tempY = b.getTopY() / 100;
                if (b.isPressed() && (board.getPiece(tempX, tempY, WHITE) != nullptr) && board.getWhiteTurn() && !replaced) {
                    board.getPiece(tempX, tempY, WHITE)->movePiece(x / 100, y / 100);
                    board.setPiece(board.getPiece(tempX, tempY, WHITE));
                    board.getPiece(tempX, tempY,
                                       WHITE)->findValidMoves(); // Finds valid moves of piece at its new position
                    if (x / 100 != tempX || y / 100 != tempY) {
                        board.popPiece(tempX, tempY, WHITE);
                        board.checkCollisions();
                        replaced = true;
                        cout << boolalpha << "Black king in check: " << board.kingInCheck(BLACK) << endl;
                        cout << boolalpha << "Game over: " << board.gameOver() << endl;
                        board.setWhiteTurn(false);
                    }
                } else if (b.isPressed() && (board.getPiece(tempX, tempY, BLACK) != nullptr) && !board.getWhiteTurn() && !replaced) {
                    board.getPiece(tempX, tempY, BLACK)->movePiece(x / 100, y / 100);
                    board.setPiece(board.getPiece(tempX, tempY, BLACK));
                    board.getPiece(tempX, tempY,
                                       BLACK)->findValidMoves(); // Finds valid moves of piece at its new position
                    if (x / 100 != tempX || y / 100 != tempY) {
                        board.popPiece(tempX, tempY, BLACK);
                        board.checkCollisions();
                        replaced = true;
                        cout << boolalpha << "White king in check: " << board.kingInCheck(WHITE) << endl;
                        cout << boolalpha << "Game over: " << board.gameOver() << endl;
                        board.setWhiteTurn(true);
                    }
                }
                b.release();
            }
        } else {
            for (Button &b : *board.getSquares()) {
                b.release(); // If user drags to an invalid spot all of the squares and pieces are released.
            }
        }
    }
    glutPostRedisplay();
}

void timer(int dummy) {

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    board.fileToBoard("../boards/default.csv"); // "../boards/default.csv"
    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Chess Game!" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}

