
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
  const unsigned int uiWidth = 800;
  const unsigned int uiHeight = 600;

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

  std::cout << "press q to quit" << std::endl;
  std::cout << "press k to turn left" << std::endl;
  std::cout << "press l to turn right" << std::endl;
  std::cout << "press a to turn forward" << std::endl;
  std::cout << "press y to turn backward" << std::endl;
  std::cout << "press + to move forward" << std::endl;
  std::cout << "press - to move backward" << std::endl;
  std::cout << "press n for next demo" << std::endl;

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

void keyboardCallback( unsigned char key, int x, int y)
{
  switch( key ) 
  {
  case 'k':
    g_pcRenderer->rotY( 2.0f );
    break;

  case 'l':
    g_pcRenderer->rotY( -2.0f );
    break;

  case 'a':
    g_pcRenderer->rotX(2.0f);
    break;

  case 'y':
	  g_pcRenderer->rotX(-2.0f);
	  break;

  case 'z':
	  g_pcRenderer->rotY(-2.0f);
	  break;

  case '+':
    g_pcRenderer->transZ(0.98f);
    break;

  case '-':
    g_pcRenderer->transZ(1.0f/0.98f);
    break;

  case 'n':
    g_iRenderer++;
    setupRenderer();
    g_pcRenderer->initGL();
    break;

  case 'p':
	  g_pcRenderer->middles();
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
    std::cout << std::endl << "Render a simple triangle" << std::endl;
    g_pcRenderer = new RenderTriangleClassic;
  }
  else 
  {
    g_iRenderer = 0;
    setupRenderer();
  }
}