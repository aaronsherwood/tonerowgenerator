#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "toneRowGenerator.h"
#include "cinder/Text.h"
#include "cinder/gl/TextureFont.h"
#include "midiToneRow.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//    click the mouse in the app window to generate a new tone row
//    press spacebar to send midi out the IAC midi driver

class toneRowApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event);
	void update();
	void draw();
    gl::Texture mBackground;
    Generator toneRow;
    Font				mFont;
	gl::TextureFontRef	mTextureFont;
    PolyRhythm polyrythm; //yes, i realize i don't know how to spell rhythm
};

void toneRowApp::setup()
{
    setWindowSize(1280, 720);
    setWindowPos(90, 90);
    mBackground= gl::Texture( loadImage( loadResource( "toneRow.jpg" ) ) );
    mFont = Font( "ITC Franklin Gothic Std Book Extra Compressed", 110 );
    mTextureFont = gl::TextureFont::create( mFont );
    polyrythm.setup();
}

void toneRowApp::mouseDown( MouseEvent event )
{
    toneRow.generate();
    polyrythm.getNotes(toneRow.mToneRow);
}

void toneRowApp::keyDown( KeyEvent event ) {
    if( event.getChar() == ' ' ){
        if (toneRow.hasRow){ 
            polyrythm.mSinglePlay=0;
            polyrythm.mPlay = !polyrythm.mPlay;
        }
    }
    if( event.getChar() == '1' ){
        if (toneRow.hasRow){
            polyrythm.mPlayOnce = !polyrythm.mPlayOnce;
            polyrythm.mPlay = true;
        }
    }
}

void toneRowApp::update()
{
    polyrythm.playNotes();
}

void toneRowApp::draw()
{
	gl::enableAlphaBlending();
	gl::clear( Color( .18f, .77f, .86f ) );
    gl::color( Color::white() );
    gl::draw( mBackground, getWindowBounds() );
    Rectf boundsRect( 7, mTextureFont->getAscent() + 350, getWindowWidth() -5, getWindowHeight() - 100 );
	gl::color( Color(.18f,.18f,.18f) );
    if (toneRow.hasRow)
        mTextureFont->drawStringWrapped( toneRow.getNotes(), boundsRect );
}


CINDER_APP_NATIVE( toneRowApp, RendererGl )
