#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <ctime>
using namespace std;

//boundary dimensions
#define Rows 80
#define Cols 80

//directions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

//key codes player 1
#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75 
#define RIGHT_KEY 77

//key codes player 2
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100

//game difficulties i.e sleep timer in milliseconds
#define EASY 100
#define MEDIUM 80
#define HARD 50

struct Position
{
    int ri, ci;

    friend bool operator== (const Position& p1, const Position& p2)
    {
        return (p1.ri == p2.ri && p1.ci == p2.ci);
    }
};

struct Snake
{
    int score;
    vector<Position> ps;
    char sym;

    int direction;
    int upKey;
    int downKey;
    int leftKey;
    int rightKey;
};

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void displayBoundary()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2);

    gotoRowCol(0, 0);
    for (int i = 0; i < Cols; i++)
        cout << "#";

    gotoRowCol(Rows - 1, 0);
    for (int i = 0; i < Cols; i++)
        cout << "#";

    gotoRowCol(1, 0);
    for (int i = 1; i < Rows; i++)
        cout << "#" << endl;

    for (int i = 1; i < Rows; i++) {
        gotoRowCol(i, Cols - 1);
        cout << "#" << endl;
    }

    SetConsoleTextAttribute(hConsole, 15);
}

void displayScore()
{
    gotoRowCol(Rows, 0);
    cout << "Score:";
}

void displayScore(int a)
{
    gotoRowCol(Rows, 0);
    cout << "P1 Score:";

    gotoRowCol(Rows, 30);
    cout << "P2 Score:";
}

void init1(Snake & s) {
    s.ps.resize(3);
    s.ps[0].ri = Rows / 2;
    s.ps[0].ci = Cols / 2;
    s.ps[1].ri = Rows / 2;
    s.ps[1].ci = (Cols / 2) + 1;
    s.ps[2].ri = Rows / 2;
    s.ps[2].ci = (Cols / 2) + 2;

    s.score = 0;
    s.sym = -37;
    s.upKey = UP_KEY;
    s.downKey = DOWN_KEY;
    s.leftKey = LEFT_KEY;
    s.rightKey = RIGHT_KEY;

    s.direction = LEFT;
}

void init2(Snake& s) {
    s.ps.resize(3);
    s.ps[0].ri = Rows / 2 + 2;
    s.ps[0].ci = (Cols / 2) + 2;
    s.ps[1].ri = Rows / 2 + 2;
    s.ps[1].ci = (Cols / 2) + 1;
    s.ps[2].ri = Rows / 2 + 2;
    s.ps[2].ci = Cols / 2;

    s.score = 0;
    s.sym = '*';
    s.upKey = W_KEY;
    s.downKey = S_KEY;
    s.leftKey = A_KEY;
    s.rightKey = D_KEY;

    s.direction = RIGHT;
}

void updateDirection1(Snake & s, int key)
{
    if (key == LEFT_KEY && s.direction!=RIGHT)
    {
        s.direction = LEFT;
    }
    else if (key == RIGHT_KEY && s.direction!=LEFT)
    {
        s.direction = RIGHT;
    }
    else if (key == UP_KEY && s.direction!=DOWN)
    {
        s.direction = UP;
    }
    else if (key == DOWN_KEY && s.direction!=UP)
    {
        s.direction = DOWN;
    }
}

void updateDirection2(Snake& s, int key)
{
    if (key == A_KEY && s.direction != RIGHT)
    {
        s.direction = LEFT;
    }
    else if (key == D_KEY && s.direction != LEFT)
    {
        s.direction = RIGHT;
    }
    else if (key == W_KEY && s.direction != DOWN)
    {
        s.direction = UP;
    }
    else if (key == S_KEY && s.direction != UP)
    {
        s.direction = DOWN;
    }
}

void displaySnake(Snake s, char sym)
{
    for (int i = 0; i < s.ps.size(); i++)
    {
        gotoRowCol(s.ps[i].ri, s.ps[i].ci);
        cout << sym;
    }
}

void moveSnake(Snake& s)
{
    for (int i = s.ps.size() - 1; i > 0; i--)
    {
        s.ps[i] = s.ps[i - 1];
    }

    switch (s.direction)
    {
    case UP:
        s.ps[0].ri--;
        break;
    case DOWN:
        s.ps[0].ri++;
        break;
    case LEFT:
        s.ps[0].ci--;
        break;
    case RIGHT:
        s.ps[0].ci++;
        break;
    }

}

bool isSnakeDead(Snake s)
{
    bool flag = false;

    //check if snake hit boundary
    for (int i = 0; i < s.ps.size(); i++)
    {
        if (s.ps[0].ri == 0 || s.ps[0].ri == Rows - 1) {
            flag = true;
            break;
        }

        if (s.ps[0].ci == 0 || s.ps[0].ci == Cols - 1) {
            flag = true;
            break;
        }
    }

    //check if snake bit itself
    if (!flag)
    {
        for (int i = 1; i < s.ps.size(); i++)
        {
            if (s.ps[0] == s.ps[i]) {
                flag = true;
                break;
            }
        }
    }

    return flag;
}

