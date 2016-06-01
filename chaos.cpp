
/* Copyright (c) Mark J. Kilgard, 1996. */

/* This program is freely distributable without licensing fees
and is provided without guarantee or warrantee expressed or
implied. This program is -not- in the public domain. */

/* This program is a response to a question posed by Gil Colgate
<gcolgate@sirius.com> about how lengthy a program is required using
OpenGL compared to using  Direct3D immediate mode to "draw a
triangle at screen coordinates 0,0, to 200,200 to 20,200, and I
want it to be blue at the top vertex, red at the left vertex, and
green at the right vertex".  I'm not sure how long the Direct3D
program is; Gil has used Direct3D and his guess is "about 3000
lines of code". */

/* X compile line: cc -o simple simple.c -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm */

#include <cmath>
#include "complex.h"
#include "glut.h"
using namespace std;

double width = 100, height = 100;
const double tol = 0.1;

void reshape(int w, int h)
{
	/* Because Gil specified "screen coordinates" (presumably with an
	upper-left origin), this short bit of code sets up the coordinate
	system to correspond to actual window coodrinates.  This code
	wouldn't be required if you chose a (more typical in 3D) abstract
	coordinate system. */

	width = w;
	height = h;
	glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
	glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
	glLoadIdentity();             /* Reset project matrix. */
	glOrtho(-1, 1, -1, 1, -1, 1);   /* Map abstract coords directly to window coords. */
	//  glScalef(1, -1, 1);           /* Invert Y axis so increasing Y goes down. */
	//  glTranslatef(0, -h, 0);       /* Shift origin up to upper-left corner. */
}


void getcolor(double x, double y, double &r, double &g, double &b)
{
	complexNum	z(x, y),
				   znew;

	for (int i = 0; i < 100; i++)
	{
		znew = z - (z*z*z*z - 1) / (4 * z*z*z);
		z = znew;

	}
	if (z.getReal() > 0.9)
	{
		r = 0.0; g = 0.0; b = 1.0;
	}
	else if (z.getImaginary() > 0.9)
	{
		r = 0.0; g = 1.0; b = 0.0;
	}
	else if (z.getReal() < 0.9)
	{
		r = 1.0; g = 0.0; b = 0.0;
	}
	else if (z.getImaginary() < 0.9)
	{
		r = 1.0; g = 1.0; b = 0.0;
	}

	return;

}


void display(void)
{
	int i, j;
	double r, g, b, x, y;

	glClear(GL_COLOR_BUFFER_BIT);

	for (i = 0; i<2 * width; i++)
	{
		glBegin(GL_POINTS);
		for (j = 0; j<2 * height; j++)
		{
			x = i / width - 1.0;
			y = j / height - 1.0;

			//			getcolor(x, y, r, g, b);
			getcolor(x, y, r, g, b);
			glColor3f(r, g, b);
			glVertex2f(x, y);
		}
		glEnd();
		glFlush();
	}
	//  glFlush();  /* Single buffered, so needs a flush. */
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Chaotic Newton's Method");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();



	return 0;             /* ANSI C requires main to return int. */
}