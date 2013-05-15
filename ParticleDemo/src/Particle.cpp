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
#include "cinder/Rand.h"

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
    mKe(0.0f),
    mMassLossDueToImpactFactor(0.0025f)
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
    
    if (mImpact > 0.0f)
    {
        damage();
        mImpact -= 1.0f;
    }
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
		gl::drawSolidCircle ( mLocImpact, (1e-16f * mKe * offset) + 1.0f, cinder::Rand::randInt(4,10));
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

/*---------------------------------------------------------------------------
 **
 */
void Particle::damage()
{
    float density = mMass / (/* 4/3 pi */ 4.18879020478638f * mRadius * mRadius * mRadius);
    
    mMass -= mMassLossDueToImpactFactor * mKe / (mVel.x * mVel.x + mVel.y * mVel.y);
    mRadius = cinder::math<float>::pow((3.0f * mMass)/(/* 4pi */ 12.56637061435917f * density), 1.0f / 3.0f);
}