void killSnake(Snake s)
{
    displaySnake(s, ' ');
    gotoRowCol(5, 90);
    cout << "Snake dead";
    displayBoundary();
}

int killSnake(Snake s, int a)
{
    static int kills = 0;
    if (s.sym == -37) {
        displaySnake(s, ' ');
        gotoRowCol(5, 90);
        cout << "P1 Snake dead";
        displayBoundary();
    }
    else {
        displaySnake(s, ' ');
        gotoRowCol(7, 90);
        cout << "P2 Snake dead";
        displayBoundary();
    }
    kills++;
    return kills;
}

void displayFood(Snake s,Position & foodPosition)
{
    int r, c;
    bool flag = false;

    while (!flag)
    {
        srand(time(0));
        r = rand() % Rows-1;
        c = rand() % Cols-1;

        for (int i = 0; i < s.ps.size(); i++)
        {
            if (r == s.ps[i].ri && c == s.ps[i].ci)
            {
                flag = false;
                break;
            }
            flag = true;
        }
    }

    foodPosition.ri = r;
    foodPosition.ci = c;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
    gotoRowCol(r, c);
    cout << "O";

    SetConsoleTextAttribute(hConsole, 15);
}

void displayFood(Snake s1, Snake s2, vector<Position>& foodPosition)
{
    int r, c;
    bool flag1, flag2, flag3;
    srand(time(0));

    if (foodPosition.empty()) {
        foodPosition.resize(5);
        for (int k = 0; k < foodPosition.size(); k++) {

            flag1 = flag2  = false;
            flag3 = true;

            while (!flag1 || !flag2 || !flag3)
            {
                r = rand() % Rows - 1;
                c = rand() % Cols - 1;

                //check that food isn't generated where snake 1 is
                for (int i = 0; i < s1.ps.size(); i++)
                {
                    if (r == s1.ps[i].ri && c == s1.ps[i].ci)
                    {
                        flag1 = false;
                        break;
                    }
                    flag1 = true;
                }

                //check that food isn't generated where snake 1 is
                for (int i = 0; i < s2.ps.size(); i++)
                {
                    if (r == s2.ps[i].ri && c == s2.ps[i].ci)
                    {
                        flag2 = false;
                        break;
                    }
                    flag2 = true;
                }

                //check if new food isn't generated where there is already food present
                for (int i = 0; i < k; i++)
                {
                    if (r == foodPosition[i].ri && c == foodPosition[i].ci)
                    {
                        flag3 = false;
                        break;
                    }
                }
            }

            foodPosition[k].ri = r;
            foodPosition[k].ci = c;
        }
 
    }
    else
    {
        int c = foodPosition.size();
        foodPosition.resize(5);
        for (int k = c; k <= 5; k++) {

            flag1 = flag2 = false;
            flag3 = true;

            while (!flag1 || !flag2 || !flag3)
            {
                r = rand() % Rows - 1;
                c = rand() % Cols - 1;

                //check that food isn't generated where snake 1 is
                for (int i = 0; i < s1.ps.size(); i++)
                {
                    if (r == s1.ps[i].ri && c == s1.ps[i].ci)
                    {
                        flag1 = false;
                        break;
                    }
                    flag1 = true;
                }

                //check that food isn't generated where snake 1 is
                for (int i = 0; i < s2.ps.size(); i++)
                {
                    if (r == s2.ps[i].ri && c == s2.ps[i].ci)
                    {
                        flag2 = false;
                        break;
                    }
                    flag2 = true;
                }

                //check if new food isn't generated where there is already food present
                for (int i = 0; i < k; i++)
                {
                    if (r == foodPosition[i].ri && c == foodPosition[i].ci)
                    {
                        flag3 = false;
                        break;
                    }
                }
            }

            foodPosition[k].ri = r;
            foodPosition[k].ci = c;
        }

    }


    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);

    for (int i = 0; i < foodPosition.size(); i++) {
        gotoRowCol(foodPosition[i].ri, foodPosition[i].ci);
        cout << "O";
    }

    SetConsoleTextAttribute(hConsole, 15);
}

bool isFoodEaten(Snake s, Position foodPosition)
{
    return (s.ps[0] == foodPosition);
}

bool isFoodEaten(Snake s, vector<Position>& foodPosition)
{
    bool flag = false;
    int i;
    for (i = 0; i < foodPosition.size(); i++)
    {
        if (s.ps[0].ri == foodPosition[i].ri && s.ps[0].ci == foodPosition[i].ci) {
            flag = true;
            break;
        }     
    }

    if (flag)
        foodPosition.erase(foodPosition.begin() + i);

    return flag;
}

void stretchSnake(Snake& s)
{
    vector<Position> v;
    v.resize(s.ps.size() + 1);

    for (int i = 0; i < s.ps.size(); i++)
    {
        v[i + 1] = s.ps[i];
    }

    switch (s.direction)
    {
    case UP:
        v[0].ri = v[1].ri - 1;
        v[0].ci = v[1].ci;
        break;
    case DOWN:
        v[0].ri = v[1].ri + 1;
        v[0].ci = v[1].ci;
        break;
    case LEFT:
        v[0].ri = v[1].ri;
        v[0].ci = v[1].ci - 1;
        break;
    case RIGHT:
        v[0].ri = v[1].ri;
        v[0].ci = v[1].ci + 1;
        break;
    }

    s.ps = v;
    s.score++;
}

