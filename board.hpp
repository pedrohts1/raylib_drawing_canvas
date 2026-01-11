//
// Created by Win_10 on 08/12/2024.
//

#ifndef DRAWING_CANVAS_BOARD_HPP
#define DRAWING_CANVAS_BOARD_HPP

#include "shape.hpp"

enum MouseMode{
    Free,
    Dragging,
    Moving,
    Drawing
};

enum DrawingMode{
    DrawingFree,
    DrawingLine,
    DrawingLineShape,
    DrawingRect,
    DrawingEllipse,
    DrawingTriangle,
};

class Board{
public:
    Camera2D camera;
    Vector2 mouseDelta;
    Vector2 prevMousePos;
    Vector2 newMousePos;
    int thickness = 1;
    RenderTexture2D render_texture;
    Shape current_shape;
    Mesh mesh;
    vector<Shape> shapes;

    DrawingMode drawingMode = DrawingMode::DrawingFree;
    MouseMode mouseMode = MouseMode::Free;
    bool movingShape;

    void update();
    void draw();
    void setMouseMode(MouseMode mouse_mode);

};

vector<Vector2> fillShapeDensity(Shape shape);

#endif //DRAWING_CANVAS_BOARD_HPP
