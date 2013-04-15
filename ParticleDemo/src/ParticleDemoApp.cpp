
/*---------------------------------------------------------------------------
**
*/

#include "ParticleController.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "Resources.h"

/*---------------------------------------------------------------------------
**
*/

using namespace ci;
using namespace ci::app;

#define FRAMERATE (60.0f)

/*---------------------------------------------------------------------------
**
*/

class ParticleDemoApp : public AppBasic {
public:
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
	
	gl::Texture mImage;
	
	ParticleController mParticleController;
};

/*---------------------------------------------------------------------------
**
*/

void ParticleDemoApp::prepareSettings( Settings *settings )
{
	settings -> setWindowSize( 1024, 768 );
	settings -> setFrameRate( FRAMERATE );
}

/*---------------------------------------------------------------------------
**
*/

void ParticleDemoApp::setup()
{
    mParticleController.setFrameRate( FRAMERATE );

    mImage = gl::Texture( loadImage( loadResource( RES_BACKGROUND_IMG ) ) );

    gl::enableAlphaBlending( false	);
    
	mParticleController.addParticles( 150 );
}

/*---------------------------------------------------------------------------
**
*/

void ParticleDemoApp::update()
{
	mParticleController.update();
}

/*---------------------------------------------------------------------------
**
*/

void ParticleDemoApp::draw()
{
	gl::clear( Color( 0, 0, 0 ), true );
	
	mImage.enableAndBind();
    gl::color( 1.0f, 1.0f, 1.0f);
	gl::draw( mImage, getWindowBounds() );
	
	glDisable( GL_TEXTURE_2D );
	glColor3f( 1, 1, 1 );
	mParticleController.draw();
}

/*---------------------------------------------------------------------------
**
*/

CINDER_APP_BASIC( ParticleDemoApp, RendererGl )
