#include "raylib.h"

#define GAME_WIDTH 320
#define GAME_HEIGHT 180

// Board Layout Constants
#define BOARD_SIZE 162
#define CELL_SIZE 18  // BOARD_SIZE / 9
#define MACRO_SIZE 54 // BOARD_SIZE / 3
#define BOARD_X 79    // (GAME_WIDTH - BOARD_SIZE) / 2
#define BOARD_Y 9     // (GAME_HEIGHT - BOARD_SIZE) / 2

// Game State
int board[9][9] = {0}; // [macro_index][micro_index], 0=Empty, 1=P1(X), 2=P2(O)
int activeMacroBoard = -1; // -1 means player can choose any board
int selectedMacroBoard = -1; // For 2-step selection: which board is currently focused
int playerTurn = 1; // 1 = X, 2 = O
int cursorMacro = 4; // 0-8, tracks board selection
int cursorMicro = 4; // 0-8, tracks cell selection

// Assets
Texture2D spriteSheet;

int main() {
    // 1. Get the current monitor's dimensions
    // We initialize a hidden window first to get monitor info
    InitWindow(0, 0, "Ultimate Tic-Tac-Toe");
    
    // Get the monitor that the window is currently on
    int monitor = GetCurrentMonitor();
    
    // Get the width and height of the primary monitor
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    
    // 2. Resize and toggle fullscreen
    // We set the window size to match the monitor size
    SetWindowSize(screenWidth, screenHeight);
    
    // Toggle fullscreen mode
    ToggleFullscreen();

    // Set the target FPS to match the monitor's refresh rate
    SetTargetFPS(GetMonitorRefreshRate(monitor));

    // Create a Render Texture for the low-res rendering
    RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); // Pixel perfect scaling

    // Load Spritesheet
    // Format: Horizontal strip, 16x16 tiles
    // Index 0: X, Index 1: O, Index 2: Cursor
    spriteSheet = LoadTexture("resources/sprites.png");
    bool hasSprites = (spriteSheet.id != 0);

    // Main game loop
    while (!WindowShouldClose()) {
        // Calculate scale factor to fit the screen while maintaining aspect ratio
        float scale = (float)GetScreenWidth() / GAME_WIDTH;
        if ((float)GetScreenHeight() / GAME_HEIGHT < scale) scale = (float)GetScreenHeight() / GAME_HEIGHT;

        // Input Handling (Keyboard)
        bool isSelectingBoard = (activeMacroBoard == -1 && selectedMacroBoard == -1);
        int targetMacro = (activeMacroBoard != -1) ? activeMacroBoard : selectedMacroBoard;

        if (isSelectingBoard) {
            // Navigate Boards
            if (IsKeyPressed(KEY_RIGHT) && (cursorMacro % 3 != 2)) cursorMacro++;
            if (IsKeyPressed(KEY_LEFT) && (cursorMacro % 3 != 0)) cursorMacro--;
            if (IsKeyPressed(KEY_DOWN) && (cursorMacro < 6)) cursorMacro += 3;
            if (IsKeyPressed(KEY_UP) && (cursorMacro > 2)) cursorMacro -= 3;

            // Select Board
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z)) {
                selectedMacroBoard = cursorMacro;
                cursorMicro = 4; // Reset cell cursor to center
            }
        } else {
            // Navigate Cells
            if (IsKeyPressed(KEY_RIGHT) && (cursorMicro % 3 != 2)) cursorMicro++;
            if (IsKeyPressed(KEY_LEFT) && (cursorMicro % 3 != 0)) cursorMicro--;
            if (IsKeyPressed(KEY_DOWN) && (cursorMicro < 6)) cursorMicro += 3;
            if (IsKeyPressed(KEY_UP) && (cursorMicro > 2)) cursorMicro -= 3;

            // Select Cell
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z)) {
                if (board[targetMacro][cursorMicro] == 0) {
                    board[targetMacro][cursorMicro] = playerTurn;
                    playerTurn = (playerTurn == 1) ? 2 : 1;
                    activeMacroBoard = cursorMicro; // Send next player to this board
                    selectedMacroBoard = -1;
                    
                    // Move board cursor to the new active board for visual continuity
                    cursorMacro = activeMacroBoard;
                    cursorMicro = 4;
                }
            }
            // Cancel Board Selection (only if free selection)
            if ((IsKeyPressed(KEY_X) || IsKeyPressed(KEY_BACKSPACE)) && activeMacroBoard == -1) {
                selectedMacroBoard = -1;
            }
        }
        
        // Draw to the low-res texture
        BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            
            // --- UI Drawing ---
            // P1 (Bottom)
            DrawText("P1", 10, GAME_HEIGHT - 15, 10, (playerTurn == 1) ? MAROON : LIGHTGRAY);
            if (hasSprites) DrawTexturePro(spriteSheet, (Rectangle){0,0,16,16}, (Rectangle){25, (float)GAME_HEIGHT - 18, 12, 12}, (Vector2){0,0}, 0, (playerTurn == 1) ? WHITE : GRAY);
            else DrawText("(X)", 25, GAME_HEIGHT - 15, 10, (playerTurn == 1) ? MAROON : LIGHTGRAY);

            // P2 (Top) - Rotated 180
            DrawTextPro(GetFontDefault(), "P2", (Vector2){ (float)GAME_WIDTH - 25, 15 }, (Vector2){ 0, 0 }, 180.0f, 10, 1, (playerTurn == 2) ? DARKBLUE : LIGHTGRAY);
            if (hasSprites) DrawTexturePro(spriteSheet, (Rectangle){16,0,16,16}, (Rectangle){(float)GAME_WIDTH - 10, 18, 12, 12}, (Vector2){0,0}, 180.0f, (playerTurn == 2) ? WHITE : GRAY);
            else DrawTextPro(GetFontDefault(), "(O)", (Vector2){ (float)GAME_WIDTH - 10, 15 }, (Vector2){ 0, 0 }, 180.0f, 10, 1, (playerTurn == 2) ? DARKBLUE : LIGHTGRAY);

            // --- Board Drawing ---
            for (int m = 0; m < 9; m++) {
                int mx = BOARD_X + (m % 3) * MACRO_SIZE;
                int my = BOARD_Y + (m / 3) * MACRO_SIZE;

                // Determine if this board is active/playable
                bool isPlayable = (activeMacroBoard == -1 || activeMacroBoard == m);
                if (activeMacroBoard == -1 && selectedMacroBoard != -1) isPlayable = (selectedMacroBoard == m);

                // Background for boards
                if (isPlayable) DrawRectangle(mx, my, MACRO_SIZE, MACRO_SIZE, Fade(SKYBLUE, 0.2f));
                
                // Highlight Hover
                if (isPlayable) {
                    if (isSelectingBoard && m == cursorMacro) {
                        if (hasSprites) DrawTexturePro(spriteSheet, (Rectangle){32,0,16,16}, (Rectangle){(float)mx, (float)my, MACRO_SIZE, MACRO_SIZE}, (Vector2){0,0}, 0, Fade(YELLOW, 0.5f));
                        else DrawRectangleLines(mx, my, MACRO_SIZE, MACRO_SIZE, DARKBLUE);
                    } else if (!isSelectingBoard && m == targetMacro) {
                        int cx = mx + (cursorMicro % 3) * CELL_SIZE;
                        int cy = my + (cursorMicro / 3) * CELL_SIZE;
                        if (hasSprites) DrawTexturePro(spriteSheet, (Rectangle){32,0,16,16}, (Rectangle){(float)cx, (float)cy, CELL_SIZE, CELL_SIZE}, (Vector2){0,0}, 0, Fade(YELLOW, 0.5f));
                        else DrawRectangle(cx, cy, CELL_SIZE, CELL_SIZE, Fade(YELLOW, 0.5f));
                    }
                }

                // Draw Pieces
                for (int c = 0; c < 9; c++) {
                    if (board[m][c] == 0) continue;
                    int cx = mx + (c % 3) * CELL_SIZE;
                    int cy = my + (c / 3) * CELL_SIZE;
                    
                    if (hasSprites) {
                        Rectangle source = (board[m][c] == 1) ? (Rectangle){0,0,16,16} : (Rectangle){16,0,16,16};
                        // Center the 16x16 sprite in the 18x18 cell (offset 1,1)
                        DrawTexturePro(spriteSheet, source, (Rectangle){(float)cx + 1, (float)cy + 1, 16, 16}, (Vector2){0,0}, 0, WHITE);
                    } else {
                        if (board[m][c] == 1) DrawText("X", cx + 5, cy + 2, 10, MAROON);
                        else DrawText("O", cx + 5, cy + 2, 10, DARKBLUE);
                    }
                }
                
                // Draw Grid Lines (Macro)
                DrawRectangleLines(mx, my, MACRO_SIZE, MACRO_SIZE, BLACK);
            }
            // Draw Grid Lines (Micro - simplified)
            for (int i = 1; i < 9; i++) if (i%3 != 0) DrawLine(BOARD_X + i*CELL_SIZE, BOARD_Y, BOARD_X + i*CELL_SIZE, BOARD_Y + BOARD_SIZE, Fade(GRAY, 0.5f));
            for (int i = 1; i < 9; i++) if (i%3 != 0) DrawLine(BOARD_X, BOARD_Y + i*CELL_SIZE, BOARD_X + BOARD_SIZE, BOARD_Y + i*CELL_SIZE, Fade(GRAY, 0.5f));
            
        EndTextureMode();

        // Draw to Screen
        BeginDrawing();
            ClearBackground(BLACK); // Letterbox bars color
            
            // Draw the render texture scaled up to the center of the screen
            Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height }; // Negative height to flip Y-axis
            Rectangle destRec = { (GetScreenWidth() - (GAME_WIDTH * scale)) * 0.5f, (GetScreenHeight() - (GAME_HEIGHT * scale)) * 0.5f, GAME_WIDTH * scale, GAME_HEIGHT * scale };
            DrawTexturePro(target.texture, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);
            
        EndDrawing();
    }

    // De-Initialization
    if (hasSprites) UnloadTexture(spriteSheet);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}