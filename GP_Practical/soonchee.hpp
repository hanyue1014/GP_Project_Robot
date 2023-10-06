#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

//Step 1: Variable declaration
BITMAP BMP;             //bitmap structure
HBITMAP hBMP = NULL;    //bitmap handle

namespace SoonChee
{
	GLuint loadTexture(LPCSTR filename)
	{
		GLuint texture = 0;     //texture name

		//Step 3: Initialize texture info
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
			filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

		GetObject(hBMP, sizeof(BMP), &BMP);

		//Step 4: Assign texture to polygon.
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0,
			GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
		DeleteObject(hBMP);
		return texture;
	}

	GLuint loadCylinderTexture(LPCSTR filename)
	{
		GLUquadricObj* cylinder = NULL;			//create quadric obj pointer
		cylinder = gluNewQuadric();				//create the quadric obj
		GLuint texture = 0;     //texture name

		//Step 3: Initialize texture info
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
			filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

		GetObject(hBMP, sizeof(BMP), &BMP);

		//Step 4: Assign texture to polygon.
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gluQuadricTexture(cylinder, true);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0,
			GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
		DeleteObject(hBMP);
		return texture;
	}

	void drawPyramid(float size)
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(0.0, 0.0, size / 10.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size, 0.0, size / 10.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, size / 10.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size, 0.0, size / 10.0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size, 0.0, size / 10.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, 0.0, 0.0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / 2.0, size / 2.0, size / 20.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, size / 10.0);
		glEnd();
	}

	void drawRect(float width, float length)
	{
		glBegin(GL_QUADS);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
	
		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width * 3, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width * 3, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width * 3, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width * 3, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);
	
		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, width * 3, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width * 3, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width * 3, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, width * 3, 0.0);
	
		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, width * 3, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width * 3, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width * 3, length);
		glTexCoord2f(0.0f, 0.0f);
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
		glBegin(GL_QUADS);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);

		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);

		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, width / 2, 0.0);

		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, width / 2, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
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

	void sword()
	{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
		//glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glTranslatef(-0.082, 0.3, 0);
		//glRotatef(0.2, 0, 1, 0);

		//sword head
		glPushMatrix();
		glTranslatef(0, 0.45, 0);
		GLuint textureArr[5];
		textureArr[0] = loadTexture("metal.bmp");
		glColor3f(0.74, 0.74, 0.73);
		drawPyramid(0.17);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0, 0, 0);
		glPopMatrix();

		//sword body
		glPushMatrix();
		glTranslatef(0, -0.06, 0);
		textureArr[0] = loadTexture("metal.bmp");
		glColor3f(0.74, 0.74, 0.73);
		drawRect(0.17, 0.02);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//sword center
		glPushMatrix();
		glTranslatef(0.085, -0.10, 0);
		textureArr[1] = loadCylinderTexture("metal.bmp");
		glColor3f(0.44, 0.44, 0.43);
		drawCylinder(0.09, 0.02);
		glDeleteTextures(1, &textureArr[1]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		//sword center face
		glPushMatrix();
		glTranslatef(0.085, -0.10, 0.019);
		glColor3f(0.44, 0.44, 0.43);
		face(0.09);
		glPopMatrix();
		//sword center face
		glPushMatrix();
		glTranslatef(0.085, -0.10, 0.0001);
		glColor3f(0.44, 0.44, 0.43);
		face(0.09);
		glPopMatrix();

		//sword left design - cone
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.009, -0.12, -0.05);
		glColor3f(0.83, 0.69, 0.22);
		drawCone(0.045, 0.05);
		glPopMatrix();

		//sword left design - cylinder
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glColor3f(0.44, 0.44, 0.43);
		glTranslatef(-0.009, -0.12, -0.16);
		drawCylinder(0.03, 0.11);
		glPopMatrix();

		//sword left design - cylinder face
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.01, -0.12, -0.16);
		glColor3f(0.85, 0.89, 0.84);
		face(0.03);
		glPopMatrix();

		//sword right design - cone
		glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0.009, -0.12, -0.22);
		glColor3f(0.83, 0.69, 0.22);
		drawCone(0.045, 0.05);
		glPopMatrix();

		//sword right design - cylinder
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glColor3f(0.44, 0.44, 0.43);
		glTranslatef(-0.009, -0.12, 0.22);
		drawCylinder(0.03, 0.11);
		glPopMatrix();

		//sword right design - cylinder face
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.01, -0.12, 0.33);
		glColor3f(0.85, 0.89, 0.84);
		face(0.03);
		glPopMatrix();

		//sword handle
		glPushMatrix();
		glTranslatef(0, -0.21, -0.017);
		textureArr[0] = loadTexture("metal.bmp");
		drawSqu(0.17, 0.05);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.025, -0.25, -0.012);
		textureArr[0] = loadTexture("metal.bmp");
		drawSqu(0.12, 0.04);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//sword grip
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.08, 0.0075, 0.22);
		textureArr[2] = loadCylinderTexture("wood.bmp");
		drawCylinder(0.01, 0.15);
		glDeleteTextures(1, &textureArr[2]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//sword bottom
		glPushMatrix();
		glTranslatef(0.08, -0.37, 0.008);
		glColor3f(0.85, 0.89, 0.84);
		drawSphereWithoutGLU(0.03);
		glPopMatrix();
		glPopMatrix();
	}

	void rect(float size)
	{
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(size, 0.0);
		glVertex2f(size, size / 2);
		glVertex2f(0, size / 2);
		glEnd();
	}

	void sheildSide(float width, float length)
	{
		glBegin(GL_QUADS);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);

		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 6, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width / 4, width / 6, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 6, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);

		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 6, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 6, 0.0);

		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width / 4, width / 6, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 6, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 6, length);
		glEnd();
	}

	void sheildSideLine(float width, float length)
	{
		glBegin(GL_LINE_LOOP);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);

		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 6, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width / 4, width / 6, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 6, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);

		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 6, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 6, 0.0);

		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width / 4, width / 6, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 6, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 6, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 6, length);
		glEnd();
	}

	void hammerBuild(float width, float length)
	{
		glBegin(GL_QUADS);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);

		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width / 4, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);

		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 2, 0.0);

		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width / 4, width / 2, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 2, length);
		glEnd();
	}

	void hammerBuildLine(float width, float length)
	{
		glBegin(GL_LINE_LOOP);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);

		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width / 4, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width / 4, width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);

		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 2, 0.0);

		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width / 4, width / 2, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width - (width / 4), width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width - (width / 4), width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width / 4, width / 2, length);
		glEnd();
	}

	void drawShield(float width, float length)
	{
		glPushMatrix();
		glTranslatef(0, 0, length + 0.001);

		glPushMatrix();
		glTranslatef(0.05, 0.4, 0);
		glColor3f(0.5, 0.5, 0.5);
		rect(0.3);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.275, 0.4, 0);
		glRotatef(90, 0, 0, 1);
		glColor3f(0.5, 0.5, 0.5);
		rect(0.3);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.275, 0.1, 0);
		glRotatef(90, 0, 0, 1);
		glColor3f(0.5, 0.5, 0.5);
		rect(0.3);
		glPopMatrix();
		glPopMatrix();

		//glClearColor(1, 1, 1, 1);
		//glColor3f(1, 0.5, 1);
		glBegin(GL_QUADS);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);

		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width * 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width * 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);

		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width * 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width * 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);

		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, width * 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width * 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);

		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width * 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, width * 2, 0.0);

		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, width * 2, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width * 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width * 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, width * 2, length);
		glEnd();
	}

	void drawShieldLine(float width, float length)
	{
		glLineWidth(2);

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

	void shield(bool combined, bool isLeft)
	{
		glPushMatrix();
		glTranslatef(-0.2, 0, 0);
		//shield
		glPushMatrix();
		glTranslatef(0, -0.5, 0);
		//glColor3f(1, 1,1);
		GLuint textureArr[1];
		textureArr[0] = loadTexture("shield.bmp");
		drawShield(0.4, 0.1);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		drawShieldLine(0.4, 0.1);
		glPopMatrix();

		//outer
		glPushMatrix();
		glTranslatef(0, 0.3, 0);
		textureArr[0] = loadTexture("shield.bmp");
		hammerBuild(0.4, 0.1);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		hammerBuildLine(0.4, 0.1);
		glPopMatrix();
		glPushMatrix();
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, 0.5, -0.1);
		textureArr[0] = loadTexture("shield.bmp");
		hammerBuild(0.4, 0.1);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		hammerBuildLine(0.4, 0.1);
		glPopMatrix();
		if (combined && isLeft) {}
		else 
		{
			glPushMatrix();
			glRotatef(90, 0, 0, -1);
			glTranslatef(-0.3, 0.4, 0);
			textureArr[0] = loadTexture("shield.bmp");
			sheildSide(0.8, 0.1);
			glDeleteTextures(1, &textureArr[0]);
			glDisable(GL_TEXTURE_2D);
			glColor3f(1, 1, 1);
			sheildSideLine(0.8, 0.1);
			glPopMatrix();
		}
		if (combined && !isLeft) {}
		else
		{
			glPushMatrix();
			glRotatef(90, 0, 0, 1);
			glTranslatef(-0.5, 0, 0);
			textureArr[0] = loadTexture("shield.bmp");
			sheildSide(0.8, 0.1);
			glDeleteTextures(1, &textureArr[0]);
			glDisable(GL_TEXTURE_2D);
			glColor3f(1, 1, 1);
			sheildSideLine(0.8, 0.1);
			glPopMatrix();
		}
		glPopMatrix();
	}

	void drawSquLine(float width, float length)
	{
		glBegin(GL_LINES);
		//bottom face 1
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		//left face 2
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, length);
		glEnd();

		glBegin(GL_LINES);
		//front face 3
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, length);
		glEnd();

		glBegin(GL_LINES);
		//right face 4
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, length);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, width / 2, length);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		//back face 5
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width / 2, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, width / 2, 0.0);
		glEnd();

		glBegin(GL_LINES);
		//top face 6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, width / 2, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0, width / 2, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0, width / 2, length);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, width / 2, length);
		glEnd();
	}

	void hammer()
	{
		glPushMatrix();
		glTranslatef(-0.2, 0, 0);
		GLuint textureArr[2];
		textureArr[0] = loadTexture("shield.bmp");
		glColor3f(0.44, 0.44, 0.43);
		drawSqu(0.4, 0.2);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		drawSquLine(0.4, 0.2);
		glPushMatrix();
		glRotatef(90, 0, 0, 1);
		textureArr[0] = loadTexture("shield.bmp");
		glColor3f(0.44, 0.44, 0.43);
		hammerBuild(0.2, 0.2);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		hammerBuildLine(0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-0.2, 0.4, 0);
		textureArr[0] = loadTexture("shield.bmp");
		glColor3f(0.44, 0.44, 0.43);
		hammerBuild(0.2, 0.2);
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		hammerBuildLine(0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.2, 0.1, 0);
		textureArr[1] = loadCylinderTexture("wood.bmp");
		glColor3f(0.79, 0.64, 0.45);
		drawCylinder(0.05, 0.3);
		glDeleteTextures(1, &textureArr[1]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.2, 0.1, 0.3);
		glColor3f(0.79, 0.64, 0.45);
		face(0.05);
		glPopMatrix();
		glPopMatrix();
	}
	void perspectiveBackground()
	{
		glPushMatrix();
		glTranslatef(0, 0, -10);
		glScalef(15, 15, 15);
		glColor3f(1, 1, 1);
		GLuint textureArr[1];
		textureArr[0] = loadTexture("background.bmp");
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(1, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(-1, 1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(-1, -1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(1, -1);
		glEnd();
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	void orthoBackground()
	{
		glPushMatrix();
		glTranslatef(0, 0, -0.9);
		glScalef(1, 1, 1);
		glColor3f(1, 1, 1);
		GLuint textureArr[1];
		textureArr[0] = loadTexture("background.bmp");
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(1, 1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(-1, 1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(-1, -1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(1, -1);
		glEnd();
		glDeleteTextures(1, &textureArr[0]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

}