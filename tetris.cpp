#include "tetris.h"
#include "shape.h"

#include <algorithm>
#include <numeric>

using namespace tetris;

Tetris::Tetris(): board(BOARD_HEIGHT), swappable(true), swap(BOARD_EMPTY) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        board[i] = std::vector<TetriminoType>(BOARD_WIDTH, BOARD_EMPTY);
    }
    
    // Initialize queue with random piece
    // Queue must be maintained to have more than 6 pieces
    AddBatchInQueue(); // Push 7 pieces in queue
    AddBatchInQueue();

    GoNextTetrimino(); // Pop the very first tetrimino of this game
}

/**
 * Add random batch of 7 pieces to the queue
 */
void Tetris::AddBatchInQueue() {
    std::vector<int> batch(7);
    std::iota(batch.begin(), batch.end(), 1);
    std::random_shuffle(batch.begin(), batch.end());
    std::copy(batch.begin(), batch.end(), std::back_inserter(queue));
}

/**
 * Move currTetrimino to desired direction
 */
bool Tetris::Move(int dx, int dy, int rot) {
    int x = currTetrimino.x + dx, y = currTetrimino.y + dy;
    int rot = (currTetrimino.rot + rot) % 4;
    
    if (CheckCollision(x, y, rot, currTetrimino.type)) {
        return false;
    }

    currTetrimino.x = x;
    currTetrimino.y = y;
    currTetrimino.rot = rot;

    return true;
}

/**
 * Materialize currTetrimino to board
 */
void Tetris::Materialize(Board & targetBoard, const Tetrimino & tetrimino) {
    const int x = tetrimino.x, y = tetrimino.y, rot = tetrimino.rot;
    const TetriminoType type = tetrimino.type;
    const int sz = sz_tetrimino_shape;
    const int * shape = tetrimino_shapes[type - 1][rot];
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            if (shape[i*sz + j] == 0) continue;
            int tx = x + i, ty = y + j;
            targetBoard[tx][ty] = type;
        }
    }
}

bool Tetris::CheckCollision(int x, int y, int rot, TetriminoType type) const {
    if (type == 0) {
        return false;
    }

    const int sz = sz_tetrimino_shape;
    const int * shape = tetrimino_shapes[type - 1][rot];
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            if (shape[i*sz + j] == 0) continue;
            int tx = x + i, ty = y + j;
            if (board[tx][ty] != BOARD_EMPTY) {
                return true;
            }
        }
    }

    return false;
}

bool Tetris::GoNextTetrimino() {
    const int ix = 0, iy = 3, irot = 0;
    if (CheckCollision(ix, iy, irot, queue[0])) {
        return false;
    }

    currTetrimino = Tetrimino(ix, iy, irot, queue[0]);
    swappable = true;

    queue.pop_front();
    if (queue.size() < 7) {
        AddBatchInQueue();
    }

    return true;
}

Tetris::~Tetris() {

}

bool Tetris::Drop() {
    return Move(1, 0, 0);
}

bool Tetris::Rotate(bool cw) {
    return Move(0, 0, cw ? 1 : 3);
}

bool Tetris::Step() {
    if (Drop()) return true;

    Materialize(board, currTetrimino);

    return GoNextTetrimino();
}

bool Tetris::Move(bool right) {
    return Move(0, right ? 1 : -1, 0);
}

bool Tetris::Swap() {
    if (!swappable) return false;
    swappable = false;
    
    const int ix = 0, iy = 3, irot = 0;
    TetriminoType currType = currTetrimino.type;
    currTetrimino = Tetrimino(ix, iy, irot, swap);
    swap = currType;

    return true;
}

bool Tetris::HardDrop() {
    while (Drop());
    
    Materialize(board, currTetrimino);

    return GoNextTetrimino();
}

const Board Tetris::GetBoardView() const {
    Board boardView(board);
    Materialize(boardView, currTetrimino);
    return boardView;
}
