#ifndef RENDERTRIANGLECLASSIC_H
#define RENDERTRIANGLECLASSIC_H

#define MINUSINFINITY -100000.0
#define PLUSINFINITY +100000.0

#include "RenderIf.h"
#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>

#define WIDTH 1024					// width of the camera
#define HEIGHT 1024					// height of the camera
#define PHONG_POWER 20				// power to which the coefficient should be rised computing Phong model of specular light
#define LIGHTSOURCERADIUS 0.25		// when drawing a light source how big should it be?
#define RECURSIONDEPTH 5			// how many times should a ray bounce?

using namespace std;

class Vec3f {									// a very basic implementation of 3 dimensional vectors
public:
	float v[3];

	Vec3f();									// sets to (0, 0, 0)
	Vec3f(float x0, float x1, float x2);		// sets to (x0, x1, x2)
	Vec3f normalise();
	float dot(Vec3f x0);						// dot product, this . x0 
	Vec3f cross(Vec3f x0);						// cross product, this x x0
	Vec3f normal(Vec3f x0);						// normal to vector this and x0
	float length();								// length
	
	string toString() { return "(" + to_string(v[0]) + ", " + to_string(v[1]) + "," + to_string(v[2]) + ")"; } // for debugging

	Vec3f operator -(const Vec3f &x0) const { return Vec3f(v[0] - x0.v[0], v[1] - x0.v[1], v[2] - x0.v[2]); }	// operator overloading for -
	Vec3f operator +(const Vec3f &x0) const { return Vec3f(v[0] + x0.v[0], v[1] + x0.v[1], v[2] + x0.v[2]); }	// operator overloading for +
	Vec3f operator *(const Vec3f &x0) const { return Vec3f(v[0] * x0.v[0], v[1] * x0.v[1], v[2] * x0.v[2]); }	// operator overloading for * _NEITHER DOT NOR CROSS PRODUCT_
	Vec3f operator *(const float &x0) const { return Vec3f(v[0] * x0, v[1] * x0, v[2] * x0); }					// operator overloading for * with a constant
	bool operator !=(const Vec3f &x0) const { return v[0] != x0.v[0] && v[1] != x0.v[1] && v[2] != x0.v[2]; }

};

class Mat3x3 {	// a very basic implemenation of 3x3 matrices
public:
	Vec3f rows[3];

	Mat3x3();								// set everything to 0s
	Mat3x3(Vec3f x0, Vec3f x1, Vec3f x2);	// set every row with the specified vector
	void print();							// for debugging
	Mat3x3 operator *(const float &x0) const { return Mat3x3(rows[0] * x0, rows[1] * x0, rows[2] * x0); }
	Mat3x3 operator *(const Mat3x3 x0) const { Mat3x3 out;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					out.rows[i].v[j] += rows[i].v[k] * rows[k].v[j];
				}
			}
		}
		return out;
	}
	float det();	// compute deteminant
};

typedef Vec3f Vector;		// Vector is now alias for Vec3f
typedef Vec3f Point;		// Point is now alias for Vec3f, but they have diffrent semantic!
typedef Vec3f Color;		// Color is now alias for Vec3f
typedef struct {
	Point position;
	Color color;
} Light;

typedef enum { SPHERE, TRIANGLE, LIGHTSOURCE } PRIMITIVE_TYPE;

typedef struct {
	Point position;
	Vector direction;
} Camera;

struct Material {
public:
	float ambient;
	float diffuse;
	float specular;
	float mirror;

	Color color;		// color of the surface
	Material(float a, float d, float s, float m, Color c);
	Material();
};

class Ray {
public:
	Point origin;
	Vector direction;

	Ray(Point o, Vector d);
};

class Primitive {
public:
	PRIMITIVE_TYPE type;
	Material material;
	virtual GLfloat intersect(Ray r) { return PLUSINFINITY; };
	virtual Vector normal(Point p) { return Vector(0.0, 0.0, 0.0); };
	virtual Vector normal() { return Vector(0.0, 0.0, 0.0); };
	virtual Color shadow(Point point, int objectID, int lightID, vector <Primitive*> & const objects, vector <Light> &lights, Vector direction) { return Color(0, 0, 0); } // Phong diffuse model
	virtual Color phong(Point p, Light l, Vector direction) = 0;	// Phong specular model
};

class Triangle : public Primitive {					// BASE CLASS for primitives that can be drawn
public:
	Point x0, x1, x2;
	Vector normalVector;

	Triangle(Point x0, Point x1, Point x2, Material m);
	GLfloat intersect(Ray r) override;					// if the ray intersects the object the function returns the normalised translation of the direction vector
	Vector normal() override { return normalVector; };
	Vector normal(Point p) { return normalVector; };
	float distanceFromPlane(Point p);					// returns the normalised distance from the plane of the point
	Color shadow(Point point, int objectID, int lightID, vector <Primitive*> & const objects, vector <Light> & const lights, Vector direction) override;
	Color phong(Point p, Light l, Vector direction) override;
};

class Sphere : public Primitive {
public:
	Sphere(Point origin, GLfloat r, Material m);
	Point origin;
	GLfloat radius;

	Color shadow(Point point, int objectID, int lightID, vector <Primitive*> & const objects, vector <Light> & const lights, Vector direction) override;
	GLfloat intersect(Ray r) override;
	Vector normal(Point p) override;
	Color phong(Point p, Light l, Vector direction);
};

class lightSource : public Primitive {			// sphere representing a light source in the scene for drawing, separte from a table with lights
												// TODO: merging lightSource with typedef Light
public:
	Sphere *sphere;
	GLfloat intersect(Ray r);
	lightSource(Point position);
	Vector normal(Point p);
	Vector normal();
	Color shadow(Point point, int objectID, int lightID, vector <Primitive*> & const objects, vector <Light> &lights, Vector direction);
	Color phong(Point p, Light l, Vector direction);
};

class RenderTriangleClassic : public RenderIf
{
public:
	// constructor
	RenderTriangleClassic();								// HERE set up the scene!
	// destructor
	virtual ~RenderTriangleClassic() {};

	// set the size of the window
	virtual void setWindowSize(const int iWidth, const int iHeight);

	// initialize the GL state/scene
	virtual void initGL();
	// free all GL resources
	virtual void uninitGL();

	void renderCamera();

	// update camera parameters and object orientation
	Color castRay(Ray ray, int depth, int objectID);
	Color reflect(Point point, Vector direction, int objectID, int depth);
	// render the scene
	virtual void render();

	// pass a key to the renderer
	virtual void keyPressed(unsigned char ucKey) {};

	// rotate scene
	virtual void rotX(const float fAngle) { m_fRotX += fAngle; }
	virtual void rotY(const float fAngle) { m_fRotY += fAngle; }
	virtual void transZ(const float fZ) { m_fTransZ *= fZ; }

	Camera globalCamera;
protected:
	int   m_iWidth;
	int   m_iHeight;
	vector <Primitive*> objects;						// the set of all objects (includeing spheres representing a light source)
	vector <Light> lights;								// set of all lights
	Color backgroundColor = Color(0.1, 0.1, 0.1);
	int width = WIDTH;
	int height = HEIGHT;
	float framebuffer[WIDTH * HEIGHT * 3];


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

};

#endif
