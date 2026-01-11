#ifndef DRAWING_CANVAS_SHAPE_HPP
#define DRAWING_CANVAS_SHAPE_HPP

#include "raylib.h"
#include "raymath.h"
#include "vector"

using std::vector;

struct Box{
    Vector2 pos;
    Vector2 fixed_point;
    float width;
    float height;
    Vector2 center;
};

Box getBoundingBox(Vector2 fixed_point, Vector2 other_point);

enum ShapeType{
    Pen,
    Line,
    LineShape,
    Rect,
    Triangle,
    Ellipse
};

class Shape{
public:
    vector<Vector2> points;
    int thickness;
    Color color;
    Box boundingBox;
    ShapeType type;

    bool selected;
    bool hovered;
    bool moving;
    Shape();

    Shape(vector<Vector2> points, Box boundingBox, int thickness, Color color);
    void update();
    void drawShape();
};

bool checkPointShapeCollision(Vector2 point, Shape &shape, float distance=5);

std::vector<Vector2> smoothPath(const std::vector<Vector2>& points, float smoothness);
std::vector<Vector2> iterativeSmoothPath(std::vector<Vector2> points, float smoothness, int iterations);
vector<Vector2> lineSmoothingChaikin(vector<Vector2> points);

Mesh CreateLineMesh(Vector2* points, int pointCount, float thickness);


#endif //DRAWING_CANVAS_SHAPE_HPP
