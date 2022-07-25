#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include "main.h"


// GLOBAL VARIABLES //

bool gameOver = false;
int tailX[125], tailY[125];
int nTail;
int input = 0;
const int width = 25;
const int height = 25;
int framerate = 128;
int x,y, fruitX, fruitY, score;
std::string scores;


// INPUT ENUM //
enum eDirection{STOP=0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void printScore()
{
    std::cout << "      ///" << "Score:" << score << "\\\\\\";
}

void printLines(int x, char y)
{
    for(int i = 0; i < x; i++)
    {
        std::cout << " ";
        for(int j = 0; j < 36; j++)
            std::cout << y;
        std::cout << std::endl;
    }
}

int getInput()
{
    int input;
    std::cin >> input;
    return input;
}

void getScore()
{
    std::ifstream MyFile("score.txt");
    while (getline (MyFile, scores)) // Output the text from the file //
        std::cout << "        " << scores << std::endl;
}

void showScores()
{
    system("cls");
    printLines(8,'#');
    printLines(1,' ');
    std::cout << " ########## HIGHEST SCORE ###########" << std::endl;
    printLines(1,' ');
    getScore();
    printLines(1, ' ');
    printLines(8,'#');
    //////////////////////////////////////////
    //  To Do: Read top scores from a file  //
    //////////////////////////////////////////
    system("pause");
    runMenu();
}

void printMenu()
{
    std::cout << R"(
################SNAKE#################
#         _.-""      ""-._           #
#      .-'                `-.        #
#    .'      __.----.__      `.      #
#   /     .-"          "-.     \     #
#  /    .'                `.    \    #
# J    /                    \    L   #
# F   J                      L   J   #
#J    F                      J    L  #
#|   J                        L   |  #
#|   |                        |   |  #
#|   J                        F   |  #
#J    L                      J    F  #
# L   J   .-""""-.           F   J   #
# J    \ /        \   __    /    F   #
#  \    (|)(|)_   .-'".'  .'    /    #
#   \    \   /_>-'  .<_.-'     /     #
#    `.   `-'     .'         .'      #
#      `--.|___.-'`._    _.-'        #
#          ^          """"           #)" << std::endl;
    //printLines(1, '-');
    std::cout << "##########" << "1 - Play Game." << "   " << "###########" << std::endl;
    std::cout << "##########" << "2 - View Scores." << " " << "###########" << std::endl;
    std::cout << "##########" << "3 - How to Play." << " " << "###########" << std::endl;
    std::cout << "##########" << "4 - Exit." << "        " << "###########" << std::endl;
}

void Visual()
{
    // PRINT LEVEL/SNAKE/TAIL/FRUIT //
    system("cls");
    for(int i = 0; i < width+2; i++)
    {
        std::cout << "#";
    }
    std::cout << std::endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(j == 0)
                std::cout << "#";
            if(i == y && j == x)
                std::cout << "O";
            else if(i == fruitY && j == fruitX)
                std::cout << "*";
            else
            {
                // PRINT TAIL //
                bool printTail = false;
                for(int k = 0; k < nTail; k++)
                {
                    if(tailX[k] == j && tailY[k] == i)
                    {
                        std::cout << "+";
                        printTail = true;
                    }
                }
                if(!printTail)
                    std::cout << " ";
            }
            if (j == width - 1)
                std::cout << "#";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < width+2; i++)
    {
        std::cout << "#";
    }
    std::cout << std::endl;
    //////////////////////////////////
    printScore();
}

void Logic()
{
    // TAIL MOVEMENT //
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X , prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for(int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // INPUT MOVEMENT //
    switch(dir)
    {
       case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // CHECK OUT OF BOUNDS //
    if(x >= width)
        x = 0;
    else if(x < 0)
        x = width -1;
    //
    if(y >= height)
        y = 0;
    else if(y < 0)
        y = height -1;

    // TAIL COLLISION //
    for(int i = 0; i < nTail; i++)
        if(tailX[i] == x && tailY[i] == y)
        gameOver = true;

    // GRAB FRUIT //
    if(x==fruitX && y==fruitY)
    {
        // INCREASES FRAMERATE //
        int scoredif = 60;
        if(score > 30 && score < 60)
            framerate = 108;
        else if(score >= scoredif)
        {
            if(framerate >= 30)
                framerate = framerate - 10;
            scoredif = scoredif +30;
        }
        // INC SCORE // SPAWN NEW FRUIT // ADD TAIL //
        score = score + 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

//  INPUT SET UP //
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void runGame()
{
    while(!gameOver)
    {
        Visual();
        Input();
        Logic();
        Sleep(framerate);
    }
}

void gameStart()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    runGame();
}

void mainMenu(int option)
{
    if(option >= 0 && option <= 4)
    {
        switch (option)
        {
        case 1:
            gameStart();
            break;
        case 2:
            showScores();
            break;
        case 3:
            showHowto();
            break;
        case 4:
            system("exit");
            break;
        }
    }
    else
    {
        input = 0;
        printLines(2,'#');
        std::cout << " ###### Type a correct number! ######" << std::endl;
        printLines(2,'#');
        system("pause");
        runMenu();
    }
}

void runMenu()
{
    gameOver = false;
    //system("pause");
    system("cls");
    printMenu();
    mainMenu(getInput());
    while(gameOver)
    {
        printgameOver();
        runMenu();
    }
}

void showHowto()
{
    system("cls");
    printLines(8,'#');
    printLines(1, ' ');
    std::cout << " Use W,A,S,D to move the snake around "<< std::endl;
    std::cout << " and collect the fruits" << std::endl;
    printLines(1, ' ');
    printLines(8, '#');
    system("pause");
    runMenu();
}

void writeScore(int score, std::string name)
{
    std::ofstream MyFile("score.txt");
    MyFile << name << "   ";
    MyFile << score;
    MyFile.close();
}

void printgameOver()
{
    system("cls");
    printLines(6, '#');
    std::cout << " #########  ++The END++ #############" << std::endl;
    std::cout << " #########   +SCORE " << score <<"+  ############" << std::endl;
    printLines(6, '#');
    printLines(1, ' ');
    std::cout << "       Enter your name" << std::endl;
    framerate = 128;
    std::string name;
    std::cin >> name;
    writeScore(score, name);
}

int main()
{
    runMenu();
    return 0;
}
