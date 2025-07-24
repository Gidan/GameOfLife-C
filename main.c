#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct {
  int alive;
  int candidateToAlive;
} Cell;

void drawGridLines();
void drawCells(Cell** grid);
int countLiveNeighbors(Cell** grid, int x, int y);
void addGlide(Cell** grid, int startX, int startY);

const int screenWidth = 800;
const int screenHeight = 450;
const int cellSize = 25;
const int gridWidth = screenWidth / cellSize;
const int gridHeight = screenHeight / cellSize;

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

  addGlide(grid, 10, 4);
  
  InitWindow(screenWidth, screenHeight, "Game of Life");
  SetTargetFPS(60);

  const int skipFrameUpdateGrid = 5;
  int frameCount = 0;
  int gameStarted = 0;
  
  while (!WindowShouldClose()) {
    frameCount = (frameCount + 1) % skipFrameUpdateGrid;
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
   
    drawCells(grid);
    drawGridLines();

    EndDrawing();
    if (gameStarted == TRUE) {
      if (frameCount == 0) {
        printf("Updating grid data\n");
        //update logic
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
    } else {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        printf("Mouse clicked x:%d  y:%d", mouseX, mouseY);
        int cellX = mouseX / cellSize;
        int cellY = mouseY / cellSize;
        grid[cellY][cellX].alive = TRUE;
      }
      if (IsKeyPressed(KEY_ENTER)) {
        gameStarted = TRUE;
      }
    }
  }

  CloseWindow();
    
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
      rect.x = x * cellSize;
      rect.y = y * cellSize;
      rect.width = cellSize;
      rect.height = cellSize;
      DrawRectangleLinesEx(rect, 1, BLACK); 
    }
  }
}

void drawCells(Cell** grid) {
  for (int y = 0; y < gridHeight; y++) {
    for (int x = 0; x < gridWidth; x++) {
      if (grid[y][x].alive == TRUE) {
        DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, RED);
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
  
