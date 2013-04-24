#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "ParticleController.h"

using namespace ci;
using namespace cinder;
using std::list;

ParticleController::ParticleController() :
    mXRes(80),
    mYRes(60),
    mDT(60.0f)
{
    for( int y = 0; y < mYRes; y++ )
    {
        for( int x = 0; x < mXRes; x++ )
        {
            addParticle( x, y );
        }
    }
}

void ParticleController::update()
{
    float time = cinder::app::getElapsedSeconds();
    
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p )
    {
        p -> update( mChannel );
		p -> update(time);
	}
    
    
}

void ParticleController::draw()
{
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p )
    {
		p->draw();
	}
}

void ParticleController::addParticle( int xi, int yi )
{    
    float x = ( xi + 0.5f ) * 10.0f;
    float y = ( yi + 0.5f ) * 10.0f;
    mParticles.push_back( Particle( Vec2f( x, y ) ) );
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}

