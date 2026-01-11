#include "board.hpp"
#include <iostream>

void Board::update(){
    prevMousePos = newMousePos;
    newMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    mouseDelta = Vector2Subtract(newMousePos, prevMousePos);

    for (Shape &shape: shapes) {
        shape.update();

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            shape.moving = false;
            movingShape = false;
        }
        if(checkPointShapeCollision(newMousePos, shape)){
            shape.hovered = true;
        }
    }

    if(mouseMode == Free){

    }else if(mouseMode == Dragging){

    }else if(mouseMode == Moving){
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            for (Shape &s: shapes) {
                if(s.moving || (!movingShape && checkPointShapeCollision(newMousePos, s))){
                    s.moving = true;
                    movingShape = true;
                    for (Vector2 &p: s.points) {
                        p = Vector2Add(p, mouseDelta);
                    }
                }
            }
        }

    }else if(mouseMode == Drawing){
        if(IsKeyPressed(KEY_Q)){
            drawingMode = DrawingMode::DrawingFree;
        }else if(IsKeyPressed(KEY_W)){
            drawingMode = DrawingMode::DrawingEllipse;
        }else if(IsKeyPressed(KEY_E)){
            drawingMode = DrawingMode::DrawingRect;
        }else if(IsKeyPressed(KEY_R)){
            drawingMode = DrawingMode::DrawingTriangle;
        }else if(IsKeyPressed(KEY_T)){
            drawingMode = DrawingMode::DrawingLine;
        }else if(IsKeyPressed(KEY_Y)){
            drawingMode = DrawingMode::DrawingLineShape;
        }

        draw();
    }

    for (Shape shape: shapes) {
        shape.drawShape();
    }
    DrawCircleLinesV(newMousePos, thickness / 2, WHITE);
}

void Board::draw(){

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        printf("Mouse Pos: %f %f", newMousePos.x, newMousePos.y);
        current_shape = {{newMousePos}, {newMousePos, newMousePos},
                         thickness, WHITE};
        return;
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && prevMousePos.x && prevMousePos.y) {

       if(drawingMode == DrawingMode::DrawingFree){
            current_shape.type = ShapeType::Pen;
            current_shape.points.push_back(newMousePos);
       }else if(drawingMode == DrawingMode::DrawingEllipse){
            //Clockwise Direction
           current_shape.boundingBox = getBoundingBox(current_shape.boundingBox.fixed_point,newMousePos);

           current_shape.type = ShapeType::Ellipse;
            current_shape.points.resize(0);

            Vector2 center = current_shape.boundingBox.center;

            float diameter = current_shape.boundingBox.width;
            int point_density = ceil(diameter);
            current_shape.points.reserve(point_density);

            float degrees = 0.f;
            double start_angle = degrees * (M_PI / 180);; // Start angle (135 degrees in radians)
            double dtheta = 2.0 * M_PI / point_density;

            float rx = current_shape.boundingBox.width/2;  // Horizontal radius (semi-major axis)
            float ry = current_shape.boundingBox.height/2;  // Vertical radius (semi-minor axis)

            for (int i = 0; i < point_density; ++i) {
                double theta = start_angle + i * dtheta;  // Angle

                // Parametric equations for an ellipse
                current_shape.points.push_back({center.x + rx * (float)cos(theta),
                                                center.y + ry * (float)sin(theta) });
                if(i==0){
                    current_shape.points.push_back(current_shape.points.front());
                }
            }
            current_shape.points.push_back(current_shape.points.front());
            current_shape.points.push_back(current_shape.points.back());

       }else if(drawingMode == DrawingMode::DrawingRect){
           current_shape.boundingBox = getBoundingBox(current_shape.boundingBox.fixed_point,newMousePos);

           current_shape.type = ShapeType::Rect;
            //Clockwise Direction
            current_shape.points.resize(5);
            Vector2 origin = current_shape.points[0]; //Top Left
            current_shape.points[1] = {newMousePos.x, origin.y}; //Top
            current_shape.points[2] = {newMousePos.x, newMousePos.y}; //Bottom Right
            current_shape.points[3] = {origin.x, newMousePos.y}; //Bottom Left
            current_shape.points[4] = origin; //Bottom Left
       }else if(drawingMode == DrawingMode::DrawingTriangle){
           current_shape.boundingBox = getBoundingBox(current_shape.boundingBox.fixed_point,newMousePos);

           current_shape.type = ShapeType::Triangle;
            //Clockwise Direction
            current_shape.points.resize(4);
            Vector2 center = current_shape.boundingBox.center;

            current_shape.points[0] = {center.x,
                                       center.y + (center.y - newMousePos.y)}; //Top
            current_shape.points[1] = {newMousePos.x,
                                       newMousePos.y}; //Bottom Right
            current_shape.points[2] = {center.x + (center.x - newMousePos.x),
                                       newMousePos.y}; //Bottom Left
            current_shape.points[3] = current_shape.points[0]; //connect

       }else if(drawingMode == DrawingMode::DrawingLine){
           current_shape.boundingBox = getBoundingBox(current_shape.boundingBox.fixed_point,newMousePos);

           current_shape.type = ShapeType::Line;
           current_shape.points.resize(2);
           current_shape.points[0] = current_shape.boundingBox.fixed_point;
           current_shape.points[1] = newMousePos;
       }else if(drawingMode == DrawingMode::DrawingLineShape){
           current_shape.type = ShapeType::LineShape;
           current_shape.points[0] = newMousePos;
       }
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){

        //vector<Vector2> points = iterativeSmoothPath(current_shape.points, 1, 5);

        //current_shape.points = fillShapeDensity(current_shape);
        if(current_shape.type == ShapeType::Ellipse){
        }

        shapes.push_back(current_shape);
        current_shape = {};
    }else{
        current_shape.drawShape();

        //Mesh lineMesh = CreateLineMesh(&current_shape[0],
        //                               current_shape.size()-1, thickness);
        //Material material = LoadMaterialDefault();
        //DrawMesh(lineMesh, material, MatrixIdentity());
    }

}

vector<Vector2> fillShapeDensity(Shape shape){
    float length = 0;
    for (int i = 0; i < shape.points.size()-1; ++i) {
        length += Vector2Distance(shape.points[i], shape.points[i+1]);
    }
    int step = shape.thickness;
    int point_density = floor(length/step);

    vector<Vector2> new_points = {};
    for (int i = 0; i < shape.points.size()-1; ++i) {
        Vector2 distance_x_y = Vector2Subtract(shape.points[i+1], shape.points[i]);
        float distance = Vector2Distance(shape.points[i+1], shape.points[i]);
        int num_of_new_points = round(distance/step)+1;
        /*
        while (new_points.size() <= num_of_new_points){
            Vector2 new_point = Vector2Add(new_points.back(), distance_per_point);
            if(Vector2Distance(new_point, shape.points[i]) >=
            Vector2Distance(shape.points[i+1], shape.points[i])){
                break;
            }

            new_points.push_back(new_point);
        }*/
    }
    return new_points;
}

void Board::setMouseMode(MouseMode mouse_mode){
    mouseMode = mouse_mode;
    switch (mouseMode) {
        case Free:
            SetMouseCursor(MouseCursor::MOUSE_CURSOR_ARROW);
            break;
        case Dragging:
            SetMouseCursor(MouseCursor::MOUSE_CURSOR_IBEAM);
            break;
        case Moving:
            SetMouseCursor(MouseCursor::MOUSE_CURSOR_POINTING_HAND);
            break;
        case Drawing:
            SetMouseCursor(MouseCursor::MOUSE_CURSOR_CROSSHAIR);
            break;
        default:
            break;
    }
}


