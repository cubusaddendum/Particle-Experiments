#include "ImpactEngine.h"
#include "cinder/Vector.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"

/*---------------------------------------------------------------------------
**
*/
using namespace ci;
using namespace cinder;
using namespace cinder::app;
using namespace std;

#define IMPACT_SURVIVALRATE (100.0f)

/*---------------------------------------------------------------------------
** Constructor
*/
Impact::Impact(const Vec2f& loc, const Vec2f& vel, const float ke, const float framerate) 
	: mLoc(loc)
	, mVel(vel)
	, mKe(ke)
	, mBrightness(1.0f)
	, mIsDead(false)
	, mElapsedTime(static_cast<float>(getElapsedSeconds()))
	, mThreshold(10.0f)
	, mDT(1.0f/framerate)
{
	addParticles( 3u );
}

/*---------------------------------------------------------------------------
** explicit copy constructor
*/
Impact::Impact(const Impact& impact)
{
	*this = impact;
}

/*---------------------------------------------------------------------------
**
*/
Impact::~Impact(void)
{
	mParticles.empty();
}

/*---------------------------------------------------------------------------
**
*/
void Impact::update(const Perlin &perlin, const Channel32f &channel)
{
	mKe *= (1.0f - exp((- mDT * IMPACT_SURVIVALRATE)));

	if (mKe < mThreshold)
	{
		mIsDead = true;
	}

	for (list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p) 
	{
		p -> update(perlin, channel);	
	}
	
}

/*---------------------------------------------------------------------------
**
*/
void Impact::draw()
{
	for (list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p) 
	{
		p -> drawNoTrail();
	}
}

/*---------------------------------------------------------------------------
**
*/
void Impact::addParticles( unsigned numParticles )
{
	for( unsigned i = 0u; i < numParticles; i++ )
	{
        float radius = 1.0f;
        float mass   = 0.0f;

		Particle p = Particle( mLoc, mDT, mass, radius );

		mParticles.push_back( p );
	}
}
