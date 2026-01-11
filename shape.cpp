#include "shape.hpp"
#include <iostream>

using std::vector;

Box getBoundingBox(Vector2 fixed_point, Vector2 other_point){
    Vector2 origin = {std::min(fixed_point.x, other_point.x), std::min(fixed_point.y, other_point.y)};
    Vector2 end = {std::max(fixed_point.x, other_point.x), std::max(fixed_point.y, other_point.y)};

    float width = abs(end.x - origin.x);
    float height = abs(end.y - origin.y);
    Vector2 center = {origin.x + width / 2, origin.y + height / 2};
    DrawRectangle(origin.x, origin.y, width, height, BLUE);
    DrawCircle(center.x, center.y, 2, RED);

    return {origin, fixed_point, width, height, center};
}

Shape::Shape(){}

Shape::Shape(vector<Vector2> points, Box boundingBox, int thickness, Color color){
    this->points = points;
    this->boundingBox = boundingBox;
    this->thickness = thickness;
    this->color = color;
}

void Shape:: update(){
    hovered = false;
}

void Shape::drawShape(){
    if(points.size() <= 0) return;

    Color draw_color = color;
    if(hovered){
        draw_color = RED;
    }
    if(points.size() >= 6) {
        DrawSplineCatmullRom(&points[0], points.size(), thickness, draw_color);
    }else{
        DrawSplineLinear(&points[0], points.size(), thickness, draw_color);
    }

    for (Vector2 p: points) {
        DrawPixelV(p, GREEN);
    }
}


bool checkPointShapeCollision(Vector2 point, Shape &shape, float distance){
    for (Vector2 p:shape.points) {
        if(Vector2Distance(point, p) < distance*shape.thickness){
            return true;
        }
    }
    return false;
}

std::vector<Vector2> smoothPath(const std::vector<Vector2>& points, float smoothness) {
    if (points.empty()) return {};
    // Ensure smoothness is clamped between 0.0 and 1.0
    smoothness = fmin(fmax(smoothness, 0.0f), 1.0f);

    std::vector<Vector2> smoothedPoints;
    smoothedPoints.push_back(points[0]); // First point remains unchanged

    for (size_t i = 1; i < points.size(); ++i) {
        Vector2 prev = smoothedPoints.back();
        Vector2 current = points[i];
        Vector2 smoothed = {
                smoothness * current.x + (1.0f - smoothness) * prev.x,
                smoothness * current.y + (1.0f - smoothness) * prev.y
        };
        smoothedPoints.push_back(smoothed);
    }

    return smoothedPoints;
}
std::vector<Vector2> iterativeSmoothPath(std::vector<Vector2> points, float smoothness, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        points = smoothPath(points, smoothness);
    }
    return points;
}
vector<Vector2> lineSmoothingChaikin(vector<Vector2> points){
    vector<Vector2> new_line;
    new_line.reserve(points.size());
    for (int i=0; i<points.size()-1; i++){
        Vector2 p0 = points[i];
        Vector2 p1 = points[i+1];
        Vector2 q = {static_cast<float>(0.75*p0.x+0.25*p1.x), static_cast<float>(0.75*p0.y+0.25*p1.y)};
        Vector2 r = {static_cast<float>(0.25*p0.x+0.75*p1.x), static_cast<float>(0.25*p0.y+0.75*p1.y)};
        new_line.push_back(q);
        new_line.push_back(r);
    }
    return new_line;
}

Mesh CreateLineMesh(Vector2* points, int pointCount, float thickness) {
    Mesh mesh = { 0 };

    // Allocate memory for vertices and indices
    int vertexCount = pointCount * 2; // Two vertices per point (top and bottom edges of the line)
    int indexCount = (pointCount - 1) * 6; // Two triangles per segment (3 indices each)

    mesh.vertexCount = vertexCount;
    mesh.triangleCount = (pointCount - 1) * 2;

    mesh.vertices = (float*)MemAlloc(vertexCount * 3 * sizeof(float)); // 3 floats per vertex (x, y, z)
    mesh.indices = (unsigned short*)MemAlloc(indexCount * sizeof(unsigned short)); // Indices

    float* vertices = mesh.vertices;
    unsigned short* indices = mesh.indices;

    // Generate vertices
    for (int i = 0; i < pointCount; i++) {
        Vector2 p = points[i];
        Vector2 dir = (i < pointCount - 1) ? Vector2Subtract(points[i + 1], p) : Vector2Subtract(p, points[i - 1]);
        dir = Vector2Normalize((Vector2){ -dir.y, dir.x }); // Perpendicular direction for thickness

        // Top and bottom vertices
        vertices[(i * 2 + 0) * 3 + 0] = p.x + dir.x * thickness / 2; // x
        vertices[(i * 2 + 0) * 3 + 1] = p.y + dir.y * thickness / 2; // y
        vertices[(i * 2 + 0) * 3 + 2] = 0.0f;                       // z

        vertices[(i * 2 + 1) * 3 + 0] = p.x - dir.x * thickness / 2; // x
        vertices[(i * 2 + 1) * 3 + 1] = p.y - dir.y * thickness / 2; // y
        vertices[(i * 2 + 1) * 3 + 2] = 0.0f;                       // z
    }

    // Generate indices
    int index = 0;
    for (int i = 0; i < pointCount - 1; i++) {
        indices[index++] = i * 2 + 0;
        indices[index++] = i * 2 + 1;
        indices[index++] = (i + 1) * 2 + 0;

        indices[index++] = i * 2 + 1;
        indices[index++] = (i + 1) * 2 + 1;
        indices[index++] = (i + 1) * 2 + 0;
    }

    // Upload the mesh to the GPU
    UploadMesh(&mesh, false);

    return mesh;
}
