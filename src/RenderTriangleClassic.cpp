#include "RenderTriangleClassic.h"

#ifdef _WIN32
#include "GL/wglew.h"
#endif
#include <GL/gl.h>
#include <GL/freeglut.h>

using namespace std;
// constructor
RenderTriangleClassic::RenderTriangleClassic()
	: m_iWidth(800)
	, m_iHeight(600)
	, m_fHeightAngle(0.4f)
	, m_dNearDistance(1.0)
	, m_dFarDistance(50.0f)
	, m_fRotX(0.0f)
	, m_fRotY(0.0f)
	, m_fTransZ(-10.0f)
{
	srand(time(0));

// \/\/\/\/\/\/\/	SETTING UP THE SCENE starts here \/\/\/\/\/\/\/\/\/\/\/

	globalCamera.position = Vec3f(0, 0, -10);
	globalCamera.direction = Vec3f(0.0, 0.0, 1.0);

	float mirror = 0.18;
	float ambient = 0.1;
	float d = 1, dd = 0.3;

	objects.push_back(new Sphere(Vector(-10000.0, 0.0, 0.0), 10000, Material(0.1, 0.3, 0.1, mirror, Color(1, 1, 1))));
	objects.push_back(new Sphere(Vector(10002.0, 0.0, 0.0), 10000, Material(0.1, 0.3, 0.1, mirror, Color(1, 1, 1))));
	objects.push_back(new Sphere(Vector(0, -10003.0, 0.0), 10000, Material(0.1, 0.3, 0.1, mirror, Color(1, 1, 1))));
	objects.push_back(new Sphere(Vector(0, 10005.0, 0.0), 10000, Material(0.1, 0.3, 0.1, mirror, Color(1, 1, 1))));
/*
	Triangle *triangle = new Triangle(Point(0, 0, 0), Point(0, 5, 0), Point(5, 0, 0), Material(ambient, 0.5, 0.5, 0.5, Color(1, 1, 1.0)));
	objects.push_back(triangle);*/

	for(int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				objects.push_back(new Sphere(Vector(i * d + dd, j * d + dd, k * d + dd), 0.2, Material(0.1, 0.3, 0.1, mirror, Color(i, k, j))));
			}
		}
	}

	Light l1;
	l1.position = Vector(1.2, 1.2, 15);
	l1.color = Color(1, 1, 1);
	lights.push_back(l1);
	Light l2;
	l2.position = Vector(0.2, 0.2, 10);
	l2.color = Color(1, 1, 1);
	lights.push_back(l2);
	objects.push_back(new lightSource(l1.position));
	objects.push_back(new lightSource(l2.position));
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^ SETTING UP THE SCENE ends here ^^^^^^^^^^^^^^^^^^^^^^^^^^^//
}

Vec3f::Vec3f() {
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 0.0;
}

Vec3f::Vec3f(float x0, float x1, float x2) {
	v[0] = x0;
	v[1] = x1;
	v[2] = x2;
}
Vec3f
Vec3f::normalise() {
	float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= length;
	v[1] /= length;
	v[2] /= length;
	return Vec3f(v[0], v[1], v[2]);
}

float
Vec3f::dot(Vec3f x0){
	return v[0] * x0.v[0] + v[1] * x0.v[1] + v[2] * x0.v[2];
}

Vec3f
Vec3f::cross(Vec3f x0) {
	return Vec3f(v[1] * x0.v[2] - v[2] * x0.v[1], -(v[0] * x0.v[2] - v[2] * x0.v[0]), v[0] * x0.v[1] - v[1] * x0.v[0]);
}

Vec3f 
Vec3f::normal(Vec3f x0) {
	Vec3f x1 = this->cross(x0);
	x1.normalise();
	return x1;
}

float Vec3f::length()
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void
RenderTriangleClassic::setWindowSize(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	glViewport(0, 0, m_iWidth, m_iHeight);      // tells OpenGL the new size of the render area
}


void
RenderTriangleClassic::initGL()
{
	//-----------------------------------------------------------------
	// init GL
	//-----------------------------------------------------------------
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);


	// set background color to black
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	// set depth buffer to far plane
	glClearDepth(1.0f);
	// enable depth test with the z-buffer
	glEnable(GL_DEPTH_TEST);

	// use flat shading (GL_SMOOTH for Gouraud shading)
	glShadeModel(GL_FLAT);

	// fill the front side of the polygone and use wireframe for back side
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	// do not use culling
	glDisable(GL_CULL_FACE);

	// enable anti-aliasing
	glEnable(GL_MULTISAMPLE_ARB);


}


