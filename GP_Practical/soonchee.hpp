#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

namespace SoonChee
{
	void drawPyramid(float size)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, size / 10.0);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glVertex3f(size, 0.0, size / 10.0);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glVertex3f(size, 0.0, 0.0);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glVertex3f(0.0, 0.0, size / 10.0);
		glVertex3f(size, 0.0, size / 10.0);
		glVertex3f(size, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();
	}

	void drawRect(float width, float length)
	{
		glBegin(GL_LINE_LOOP);
		//bottom face 1
		glColor3f(1.0, 1.0, 1.0); //red
		glVertex3f(0.0, 0.0, length);
		glVertex3f(width, 0.0, length);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//left face 2
		glColor3f(1.0, 1.0, 1.0); //green
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width * 3, 0.0);
		glVertex3f(0.0, width * 3, length);
		glVertex3f(0.0, 0.0, length);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//front face 3
		glColor3f(1.0, 1.0, 1.0); //blue
		glVertex3f(0.0, 0.0, length);
		glVertex3f(0.0, width * 3, length);
		glVertex3f(width, width * 3, length);
		glVertex3f(width, 0.0, length);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//right face 4
		glColor3f(1.0, 1.0, 1.0); //yellow
		glVertex3f(width, 0.0, length);
		glVertex3f(width, width * 3, length);
		glVertex3f(width, width * 3, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//back face 5
		glColor3f(1.0, 1.0, 1.0); //cyan
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width * 3, 0.0);
		glVertex3f(width, width * 3, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//top face 6
		glColor3f(1.0, 1.0, 1.0); //magneta
		glVertex3f(width, width * 3, 0.0);
		glVertex3f(0.0, width * 3, 0.0);
		glVertex3f(0.0, width * 3, length);
		glVertex3f(width, width * 3, length);
		glEnd();
	}

	void drawCylinder(float radius, float height)
	{
		GLUquadricObj* cylinder = NULL;			//create quadric obj pointer
		cylinder = gluNewQuadric();				//create the quadric obj
		gluQuadricDrawStyle(cylinder, GLU_FILL);	//set the draw style
		gluCylinder(cylinder, radius, radius, height, 30, 30);//draw sphere
		gluDeleteQuadric(cylinder);				//delete the quadric obj
	}

	void drawCone(float radius, float height)
	{
		GLUquadricObj* cylinder = NULL;			//create quadric obj pointer
		cylinder = gluNewQuadric();				//create the quadric obj
		gluQuadricDrawStyle(cylinder, GLU_FILL);	//set the draw style
		gluCylinder(cylinder, radius, 0, height, 20, 6);//draw sphere
		gluDeleteQuadric(cylinder);				//delete the quadric obj
	}

	void face(float radius)
	{
		float x = 0.0;
		float y = 0.0;
		float x2 = 0.0;
		float y2 = 0.0;

		//glLineWidth(3);
		glBegin(GL_TRIANGLE_FAN);

		for (float angle = 0; angle <= (2 * 3.1429); angle += (2 * 3.1429) / 100)
		{
			float x2 = x + radius * cos(angle);
			float y2 = y + radius * sin(angle);
			glVertex2f(x2, y2);
		}
		glEnd();
	}

	void drawSqu(float width, float length)
	{
		glBegin(GL_LINE_LOOP);
		//bottom face 1
		glColor3f(1.0, 1.0, 1.0); //red
		glVertex3f(0.0, 0.0, length);
		glVertex3f(width, 0.0, length);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//left face 2
		glColor3f(1.0, 1.0, 1.0); //green
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width / 2, 0.0);
		glVertex3f(0.0, width / 2, length);
		glVertex3f(0.0, 0.0, length);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//front face 3
		glColor3f(1.0, 1.0, 1.0); //blue
		glVertex3f(0.0, 0.0, length);
		glVertex3f(0.0, width / 2, length);
		glVertex3f(width, width / 2, length);
		glVertex3f(width, 0.0, length);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//right face 4
		glColor3f(1.0, 1.0, 1.0); //yellow
		glVertex3f(width, 0.0, length);
		glVertex3f(width, width / 2, length);
		glVertex3f(width, width / 2, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//back face 5
		glColor3f(1.0, 1.0, 1.0); //cyan
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width / 2, 0.0);
		glVertex3f(width, width / 2, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//top face 6
		glColor3f(1.0, 1.0, 1.0); //magneta
		glVertex3f(width, width / 2, 0.0);
		glVertex3f(0.0, width / 2, 0.0);
		glVertex3f(0.0, width / 2, length);
		glVertex3f(width, width / 2, length);
		glEnd();
	}

	void drawSphereWithoutGLU(float givenRadius)
	{
		//const float PI = 3.141592f;
		GLfloat x, y, z, sliceA, stackA;
		GLfloat radius = givenRadius;
		int sliceNo = 30, stackNo = 30;

		for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
		{
			glBegin(GL_TRIANGLE_FAN);
			for (stackA = 0.0; stackA < 2 * PI; stackA += PI / stackNo)
			{
				x = radius * cos(stackA) * sin(sliceA);
				y = radius * sin(stackA) * sin(sliceA);
				z = radius * cos(sliceA);
				glVertex3f(x, y, z);
				x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
				y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
				z = radius * cos(sliceA + PI / sliceNo);
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}

	void rect(float size)
	{
		glColor3f(1, 1, 0);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(size, 0.0);
		glVertex2f(size, size / 2);
		glVertex2f(0, size / 2);
		glEnd();
	}

	void drawShield(float width, float length)
	{
		glPushMatrix();
		//glColor3f(1, 1, 0);
		rect(0.2);
		glPopMatrix();

		//glClearColor(1, 1, 1, 1);
		//glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		//bottom face 1
		//glColor3f(1.0, 1.0, 1.0); //red
		glVertex3f(0.0, 0.0, length);
		glVertex3f(width, 0.0, length);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_QUADS);
		//left face 2
		//glColor3f(1.0, 1.0, 1.0); //green
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width * 2, 0.0);
		glVertex3f(0.0, width * 2, length);
		glVertex3f(0.0, 0.0, length);
		glEnd();

		glBegin(GL_QUADS);
		//front face 3
		//glColor3f(1.0, 1.0, 1.0); //blue
		glVertex3f(0.0, 0.0, length);
		glVertex3f(0.0, width * 2, length);
		glVertex3f(width, width * 2, length);
		glVertex3f(width, 0.0, length);
		glEnd();

		glBegin(GL_QUADS);
		//right face 4
		//glColor3f(1.0, 1.0, 1.0); //yellow
		glVertex3f(width, 0.0, length);
		glVertex3f(width, width * 2, length);
		glVertex3f(width, width * 2, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glEnd();

		glBegin(GL_QUADS);
		//back face 5
		//glColor3f(1.0, 1.0, 1.0); //cyan
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width * 2, 0.0);
		glVertex3f(width, width * 2, 0.0);
		glEnd();

		glBegin(GL_QUADS);
		//top face 6
		//glColor3f(1.0, 1.0, 1.0); //magneta
		glVertex3f(width, width * 2, 0.0);
		glVertex3f(0.0, width * 2, 0.0);
		glVertex3f(0.0, width * 2, length);
		glVertex3f(width, width * 2, length);
		glEnd();
	}

	void drawShieldLine(float width, float length)
	{
		//rect(0.2);

		glBegin(GL_LINES);
		//bottom face 1
		//glColor3f(1.0, 1.0, 1.0); //red
		glVertex3f(0.0, 0.0, length);
		glVertex3f(width, 0.0, length);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		//left face 2
		//glColor3f(1.0, 1.0, 1.0); //green
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width * 2, 0.0);
		glVertex3f(0.0, width * 2, length);
		glVertex3f(0.0, 0.0, length);
		glEnd();

		glBegin(GL_LINES);
		//front face 3
		//glColor3f(1.0, 1.0, 1.0); //blue
		glVertex3f(0.0, 0.0, length);
		glVertex3f(0.0, width * 2, length);
		glVertex3f(width, width * 2, length);
		glVertex3f(width, 0.0, length);
		glEnd();

		glBegin(GL_LINES);
		//right face 4
		//glColor3f(1.0, 1.0, 1.0); //yellow
		glVertex3f(width, 0.0, length);
		glVertex3f(width, width * 2, length);
		glVertex3f(width, width * 2, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		//back face 5
		//glColor3f(1.0, 1.0, 1.0); //cyan
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width * 2, 0.0);
		glVertex3f(width, width * 2, 0.0);
		glEnd();

		glBegin(GL_LINES);
		//top face 6
		//glColor3f(1.0, 1.0, 1.0); //magneta
		glVertex3f(width, width * 2, 0.0);
		glVertex3f(0.0, width * 2, 0.0);
		glVertex3f(0.0, width * 2, length);
		glVertex3f(width, width * 2, length);
		glEnd();
	}

	void sword()
	{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPushMatrix();
		glRotatef(0.2, 0, 1, 0);


		//sword head
		glPushMatrix();
		glTranslatef(0, 0.45, 0);
		drawPyramid(0.17);
		glPopMatrix();

		//sword body
		glPushMatrix();
		glTranslatef(0, -0.06, 0);
		drawRect(0.17, 0.02);
		glPopMatrix();

		//sword center
		glPushMatrix();
		glTranslatef(0.085, -0.10, 0);
		glColor3f(1, 0, 0);
		drawCylinder(0.09, 0.017);
		glPopMatrix();
		//sword center face
		glPushMatrix();
		glTranslatef(0.085, -0.10, 0.021);
		glColor3f(0, 1, 0);
		face(0.09);
		glPopMatrix();
		//sword center face
		glPushMatrix();
		glTranslatef(0.085, -0.10, 0);
		glColor3f(0, 0, 1);
		face(0.09);
		glPopMatrix();

		//sword left design - cone
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.009, -0.12, -0.05);
		glColor3f(1, 1, 0);
		drawCone(0.045, 0.05);
		glPopMatrix();
		//sword left design - cylinder
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glColor3f(1, 1, 1);
		glTranslatef(-0.009, -0.12, -0.16);
		drawCylinder(0.03, 0.11);
		glPopMatrix();

		//sword left design - cylinder ii
		glPushMatrix();;
		glColor3f(1, 0, 1);
		glTranslatef(-0.17, -0.12, -0.022);
		drawCylinder(0.03, 0.04);
		glPopMatrix();

		//sword left design - cylinder face
		glPushMatrix();
		glTranslatef(-0.17, -0.118, 0.025);
		glColor3f(0, 1, 1);
		face(0.029);
		glPopMatrix();

		//sword left design - cylinder face
		glPushMatrix();
		glTranslatef(-0.17, -0.118, -0.025);
		glColor3f(1, 1, 0);
		face(0.029);
		glPopMatrix();

		//sword right design - cone
		glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		glTranslatef(-0.009, -0.12, -0.22);
		glColor3f(1, 1, 0);
		drawCone(0.045, 0.05);
		glPopMatrix();

		//sword right design - cylinder
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glColor3f(1, 1, 1);
		glTranslatef(-0.009, -0.12, 0.22);
		drawCylinder(0.03, 0.12);
		glPopMatrix();

		//sword right design - cylinder ii
		glPushMatrix();;
		glColor3f(1, 0, 1);
		glTranslatef(0.35, -0.12, -0.022);
		drawCylinder(0.03, 0.04);
		glPopMatrix();

		//sword right design - cylinder face
		glPushMatrix();
		glTranslatef(0.35, -0.118, 0.025);
		glColor3f(0, 1, 1);
		face(0.029);
		glPopMatrix();

		//sword right design - cylinder face
		glPushMatrix();
		glTranslatef(0.35, -0.118, -0.025);
		glColor3f(1, 1, 0);
		face(0.029);
		glPopMatrix();

		//sword handle
		glPushMatrix();
		glTranslatef(0, -0.21, -0.017);
		drawSqu(0.17, 0.05);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.025, -0.25, -0.012);
		drawSqu(0.12, 0.04);
		glPopMatrix();

		//sword grip
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.08, 0.0075, 0.22);
		drawCylinder(0.01, 0.15);
		glPopMatrix();

		//sword bottom
		glPushMatrix();
		glTranslatef(0.08, -0.37, 0.008);
		drawSphereWithoutGLU(0.03);
		glPopMatrix();
		//glPopMatrix();
	}

	void shield()
	{
		glPushMatrix();
		glTranslatef(0, -0.5, 0);
		glColor3f(1, 0.5, 0.5);
		drawShield(0.5, 0.1);
		glColor3f(1, 1, 1);
		drawShieldLine(0.4, 0.1);
		glPopMatrix();
	}

	void hammerBuild(float width, float length)
	{
		glBegin(GL_LINE_LOOP);
		//bottom face 1
		glColor3f(1.0, 1.0, 1.0); //red
		glVertex3f(0.0, 0.0, length);
		glVertex3f(width, 0.0, length);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//left face 2
		glColor3f(1.0, 1.0, 1.0); //green
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(width / 4, width / 2, 0.0);
		glVertex3f(width / 4, width / 2, length);
		glVertex3f(0.0, 0.0, length);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//front face 3
		glColor3f(1.0, 1.0, 1.0); //blue
		glVertex3f(0.0, 0.0, length);
		glVertex3f(width / 4, width / 2, length);
		glVertex3f(width - (width / 4), width / 2, length);
		glVertex3f(width, 0.0, length);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//right face 4
		glColor3f(1.0, 1.0, 1.0); //yellow
		glVertex3f(width, 0.0, length);
		glVertex3f(width - (width / 4), width / 2, length);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//back face 5
		glColor3f(1.0, 1.0, 1.0); //cyan
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glVertex3f(width / 4, width / 2, 0.0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		//top face 6
		glColor3f(1.0, 1.0, 1.0); //magneta
		glVertex3f(width / 4, width / 2, 0.0);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glVertex3f(width - (width / 4), width / 2, length);
		glVertex3f(width / 4, width / 2, length);
		glEnd();
	}

	void hammer()
	{
		drawSqu(0.4, 0.2);
		glPushMatrix();
		glRotatef(90, 0, 0, 1);
		hammerBuild(0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-0.2, 0.4, 0);
		hammerBuild(0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.2, 0.1, 0);
		glColor3f(1, 1, 1);
		drawCylinder(0.05, 0.3);
		glPopMatrix();
	}
}