#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#include "Canvas.h"

#define PI 22/7.0

Canvas& Canvas::setColor(Color c)
{
    // only change color if the point color is different
    if (c != currColor && !c.isNotInit())
    {
        glColor4ub(c.r, c.g, c.b, c.a);
        currColor = c;
    }
    // default to opaque black if never got color
    if (currColor.isNotInit())
    {
        glColor4ub(0, 0, 0, 255);
        currColor = { 0, 0, 0, 255 };
    }

    return *this;
}

Canvas& Canvas::reflect()
{
    reflectX = reflectY = reflectZ = false;

    return *this;
}

Canvas& Canvas::reflect(ReflectAxis ra, bool enable)
{
    if (ra == REFLECT_X || ra == REFLECT_ALL)
        reflectX = enable;
    
    if (ra == REFLECT_Y || ra == REFLECT_ALL)
        reflectY = enable;

    if (ra == REFLECT_Z || ra == REFLECT_ALL)
        reflectZ = enable;

    return *this;
}

void Canvas::plot2D(Point2D p)
{
    setColor(p.c);
    glVertex2f(p.x * (reflectY ? -1 : 1) / (float)width, p.y * (reflectZ ? -1 : 1) / (float)height);
}

std::vector<Point3D> Canvas::getCurvePoints(Point3D center, float rx, float ry, float startAngle, float fullAngle, int vertexCount)
{
    std::vector<Point3D> ps;

    const float anglePerSegment = fullAngle / vertexCount;
    for (int i = 0; i <= vertexCount; i++)
    {
        float x = rx * cos(anglePerSegment * i + startAngle) + center.x;
        float y = ry * sin(anglePerSegment * i + startAngle) + center.y;
        ps.push_back({ x, y, center.z, center.c });
    }

    return ps;
}

void Canvas::ellipsePlot2D(
    Point2D center,
    float rx, float ry,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount,
    bool clockwise
)
{
    const float anglePerSegment = fullAngle / vertexCount;
    for (int i = 0; i <= vertexCount; i++)
    {
        float x = rx * cos(anglePerSegment * i + startAngle) + center.x;
        float y = ry * sin(anglePerSegment * i + startAngle) + center.y;
        // if no rotation then no need count alrd la waste life
        if (rotation != 0)
        {
            // rotation formula -> translate to origin, calc rotation (x' = xcosTheta - ysinTheta, y' = xsinTheta + ycosTheta), translate back
            x = (x - center.x) * cos(rotation) - (y - center.y) * sin(rotation) + center.x;
            y = (x - center.x) * sin(rotation) + (y - center.y) * cos(rotation) + center.y;
        }
        if (clockwise)
        {
            x = -x;
            y = -y;
        }
        plot2D({ x, y, center.c });
    }
}

void Canvas::plot3D(Point3D p)
{
    setColor(p.c);
    glVertex3f(p.x * (reflectY ? -1 : 1) / (float)width, p.y * (reflectZ ? -1 : 1) / (float)height, p.z * (reflectX ? -1 : 1) / (float)depth);
    prev3DPoints.push_back(p);
}

void Canvas::plotCube(
    float top, float bottom, float left, float right, float front, float back, 
    Color topC, Color bottomC, Color leftC, Color rightC, Color frontC, Color backC
)
{
    begin(GL_QUADS);
    polyPlot3D({
        // front face
        { left, top, front, frontC }, // top left front
		{ right, top, front }, // top right front
		{ right, bottom, front }, // bottom right front
		{ left, bottom, front }, // bottom left front

		// left face
        { left, bottom, front, leftC }, // bottom left front
        { left, top, front }, // top left front
		{ left, top, back }, // top left back
		{ left, bottom, back }, // bottom left back

        // bottom face
        { left, bottom, back, bottomC }, // bottom left back
        { left, bottom, front }, // bottom left front
        { right, bottom, front }, // bottom right front
		{ right, bottom, back }, // bottom right back

        // back face
        { right, bottom, back, backC }, // bottom right back
		{ left, bottom, back }, // bottom left back
		{ left, top, back }, // top left back
		{ right, top, back }, // top right back

        // right face
        { right, top, back, rightC }, // top right back
        { right, bottom, back }, // right bottom back
        { right, bottom, front }, // bottom right front
		{ right, top, front }, // top right front

        // top face
        { right, top, front, topC }, // top right front
		{ right, top, back }, // top right back
        { left, top, back }, // top left back
		{ left, top, front }, // top left front
        });
    end();
}