void
RenderTriangleClassic::uninitGL()
{
	//-----------------------------------------------------------------
	// uninit GL
	//-----------------------------------------------------------------
	glPopClientAttrib();
	glPopAttrib();
}

void
RenderTriangleClassic::renderCamera()
{
	// render camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double left, right, bottom, top;
	top = m_dNearDistance * std::tan(m_fHeightAngle / 2.0);
	bottom = -top;
	right = top * (double)m_iWidth / (double)m_iHeight;
	left = -right;
	glFrustum((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)m_dNearDistance, (GLdouble)m_dFarDistance);

	// camera placed at (0 0 10) looking in -z direction
	glTranslatef(0, 0, 0);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Sphere::Sphere(Point o, GLfloat r, Material m) {
	type = SPHERE;
	origin = o;
	radius = r;
	material = m;
}

GLfloat Sphere::intersect(Ray ray)
{
	float t0, t1; // solutions for t if the ray intersects 
				  // geometric solution
	float t;		// final solution
	Vec3f L = origin - ray.origin;
	float tca = L.dot(ray.direction);
	if (tca < 0)
		return PLUSINFINITY;
	float d2 = L.dot(L) - tca * tca;
	if (d2 > radius * radius)
		return PLUSINFINITY;
	float thc = sqrt(radius*radius - d2);
	t0 = tca - thc;
	t1 = tca + thc;
				  // analytic solution
	L = ray.origin - origin;
	float x0 = ray.direction.dot(ray.direction);
	float x1 = 2 * ray.direction.dot(L);
	float x2 = L.dot(L) - radius*radius;

	float discr = x1 * x1 - 4 * x0 * x2;
	if (discr < 0)
		return PLUSINFINITY;
	else if (discr == 0) {
		t0 = t1 = -0.5 * x1 / x0;
	}
	else {
		float q = (x1 > 0) ?
			-0.5 * (x1 + sqrt(discr)) :
			-0.5 * (x1 - sqrt(discr));
		t0 = q / x0;
		t1 = x2 / q;
	}
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0)
			return PLUSINFINITY; // both t0 and t1 are negative 
	}

	t = t0;

	Vector p = ray.origin + ray.direction * t;
	return t;
}

Vector Sphere::normal(Point p)
{
	return (p - origin).normalise();
}

float sgn(float x) {
	return (0.0 < x - x < 0);
}

Color Triangle::shadow(Point point, int objectID, int lightID, vector <Primitive*> & const objects, vector <Light> & const lights, Vector direction) {

	Vector lightPosition = lights[lightID].position;
	Vector z = lightPosition - point;
	float distance = z.length();
	z = z.normalise();
	float min_t = PLUSINFINITY;
	float lDistance = distanceFromPlane(lights[lightID].position);
	float pDistance = distanceFromPlane(point - direction);

	//printf("ldistance = %f, pdistance = %f\n", lDistance, pDistance);
	if (sgn(pDistance) != sgn(lDistance)) {
		return Color(0,0,0);
	}
	Ray ray = Ray(point, z);
	for (int i = 0; i < objects.size(); i++) {

		float t = objects[i]->intersect(ray);
		if (t > 0 && min_t > t && objectID != i) {
			min_t = t;
		}
	}

	if (min_t < 0 || min_t > distance && min_t != PLUSINFINITY) {
		Vector normal = objects[objectID]->normal(point);

		float dot = normal.dot(z);
		if (dot < 0) {
			//printf("dot = %f\n", dot);
			return Color(0, 0, 0);
		}
		Color diffuse = lights[lightID].color * objects[objectID]->material.color * dot * objects[objectID]->material.diffuse;
		return diffuse;
	}
	return Color(0.0, 0.0, 0.0);
}

Color Sphere::shadow(Point point, int objectID, int lightID, vector <Primitive*> & const objects, vector <Light> & const lights, Vector direction) {

	Vector lightPosition = lights[lightID].position;
	Vector z = lightPosition - point;
	float distance = z.length();
	z = z.normalise();
	float min_t = PLUSINFINITY;
	Ray ray = Ray(point, z);
	for (int i = 0; i < objects.size(); i++) {

		float t = objects[i]->intersect(ray);
		if (t > 0 && min_t > t && objectID != i) {
			min_t = t;
		}
	}

	if (min_t < 0 || min_t > distance && min_t != PLUSINFINITY) {
		Vector normal = objects[objectID]->normal(point);
		float dot = normal.dot(z);
		Color diffuse = lights[lightID].color * objects[objectID]->material.color * dot * objects[objectID]->material.diffuse;
		return diffuse;
	}
	return Color(0.0, 0.0, 0.0);
}

