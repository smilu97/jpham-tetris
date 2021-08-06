#include "game.h"
#include "shape.h"

#include <cstdio>
#include <GL/glut.h>

TetrisGame::TetrisGame():
    stepTimeThreshold(1000),
    screenWidth(800.f),
    screenHeight(600.f),
    boxSize(50) {

}

TetrisGame::~TetrisGame() {

}

void TetrisGame::Init(int * argc, char ** argv,
    DisplayFunc displayFunc,
    UpdateFunc updateFunc,
    KeyboardFunc keyboardFunc,
    SpecialFunc specialFunc
) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tetris");
    glutDisplayFunc(displayFunc);
    glutTimerFunc(0, updateFunc, 0);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMainLoop();
}

void TetrisGame::Update() {
    int dt = GetDt();

    StepTimer(dt);
}

void TetrisGame::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < 22; i++) {
        DrawTetriminoBox(WALL_TETRIMINO, i, 5);
        DrawTetriminoBox(WALL_TETRIMINO, i, 16);
    }
    for (int i = 6; i <= 15; i++) {
        DrawTetriminoBox(WALL_TETRIMINO, 0, i);
        DrawTetriminoBox(WALL_TETRIMINO, 21, i);
    }

    auto view = tetris.GetBoardView();
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (view[i][j] != 0) {
                DrawTetriminoBox(view[i][j], i+1, j+6);
            }
        }
    }

    auto queue = tetris.GetQueue();
    for (int i = 0; i < 5; i++) {
        TetriminoType type = queue[i];
        DrawTetrimino(type, i << 2, 18);
    }

    auto swap = tetris.GetSwap();
    if (swap != BOARD_EMPTY) {
        DrawTetrimino(swap, 0, 0);
    }

    glFlush();
    glutSwapBuffers();
}

void TetrisGame::OnKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'z':
            tetris.Rotate(false);
            break;
        case 'c':
            tetris.Swap();
            break;
        case ' ':
            tetris.HardDrop();
            break;
    }
}

void TetrisGame::OnSpecial(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:
            tetris.Move(false);
            break; 
        case GLUT_KEY_RIGHT:
            tetris.Move(true);
            break;
        case GLUT_KEY_UP:
            tetris.Rotate(true);
            break; 
        case GLUT_KEY_DOWN:
            tetris.Drop();
            stepTimer = 0;
            break;
    }
}

int TetrisGame::GetDt() {
    int time = glutGet(GLUT_ELAPSED_TIME);
    int dt = time - oldTime;
    oldTime = time;
    return dt;
}

bool TetrisGame::StepTimer(int dt) {
    stepTimer += dt;
    if (stepTimer > stepTimeThreshold) {
        stepTimer -= stepTimeThreshold;
        tetris.Step();
        return true;
    }
    return false;
}

void TetrisGame::DrawTetriminoBox(TetriminoType type, int x, int y) {
    const float * color = tetrimino_colors[type];
    glColor3f(color[0], color[1], color[2]);

    float gl_x1 = -1.0 + y * boxSize / screenWidth;
    float gl_y1 = 1.0 - x * boxSize / screenHeight;
    glRectf(gl_x1, gl_y1, gl_x1 + boxSize / screenWidth, gl_y1 - boxSize / screenHeight);
}

void TetrisGame::DrawTetrimino(TetriminoType type, int x, int y) {
    auto shape = tetris.GetTetriminoShape(type);
    for (int i = 0; i < sz_tetrimino_shape; i++) {
        for (int j = 0; j < sz_tetrimino_shape; j++) {
            if (shape[i][j] == 0) continue;
            DrawTetriminoBox(type, x + i, y + j);
        }
    }
}