#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

namespace YiKit
{
	
	void cuboid()
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		//bottom
		glVertex3f(-0.3, 0.0, -1.0);
		glVertex3f(-0.3, 0.0, 0.0);
		glVertex3f(0.7, 0.0, 0.0);
		glVertex3f(0.7, 0.0, -1.0);
		//inside z
		glVertex3f(0.7, 0.0, -1.0);
		glVertex3f(-0.3, 0.0, -1.0);
		glVertex3f(-0.3, 0.3, -1.0);
		glVertex3f(0.7, 0.3, -1.0);

		//right
		glVertex3f(0.7, 0.3, -1.0);
		glVertex3f(0.7, 0.0, -1.0);
		glVertex3f(0.7, 0.0, 0.0);
		glVertex3f(0.7, 0.3, 0.0);
		//front 
		glVertex3f(0.7, 0.3, 0.0);
		glVertex3f(0.7, 0.0, 0.0);
		glVertex3f(-0.3, 0.0, 0.0);
		glVertex3f(-0.3, 0.3, 0.0);

		//top
		glVertex3f(-0.3, 0.3, 0.0);
		glVertex3f(0.7, 0.3, 0.0);
		glVertex3f(0.7, 0.3, -1.0);
		glVertex3f(-0.3, 0.3, -1.0);

		//left
		glVertex3f(-0.3, 0.3, -1.0);
		glVertex3f(-0.3, 0.0, -1.0);
		glVertex3f(-0.3, 0.0, 0.0);
		glVertex3f(-0.3, 0.3, 0.0);


