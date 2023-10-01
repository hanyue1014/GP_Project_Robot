#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Canvas.h"
#include "Transform.h"
#include "Util.h"

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

namespace Robot
{
	class Hand
	{
	private:
		float newX, newY, newZ; // marks the newX and newY of the top side's sphere
		float rootAngle = 90;
		float jointAngle = 180;
		const float upperArmLength = 5, botArmLength = 5;
	public:
		// creates the hand at the position
		Hand(Point3D p) : newX(p.x), newY(p.y), newZ(p.z) {}
		
		void draw()
		{
			cv.pushMatrix().translate(newX, newY, newZ);

			cv
				.pushMatrix()
				.rotate(rootAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1) // root bola
				.cuboid({ -1, 1, -1, {255, 255, 0} }, { 1, -1, -(0 + upperArmLength) }) // 0 cuz wanna factor in the bola
				.replotPrevBlocky3D(GL_LINE_LOOP, {0, 0, 0})
				;

			cv
				.pushMatrix()
				.translate(0, 0, -6)
				.rotate(jointAngle, -1, 0, 0) // elbow bola
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, 0 + botArmLength }) // 0 cuz wanna factor in the bola as well
				.replotPrevBlocky3D(GL_LINE_LOOP, {0, 0, 0})
				.popMatrix()
				;
			cv.popMatrix(); // from root