Color RenderTriangleClassic::castRay(Ray ray, int depth, int oID) {

	float min_t = PLUSINFINITY;
	ray.direction = ray.direction.normalise();
	int objectID = -1;
	for (int i = 0; i < objects.size(); i++) {

		float t = objects[i]->intersect(ray);
		//printf("ts: %f\n", t);
		if (t > 0 && min_t > t && oID != i) {
			min_t = t;
			objectID = i;
		}
	}
	if (min_t == PLUSINFINITY) {

		return Color(0,0,0);
	}

	Color c;

	if (objects[objectID]->type == LIGHTSOURCE) {
		c = objects[objectID]->material.color;
		return c + castRay(Ray(Point(ray.origin + ray.direction * LIGHTSOURCERADIUS), ray.direction), depth, objectID);
	}

	Point p = ray.origin + ray.direction * min_t;

	for (int i = 0; i < lights.size(); i++) {
		c = c + objects[objectID]->material.color * lights[i].color * objects[objectID]->material.diffuse *objects[objectID]->shadow(p, objectID, i, objects, lights, ray.direction);
		c = c + lights[i].color *objects[objectID]->phong(p, lights[i], ray.direction) * objects[objectID]->material.specular;
	}

	c = c + objects[objectID]->material.color * objects[objectID]->material.ambient;

	if (depth <= 0)
		return c;

	return c + reflect(p, ray.direction, objectID, depth - 1) * objects[objectID]->material.mirror;
	
}

Color RenderTriangleClassic::reflect(Point point, Vector direction, int objectID, int depth)
{
	if (depth < 0)
		return Color(0.0, 0.0, 0.0);
	Vector normal = objects[objectID]->normal(point);
	Vector dir = Vector(-direction.v[0], -direction.v[1], -direction.v[2]);
	//printf("dir = %f %f %f\n", direction.v[0], direction.v[1], direction.v[2]);

	if (normal.dot(dir) < 0) {
		//printf("%d ", objectID);
		normal = normal * (-1.0);
	}
	Vector r = normal * 2 * (normal.dot(dir)) - dir;
	//printf("ray = %f %f %f\n", r.v[0], r.v[1], r.v[2]);
	r = r.normalise();
	//if (objectID == 27)
	//	printf("odbicie= %f %f %f\n", r.v[0], r.v[1], r.v[2]);
	return castRay(Ray(point, r), depth - 1, objectID);
}

Ray::Ray(Vector o, Vector d) {
	origin = o;
	direction = d;
}


void
RenderTriangleClassic::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);

	float mirror = 0.9;

	float scale = tan(3.14 * (90.0 * 0.5) / 180.0);
	float imageAspectRatio = width / (float)height;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			float x = (2 * (i + 0.5) / (float)width - 1) * imageAspectRatio * scale;
			float y = (1 - 2 * (j + 0.5) / (float)height) * scale; 
					
			Vector n = Vector(x + this->globalCamera.direction.v[0], y + this->globalCamera.direction.v[1], this->globalCamera.direction.v[2]);
			Point cameraPosition = this->globalCamera.position;
			Ray ray = Ray(cameraPosition, n);

			Color c = castRay(ray, RECURSIONDEPTH, -1);
/*			if (objects[32]->intersect(ray) != PLUSINFINITY)
				printf("color: %s\n", c.toString());*/

			framebuffer[3 * (i * width + j)] = c.v[0];
			framebuffer[3 * (i * width + j) + 1] = c.v[1];
			framebuffer[3 * (i * width + j) + 2] = c.v[2];

		}
	}

	/*for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			printf("%d ", fbuf[i][j]);
		}
		printf("\n");
	}*/
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, framebuffer);
}

Material::Material(float a, float d, float s, float m, Color c)
{
	color = c;
	specular = s;
	diffuse = d;
	ambient = a;
	mirror = m;
}

Material::Material()
{
	specular = 1.0;
	diffuse = 1.0;
	ambient = 1.0;
	color = Color(0.5, 0.5, 0.5);
}