Canvas::Canvas(int width, int height, int depth) : width(width), height(height), depth(depth) {}

Canvas& Canvas::clear(Color c)
{
    glClearColor(c.r / 255.0, c.g / 255.0, c.b / 255.0, c.a / 255.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return *this;
}

Canvas& Canvas::showDebugGrid(bool centerDiffColor, Color c, Color centerC)
{
    // draw horizontal lines
    for (int i = -height; i <= height; i++)
    {
        straightLine3D(
            {
                -(float)width, (float)i, (float)0,
                centerDiffColor && i == 0 ? centerC : c
            },
            {
                (float)width, (float)i, (float)0,
                centerDiffColor && i == 0 ? centerC : c
            }
            );
    }

    // vertical lines
    for (int i = -width; i <= width; i++)
    {
        straightLine3D(
            {
                (float)i, -(float)height, -(float)0,
                centerDiffColor && i == 0 ? centerC : c
            },
            {
                (float)i, (float)height, (float)0,
                centerDiffColor && i == 0 ? centerC : c
            }
            );
    }

    // depth lines
	// depth horizontal lines
	for (int i = -height; i <= height; i++)
	{
		straightLine3D(
			{
				(float)0, (float)i, -(float)depth,
				centerDiffColor && i == 0 ? centerC : c
			},
			{
				(float)0, (float)i, (float)depth,
				centerDiffColor && i == 0 ? centerC : c
			}
			);
	}

	// depth vertical lines
	for (int i = -depth; i <= depth; i++)
	{
		straightLine3D(
			{
				(float)0, -(float)height, (float)i,
				centerDiffColor && i == 0 ? centerC : c
			},
			{
				(float)0, (float)height, (float)i,
				centerDiffColor && i == 0 ? centerC : c
			}
			);
	}

    // depth ground lines
    for (int i = -depth; i <= depth; i++)
    {
		straightLine3D(
			{
				-(float)width, (float)0, (float)i,
				centerDiffColor && i == 0 ? centerC : c
			},
			{
				(float)width, (float)0, (float)i,
				centerDiffColor && i == 0 ? centerC : c
			}
			);
    }

	for (int i = -width; i <= width; i++)
	{
		straightLine3D(
			{
				(float)i, (float)0, -(float)depth,
				centerDiffColor && i == 0 ? centerC : c
			},
			{
				(float)i, (float)0, (float)depth,
				centerDiffColor && i == 0 ? centerC : c
			}
			);
	}

    return *this;
}

Canvas& Canvas::begin(GLenum mode, bool clearPrev)
{
    glBegin(mode);
    isTracking = true;
    // clear the previous points
    if (clearPrev)
        prev3DPoints.clear();
    return *this;
}

Canvas& Canvas::end()
{
    glEnd();
    isTracking = false;
    return *this;
}

Canvas& Canvas::pointSize(float size)
{
    glPointSize(size);
    return *this;
}

Canvas& Canvas::point(Point2D p)
{
    glBegin(GL_POINTS);
    plot2D(p);
    glEnd();

    return *this;
}

Canvas& Canvas::point(Point3D p)
{
    begin(GL_POINTS);
    plot3D(p);
    end();

    return *this;
}

Canvas& Canvas::polyPlot2D(std::vector<Point2D> points)
{
    for (Point2D p : points)
        plot2D(p);
    return *this;
}

Canvas& Canvas::polyPlot3D(std::vector<Point3D> points)
{
    for (Point3D p : points)
        plot3D(p);
    return *this;
}

Canvas& Canvas::lineWidth(float width)
{
    glLineWidth(width);

    return *this;
}

Canvas& Canvas::line(std::vector<Point2D> points)
{
    glBegin(GL_LINE_STRIP);
    polyPlot2D(points);
    glEnd();
    return *this;
}

Canvas& Canvas::straightLine(Point2D from, Point2D to)
{
    line({ from, to });
    return *this;
}

Canvas& Canvas::lineLoop(std::vector<Point2D> points)
{
    glBegin(GL_LINE_LOOP);
    polyPlot2D(points);
    glEnd();
    return *this;
}

Canvas& Canvas::line3D(std::vector<Point3D> points)
{
    begin(GL_LINE_STRIP);
    polyPlot3D(points);
    end();
    return *this;
}

Canvas& Canvas::straightLine3D(Point3D from, Point3D to)
{
    line3D({ from, to });
    return *this;
}

Canvas& Canvas::lineLoop3D(std::vector<Point3D> points)
{
    begin(GL_LINE_LOOP);
    polyPlot3D(points);
    end();
    return *this;
}

Canvas& Canvas::lineEllipsis(
    Point2D center,
    float rx, float ry,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount
)
{
    // use line strip so can shunbian use to draw arc (it wont close it automatically)
    glBegin(GL_LINE_STRIP);
    const float anglePerSegment = fullAngle / vertexCount;
    ellipsePlot2D(center, rx, ry, rotation, startAngle, fullAngle, vertexCount);
    glEnd();

    return *this;
}

Canvas& Canvas::lineCircle(
    Point2D center,
    float radius,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount
)
{
    lineEllipsis(center, radius, radius, rotation, startAngle, fullAngle, vertexCount);
    return *this;
}

Canvas& Canvas::poly(std::vector<Point2D> points)
{
    glBegin(GL_POLYGON);
    polyPlot2D(points);
    glEnd();

    return *this;
}

Canvas& Canvas::rect(Point2D topLeft, Point2D bottomRight)
{
    poly({
        topLeft,
        { bottomRight.x, topLeft.y },
        bottomRight,
        { topLeft.x, bottomRight.y }
        });

    return *this;
}

Canvas& Canvas::square(Point2D topLeft, float length)
{
    rect(
        topLeft,
        {
            topLeft.x + length,
            topLeft.y - length,
            topLeft.c
        }
    );

    return *this;
}

Canvas& Canvas::ellipsis(
    Point2D center,
    float rx, float ry,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount
)
{
    float anglePerSegment = fullAngle / vertexCount;
    glBegin(GL_TRIANGLE_FAN);
    plot2D(center);
    ellipsePlot2D(center, rx, ry, rotation, startAngle, fullAngle, vertexCount);
    glEnd();

    return *this;
}

Canvas& Canvas::circle(
    Point2D center, float radius,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount
)
{
    ellipsis(center, radius, radius, rotation, startAngle, fullAngle, vertexCount);
    return *this;
}

Canvas& Canvas::ellipsisGrad(
    Point2D center,
    float rx, float ry,
    Color circColor, Color centerColor,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount
)
{
    float anglePerSegment = fullAngle / vertexCount;
    glBegin(GL_TRIANGLE_FAN);
    plot2D({ center.x, center.y, centerColor});
    ellipsePlot2D({ center.x, center.y, circColor }, rx, ry, rotation, startAngle, fullAngle, vertexCount);
    glEnd();

    return *this;
}

Canvas& Canvas::circleGrad(
    Point2D center, float radius,
    Color circColor, Color centerColor,
    float rotation,
    float startAngle, float fullAngle,
    int vertexCount
)
{
    ellipsisGrad(center, radius, radius, circColor, centerColor, rotation, startAngle, fullAngle, vertexCount);
    return *this;
}

Canvas& Canvas::crescent(Point2D center, float rx, float ry, float fullness, float rotation, int vertex)
{
    float anglePerSegment = PI / vertex;
    glBegin(GL_TRIANGLE_STRIP);

    float x = center.x, y = center.y;

    // 90deg on top (need factor in rotation tho)
    y += ry;

    if (rotation != 0)
    {
        x = (x - center.x) * cos(rotation) - (y - center.y) * sin(rotation) + center.x;
        y = (x - center.x) * sin(rotation) + (y - center.y) * cos(rotation) + center.y;
    }
    plot2D({ x, y, center.c }); // 90deg on top

    for (int i = 1; i < vertex; i++)
    {
        x = rx * cos(anglePerSegment * i + PI / 2) + center.x;
        y = ry * sin(anglePerSegment * i + PI / 2) + center.y;

        if (rotation != 0)
        {
            x = (x - center.x) * cos(rotation) - (y - center.y) * sin(rotation) + center.x;
            y = (x - center.x) * sin(rotation) + (y - center.y) * cos(rotation) + center.y;
        }

        plot2D({ x, y });

        x = (1 - fullness) * rx * cos(anglePerSegment * i + PI / 2) + center.x;
        y = ry * sin(anglePerSegment * i + PI / 2) + center.y;

        if (rotation != 0)
        {
            x = (x - center.x) * cos(rotation) - (y - center.y) * sin(rotation) + center.x;
            y = (x - center.x) * sin(rotation) + (y - center.y) * cos(rotation) + center.y;
        }

        plot2D({ x, y });
    }

    // 90deg below (need factor in rotation)
    x = center.x, y = center.y - ry;

    if (rotation != 0)
    {
        x = (x - center.x) * cos(rotation) - (y - center.y) * sin(rotation) + center.x;
        y = (x - center.x) * sin(rotation) + (y - center.y) * cos(rotation) + center.y;
    }

    plot2D({ x, y }); // 90deg below
    glEnd();

    return *this;
}


Canvas& Canvas::loadIdentity()
{
    glLoadIdentity();

    return *this;
}

Canvas& Canvas::pushMatrix()
{
    glPushMatrix();

    return *this;
}

Canvas& Canvas::popMatrix()
{
    glPopMatrix();

    return *this;
}

Canvas& Canvas::translate(float x, float y, float z)
{
    glTranslatef(x / width, y / height, z / depth);

    return *this;
}

Canvas& Canvas::rotate(float angle, float x, float y, float z)
{
    glRotatef(angle, x, y, z);

    return *this;
}

Canvas& Canvas::scale(float x, float y, float z)
{
    glScalef(x, y, z);

    return *this;
}

Canvas& Canvas::polygonMode(GLenum face, GLenum mode)
{
    glPolygonMode(face, mode);
    return *this;
}

Canvas& Canvas::poly3D(std::vector<Point3D> points)
{
    begin(GL_POLYGON);
    polyPlot3D(points);
    end();
    return *this;
}

Canvas& Canvas::cube(Point3D center, float lenFromCenter)
{
    // TODO: insert return statement here
	float 
		left = center.x - lenFromCenter, 
		right = center.x + lenFromCenter,
		top = center.y + lenFromCenter,
		bottom = center.y - lenFromCenter,
		front = center.z + lenFromCenter,
		back = center.z - lenFromCenter;
   
    // single color
    plotCube(top, bottom, left, right, front, back, center.c, center.c, center.c, center.c, center.c, center.c);

    return *this;
}

Canvas& Canvas::cuboid(Point3D topLeftFront, Point3D bottomRightBack)
{
    float
        left = topLeftFront.x,
        right = bottomRightBack.x,
        top = topLeftFront.y,
        bottom = bottomRightBack.y,
        front = topLeftFront.z,
        back = bottomRightBack.z;

    // top left front same c, bottomrightback same c
    plotCube(top, bottom, left, right, front, back, topLeftFront.c, bottomRightBack.c, topLeftFront.c, bottomRightBack.c, topLeftFront.c, bottomRightBack.c);
    return *this;
}

Canvas& Canvas::cuboid(Point3D topLeftFront, Point3D topRightBack, Point3D bottomLeftFront, Point3D bottomRightBack)
{
    begin(GL_QUADS);
    polyPlot3D({
        // front face
        topLeftFront, // top left front
        { topRightBack.x, topRightBack.y, topLeftFront.z }, // top right front
        { bottomRightBack.x, bottomRightBack.y, bottomLeftFront.z }, // bottom right front
        bottomLeftFront, // bottom left front

        // left face
        topLeftFront, // top left front
        bottomLeftFront, // bottom left front
        { bottomLeftFront.x, bottomLeftFront.y, bottomRightBack.z }, // bottom left back
        { topLeftFront.x, topLeftFront.y, topRightBack.z }, // top left back

        // top face
        { topLeftFront.x, topLeftFront.y, topRightBack.z }, // top left back
        topLeftFront, // top left front
        { topRightBack.x, topRightBack.y, topLeftFront.z }, // top right front
        topRightBack, // top right back

        // back face
        topRightBack, // top right back
        { topLeftFront.x, topLeftFront.y, topRightBack.z }, // top left back
        { bottomLeftFront.x, bottomLeftFront.y, bottomRightBack.z }, // bottom left back
        bottomRightBack, // bottom right back

        // right face
        bottomRightBack, // bottom right back
        topRightBack, // top right back
        { topRightBack.x, topRightBack.y, topLeftFront.z }, // top right front
        { bottomRightBack.x, bottomRightBack.y, bottomLeftFront.z }, // bottom right front

        // bottom face
        { bottomRightBack.x, bottomRightBack.y, bottomLeftFront.z }, // bottom right front
        bottomRightBack, // bottom right back
        { bottomLeftFront.x, bottomLeftFront.y, bottomRightBack.z }, // bottom left back
        bottomLeftFront, // bottom left front
        });
    end();

    return *this;
}

Canvas& Canvas::pyramid(Point3D centerBase, float height, float lenFromCenter)
{
    float
        left = centerBase.x - lenFromCenter,
        right = centerBase.x + lenFromCenter,
        front = centerBase.z + lenFromCenter,
        back = centerBase.z - lenFromCenter,
        top = centerBase.y + height;
    Point3D mid = { centerBase.x, top, centerBase.z };

    begin(GL_QUADS);
    polyPlot3D({
        // base
        { left, centerBase.y, front, centerBase.c}, // front left
        { right, centerBase.y, front }, // front right
        { right, centerBase.y, back }, // back right
        { left, centerBase.y, back }, // back left
        });
	end();

    begin(GL_TRIANGLES, false); // but don't stop tracking those i plotted

    polyPlot3D({
        // left pane
        { left, centerBase.y, back },
        { left, centerBase.y, front },
        mid,

        // back pane
        { left, centerBase.y, back },
        { right, centerBase.y, back },
        mid,

        // right pane
        { right, centerBase.y, back },
        { right, centerBase.y, front },
        mid,

        // front pane
        { left, centerBase.y, front },
        { right, centerBase.y, front},
        mid,
        });
    end();

    return *this;
}

Canvas& Canvas::pyramid(Point3D centerBaseLeftFront, Point3D centerBaseRightBack, Point3D mid)
{
    float
        left = centerBaseLeftFront.x,
        right = centerBaseRightBack.x,
        front = centerBaseLeftFront.z,
        back = centerBaseRightBack.z,
        base = centerBaseLeftFront.y;

    begin(GL_QUADS);
    polyPlot3D({
        // base
        { left, base, front, centerBaseLeftFront.c}, // front left
        { right, base, front }, // front right
        { right, base, back }, // back right
        { left, base, back }, // back left
        });
	end();

    begin(GL_TRIANGLES, false); // but don't stop tracking those i plotted

    polyPlot3D({
        // left pane
        { left, base, back },
        { left, base, front },
        mid,

        // back pane
        { left, base, back },
        { right, base, back },
        mid,

        // right pane
        { right, base, back },
        { right, base, front },
        mid,

        // front pane
        { left, base, front },
        { right, base, front},
        mid,
        });
    end();

    return *this;
}

Canvas& Canvas::replotPrevBlocky3D(GLenum type, Color c)
{
    // cannot use the begin() method or it will clear the thing
    glBegin(type);
    // custom handling cuz nid handle special color
    if (c.isNotInit())
        polyPlot3D(prev3DPoints);
    else
    {
		setColor(c); 
        for (Point3D p : prev3DPoints)
        {
            plot3D({ p.x, p.y, p.z });
        }
    }
    glEnd();
    return *this;
}

Canvas& Canvas::sphere(Point3D center, float radius, GLenum drawStyle, int slices, int stacks)
{
    // translate the sphere to the correct coord
    pushMatrix();
    translate(center.x, center.y, center.z);
    setColor(center.c);
    // actually drawing the sphere with glu
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, drawStyle);
    gluSphere(quadric, radius / width, slices, stacks); // divide any dimension also will give same result since it is sphere
    gluDeleteQuadric(quadric);
    popMatrix();
    return *this;
}