			cv.popMatrix();
		}
		
		// only works if x = 0 after normalized
		void solveIK(Point3D target)
		{
			// normalize target
			float 
				x = target.x - newX, 
				y = target.y - newY, 
				z = target.z - newZ;

			Point3D v1 = { x, y, z }; // the root -> target vector
			Point3D v2 = { 0, 0, -1 };  // the ori vector which z shouldn't actually matter

			float magV1 = magnitude(v1);
			float magV2 = magnitude(v2);

			
			float dotProd = dot(v1, v2);
			float cosAlpha = dotProd / (magV1 * magV2);
			float alpha;
			if (magV1 * magV2 == 0)
				alpha = 0;
			else
				alpha = acosf(cosAlpha);

			rootAngle = alpha / PI * 180;
			if (y > 0)
				rootAngle *= -1;
			// if longer than leg no need solve d, direct rotate 
			if (magV1 > upperArmLength + botArmLength)
			{
				return;
			}

			float cosPhi = magV1 / 2;
			float phi = acosf(cosPhi / upperArmLength);
			// if magV1 is 0 then mean same as origin point, then should not rotate anything
			if (magV1 == 0)
			{
				phi = 0;
			}
			phi = phi / PI * 180;
			rootAngle += phi;

			float theta = 180 - 90 - phi;
			// if magV1 is 0 then mean same as origin point, then should not rotate anything
			if (magV1 == 0)
			{
				theta = 0;
			}
			jointAngle = theta * 2;
		}
	};

	class Leg
	{
	private: 
		float newX, newY, newZ; // marks the newX and newY of the top side's sphere
		float rootAngle = 90;
		float jointAngle = 180;
		const float upperLegLength = 5, botLegLength = 5;

	public:
		// creates the hand at position
		Leg(Point3D p) : newX(p.x), newY(p.y), newZ(p.z) {}

		void draw()
		{
			cv.pushMatrix().translate(newX, newY, newZ);

			cv
				.pushMatrix()
				// root joint
				.rotate(rootAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, -1, {255, 255, 0} }, { 1, -1, -5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, {0, 0, 0})
				;

			cv
				.pushMatrix()
				// knee joint
				.translate(0, 0, -6)
				.rotate(jointAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, 5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, {0, 0, 0})
				.popMatrix()
				;
			cv.popMatrix();

			cv.popMatrix();
		}

		// x of the points should be 0 after normalized with the origin
		void solveIK(Point3D target)
		{
			// normalize target
			float 
				x = target.x - newX, 
				y = target.y - newY, 
				z = target.z - newZ;

			Point3D v1 = { x, y, z }; // the root -> target vector
			Point3D v2 = { 0, 0, -1 };  // the ori vector which z shouldn't actually matter

			float magV1 = magnitude(v1);
			float magV2 = magnitude(v2);

			
			float dotProd = dot(v1, v2);
			float cosAlpha = dotProd / (magV1 * magV2);
			float alpha = acosf(cosAlpha);

			rootAngle = alpha / PI * 180;
			// if longer than leg no need solve d, direct rotate 
			if (magV1 > upperLegLength + botLegLength)
			{
				return;
			}

			float cosPhi = magV1 / 2;
			float phi = acosf(cosPhi / upperLegLength);
			phi = phi / PI * 180;
			rootAngle += phi;

			float theta = 180 - 90 - phi;
			jointAngle = theta * 2;
		}
	};

	// leftLeg would go upwards first
	Point3D leftLegRestTarget = { -2, -13, 0 };
	// TODO: tween these when walking
	Point3D leftLegWalkTargets[] = {
		{ -2, -9, 0 }, // go upwards 
		{ -2, -9, 1 }, // make a parabola when walking towards front
		{ -2, -9, 2 },
		{ -2, -10, 3 },
		{ -2, -11, 4 },
		{ -2, -12, 4 },
		{ -2, -13, 4 }, // touched ground
		{ -2, -13, 3 }, // start glide backwards
		{ -2, -13, 2 },
		{ -2, -13, 1 },
		{ -2, -13, 0 },
		{ -2, -13, -1 },
		{ -2, -13, -2 },
		{ -2, -13, -3 },
		{ -2, -13, -4 }, // reached max backwards
		{ -2, -12, -4 }, // start moving upwards in a parabola
		{ -2, -11, -4 },
		{ -2, -10, -3 },
		{ -2, -9, -2 },
		{ -2, -9, -1 } // next will be upwards center d (wrap around back to top)
	};
	Point3D leftLegCurrentTarget = leftLegRestTarget;

	// rightLeg would go backwards first
	Point3D rightLegRestTarget = { 2, -13, 0 };
	Point3D rightLegWalkTargets[] = {
		{ 2, -13, -1 }, // go backwards 
		{ -2, -13, -2 }, // continue glide backwards
		{ -2, -13, -3 },
		{ -2, -13, -4 }, // reached max backwards
		{ -2, -12, -4 }, // start moving upwards in a parabola
		{ -2, -11, -4 },
		{ -2, -10, -3 },
		{ -2, -9, -2 },
		{ -2, -9, -1 },
		{ -2, -9, 0 }, // up center
		{ -2, -9, 1 }, // make a parabola when walking towards front
		{ -2, -9, 2 },
		{ -2, -10, 3 },
		{ -2, -11, 4 },
		{ -2, -12, 4 },
		{ -2, -13, 4 }, // touched ground
		{ -2, -13, 3 }, // start glide backwards
		{ -2, -13, 2 },
		{ -2, -13, 1 },
		{ -2, -13, 0 }, // next should wrap around
	};
	Point3D rightLegCurrentTarget = rightLegRestTarget;
	// due to array arrangement only need one to keep track both
	int bothLegWalkingIndex = 0;

	void main()
	{
		// upper body
		cv
			.cube({ 0, 1.5, 0, {255, 255, 0} }, 0.5)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -1.5, 3, 1.5, {255, 0, 0} },
				{ 1.5, 3, -1.5 },
				{ -0.5, 2, 0.5 },
				{ 0.5, 2, -0.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -2.5, 4, 2, {255, 0, 0} },
				{ 2.5, 4, -2 },
				{ -1.5, 3, 1.5 },
				{ 1.5, 3, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid({ -2.5, 6, 2, {255, 0, 0} }, { 2.5, 4, -2 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -2, 7, 1.5, {255, 0, 0} },
				{ 2, 7, -1.5 },
				{ -2.5, 6, 2 },
				{ 2.5, 6, -2 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -1, 8, 1, {255, 0, 0} },
				{ 1, 8, -1 },
				{ -2, 7, 1.5 },
				{ 2, 7, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			;

		// arm joint to body
		// right
		cv
			.pushMatrix()
			.translate(2, 7, 0)
			.rotate(20, 0, 0, 1)
			.cuboid({ -0.5, 0.5, 0.5, {0, 255, 255} }, { 3.5, -0.5, -0.5 })
			.popMatrix()
			;

		// left, lazy rethink coords d
		cv
			.pushMatrix()
			.rotate(180, 0, 1, 0)
			.translate(2, 7, 0)
			.rotate(20, 0, 0, 1)
			.cuboid({ -0.5, 0.5, 0.5, {0, 255, 255} }, { 3.5, -0.5, -0.5 })
			.popMatrix()
			;

		Hand rightHand({5, 8, 0});
		rightHand.draw();

		Hand leftHand({ -5, 8, 0 });
		leftHand.draw();

		// body
		cv
			.cuboid({ -1, -1, 1, {0, 0, 255} }, { 1, -3, -1 })
			.replotPrevBlocky3D(GL_LINE_LOOP, {0, 0, 0})
			.cuboid(
				{ -1.5, 0, 1.5, {0, 255, 255} },
				{ 1.5, 0, -1.5 },
				{ -1, -1, 1 },
				{ 1, -1, -1 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -0.5, 1, 0.5, {0, 255, 255} },
				{ 0.5, 1, -0.5 },
				{ -1.5, 0, 1.5 },
				{ 1.5, 0, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			;

		Leg leftLeg({2, -2, 0});
		leftLeg.solveIK(leftLegCurrentTarget);
		leftLeg.draw();
		Leg rightLeg({-2, -2, 0});
		rightLeg.solveIK(rightLegCurrentTarget);
		rightLeg.draw();
	}

	void handleKeyDownEvent(WPARAM key)
	{
		switch (key)
		{
		case 'W': // start walking, should move into main
			int walkIndex = bothLegWalkingIndex++;
			if (bothLegWalkingIndex >= sizeof(leftLegWalkTargets) / sizeof(leftLegWalkTargets[0]))
				bothLegWalkingIndex = 0;
			leftLegCurrentTarget = leftLegWalkTargets[walkIndex];
			rightLegCurrentTarget = rightLegWalkTargets[walkIndex];
			break;
		}
	}
}

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

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

