//
//  Particle.cpp
//  ParticleDemo
//
//  Created by Robert Bantin on 4/2/13.
//
//

/*---------------------------------------------------------------------------
**
*/

#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"

#define NTRAIL (10UL)
#define NSCALE (0.1f) // 1/NTRAIL

/*---------------------------------------------------------------------------
**
*/

using namespace ci;
using namespace ci::app;
using namespace std;

/*---------------------------------------------------------------------------
**
*/

Particle::Particle( Vec2f location, float frameRate ) :
    mPastLocations(),
    mLoc(location),
    mVel(Vec2f(0.0f, 0.0f)),
    mLocImpact(Vec2f(0.0f, 0.0f)),
    mMass(1500000000000000.0f), //1500000000000000.0f
    mField(1.0f),
    mRadius(5.0f),
    mDT(1.0f/frameRate),
	mImpact(0.0f),
    mKe(0.0f)
{
    mPastLocations.resize(NTRAIL);
    mPastLocations.assign (NTRAIL, mLoc);
}

/*---------------------------------------------------------------------------
 **
 */
Particle::Particle( Vec2f location, float frameRate, float mass ) :
    mPastLocations(),
    mLoc(location),
    mVel(Vec2f(0.0f, 0.0f)),
    mLocImpact(Vec2f(0.0f, 0.0f)),
    mMass(mass),
    mField(1.0f),
    mRadius(5.0f),
    mDT(1.0f/frameRate),
    mImpact(0.0f),
    mKe(0.0f)
{
    mPastLocations.resize(NTRAIL);
    mPastLocations.assign (NTRAIL, mLoc);
}

/*---------------------------------------------------------------------------
 **
 */
Particle::Particle( Vec2f location, float frameRate, float mass, float radius ) :
    mPastLocations(),
    mLoc(location),
    mVel(Vec2f(0.0f, 0.0f)),
    mLocImpact(Vec2f(0.0f, 0.0f)),
    mMass(mass),
    mField(1.0f),
    mRadius(radius),
    mDT(1.0f/frameRate),
    mImpact(0.0f),
    mKe(0.0f)
{
    mPastLocations.resize(NTRAIL);
    mPastLocations.assign (NTRAIL, mLoc);
}

/*---------------------------------------------------------------------------
**
*/

void Particle::update()
{
    mLoc += mVel * mDT;
}

/*---------------------------------------------------------------------------
**
*/

void Particle::draw()
{

    updateTrail();

	float scale = cinder::math<float>::pow(1.2f, -(float)NTRAIL);	
    
    for (unsigned i = 0; i < NTRAIL; ++i)
    {
        gl::color (0.9f, 0.6f + scale * 0.4f, 0.55f + scale * 0.45f, 0.5f);
        gl::drawSolidCircle( mPastLocations[i], mRadius * scale );

		scale *= 1.2f;
    }

	if (mImpact > 0.0f)
	{
		float offset  = 4.0f - mImpact;
		
		gl::color (1.0f, 1.0f, 0.9f, 0.125f * mImpact);
		gl::drawSolidCircle ( mLocImpact, (mKe * offset) + 1.0f);

		mImpact -= 1.0f;
        
        //mMass -= mKe / (mVel.x * mVel.x + mVel.y * mVel.y);
        //mRadius *= 0.9f;
	}
}

/*---------------------------------------------------------------------------
**
*/

void Particle::updateTrail()
{
    for (unsigned i = 0; i < NTRAIL - 1UL; ++i)
    {
        mPastLocations[i] = mPastLocations[i + 1u];
    }
    
    mPastLocations[NTRAIL - 1UL] = mLoc;
}
