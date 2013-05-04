#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "ParticleController.h"

#define HORIZONTAL_REZ (800)
#define VERTICAL_REZ (600)
#define FRAMERATE (30.0f)

using namespace ci;
using namespace ci::app;

class NewpaperPrintApp : public AppBasic {
public:
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
    void keyDown( KeyEvent event );
	
	Channel32f mChannel;
	
	ParticleController mParticleController;
};

void NewpaperPrintApp::prepareSettings( Settings *settings )
{
	settings -> setWindowSize( HORIZONTAL_REZ, VERTICAL_REZ );
	settings -> setFrameRate( FRAMERATE );
}

void NewpaperPrintApp::setup()
{
    Url url( "http://www.flight404.com/_images/paris.jpg" );
    mChannel = Channel32f( loadImage( loadUrl( url ) ) );
    
    mParticleController.setFramerate( FRAMERATE );
    mParticleController.setChannel(mChannel);
}

void NewpaperPrintApp::update()
{
	mParticleController.update();
}

void NewpaperPrintApp::draw()
{
	gl::clear( Color( 0, 0, 0 ), true );
	
	glDisable( GL_TEXTURE_2D );
	glColor3f( 1, 1, 1 );
	mParticleController.draw();
}

void NewpaperPrintApp::keyDown( KeyEvent event )
{
    if ( event.getChar() == '1' )
    {
        mParticleController.setMode(1);
    }
    else if ( event.getChar() == '2' )
    {
        mParticleController.setMode(2);
    }
    else if ( event.getChar() == '3' )
    {
        mParticleController.setMode(3);
    }
    else if ( event.getChar() == '4' )
    {
        mParticleController.setMode(4);
    }
}


CINDER_APP_BASIC( NewpaperPrintApp, RendererGl )
