#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/CinderMath.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"



Particle::Particle()
{
}

Particle::Particle( Vec2f loc ) :
    mLoc(loc),
    mDir(Vec2f(0.0f,0.0f)),
    mVel(0.0f),
    mColor(Color(1.0f,1.0f,1.0f)),
    mScale(1.0f),
    mRadius(1.0f)
{
}

void Particle::update( const float& dT )
{
    mScale = (cos( 0.628318530717959f * dT ) + 0.5f ) * 10.0f;
    
    //float xyOffset = sin( cos( sin( mLoc.y * 0.3183f ) + cos( mLoc.x * 0.3183f ) ) ) + 1.0f;
    //mRadius = (sin( 0.628318530717959f * dT ) + 0.5f ) * xyOffset * xyOffset * 1.8f;
    //mRadius = ( sin( 0.000628318530717959f * dT * mLoc.y * mLoc.x ) + 1.0f ) * 2.0f;
    //mRadius = cos( 0.628318530717959f * dT * mLoc.y * 0.1f ) + sin( 0.942477796076938 * dT * mLoc.x * 0.1f ) + 2.0f;
}

void Particle::update( const Channel32f& channel )
{
    //float gray = channel.getValue( mLoc );
    //mColor = Color( gray, gray, gray );
    
    mRadius = channel.getValue( mLoc ) * mScale;
}

void Particle::draw()
{
    gl::color( mColor );
	gl::drawSolidCircle( mLoc, mRadius );
}