Canvas& Canvas::cylinder(Point3D center, float baseRadius, float topRadius, float height, bool closed, GLenum drawStyle, int slices, int stacks)
{
    // translate the cylinder to the correct coord
    // gluCylinder generates base circle at z = 0, so if wan fully center, need to translate it by height / 2 in negative z direction first
    pushMatrix();
    translate(center.x, center.y, center.z);
    translate(0, 0, -(height / depth / 2));
    // if closed then need draw 2 circle then translate them to cover them up
    if (closed)
    {
        // if drawStyle is GLU_LINE, then use the same render method (GL_LINE)
        if (drawStyle == GLU_LINE)
			polygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // base circle no need translate already since already at same point as baseRadius
        pushMatrix();
		// trough trial and error it seems that 6 degree rotation makes the triangles align with the cylinder
		circle({ 0, 0, center.c }, topRadius, 6.0 / 180 * PI, 0, 2 * PI, slices);
        popMatrix();

        // if topRadius is not 0 then is not cone and need to draw the circle to cover as well
        if (topRadius > 0)
        {
            pushMatrix();
            translate(0, 0, height);
			circle({ 0, 0, center.c }, topRadius, 6.0 / 180 * PI, 0, 2 * PI, slices);
            popMatrix();
        }

        // reset polygon mode
        polygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    setColor(center.c);
    // actually drawing the sphere with glu
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, drawStyle);
    gluCylinder(quadric, baseRadius / width, topRadius / width, height / depth, slices, stacks); // divide any dimension also will give same result since it is sphere
    gluDeleteQuadric(quadric);
    popMatrix();
    return *this;
}