Mat3x3::Mat3x3()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			(rows[i]).v[j] = 0.0;
}

Mat3x3::Mat3x3(Vec3f x0, Vec3f x1, Vec3f x2)
{
	rows[0] = x0;
	rows[1] = x1;
	rows[2] = x2;
}

void Mat3x3::print()
{
	for (int i = 0; i < 3; i++) {
		printf("|%f, %f, %f|\n", rows[i].v[0], rows[i].v[1], rows[i].v[2]);
	}
}

float Mat3x3::det()
{
	return rows[0].v[0] * rows[1].v[1] * rows[2].v[2] +
		+rows[1].v[0] * rows[2].v[1] * rows[0].v[2] +
		+rows[2].v[0] * rows[0].v[1] * rows[1].v[2] -
		 (
			rows[0].v[2] * rows[1].v[1] * rows[2].v[0] +
			+rows[1].v[2] * rows[2].v[1] * rows[0].v[0] +
			+rows[2].v[2] * rows[0].v[1] * rows[1].v[0]
		 );
}

Triangle::Triangle(Point _a, Point _b, Point _c, Material m){

	type = TRIANGLE;
	material = m;
	x0 = _a;
	x1 = _b;
	x2 = _c;
	this->normalVector = (x1 - x0).cross(x2 - x0).normalise();
}

GLfloat Triangle::intersect(Ray ray)
{
	Vec3f v = x2 - x0;
	Vec3f u = x1 - x0;
	Vec3f w = ray.origin - x0;

	Vec3f d = ray.direction;
	Vec3f p = d.cross(v);
	Vec3f q = w.cross(u);

	Vec3f y = Vec3f(q.dot(v), p.dot(w), q.dot(d));

	y = y * (1 / (p.dot(u)));

	float t = y.v[0];
	float r = y.v[1];
	float s = y.v[2];

	//printf("%f %f %f\n", t, r, s);

	//printf("t = %f\n", t);

	if (!(0 <= r + s && r + s <= 1 && 0 <= r && r <= 1 && 0 <= s && s <= 1 && t >= 0)) {
		return PLUSINFINITY;
	}
	//printf("t=%f\n", t);
	return t;
}
float Triangle::distanceFromPlane(Point p)
{
	float d = -(normalVector.dot(x0));

	return normalVector.dot(p) + d;
}

float power(float x, int n) {
	float xx = 1.0;
	for (int i = 0; i < n; i++)
		xx *= x;
	return xx;
}

Color Sphere::phong(Point p, Light l, Vector direction)
{
	Point g = p - direction;
	Vector normal = this->normal(p);
	Vector toLight = (l.position - p).normalise();
	Vector toViewer = (g - p).normalise();
	Vector reflectedRay = normal * 2 * (normal.dot(toLight)) - toLight;

	return l.color * power(reflectedRay.dot(toViewer), PHONG_POWER);
};

Color Triangle::phong(Point p, Light l, Vector direction)
{
	Point g = p - direction;
	float cDistance = distanceFromPlane(g);
	float lDistance = distanceFromPlane(l.position);
	//printf("ldistance = %f, pdistance = %f\n", lDistance, pDistance);
	if (sgn(cDistance) != sgn(lDistance)) {
		return Color(0, 0, 0);
	}
	Vector normal = this->normal(p);
	if (normal.dot(direction * (-1)) < 0) {
		normal = normal * (-1.0);
	}
	Vector toLight = (l.position - p).normalise();
	Vector toViewer = (g - p).normalise();
	Vector reflectedRay = normal * 2 * (normal.dot(toLight)) - toLight;

	return l.color * power(reflectedRay.dot(toViewer), PHONG_POWER);
};

GLfloat lightSource::intersect(Ray r)
{
	return sphere->intersect(r);
}

lightSource::lightSource(Point position)
{
	sphere = new Sphere(position, LIGHTSOURCERADIUS, Material(0, 0, 0, 0, Color(0, 0.843, 0)));
}

Vector lightSource::normal(Point p)
{
	return Vector();
}

Vector lightSource::normal()
{
	return Vector();
}

Color lightSource::shadow(Point point, int objectID, int lightID, vector<Primitive*>& const objects, vector<Light>& lights, Vector direction)
{
	return Color();
}

Color lightSource::phong(Point p, Light l, Vector direction)
{
	return Color();
}
