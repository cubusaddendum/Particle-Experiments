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
** Static!
*/
bool
Particle::outofbounds(Particle& particle)
{
	float rightbound = 1.2f * static_cast<float>(getWindowWidth());
	float lowerbound = 1.2f * static_cast<float>(getWindowHeight());
	float leftbound  = - (rightbound - static_cast<float>(getWindowWidth()));
	float upperbound = - (lowerbound - static_cast<float>(getWindowHeight()));

	if (   ( particle.getLocation().x > rightbound ) 
		|| ( particle.getLocation().y > lowerbound) 
		|| ( particle.getLocation().x < leftbound)
		|| ( particle.getLocation().y < upperbound)
		)
	{
		return true;
	}

	return false;
}

/*---------------------------------------------------------------------------
 **
 */
Particle::Particle( Vec2f location, float frameRate, float mass, float radius ) 
    : mPastLocations()
    , mLoc(location)
    , mVel(Vec2f(0.0f, 0.0f))
    , mLocImpact(Vec2f(0.0f, 0.0f))
    , mMass(35809862195.676f * mass)
    , mField(1.0f)
    , mRadius(radius)
    , mDT(1.0f/frameRate)
    , mImpact(0.0f)
    , mKe(0.0f)
    , mMassLossDueToImpactFactor(0.0025f)
	, mBrightness(1.0f)
    , bIsColliding(false)
	, mDecay(Rand::randFloat( 0.96f, 0.99f ))
	, mScale(1.5f)
	, mAge(0)
	, mLifespan(Rand::randInt( 50, 125 ))
	, mAgePer(1.0f)
	, mIsDead(false)
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
        //damage();
        mImpact -= 1.0f;
    }
    else
    {
        bIsColliding = false;
    }
}

/*---------------------------------------------------------------------------
**
*/
void Particle::update( const Perlin &perlin, const Channel32f &channel)
{	

	float nX = mLoc.x * 0.005f;
	float nY = mLoc.y * 0.005f;
	float nZ = static_cast<float>(app::getElapsedSeconds()) * 0.1f;
	float noise = perlin.fBm( nX, nY, nZ );
	float angle = noise * 15.0f;
	Vec2f noiseVector( cos( angle ), sin( angle ) );

	mVel += noiseVector * 0.2f * ( 1.0f - mAgePer );
	mLoc += mVel;
	mVel *= mDecay;

	mRadius	= mScale * mAgePer;
	
	mAge++;
	
	if( mAge > mLifespan ) 
	{
			mIsDead = true;
	}
	
	mAgePer = 1.0f - ( (float)mAge/(float)mLifespan );

	mBrightness = channel.getValue(mLoc);
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
        gl::color (0.9f, 0.6f + scale * 0.4f, 0.55f + scale * 0.45f, mBrightness * 0.5f);
        gl::drawSolidCircle( mPastLocations[i], mRadius * scale );

		scale *= 1.2f;
    }

	if (bIsColliding)
	{
		float offset  = 4.0f - mImpact;
		
		gl::color (1.0f, 1.0f, 0.9f, 0.125f * mImpact);
		gl::drawSolidCircle ( mLocImpact, (5e-19f * mKe * offset) + 1.0f/*, cinder::Rand::randInt(7,10)*/);
	}
}

/*---------------------------------------------------------------------------
**
*/
void Particle::drawNoTrail()
{   
    gl::color (0.9f, 1.0f, 1.0f, 0.25f + mBrightness * 0.5f);
    gl::drawSolidCircle( mLoc, mRadius );
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
