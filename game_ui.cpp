#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "./media/raygui_styles/style_dark.h"

#include "raylib.h"
#include "game_ui.hpp"

void GameUI::init(const std::string& font_ttf) {
    GuiLoadStyleDark();
    font = LoadFontEx(font_ttf.c_str(), 32, 0, 250);
    GuiSetFont(font);
}

void GameUI::setFont(const std::string& font_ttf) const {
    GuiSetFont(font);
}

void GameUI::drawShapeButtons(Board *board){

    bool freeshape = GuiButton((Rectangle){windowSizeX/2-74, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_WAVE_SINUS, windowSizeX/2-74,10,2, BLUE);

    bool rect = GuiButton((Rectangle){windowSizeX/2-37, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_BOX, windowSizeX/2-37,10,2, BLUE);

    bool ellipse = GuiButton((Rectangle){windowSizeX/2, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_BREAKPOINT_ON, windowSizeX/2,10,2, BLUE);

    bool triangle = GuiButton((Rectangle){windowSizeX/2+37, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_ARROW_UP_FILL, windowSizeX/2+37,10,2, BLUE);

    bool line = GuiButton((Rectangle){windowSizeX/2+74, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_CROSSLINE, windowSizeX/2+74,10,2, BLUE);


    if(freeshape){
        board->drawingMode = DrawingMode::DrawingFree;
    }else if(rect){
        board->drawingMode = DrawingMode::DrawingRect;
    }else if(ellipse){
        board->drawingMode = DrawingMode::DrawingEllipse;
    }else if(triangle){
        board->drawingMode = DrawingMode::DrawingTriangle;
    }else if(line){
        board->drawingMode = DrawingMode::DrawingLine;
    }


    bool freemouse = GuiButton((Rectangle){200, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_CURSOR_CLASSIC, 200,10,2, BLUE);

    bool dragmouse = GuiButton((Rectangle){240, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_CURSOR_MOVE_FILL, 240,10,2, BLUE);

    bool movemouse = GuiButton((Rectangle){280, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_CURSOR_HAND, 280,10,2, BLUE);

    bool drawmouse = GuiButton((Rectangle){320, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_PENCIL, 320,10,2, BLUE);

    if(freemouse){
        board->setMouseMode(MouseMode::Free);
    }else if(dragmouse){
        board->setMouseMode(MouseMode::Dragging);
    }else if(movemouse){
        board->setMouseMode(MouseMode::Moving);
    }else if(drawmouse){
        board->setMouseMode(MouseMode::Drawing);
    }

    bool print = GuiButton((Rectangle){windowSizeX-40, 10,32, 32},"");
    GuiDrawIcon(GuiIconName::ICON_PHOTO_CAMERA, windowSizeX-40,10,2, BLUE);

    if(print){
        TakeScreenshot("C:\\Users\\Win_10\\Downloads\\drawing_canvas.png");
    }
}

void GameUI::update(float window_size_x, float window_size_y) {
    windowSizeX = window_size_x;
    windowSizeY = window_size_y;
}


void GameUI::stop() {
    UnloadFont(font);
}