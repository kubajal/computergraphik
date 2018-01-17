#ifndef RENDERTRIANGLECLASSIC_H
#define RENDERTRIANGLECLASSIC_H


#include "RenderIf.h"
#include <GL/glew.h>

#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <cmath>
#include <ctime>

#include "Common.h"

using namespace std;

class RenderTriangleClassic : public RenderIf
{
public:
	// constructor
	RenderTriangleClassic();
	// destructor
	virtual ~RenderTriangleClassic() {};

	// set the size of the window
	virtual void setWindowSize(const int iWidth, const int iHeight);

	// initialize the GL state/scene
	virtual void initGL();
	// free all GL resources
	virtual void uninitGL();

	// update camera parameters and object orientation
	virtual void renderCamera();
	// render the scene
	virtual void render();

	virtual void middles();
	virtual void normalise();

	// pass a key to the renderer
	virtual void keyPressed(unsigned char ucKey) {};
	virtual void middle(int i);
	double solve_quadratic(double a, double b, double c);

	// rotate scene
	virtual void rotX(const float fAngle) { m_fRotX += fAngle; }
	virtual void rotY(const float fAngle) { m_fRotY += fAngle; }
	virtual void transZ(const float fZ) { m_fTransZ *= fZ; }


protected:
	int   m_iWidth;
	int   m_iHeight;
	map<pair<int, int>, int> middles_map;

	// vertical viewing angle
	float m_fHeightAngle;

	// near and far distance of scene
	double m_dNearDistance;
	double m_dFarDistance;

	// rotation angle around x-axis
	float  m_fRotX;
	// rotation angle around y-axis
	float  m_fRotY;
	// translation in z-direction
	float  m_fTransZ;

	double X1 = 0.52573111212;
	double Z1 = 0.85065080835;

	vector <unsigned> triangles_tmp;

	/*vector <double> vertices = {
		- X1, 0.0, Z1,
		X1, 0.0, Z1,
		-X1, 0.0, -Z1,
		X1, 0.0, -Z1,
		0, Z1, X1,
		0, Z1, -X1,
		0, -Z1, X1,
		0, -Z1, -X1,
		Z1, X1, 0,
		-Z1, X1, 0,
		Z1, -X1, 0,
		-Z1, -X1, 0,
		0, 0, 0,
		0, 1, 0,
		0, 0, 1
	};

	vector <unsigned> triangles = {
		0, 4, 1,
		0, 9, 4,
		9, 5, 4,
		4, 5, 8,
		4, 8, 1,
		8, 10, 1,
		8, 3, 10,
		5, 3, 8,
		5, 2, 3,
		2, 7, 3,
		7, 10, 3,
		7, 6, 10,
		7, 11, 6,
		11, 0, 6,
		0, 1, 6,
		6, 1, 10,
		9, 0, 11,
		9, 11, 2,
		9, 2, 5,
		7, 2, 11
	};

	vector <double> col = {
		0.1, 0.5, 0.3,
		0.3, 0.5, 0.5,
		0.5, 0.5, 0.7,
		0.7, 0.7, 0.9,
		0.9, 0.5, 0.1,

		0.3, 0.1, 0.9,
		0.3, 0.3, 0.7,
		0.3, 0.5, 0.5,
		0.3, 0.8, 0.3,
		0.3, 0.9, 0.1,

		0.3, 0.1, 0.9,
		0.3, 0.3, 0.7,
		0.3, 0.5, 0.5,
		0.3, 0.8, 0.3,
		0.3, 0.9, 0.1,

		0.1, 0.5, 0.3,
		0.3, 0.5, 0.5,
		0.5, 0.5, 0.7,
		0.7, 0.7, 0.9,
		0.9, 0.5, 0.1

	};*/
	
	
	vector <GLdouble> vertices = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0
//		1.0, 0.0, 0.0
	};

	vector <GLuint> triangles = {
		1, 0, 2
	};

	vector <GLdouble> col = {
		0.0, 0.0, 1.0
	};
	
};



#endif