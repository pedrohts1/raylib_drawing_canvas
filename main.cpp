#include <iostream>
#include "random"
#include "format"
#include "vector"

#include "raylib.h"
#include "raymath.h"

#include "board.hpp"
#include "shape.hpp"
#include "game_ui.hpp"

#include "utils.hpp"

using std::vector;

const int SCREEN_X = 1200;
const int SCREEN_Y = 800;

const float MAX_ZOOM = 30.0f;
const float MIN_ZOOM = 1.0f;


void updateCamera(Camera2D* camera){
    //HANDLE ZOOM Ray Implementation
    /*
    float mouse_move = GetMouseWheelMove();

    if(mouse_move != 0){
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

        // Set the offset to where the mouse is
        camera->offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera->target = mouseWorldPos;

        // Zoom increment
        float scaleFactor = 1.0f + (0.25f*fabsf(mouse_move));
        if (mouse_move < 0) scaleFactor = 1.0f/scaleFactor;
        camera->zoom = Clamp(camera->zoom*scaleFactor, 0.125f, 64.0f);
    }*/

    //HANDLE ZOOM My Implementation
    float zoom_factor = map_range(camera->zoom, MIN_ZOOM, MAX_ZOOM, 0.05f, 1.0);
    float mouse_move = GetMouseWheelMove();

    if(mouse_move != 0){
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
        camera->target = mouseWorldPos;
        //camera->offset = mouseWorldPos;
        camera->offset = GetMousePosition();
        //camera->target = GetMousePosition();

        camera->zoom += (mouse_move*zoom_factor); //Zoom Factor is a percentage value
        if (camera->zoom > MAX_ZOOM) camera->zoom = MAX_ZOOM;
        else if (camera->zoom < MIN_ZOOM) camera->zoom = MIN_ZOOM;
    }

    if(camera->zoom <=1){
        camera->target = (Vector2) {SCREEN_X/2.0f, SCREEN_Y/2.0f};
        camera->offset = (Vector2) {SCREEN_X/2.0f, SCREEN_Y/2.0f};
    }else{
        //HANDLE MOVEMENT
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera->zoom);
            camera->target = Vector2Add(camera->target, delta);
            //camera->offset = (Vector2) {SCREEN_X/2.0f, SCREEN_Y/2.0f};
        }
    }


}


void handleInput(Board* board){
    if(IsKeyPressed(KEY_ONE)){
        board->setMouseMode(MouseMode::Free);
    }else if(IsKeyPressed(KEY_TWO)){
        board->setMouseMode(MouseMode::Dragging);
    }else if(IsKeyPressed(KEY_THREE)){
        board->setMouseMode(MouseMode::Moving);
    }else if(IsKeyPressed(KEY_FOUR)){
        board->setMouseMode(MouseMode::Drawing);
    }
}



int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_X,SCREEN_Y,"Canvas");
    SetTargetFPS(60);

    Board board;
    board.camera = {0};
    board.camera.target = (Vector2) {SCREEN_X / 2.0f, SCREEN_Y / 2.0f};
    board.camera.offset = (Vector2) {SCREEN_X / 2.0f, SCREEN_Y / 2.0f};
    board.camera.zoom = 1.0f;
    board.render_texture = LoadRenderTexture(SCREEN_X, SCREEN_Y);
    board.thickness = 5;
    GameUI ui = {}; ui.init();


    board.setMouseMode(Drawing);
    //DisableCursor();

    while(!WindowShouldClose()){
        handleInput(&board);
        updateCamera(&board.camera);

        BeginTextureMode(board.render_texture);
        BeginMode2D(board.camera);

        ClearBackground(BLACK);

        board.update();

        EndMode2D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        //BeginMode2D(board.camera);
        DrawTexturePro(board.render_texture.texture,
                       {0, 0,(float)board.render_texture.texture.width,
                        -(float)board.render_texture.texture.height},

                       {0, 0, (float)board.render_texture.texture.width,
                        (float)board.render_texture.texture.height},

                       {0, 0}, 0, WHITE);

        DrawText(std::format("{:.0f} {:.0f}", board.newMousePos.x, board.newMousePos.y).c_str(),
                 10, 10, 22, WHITE);

        ui.update(SCREEN_X, SCREEN_Y);
        ui.drawShapeButtons(&board);
        EndDrawing();
    }
    UnloadRenderTexture(board.render_texture);
    CloseWindow();

    return 0;
}