void updateScore(Snake s)
{
    gotoRowCol(Rows, 6);
    cout << s.score;
}

void updateScore(Snake s1, Snake s2)
{
    gotoRowCol(Rows, 10);
    cout << s1.score;

    gotoRowCol(Rows, 40);
    cout << s2.score;
}

void decideWinner(Snake s1, Snake s2)
{
    gotoRowCol(12, 90);
    if (s1.score > s2.score)
        cout << "P1 won!";
    else if (s2.score > s1.score)
        cout << "P2 won!";
    else
        cout << "Game drawn!";
}

void runSinglePlayer(int difficulty)
{
    Snake s;
    Position foodPosition;

    displayBoundary();
    displayScore();

    init1(s);
    displaySnake(s, s.sym);
    displayFood(s, foodPosition);
    updateScore(s);

    int d;

    switch (difficulty)
    {
    case 1:
        d = EASY;
        break;
    case 2:
        d = MEDIUM;
        break;
    case 3:
        d = HARD;
        break;
    }

    while (true)
    {
        if (_kbhit())
        {
            int key = _getch();
            //player 1 key pressed
            if (key == 224)
            {
                key = _getch();
                updateDirection1(s, key);
            }
        }

        Sleep(d);
        displaySnake(s, ' ');
        moveSnake(s);
        if (isSnakeDead(s)) {
            killSnake(s);
            break;
        }
        displaySnake(s, s.sym);

        if (isFoodEaten(s, foodPosition)) {
            displaySnake(s, ' ');
            stretchSnake(s);
            displayFood(s, foodPosition);
            displaySnake(s, s.sym);
            updateScore(s);
        }

    }

}

void runMultiPlayer(int difficulty)
{
    Snake s1, s2;
    bool b1 = true, b2 = true;
    vector<Position> foodPosition;

    displayBoundary();
    displayScore(0);

    init1(s1);
    init2(s2);
    displaySnake(s1, s1.sym);
    displaySnake(s2, s2.sym);
    displayFood(s1, s2, foodPosition);
    updateScore(s1,s2);

    int d;

    switch (difficulty)
    {
    case 1:
        d = EASY;
        break;
    case 2:
        d = MEDIUM;
        break;
    case 3:
        d = HARD;
        break;
    }

    while (true)
    {
        if (_kbhit())
        {
            int key = _getch();
            //player 1 key pressed
            if (b1) {
                if (key == 224)
                {
                    key = _getch();
                    updateDirection1(s1, key);
                }
            }
            //player 2 key pressed
            if (b2) {
                if (key == W_KEY || key == A_KEY || key == S_KEY || key == D_KEY)
                    updateDirection2(s2, key);
            }
        }

        Sleep(d);
        if (b1) {
            displaySnake(s1, ' ');
            moveSnake(s1);
        }
        if (b2) {
            displaySnake(s2, ' ');
            moveSnake(s2);
        }

        if (b1) {
            if (isSnakeDead(s1)) {
                b1 = false;
                if (killSnake(s1, 0) == 2)
                    break;
            }
        }
        if (b2) {
            if (isSnakeDead(s2)) {
                b2 = false;
                if (killSnake(s2, 0) == 2)
                    break;
            }
        }

        if (b1)
            displaySnake(s1, s1.sym);
        if (b2)
            displaySnake(s2, s2.sym);
        if (b1) {
            if (isFoodEaten(s1, foodPosition)) {
                displaySnake(s1, ' ');
                stretchSnake(s1);
                displaySnake(s1, s1.sym);
                displayFood(s1, s2, foodPosition);
                updateScore(s1, s2);
            }
        }
        if (b2) {
            if (isFoodEaten(s2, foodPosition)) {
                displaySnake(s2, ' ');
                stretchSnake(s2);
                displaySnake(s2, s2.sym);
                displayFood(s1, s2, foodPosition);
                updateScore(s1, s2);
            }
        }
    }

    decideWinner(s1, s2);

}

int getDifficulty()
{
    bool flag = false;
    int choice;

    while (!flag) {
        cout << endl;
        cout << "1. Easy\t 2. Medium\t 3. Hard\n";
        cin >> choice;
        if (choice == 1 || choice == 2 || choice == 3)
            flag = true;
        else
            cout << "Invalid choice!" << endl;
    }
    return choice;
}

void run()
{
    bool flag = false;
    int choice;

    while (!flag) {
        cout << "1. Single Player\t 2. Two Player\n";
        cin >> choice;
        if (choice == 1 || choice == 2)
            flag = true;
        else
            cout << "Invalid choice!" << endl;
    }

    int difficulty = getDifficulty();

    system("cls");

    ShowConsoleCursor(false);

    switch (choice)
    {
    case 1:
        runSinglePlayer(difficulty);
        break;
    case 2:
        runMultiPlayer(difficulty);
        break;
    }
}

int main()
{
    _getch();
    run();
    _getch();
}

