#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include "Tex.h"


//#pragma comment(lib, "GL\\glut32.lib")

const double M_PI = 3.141592653589793238463;

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);	

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(3.0, 10.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	DrawLegoWall(3.0, 1.0, 2.0);

	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------	
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	double aspect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, aspect, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawBox(double x, double y, double z)
{
	glBegin(GL_QUADS);

		//napred
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-x/2,-y/2,z/2);
		glVertex3f(x/2,-y/2,z/2);
		glVertex3f(x/2,y/2,z/2);
		glVertex3f(-x/2,y/2,z/2);

		//desno
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(x/2,-y/2,z/2);
		glVertex3f(x/2,-y/2,-z/2);
		glVertex3f(x/2,y/2,-z/2);
		glVertex3f(x/2,y/2,z/2);

		//nazad
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(x/2,-y/2,-z/2);
		glVertex3f(-x/2,-y/2,-z/2);
		glVertex3f(-x/2,y/2,-z/2);
		glVertex3f(x/2,y/2,-z/2);

		//levo
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-x/2,-y/2,-z/2);
		glVertex3f(-x/2,-y/2,z/2);
		glVertex3f(-x/2,y/2,z/2);
		glVertex3f(-x/2,y/2,-z/2);

		//gore
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-x/2,y/2,z/2);
		glVertex3f(x/2,y/2,z/2);
		glVertex3f(x/2,y/2,-z/2);
		glVertex3f(-x/2,y/2,-z/2);

		//dole
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-x/2,-y/2,z/2);
		glVertex3f(x/2,-y/2,z/2);
		glVertex3f(x/2,-y/2,-z/2);
		glVertex3f(-x/2,-y/2,-z/2);

	glEnd();
}

void CGLRenderer::DrawCylinder(double r, double h, int no)
{
	//telo
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0; angle <= 360; angle += 360 / no)
	{
		glNormal3f(cos(toRad(angle)), 0.0, sin(toRad(angle)));
		//gore
		glVertex3f(r * cos(toRad(angle)), -h / 2, r * sin(toRad(angle)));
		//dole
		glVertex3f(r * cos(toRad(angle)), h / 2, r * sin(toRad(angle)));
	}
	glEnd();

	//gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, h/2, 0.0);
	for (float angle = 0; angle <= 360; angle += 360 / no)
	{
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(r * cos(toRad(angle)), h / 2, r * sin(toRad(angle)));
	}
	glEnd();
}

void CGLRenderer::DrawLegoBox(double x, double y, double z)
{
	glColor3f(0.3, 0.5, 1.0);
	DrawBox(x, y, z);
	
	glColor3f(0.5, 0.5, 0.5);
	// za pomeranje po x i z osi
	float divX = x / 3;
	float divZ = z / 2;	
	// za pomeranje cilidnra po y osi
	float moveY = y / 2;

	//0
	glPushMatrix();
		glTranslatef(-divX, moveY, -divZ / 2);
		DrawCylinder(z * 0.15, y * 0.3, 100);
	glPopMatrix();

	//1
	glPushMatrix();
		glTranslatef(0.0, moveY, -divZ / 2);
		DrawCylinder(z * 0.15, y * 0.3, 100);
	glPopMatrix();

	//2
	glPushMatrix();
		glTranslatef(divX, moveY, -divZ / 2);
		DrawCylinder(z * 0.15, y * 0.3, 100);
	glPopMatrix();

	//3
	glPushMatrix();
		glTranslatef(-divX, moveY, divZ / 2);
		DrawCylinder(z * 0.15, y * 0.3, 100);
	glPopMatrix();

	//4
	glPushMatrix();
		glTranslatef(0.0, moveY, divZ / 2);
		DrawCylinder(z * 0.15, y * 0.3, 100);
	glPopMatrix();

	//5
	glPushMatrix();
		glTranslatef(divX, moveY, divZ / 2);
		DrawCylinder(z * 0.15, y * 0.3, 100);
	glPopMatrix();
}

void CGLRenderer::DrawLegoWall(float x, float y, float z)
{
	DrawLegoBox(x,y,z);

	float divX = x / 3;
	float divZ = z / 2;

	glPushMatrix();
		float moveX = -((2 * divX) + (divX / 2));
		float moveY = 0.0;
		float moveZ = -divZ / 2;		
		glTranslatef(moveX, moveY, moveZ);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		DrawLegoBox(x, y, z);
	glPopMatrix();

	glPushMatrix();
		moveX = -(2 * divX);
		moveY = y;
		moveZ = 0.0;
		glTranslatef(moveX, moveY, moveZ);
		DrawLegoBox(x, y, z);
	glPopMatrix();
}

float CGLRenderer::toRad(float angle)
{
	return (angle * M_PI) / 180;
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

double CGLRenderer::R(double a, double alpha, int n, double h)
{
	alpha *= 0.01745329251994329577;
	double PI2 = 6.2831853071795864769;
	double alpha1 = (PI2 / (2 * n) - alpha);
	double alpha2;
	double R1, R;

	R = a / (2 * sin(alpha1 / 2.0));
	double delta = R / 4.;
	R1 = R;
	do {
		alpha1 = 2 * asin(a / (2 * R1));
		alpha2 = 2 * asin(a / (2 * (R1 - h)));
		if ((alpha1 + alpha2) > ((PI2 / n) - (2 * alpha)))
			R1 += delta;
		else
			R1 -= delta;
		delta /= 2.0;
	} while (delta > 1e-6);
	return R;
}

void CGLRenderer::NormCrossProd(double x1, double y1, double z1, double x2, double y2, double z2, double& x, double& y, double& z)
{
	x = y1 * z2 - z1 * y2;
	y = z1 * x2 - x1 * z2;
	z = x1 * y2 - y1 * x2;
	double d = sqrt(x*x + y*y + z*z);
	if (d != 0.0)
	{
		x /= d; y /= d; z /= d;
	}
}
