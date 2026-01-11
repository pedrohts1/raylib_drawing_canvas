#ifndef DRAWING_CANVAS_GAME_UI_HPP
#define DRAWING_CANVAS_GAME_UI_HPP

#include "raylib.h"
#include "string"

#include "board.hpp"

struct GameUIFonts{
    inline static std::string RobotoBlack = "resources/fonts/Roboto-Black.ttf";
    inline static std::string RobotoBold = "resources/fonts/Roboto-Bold.ttf";
    inline static std::string RobotoMedium = "resources/fonts/Roboto-Medium.ttf";
    inline static std::string RobotoRegular = "resources/fonts/Roboto-Regular.ttf";
    inline static std::string RobotoThin = "resources/fonts/Roboto-Thin.ttf";
    inline static std::string RobotoLight = "resources/fonts/Roboto-Light.ttf";
};

class GameUI {
private:

public:
    float windowSizeX;
    float windowSizeY;
    float game_speed_slider = 1;
    Font font;

    GameUI() = default;

    void init(const std::string& font_ttf=GameUIFonts::RobotoMedium);
    void update(float window_size_x, float window_size_y);
    void drawShapeButtons(Board *board);
    void stop();
    void setFont(const std::string& font_ttf) const;
};

#endif //DRAWING_CANVAS_GAME_UI_HPP