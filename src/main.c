#include <stdio.h>
#include "../include/raylib.h"
#include <stdlib.h>
#include "ui.h"

#define TRUE 1
#define FALSE 0
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define CELL_SIZE 25

typedef struct {
  int alive;
  int candidateToAlive;
} Cell;

void drawGridLines();
void drawCells(Cell** grid);
int countLiveNeighbors(Cell** grid, int x, int y);
void addGlide(Cell** grid, int startX, int startY);
void reset_grid(Cell **grid);

const int gridWidth = SCREEN_WIDTH / CELL_SIZE;
const int gridHeight = SCREEN_HEIGHT / CELL_SIZE;

short ui_mode = UI_MODE_DARK;

int main(void) {
  Cell **grid = (Cell **)malloc(gridHeight * sizeof(Cell *));
  if (grid == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }
  for (int y = 0; y < gridHeight; y++) {
    grid[y] = (Cell *)malloc(gridWidth * sizeof(Cell));
    if (grid[y]==NULL){
      fprintf(stderr, "Memory allocation failed\n");
      return 1;
    }
  }
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");
  SetTargetFPS(60);

  const int skipFrameUpdateGrid = 5;
  int frameCount = 0;
  int gameStarted = 0;
  
  reset_grid(grid);

  while (!WindowShouldClose()) {
    frameCount = (frameCount + 1) % skipFrameUpdateGrid;
    
    BeginDrawing();

    Color background = ui_mode == UI_MODE_LIGHT ? BACKGROUND_COLOR_LIGHT: BACKGROUND_COLOR_DARK;
    ClearBackground(background);
   
    drawCells(grid);
    drawGridLines();

    EndDrawing();
    if (gameStarted == TRUE) {
      if (frameCount == 0) {
        for (int y = 0; y < gridHeight; y++) {
          for (int x = 0; x < gridWidth; x++) {
            int aliveNeighbors = countLiveNeighbors(grid, x, y);
            if (grid[y][x].alive == TRUE) {
              // 2 or 3 neighbors are alive? remain alive, survive
              // less than 2 alive neighboring cells? die
              // more than 3 neighboring cells alive, die overpopulation
              if (aliveNeighbors == 2 || aliveNeighbors == 3) {
                grid[y][x].candidateToAlive = TRUE;
              } else if (aliveNeighbors < 2) {
                grid[y][x].candidateToAlive = FALSE;
              } else {
                grid[y][x].candidateToAlive = FALSE;
              }
            } else {
              // dead cell
              // if 3 neighboring cells are alive, become alive
              if (aliveNeighbors == 3) {
                grid[y][x].candidateToAlive = TRUE;
              }
            }
          }
        }

        for (int y = 0; y < gridHeight; y++) {
          for (int x = 0; x < gridWidth; x++) {
            grid[y][x].alive = grid[y][x].candidateToAlive;
            grid[y][x].candidateToAlive = FALSE;
          }
        }
      }

      if (IsKeyPressed(KEY_ENTER)) {
        gameStarted = FALSE;
        reset_grid(grid);
      }

    } else {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int cellX = mouseX / CELL_SIZE;
        int cellY = mouseY / CELL_SIZE;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
          grid[cellY][cellX].alive = FALSE;
        } else {
          grid[cellY][cellX].alive = TRUE;
        }
      }
      if (IsKeyPressed(KEY_ENTER)) {
        gameStarted = TRUE;
      }
    }
  }

  CloseWindow();

  free(grid);
    
  return 0;
}

void addGlide(Cell** grid, int startX, int startY) {
  if (startX < 0 || startX > gridWidth-3 || startY < 0 || startY > gridHeight - 3) {
    printf("Glide does not fit in that space.\n");
  } else {
    grid[startY][startX].alive = TRUE;
    grid[startY][startX+1].alive = TRUE;
    grid[startY][startX+2].alive = TRUE;
    grid[startY-1][startX+2].alive = TRUE;
    grid[startY-2][startX+1].alive = TRUE;
  }
  
}

void drawGridLines() {
  for (int y = 0; y < gridHeight; y++) {
    for (int x = 0; x < gridWidth; x++) {
      Rectangle rect;
      rect.x = x * CELL_SIZE;
      rect.y = y * CELL_SIZE;
      rect.width = CELL_SIZE;
      rect.height = CELL_SIZE;
      Color color = ui_mode == UI_MODE_LIGHT ? GRIDLINE_COLOR_LIGHT : GRIDLINE_COLOR_DARK;
      DrawRectangleLinesEx(rect, 1, color);
    }
  }
}

void drawCells(Cell** grid) {
  for (int y = 0; y < gridHeight; y++) {
    for (int x = 0; x < gridWidth; x++) {
      if (grid[y][x].alive) {
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
      } 
    }
  }
}

int countLiveNeighbors(Cell** grid, int x, int y) {
  int liveNeighbors = 0;
  const int directions[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
  };

  for (int i = 0; i < 8; i++) {
    int nx = x + directions[i][0];
    int ny = y + directions[i][1];

    // Check if the neighbor is within the grid bounds
    if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
      liveNeighbors += grid[ny][nx].alive;
    }
  }

  return liveNeighbors;
}

void reset_grid(Cell **grid) {
  for (int y = 0; y < gridHeight; y++) {
    for (int x = 0; x < gridWidth; x++) {
      grid[y][x].alive = FALSE;
      grid[y][x].candidateToAlive = FALSE;
    }
  }
  
  addGlide(grid, 10, 4);
}

