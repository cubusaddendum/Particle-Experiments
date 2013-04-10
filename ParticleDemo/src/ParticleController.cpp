//
//  ParticleController.cpp
//  ParticleDemo
//
//  Created by Robert Bantin on 4/2/13.
//
//

/*---------------------------------------------------------------------------
**
*/

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/CinderMath.h"
#include "ParticleController.h"

/*---------------------------------------------------------------------------
**
*/

using namespace ci;
using std::list;

namespace 
{
	bool outofbounds(Particle& particle) 
	{
		float rightbound = 1.1f * static_cast<float>(app::getWindowWidth());
		float lowerbound = 1.1f * static_cast<float>(app::getWindowHeight());
		float leftbound  = - (rightbound - static_cast<float>(app::getWindowWidth()));
		float upperbound = - (lowerbound - static_cast<float>(app::getWindowHeight()));

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
};

/*---------------------------------------------------------------------------
**
*/

ParticleController::ParticleController()
{
}

/*---------------------------------------------------------------------------
**
*/

void ParticleController::update()
{
    unsigned numParticles = mParticles.size();
	float dT = 1.0f/mFrameRate;
	float cscale = 1.0f/numParticles;
	unsigned n = 0u;

	mParticles.remove_if (outofbounds);

	addParticles( numParticles - mParticles.size() );
 
    mLocations.assign(numParticles, Vec2f(0.0f,0.0f));
    mVelocities.assign(numParticles, Vec2f(0.0f,0.0f));
	mCollisions.clear();

	Vec2f cog = Vec2f(0.0f,0.0f);
    
   	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p)
    {
		mLocations[n] = ( p -> getLocation() );
		cog += mLocations[n] * cscale; 
        mVelocities[n++] = ( p -> getVelocity() );
	}
    
    for ( unsigned i = 0u; i < numParticles; ++i )
    {
		Vec2f d = cog - mLocations[i];

		float sdx; (d.x < 0.0f ? sdx = -1.0f : sdx = 1.0f);
		float sdy; (d.y < 0.0f ? sdy = -1.0f : sdy = 1.0f);

        Vec2f acc = Vec2f(sdx * numParticles * 100000.0f / cinder::math< float >::pow(Vec2f(cog - dT*mVelocities[i]).x, 2.0f), 
			sdy * numParticles * 100000.0f / cinder::math< float >::pow(Vec2f(cog - dT*mVelocities[i]).y, 2.0f));
        
        mVelocities[i] += acc * dT;
    }

	list<Particle>::iterator Ploc = mParticles.begin();
	list<Particle>::iterator Qloc = mParticles.begin();

	for ( unsigned i = 0u; i < numParticles; ++i )
    {
        for ( unsigned j = 0u; j < numParticles; ++j )
        {
            if (i != j)
            {
				Vec2f A = mLocations[i] - mLocations[j];
				Vec2f B = mVelocities[i] - mVelocities[j];

				float d2 = (((A.x * A.x) + (A.y * A.y)) - ((A.x * B.x) + (A.y * B.y)) / ((B.x * B.x) + (B.y * B.y)));

				if (d2 <= 500.0f) // make mRadius 11.180339887498948482045868343656f
				{
					std::multimap<unsigned,unsigned>::iterator it = mCollisions.end();
					std::pair<std::multimap<unsigned,unsigned>::iterator, std::multimap<unsigned,unsigned>::iterator> range = mCollisions.equal_range(i); 
					
					if ((it = range.first) == mCollisions.end())
					{
						mCollisions.insert(std::pair<unsigned,unsigned>(i,j));
						mCollisions.insert(std::pair<unsigned,unsigned>(j,i));

						float X     = cinder::math<float>::sqrt(A.x * A.x);
						float Y     = cinder::math<float>::sqrt(A.y * A.y);
						float theta = 1.5707963267948966192313216916398f - cinder::math<float>::atan2(Y, X);

						Vec2f tempP = mVelocities[i];
						Vec2f tempQ = mVelocities[j];
				
						mVelocities[i].x = (cinder::math<float>::sin(theta) * tempQ.y - cinder::math<float>::cos(theta) * tempQ.x);
						mVelocities[i].y = (cinder::math<float>::cos(theta) * tempQ.y + cinder::math<float>::sin(theta) * tempQ.x);
						mVelocities[j].x = (cinder::math<float>::sin(theta) * tempP.y - cinder::math<float>::cos(theta) * tempP.x);
						mVelocities[j].y = (cinder::math<float>::cos(theta) * tempP.y + cinder::math<float>::sin(theta) * tempP.x);
					}

					Ploc -> setImpact();
					Qloc -> setImpact();
				}
            }
        }

		++Ploc;
		++Qloc;
	}
    
    n = 0u;
    
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p)
    {
        p -> setVelocity(mVelocities[n++]);
		p -> update();
	}
}

/*---------------------------------------------------------------------------
**
*/

void ParticleController::draw()
{
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p )
    {
		p -> draw();
	}
}

/*---------------------------------------------------------------------------
**
*/

void ParticleController::addParticles( unsigned numParticles )
{
	for( unsigned i = 0u; i < numParticles; i++ )
	{
		Vec2f centre = Vec2f(static_cast<float>(app::getWindowWidth() >> 1), static_cast<float>(app::getWindowHeight() >> 1)); 

		float rwidth = Rand::randFloat(-1.0f,1.0f) * static_cast<float>(app::getWindowWidth() < app::getWindowHeight() ? app::getWindowWidth() >> 1 : app::getWindowHeight() >> 1);
		float theta  = Rand::randFloat(6.283185307179586476925286766559f); 

		float x = rwidth * cinder::math<float>::sin(theta); 	
		float y = rwidth * cinder::math<float>::cos(theta);

		centre.x += x;
		centre.y += y;
        
		mParticles.push_back( Particle( centre, mFrameRate ) );
	}
}

/*---------------------------------------------------------------------------
**
*/

void ParticleController::removeParticles( unsigned numParticles )
{
	for( unsigned i = 0u; i < numParticles; i++ )
	{
		mParticles.pop_back();
        mLocations.pop_back();
	}
}
