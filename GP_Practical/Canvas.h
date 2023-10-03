#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define PI 3.14159724

struct Color
{
    int r, g, b, a;
    Color() { r = -1; g = -1; b = -1; a = -1; }
    Color(int r, int g, int b) : r(r), g(g), b(b), a(255) {}
    Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

    bool isNotInit() { return r == -1 || g == -1 || b == -1 || a == -1; }
    bool operator==(const Color& o) { return r == o.r && g == o.g && b == o.b && a == o.a; }
    bool operator!=(const Color& o) { return r != o.r || g != o.g || b != o.b || a != o.b; }
};

struct Point2D { float x, y; Color c; };
struct Point3D { 
    float x, y, z; Color c;
    Point3D(float x, float y, float z) : x(x), y(y), z(z), c(Color()) {}
    Point3D(float x, float y, float z, Color c) : x(x), y(y), z(z), c(c) {}
    // only care bout the position, color diff still count as same point
    bool operator==(const Point3D& o) { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Point3D& o) { return x != o.x || y != o.y || z != o.z; }
};

// to enable reflection (each is 2 planes) [X => z = -z, Y => x = -x, Z => y = -y]
// will see in the future whether need to expand to 3D or not
enum ReflectAxis { REFLECT_X, REFLECT_Y, REFLECT_Z, REFLECT_ALL };

enum ProjectionMode { ORTHO, PERSPECTIVE };

