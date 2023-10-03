#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Canvas.h"
#include "Transform.h"
#include "Util.h"
#include "hanyue1014.hpp"
#include "soonchee.hpp"

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"
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
bool debug = true;
Transform debugTrans;
void display()
{
	// grayish color easier to see stuff
	cv.clear({50, 50, 50});

	cv.setProjection(projectionMode);
	cv.rotate(debugTrans.rotAngle, debugTrans.rotX, debugTrans.rotY, debugTrans.rotZ);

	// cv.rotate(0.01, 1, 1, 1);
	//cv.cuboid({ 0, 0, 0, {255, 0, 0} }, { 5, 5, 5, {255, 255, 0}})
	Robot::main();
	//Robot::Head();
	if (debug)
		cv.showDebugGrid();
}

// event handling
void handleKeyDownEvent(WPARAM key)
{
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
		return;
	case 'Y':
		debugTrans.rotAngle = 90;
		debugTrans.rotX = debugTrans.rotX == 1 ? -1 : 1;
		debugTrans.rotZ = debugTrans.rotY = 0;
		return;
	case 'Z':
		debugTrans.rotAngle = debugTrans.rotAngle == 180 ? 0 : 180;
		debugTrans.rotY = 1;
		debugTrans.rotX = debugTrans.rotZ = 0;
		return;
	case VK_LEFT:
		debugTrans.rotAngle += 1;
		debugTrans.rotY = 1;
		return;
	case VK_RIGHT:
		debugTrans.rotAngle -= 1;
		debugTrans.rotY = 1;
		return;
	case VK_UP:
		debugTrans.rotAngle -= 1;
		debugTrans.rotX = 1;
		return;
	case VK_DOWN:
		debugTrans.rotAngle += 1;
		debugTrans.rotX = 1;
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

