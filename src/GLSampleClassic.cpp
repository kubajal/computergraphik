
#include <GL/glew.h>
#ifdef _WIN32
#include "GL/wglew.h"
#endif
#include <GL/gl.h>
#include <GL/freeglut.h>


#include "RenderTriangleClassic.h"


#include <iostream>


// define prototypes
void displayCallback(void);
void reshapeCallback( int width, int height );
void keyboardCallback( unsigned char key, int x, int y);


// create the render class (needed later for multiple examples)
void setupRenderer();

// global variables :(
RenderTriangleClassic* g_pcRenderer = NULL;
int       g_iRenderer  = 0;

int main(int argc, char* argv[])
{
  const unsigned int uiWidth = 1024;
  const unsigned int uiHeight = 1024;

  glutInit( &argc, argv );                                      // initialize library

  glutInitWindowSize( uiWidth, uiHeight );                      // set desired window size
  // asks for double buffering and z-buffer as well as anti-aliasing support
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  glutCreateWindow( "OpenGL Example Program" );                 // opens the window and return the window id

  glewInit();                                                   // initialize glew, OpenGL context must already be available!!
  
  // setup the renderer
  setupRenderer( );
  g_pcRenderer->setWindowSize(uiWidth, uiHeight);
  // initialize the scene and OpenGL state
  g_pcRenderer->initGL();

  // set callback functions
  glutDisplayFunc( &displayCallback );                          // sets the callback for redrawing
  glutReshapeFunc( &reshapeCallback );                          // set the callback in case of window resizing
  glutKeyboardFunc( &keyboardCallback );                        // set the callback for key presses

  std::cout << "use a, s, d, w, z, x, i, j, k, l to move" << std::endl;

  glutMainLoop();

  // free renderer
  if( g_pcRenderer) 
  {
    g_pcRenderer->uninitGL();
    delete g_pcRenderer;
  }

  return 0;
}



void displayCallback()
{
  // render the scene
  g_pcRenderer->render();

  // flush the command stream
  glFlush();

  // causes double buffered windows to show rendered content
  glutSwapBuffers();
}

void reshapeCallback( int width, int height )
{
  g_pcRenderer->setWindowSize( width, height );
  g_pcRenderer->renderCamera();
  glutPostRedisplay();                                          // creates an event for redrawing (calls displayCallback() )
}

float k = .5;

void keyboardCallback( unsigned char key, int x, int y)
{
  switch( key ) 
  {
  case 'a':
	g_pcRenderer->globalCamera.position = g_pcRenderer->globalCamera.position + Vec3f(k, 0.0, 0.0);
    break;

  case 'd':
	 g_pcRenderer->globalCamera.position =g_pcRenderer->globalCamera.position + Vec3f(-k, 0.0, 0.0);
    break;

  case 's':
	 g_pcRenderer->globalCamera.position =g_pcRenderer->globalCamera.position + Vec3f(0.0, -k, 0.0);
    break;

  case 'w':
	 g_pcRenderer->globalCamera.position =g_pcRenderer->globalCamera.position + Vec3f(0.0, k, 0.0);
	  break;

  case 'z':
	  g_pcRenderer->globalCamera.position = g_pcRenderer->globalCamera.position + Vec3f(0.0, 0.0, k);
	  break;

  case 'x':
	  g_pcRenderer->globalCamera.position = g_pcRenderer->globalCamera.position + Vec3f(0.0, 0.0, -k);
	  break;

  case 'i':
	  g_pcRenderer->globalCamera.direction = g_pcRenderer->globalCamera.direction + Vec3f(0.0, 0.5, 0.0);
	  break;

  case 'k':
	  g_pcRenderer->globalCamera.direction = g_pcRenderer->globalCamera.direction + Vec3f(0.0, -0.5, 0.0);
	  break;

  case 'j':
	  g_pcRenderer->globalCamera.direction = g_pcRenderer->globalCamera.direction + Vec3f(0.5, 0.0, 0.0);
	  break;

  case 'l':
	  g_pcRenderer->globalCamera.direction = g_pcRenderer->globalCamera.direction + Vec3f(-0.5, 0.0, 0.0);
	  break;

  case 'q':
	  glutLeaveMainLoop();
	  break;
  default:
    g_pcRenderer->keyPressed(key);
  }

  g_pcRenderer->renderCamera();
  glutPostRedisplay();                                          // creates an event for redrawing (calls displayCallback() )
}

void
setupRenderer()
{
  if( g_pcRenderer ) 
  {
    g_pcRenderer->uninitGL();
    delete g_pcRenderer;
    g_pcRenderer = NULL;
  }

  if( 0 == g_iRenderer ) 
  {
    g_pcRenderer = new RenderTriangleClassic;
  }
  else 
  {
    g_iRenderer = 0;
    setupRenderer();
  }
}