// normally the whole window is our canvas
class Canvas
{
private:
    int width;
    int height;
    int depth;
    Color currColor;
    std::vector<Point3D> prev3DPoints;
    bool isTracking = false;
    bool reflectX = false;
    bool reflectY = false;
    bool reflectZ = false;
    void plot2D(Point2D p);
    // calculate the point to plot to get an ellipsis
    void ellipsePlot2D(Point2D center, float rx, float ry, float rotation, float startAngle, float fullAngle, int vertexCount, bool clockwise = false);
    void plot3D(Point3D p);
    void plotCube(float top, float bottom, float left, float right, float front, float back, Color topC, Color bottomC, Color leftC, Color rightC, Color frontC, Color backC);
public:
    // create a canvas with width, height and depth (does not affect actual size, this is just used to scale down from int to GLFloat)
    Canvas(int width, int height, int depth);
    Canvas& setColor(Color c);
    // resets all reflections
    Canvas& reflect();
    // toggles reflection on axis
    Canvas& reflect(ReflectAxis ra, bool enable);
    // clear canvas with c as background color
    Canvas& clear(Color c = { 0, 0, 0 });
    // renders a debug grid behind (scale 1 unit at a time (10 (-10 -> 10) x 10 canvas will have 400 rects))
    // with red as default color
    Canvas& showDebugGrid(bool centerDiffColor = true, Color c = { 255, 0, 0 }, Color centerC = { 0, 255, 0 });
    // in case we want custom types
    Canvas& begin(GLenum mode, bool clearPrev = true);
    Canvas& end();
    Canvas& pointSize(float size);
    Canvas& point(Point2D p);
    Canvas& point(Point3D p);
    // generates glVertex2f for every point (scaled int to 0 - 1.0f)
    Canvas& polyPlot2D(std::vector<Point2D> points);
    // generates glVertex3f for every point (scaled int to 0 - 1.0f)
    Canvas& polyPlot3D(std::vector<Point3D> points);
    // sets line width (use GlFloat)
    Canvas& lineWidth(float width);
    // draws a line with the points
    Canvas& line(std::vector<Point2D> points);
    // draw a straight line
    Canvas& straightLine(Point2D from, Point2D to);
    // draw a loop of line
    Canvas& lineLoop(std::vector<Point2D> points);
    // draws a line with the points
    Canvas& line3D(std::vector<Point3D> points);
    // draw a straight line
    Canvas& straightLine3D(Point3D from, Point3D to);
    // draw a loop of line
    Canvas& lineLoop3D(std::vector<Point3D> points);
    // draw line ellipsis
    Canvas& lineEllipsis(Point2D center, float rx, float ry, float rotation = 0.0f, float startAngle = 0.0f, float fullAngle = 2 * PI, int vertexCount = 100);
    // draw a line circle
    Canvas& lineCircle(Point2D center, float radius, float rotation = 0.0f, float startAngle = 0.0f, float fullAngle = 2 * PI, int vertexCount = 100);
    // draws a polygon with the given points (draw triangle with this as well since they call the same things)
    Canvas& poly(std::vector<Point2D> points);
    // draws a rectangle
    Canvas& rect(Point2D topLeft, Point2D bottomRight);
    // draws a square (solid color)
    Canvas& square(Point2D topLeft, float length);
    // draws an ellipsis
    Canvas& ellipsis(Point2D center, float rx, float ry, float rotation = 0.0f, float startAngle = 0.0f, float fullAngle = 2 * PI, int vertexCount = 100);
    Canvas& circle(Point2D center, float radius, float rotation = 0.0f, float startAngle = 0.0f, float fullAngle = 2 * PI, int vertexCount = 100);
    // draw ellipsis with radial gradient
    Canvas& ellipsisGrad(Point2D center, float rx, float ry, Color circColor, Color centerColor, float rotation = 0.0f, float startAngle = 0.0f, float fullAngle = 2 * PI, int vertexCount = 100);
    // draw circle with radial gradient
    Canvas& circleGrad(Point2D center, float radius, Color circColor, Color centerColor, float rotation = 0.0f, float startAngle = 0.0f, float fullAngle = 2 * PI, int vertexCount = 100);
    // only 180 degree
    Canvas& crescent(Point2D center, float rx, float ry, float fullness, float rotation = 0.0f, int vertexCount = 100);
    // transformation related
    Canvas& loadIdentity();
    Canvas& pushMatrix();
    Canvas& popMatrix();
    // translate relative to the canvas size
    Canvas& translate(float x, float y, float z);
    // rotaeno, not influenced by canvas size
    Canvas& rotate(float angle, float x, float y, float z);
    // scale, not influenced by canvas size
    Canvas& scale(float x, float y, float z);
    Canvas& polygonMode(GLenum face, GLenum mode);
    Canvas& poly3D(std::vector<Point3D> points);
    Canvas& cube(Point3D center, float lenFromCenter);
    // fixed cuboid, if wan special special then use poly3D
    Canvas& cuboid(Point3D topLeftFront, Point3D bottomRightBack);
    Canvas& cuboid(Point3D topLeftFront, Point3D topRightBack, Point3D bottomLeftFront, Point3D bottomRightBack);
    Canvas& pyramid(Point3D centerBase, float height, float lenFromCenter);
    Canvas& pyramid(Point3D centerBaseLeftFront, Point3D centerBaseRightBack, Point3D mid);

    // only works for cube, cuboid and pyramid, doesn't work for sphere, cylinder and cones
    // replots every point with a different primitive
    // c is optional and can leave empty if wan use back prev color
    Canvas& replotPrevBlocky3D(GLenum type, Color c = Color());
    Canvas& sphere(Point3D center, float radius, GLenum drawStyle = GLU_FILL, int slices = 30, int stacks = 30);
    Canvas& cylinder(Point3D center, float baseRadius, float topRadius, float height, bool closed = false, GLenum drawStyle = GLU_FILL, int slices = 30, int stacks = 30);
    Canvas& setProjection(ProjectionMode p);
    // true to enable light, false to disable
    Canvas& switchLight(bool onOff);
    // once defined, automatically enables the light
    Canvas& defineLight(GLenum light, GLenum paramName, float* param);
    // to disable the light defined
    Canvas& disableLight(GLenum light);
    Canvas& defineMaterial(GLenum face, GLenum paramName, float* param);
};