		glEnd();
	}
	void handle() {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		//top
		glVertex3f(0.25, 0.0, -1.0);
		glVertex3f(0.45, 0.0, -1.0);
		glVertex3f(0.45, 0.0, 0.0);
		glVertex3f(0.25, 0.0, 0.0);

		//left
		glVertex3f(0.25, 0.0, 0.0);
		glVertex3f(0.25, -0.55, 0.0);
		glVertex3f(0.25, -0.55, -1.0);
		glVertex3f(0.25, 0.0, -1.0);
		//behind
		glVertex3f(0.25, 0.0, -1.0);
		glVertex3f(0.25, -0.55, -1.0);
		glVertex3f(0.45, -0.55, -1.0);
		glVertex3f(0.45, 0.0, -1.0);
		//right
		glVertex3f(0.45, 0.0, -1.0);
		glVertex3f(0.45, -0.55, -1.0);
		glVertex3f(0.45, -0.55, 0.0);
		glVertex3f(0.45, 0.0, 0.0);
		//front
		glVertex3f(0.45, 0.0, 0.0);
		glVertex3f(0.25, 0.0, 0.0);
		glVertex3f(0.25, -0.55, 0.0);
		glVertex3f(0.45, -0.55, 0.0);
		//bottom
		glVertex3f(0.45, -0.55, 0.0);
		glVertex3f(0.45, -0.55, -1.0);
		glVertex3f(0.25, -0.55, -1.0);
		glVertex3f(0.25, -0.55, 0.0);
		glEnd();

	}
	void presser() {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		//top
		glVertex3f(0.05, 0.0, 0.0);
		glVertex3f(0.05, 0.0, -1.0);
		glVertex3f(0.1, 0.0, -1.0);
		glVertex3f(0.1, 0.0, 0.0);
		//front 
		glVertex3f(0.1, 0.0, 0.0);
		glVertex3f(0.1, -0.2, 0.0);
		glVertex3f(0.05, -0.2, 0.0);
		glVertex3f(0.05, 0.0, 0.0);
		//left
		glVertex3f(0.05, 0.0, 0.0);
		glVertex3f(0.05, 0.0, -1.0);
		glVertex3f(0.05, -0.2, -1.0);
		glVertex3f(0.05, -0.2, 0.0);
		//bottom
		glVertex3f(0.05, -0.2, 0.0);
		glVertex3f(0.05, -0.2, -1.0);
		glVertex3f(0.1, -0.2, -1.0);
		glVertex3f(0.1, -0.2, 0.0);
		//right
		glVertex3f(0.1, -0.2, 0.0);
		glVertex3f(0.1, -0.2, -1.0);
		//glVertex3f(0.1, 0.0, -1.0);
		glVertex3f(0.1, 0.0, 0.0);
		glVertex3f(0.1, 0.0, -1.0);
		//inside
		glVertex3f(0.1, 0.0, -1.0);
		glVertex3f(0.05, 0.0, -1.0);
		glVertex3f(0.05, -0.2, 0.0);
		glVertex3f(0.1, -0.2, 0.0);
		glEnd();
	}
	void presser2() {
		glBegin(GL_LINE_LOOP);
		//left
		glVertex3f(0.1, -0.15, 0.0);
		glVertex3f(0.1, -0.15, -1.0);
		glVertex3f(0.1, -0.2, -1.0);
		glVertex3f(0.1, -0.2, 0.0);

		//bottom
		glVertex3f(0.1, -0.2, 0.0);
		glVertex3f(0.1, -0.2, -1.0);
		glVertex3f(0.25, -0.2, -1.0);
		glVertex3f(0.25, -0.2, 0.0);

		//left
		glVertex3f(0.25, -0.2, 0.0);
		glVertex3f(0.25, -0.2, -1.0);
		glVertex3f(0.25, -0.15, -1.0);
		glVertex3f(0.25, -0.15, 0.0);

		//front
		glVertex3f(0.25, -0.15, 0.0);
		glVertex3f(0.25, -0.2, 0.0);
		glVertex3f(0.1, -0.2, 0.0);
		glVertex3f(0.1, -0.15, 0.0);
		//top
		glVertex3f(0.1, -0.15, 0.0);
		glVertex3f(0.25, -0.15, 0.0);
		glVertex3f(0.25, -0.15, -1.0);
		glVertex3f(0.1, -0.15, -1.0);
		//behind
		glVertex3f(0.1, -0.15, -1.0);
		glVertex3f(0.25, -0.15, -1.0);
		glVertex3f(0.25, -0.2, -1.0);
		glVertex3f(0.1, -0.2, -1.0);
		glEnd();

	}
	void presser3() {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		//top
		glVertex3f(0.05, 0.0, 0.0);
		glVertex3f(0.05, 0.0, -1.0);
		glVertex3f(0.1, 0.0, -1.0);
		glVertex3f(0.1, 0.0, 0.0);
		//front 
		glVertex3f(0.1, 0.0, 0.0);
		glVertex3f(0.1, -0.2, 0.0);
		glVertex3f(0.05, -0.2, 0.0);
		glVertex3f(0.05, 0.0, 0.0);
		//left
		glVertex3f(0.05, 0.0, 0.0);
		glVertex3f(0.05, 0.0, -1.0);
		glVertex3f(0.05, -0.2, -1.0);
		glVertex3f(0.05, -0.2, 0.0);
		//bottom
		glVertex3f(0.05, -0.2, 0.0);
		glVertex3f(0.05, -0.2, -1.0);
		glVertex3f(0.1, -0.2, -1.0);
		glVertex3f(0.1, -0.2, 0.0);
		//right
		glVertex3f(0.1, -0.2, 0.0);
		glVertex3f(0.1, -0.2, -1.0);
		//glVertex3f(0.1, 0.0, -1.0);
		glVertex3f(0.1, 0.0, 0.0);
		glVertex3f(0.1, 0.0, -1.0);
		//inside
		glVertex3f(0.1, 0.0, -1.0);
		glVertex3f(0.05, 0.0, -1.0);
		glVertex3f(0.05, -0.2, 0.0);
		glVertex3f(0.1, -0.2, 0.0);
		glEnd();
	}


	void display()
	{
		//glRotatef(0.01, 1.0, 1.0, 1.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glPushMatrix();
		glRotatef(15, 1.0, 0.0, 1.0);
		//glRotatef(-90, 0.0, 1.0, 0.0);
		cuboid();
		handle();
		presser();
		glPushMatrix();
		glTranslatef(0.15, 0.1, 0.0);
		presser3();
		glPopMatrix();
		//glLoadIdentity();
		/*
		glPushMatrix();
		glRotatef(-110, 0.0, 0.5, 1.8);
		presser();
		glPopMatrix();
		*/
		presser2();
		glPopMatrix();
		glPopMatrix();
	}
}
