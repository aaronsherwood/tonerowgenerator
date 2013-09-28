#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class toneRowApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void toneRowApp::setup()
{
}

void toneRowApp::mouseDown( MouseEvent event )
{
}

void toneRowApp::update()
{
}

void toneRowApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( toneRowApp, RendererGl )
