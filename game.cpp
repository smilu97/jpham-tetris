#include "game.h"
#include "shape.h"

#include <iostream>
#include <termio.h>
#include <thread>

using namespace tetris;

int getch(void){
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

void TetrisGame::Run() {
    std::cout << "\x1b[2J";
    Render();

    ResetTimer();
    std::thread th_input(&TetrisGame::InputThreadFunc, this);

    TimerThreadFunc();
    th_input.join();
}

void TetrisGame::TimerThreadFunc() {
    while (true) {
        if (gameOver) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        stepTimerCount--;
        if (stepTimerCount <= 0) {
            ResetTimer();
            if (false == tetris.Step()) {
                gameOver = true;
                break;
            }
            Render();
        }
    }
}

void TetrisGame::InputThreadFunc() {
    while (true) {
        if (gameOver) break;

        int ch = getch();
        
        switch (ch) {
            case 27:
                if (getch() == 91) {
                    int b = getch();
                    if (b == 65) { // up
                        tetris.Rotate(true);
                    } else if (b == 66) { // right
                        if (tetris.Drop()) {
                            ResetTimer();
                        }
                    } else if (b == 67) { // down
                        tetris.Move(true);
                    } else if (b == 68) { // left
                        tetris.Move(false);
                    }
                }
                break;
            case 99: // c
                tetris.Rotate(false);
                break;
            case 122: // z
                tetris.Swap();
                break;
            case 32: // space
                if (false == tetris.HardDrop()) {
                    gameOver = true;
                    break;
                }
                break;
        }
        Render();
    }
}

void TetrisGame::ResetTimer() {
    stepTimerCount = 100;
}

void TetrisGame::Render() {
    std::cout << "\x1b[H";

    std::vector< std::vector<int> > buf(22, std::vector<int>(17, 0));

    for (int i = 0; i < BOARD_WIDTH + 2; i++) {
        buf[0][i] = 1;
        buf[21][i] = 1;
    }
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        buf[1+i][0] = 1;
        buf[1+i][BOARD_WIDTH+1] = 1;
    }

    tetris::Board view = tetris.GetBoardView();

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            buf[1+i][1+j] = view[i][j] ? 1 : 2;
        }
    }

    const tetris::TetriminoQueue& queue = tetris.GetQueue();

    for (int k = 0; k < 4; k++) {
        const int * shape = tetrimino_shapes[queue[k]-1][0];
        for (int i = 0; i < sz_tetrimino_shape; i++) {
            for (int j = 0; j < sz_tetrimino_shape; j++) {
                buf[k*4+i][13+j] = shape[i*sz_tetrimino_shape+j];
            }
        }
    }

    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 17; j++) {
            if (buf[i][j] == 1) std::cout << "@";
            else if (buf[i][j] == 2) std::cout << ".";
            else std::cout << " ";
        }
        std::cout << std::endl;
    }
}