Canvas& Canvas::setProjection(ProjectionMode p)
{
	glMatrixMode(GL_PROJECTION); // to change the projection matrix
	glLoadIdentity();
	// use 1 for all since canvas already handles all the stuff
	if (p == ProjectionMode::ORTHO)
        glOrtho(-1, 1, -1, 1, -1, 1);
    else
		// dc d for some reason discover tiok this best idk why
		glFrustum(-1, 1, -1, 1, 1, 100000);
	glMatrixMode(GL_MODELVIEW); // reset back to model matrix
	glLoadIdentity();
	// kena translate the whole thing away so it won't kena cut off by near plane if projection mode is perspective
    if (p == ProjectionMode::PERSPECTIVE)
    {
		gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0); // look at the origin from z = 2 since clipping pane is at z = 1
        // gluLookAt just easier help us calculate need translate how much
    }

    return *this;
}

Canvas& Canvas::switchLight(bool onOff)
{
    onOff ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
    return *this;
}

Canvas& Canvas::defineLight(GLenum light, GLenum paramName, float* params)
{
    // only enable if it is not enabled already
    if (!glIsEnabled(light))
        glEnable(light);
    if (paramName == GL_POSITION)
    {
        // this is unsafe operation, but ik what i am doing
        float positionCopy[4]; 
        std::copy(params, params + 4, positionCopy);
        // if is position then change the parameters to be relative to the canvas
        *(positionCopy) /= width;
        *(positionCopy+1) /= height;
        *(positionCopy+2) /= depth;
		glLightfv(light, paramName, positionCopy);
        return *this;
    }
    glLightfv(light, paramName, params);
    return *this;
}

Canvas& Canvas::disableLight(GLenum light)
{
    if (glIsEnabled(light))
        glDisable(light);
    return *this;
}

Canvas& Canvas::defineMaterial(GLenum face, GLenum paramName, float* param)
{
    glMaterialfv(face, paramName, param);
    return *this;
}

