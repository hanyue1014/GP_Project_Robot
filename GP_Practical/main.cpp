#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Canvas.h"
#include "Transform.h"
#include "Util.h"
#include "Robot.hpp"
#include "WeaponBackground.hpp"

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Pacific Robot"
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool initPixelFormat(HDC hdc);
void display();

int WINAPI WinMain(
	_In_ HINSTANCE hInst, 
	_In_opt_ HINSTANCE, 
	_In_ LPSTR, 
	_In_ int nCmdShow
)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hInstance		= GetModuleHandle(NULL);
	wc.lpfnWndProc		= WindowProcedure;
	wc.lpszClassName	= WINDOW_TITLE;
    wc.style			= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindowA(	WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, WIN_WIDTH, WIN_HEIGHT,
								NULL, NULL, wc.hInstance, NULL );
	
	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);
	
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// enable use of alpha value
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	// only need to init once, no need call everytime in loop
	Robot::initWeaponRestAndOriStates();
	Robot::initWeaponActiveStates();

	while(true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();
		
		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// no need create canvas everytime
Canvas cv(20, 20, 20);

ProjectionMode projectionMode = ORTHO;
bool debug = false;
Transform debugTrans;
Transform debugTrans2;
Transform cameraTrans;
bool inCameraTranslateMode = false;
bool activateGreenScreen = false;

enum SelectedLightColor { LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE };

// environment ambient light (some what cold color)
float amb[] = { 0.1, 0.1, 0.3 };
float intensity[] = { 1.0,1.0,1.0 };   // diffuse light
float pos[] = { 0.0,0.0,1.0 };
bool isLightOn = false;
bool editingLight = false;
SelectedLightColor editingColor = LIGHT_RED;

void lighting()
{
	if (isLightOn)
	{
		glEnable(GL_LIGHTING);  //enable the lighting for the whole scene
		glEnable(GL_COLOR_MATERIAL);
		//Light 0: Ambient + DIFFUSE light
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, intensity);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glEnable(GL_LIGHT0);

		if (editingLight)
		{
			glColor3f(intensity[0], intensity[1], intensity[2]);
			glPointSize(20);
			glBegin(GL_POINTS);
			glVertex3f(pos[0], pos[1], pos[2]);
			glEnd();
		}
	}
	else
	{
		glDisable(GL_LIGHTING); //disable the lighting for the whole scene
	}
}

void display()
{
	// green color let editor easier to key out
	cv.clear({0, 255, 0}).loadIdentity();

	cv.setProjection(projectionMode);

	if (!activateGreenScreen)
	{
		if (projectionMode == ORTHO)
			WeaponProjectionBackground::orthoBackground();
		else
			WeaponProjectionBackground::perspectiveBackground();
	}

	cv
		.pushMatrix()
		.translate(cameraTrans.transX, cameraTrans.transY, cameraTrans.transZ)
		.rotate(debugTrans2.rotAngle, 1, 0, 0)
		.rotate(debugTrans.rotAngle, debugTrans.rotX, debugTrans.rotY, debugTrans.rotZ);

	lighting();

	Robot::main();
	if (debug)
		cv.showDebugGrid();
	cv.popMatrix();
}

// event handling
void handleKeyDownEvent(WPARAM key)
{
	if (key == VK_OEM_2) // the '/' key
	{
		activateGreenScreen = !activateGreenScreen;
		return;
	}

	// press 0 to toggle move camera mode
	if (key == '0')
	{
		inCameraTranslateMode = !inCameraTranslateMode;
		return;
	}

	if (inCameraTranslateMode)
	{
		switch (key)
		{
		case 'W':
			cameraTrans.transY -= 0.5;
			return;
		case 'S':
			cameraTrans.transY += 0.5;
			return;
		case 'A':
			cameraTrans.transX += 0.5;
			return;
		case 'D':
			cameraTrans.transX -= 0.5;
			return;
		case 'E': // move camera far
			cameraTrans.transZ -= 0.5;
			return;
		case 'Q': // move camera near
			cameraTrans.transZ += 0.5;
			return;
		}
	}

	switch (key)
	{
	case VK_F1:
		debug = !debug;
		return;
	case 'P':
		projectionMode = projectionMode == ORTHO ? PERSPECTIVE : ORTHO;
		return; // alrd handle, no need proceed
	// for debug purpose only, rotate whole canvas so x/y/z is facing us
	case 'X':
		debugTrans.rotAngle = 90;
		debugTrans.rotY = debugTrans.rotY == 1 ? -1 : 1;
		debugTrans.rotX = debugTrans.rotZ = 0;
		debugTrans2 = Transform();
		return;
	case 'Y':
		debugTrans.rotAngle = 90;
		debugTrans.rotX = debugTrans.rotX == 1 ? -1 : 1;
		debugTrans.rotZ = debugTrans.rotY = 0;
		debugTrans2 = Transform();
		return;
	case 'Z':
		debugTrans.rotAngle = debugTrans.rotAngle == 180 ? 0 : 180;
		debugTrans.rotY = 1;
		debugTrans.rotX = debugTrans.rotZ = 0;
		debugTrans2 = Transform();
		return;
	case VK_LEFT:
		debugTrans.rotAngle += 1;
		debugTrans.rotX = debugTrans.rotZ = 0;
		debugTrans.rotY = 1;
		return;
	case VK_RIGHT:
		debugTrans.rotAngle -= 1;
		debugTrans.rotX = debugTrans.rotZ = 0;
		debugTrans.rotY = 1;
		return;
	case VK_UP:
		debugTrans2.rotAngle += 1;
		return;
	case VK_DOWN:
		debugTrans2.rotAngle -= 1;
		return;
	}

	switch (key)
	{
	case '9':
		isLightOn = !isLightOn;
		break;
	case '8':
		editingLight = !editingLight;
		break;
	}

	if (editingLight)
	{
		switch (key)
		{
		case 'W':
			pos[1] += 0.01;
			break;
		case 'S':
			pos[1] -= 0.01;
			break;
		case 'A':
			pos[0] -= 0.01;
			break;
		case 'D':
			pos[0] += 0.01;
			break;
		case 'E': // move light in
			pos[2] -= 0.01;
			break;
		case 'Q': // move light out
			pos[2] += 0.01;
			break;
		case 'R':
			editingColor = LIGHT_RED;
			break;
		case 'G':
			editingColor = LIGHT_GREEN;
			break;
		case 'B':
			editingColor = LIGHT_BLUE;
			break;
		}

		int editingColorIdx;
		if (editingColor == LIGHT_RED)
			editingColorIdx = 0;
		else if (editingColor == LIGHT_GREEN)
			editingColorIdx = 1;
		else
			editingColorIdx = 2;

		if (key == VK_OEM_PLUS)
		{
			if (intensity[editingColorIdx] < 1)
				intensity[editingColorIdx] += 0.1;
		}

		if (key == VK_OEM_MINUS)
		{
			if (intensity[editingColorIdx] > 0)
				intensity[editingColorIdx] -= 0.1;
		}

		return;
	}

	Robot::handleKeyDownEvent(key);
}

void handleKeyUpEvent(WPARAM key)
{
	Robot::handleKeyUpEvent(key);
}

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		handleKeyDownEvent(wParam);
		break;

	case WM_KEYUP:
		handleKeyUpEvent(wParam);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

