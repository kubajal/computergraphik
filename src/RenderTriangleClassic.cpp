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
	, m_dNearDistance(5.0)
	, m_dFarDistance(15.0f)
	, m_fRotX(0.0f)
	, m_fRotY(0.0f)
	, m_fTransZ(-10.0f)
{
	middles_map = map<pair<int, int>, int>();
	srand(time(0));
	texIndices = vector<double>();
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
	glTranslatef(0, 0, m_fTransZ);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void
RenderTriangleClassic::RenderTriangleClassic::middles() {

	printf("number of triangles: %d, number of verticies: %d, edges: %d\n", triangles.size(), vertices.size(), 3 * triangles.size() / 2);
	for (int i = 0; i < triangles.size() / 3; i++) {
		middle(i);
	}
	triangles = triangles_tmp;
	normalise();
	triangles_tmp.clear();
	middles_map.clear();
}

void RenderTriangleClassic::normalise() {

	for (int i = 0; i < vertices.size() / 3; i++) {

		double dx = vertices[i * 3];
		double dy = vertices[i * 3 + 1];
		double dz = vertices[i * 3 + 2];

		double distance = sqrt(dx * dx + dy * dy + dz * dz);

		dx = dx / distance;
		dy = dy / distance;
		dz = dz / distance;

		vertices[i * 3] = dx;
		vertices[i * 3 + 1] = dy;
		vertices[i * 3 + 2] = dz;
	}
}

void
RenderTriangleClassic::middle(int i) {

	for (int j = 0; j < 3; j++) {

		double new_vert[3][3]; // 0 - x, y - 1, z - 2
		int i1 = i * 3 + j, i2 = i * 3 + (j + 1) % 3;

		if (middles_map.find(pair<GLuint, GLuint>(triangles[i1], triangles[i2])) == middles_map.end() || middles_map.size() == 0) {

			middles_map[pair<GLuint, GLuint>(triangles[i1], triangles[i2])] = (vertices.size() / 3);
			middles_map[pair<GLuint, GLuint>(triangles[i2], triangles[i1])] = (vertices.size() / 3);

			new_vert[j][0] = (vertices[3 * triangles[i1]] + vertices[3 * triangles[i2]]) / 2;
			new_vert[j][1] = (vertices[3 * triangles[i1] + 1] + vertices[3 * triangles[i2] + 1]) / 2;
			new_vert[j][2] = (vertices[3 * triangles[i1] + 2] + vertices[3 * triangles[i2] + 2]) / 2;

			vertices.push_back(new_vert[j][0]);
			vertices.push_back(new_vert[j][1]);
			vertices.push_back(new_vert[j][2]);
			col.push_back(((double)rand() / RAND_MAX) * 0.25 + 0.25);
			col.push_back(((double)rand() / RAND_MAX) * 0.25 + 0.75);
			col.push_back(((double)rand() / RAND_MAX) * 0.5);
		}
	}

	unsigned middle0 = middles_map[pair<GLuint, GLuint>(triangles[i * 3], triangles[i * 3 + 1])],
		middle1 = middles_map[pair<GLuint, GLuint>(triangles[i * 3], triangles[i * 3 + 2])],
		middle2 = middles_map[pair<GLuint, GLuint>(triangles[i * 3 + 1], triangles[i * 3 + 2])];

	triangles_tmp.push_back(middle0);
	triangles_tmp.push_back(middle1);
	triangles_tmp.push_back(triangles[i * 3]);

	triangles_tmp.push_back(middle2);
	triangles_tmp.push_back(middle0);
	triangles_tmp.push_back(triangles[i * 3 + 1]);

	triangles_tmp.push_back(middle1);
	triangles_tmp.push_back(middle2);
	triangles_tmp.push_back(triangles[i * 3 + 2]);

	triangles_tmp.push_back(middle2);
	triangles_tmp.push_back(middle1);
	triangles_tmp.push_back(middle0);

}
void
RenderTriangleClassic::calculate_uv_mapping(int i) {

	double length = sqrt(vertices[i] * vertices[i] + vertices[i + 1] * vertices[i + 1] + vertices[i + 2] * vertices[i + 2]);
	double x = 0.5 + atan2(vertices[i + 2] / length, vertices[i] / length) / 6.28;
	double y = 0.5 + asin(vertices[i + 1] / length) / 2;

	texIndices.push_back(x);
	texIndices.push_back(y);
}

void
RenderTriangleClassic::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);

	vector <unsigned char> v1;
	unsigned int width, height;

	loadPNG("data/erde.png", v1, width, height);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glActiveTexture(GL_TEXTURE0);
	GLuint texture_ID;
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, v1.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texIndices.clear();

	for (int i = 0; i < vertices.size(); i += 3) {

		calculate_uv_mapping(i);
	}

	glEnable(GL_TEXTURE_2D);
	glTexCoordPointer(2, GL_DOUBLE, 0, texIndices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
	//	glEnableClientState(GL_COLOR_ARRAY);
	//	glColorPointer(3, GL_DOUBLE, 0, col.data());

	glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, triangles.data());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}
