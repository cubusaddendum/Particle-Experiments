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
using namespace cinder;
using std::list;

namespace 
{
    static const float GravConst = 6.667221937e-11;
    
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
	

	mParticles.remove_if (outofbounds);

	addParticles( numParticles - mParticles.size() );
 
    mLocations.assign(numParticles, Vec2f(0.0f,0.0f));
    mVelocities.assign(numParticles, Vec2f(0.0f,0.0f));

	mCollisions.clear();
    
    //! Acquire total particle mass
    float Nmass = 0.0f;
    
    for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p)
    {
        Nmass += p -> getMass();
    }
    
    //! Invert Nmass to take division out of the following recursion
    Nmass = 1.0f / Nmass;
    
    //! Acquire centre of particle gravity
    Vec2f cog = Vec2f(0.0f,0.0f);
    unsigned n = 0u;
    
   	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p)
    {
		mLocations[n] = ( p -> getLocation() );
		cog += mLocations[n] * p -> getMass() * Nmass; //! effectively divided-by Nmass
        mVelocities[n++] = ( p -> getVelocity() );
	}
    
    Nmass = 1.0f / Nmass; //! Invert again to avoid division in the next recursion
    
    for ( unsigned i = 0u; i < numParticles; ++i )
    {
		Vec2f r = cog - mLocations[i];

		float sdx; (r.x < 0.0f ? sdx = -1.0f : sdx = 1.0f);
		float sdy; (r.y < 0.0f ? sdy = -1.0f : sdy = 1.0f);
        
        Vec2f d  = cog - dT * mVelocities[i];
        float d2 = d.x * d.x + d.y * d.y;
        Vec2f dhat = d / cinder::math<float>::sqrt(d2);
        
        Vec2f g = dhat * Vec2f(GravConst * sdx * Nmass / d2, GravConst * sdy * Nmass / d2);
        
        mVelocities[i] += g * dT;
        
        /*Vec2f acc = Vec2f(GravConst * sdx * Nmass / cinder::math< float >::pow(Vec2f(cog - dT * mVelocities[i]).x, 2.0f),
			GravConst * sdy * Nmass / cinder::math< float >::pow(Vec2f(cog - dT * mVelocities[i]).y, 2.0f));
        
        mVelocities[i] += acc * dT;*/
    }

	list<Particle>::iterator Ploc = mParticles.begin();
	
	for ( unsigned i = 0u; i < numParticles - 1u; ++i )
    {
		list<Particle>::iterator Qloc = mParticles.begin();

		for ( unsigned j = 0; j < i + 1u; ++j )
        {
			Qloc++;
		}

        for ( unsigned j = i + 1u; j < numParticles; ++j )
        {

			Vec2f A = mLocations[j] - mLocations[i];
			Vec2f B = mVelocities[j] - mVelocities[i];

			float d2 = (((A.x * A.x) + (A.y * A.y)) - ((A.x * B.x) + (A.y * B.y)) / ((B.x * B.x) + (B.y * B.y)));

			if (d2 <= 100.0f) // assume radius is 5
			{
				std::multimap<unsigned,unsigned>::iterator it = mCollisions.end();
				bool bUnique = true;

				for (it = mCollisions.begin(); it != mCollisions.end(); ++it)
				{
					if ((it -> first == i) && (it -> second == j))
					{
						bUnique = false;
					}
				}
				
				if (bUnique)
				{
					mCollisions.insert(std::pair<unsigned,unsigned>(i,j));
					mCollisions.insert(std::pair<unsigned,unsigned>(j,i));

					float phi; A.x == 0.0f ? phi = 1.5707963267948966192313216916398f : phi = atan2(A.y,A.x); // does this deal with dX = 0?

					float Pvi = sqrt(mVelocities[i].x * mVelocities[i].x + mVelocities[i].y * mVelocities[i].y);
					float Qvi = sqrt(mVelocities[j].x * mVelocities[j].x + mVelocities[j].y * mVelocities[j].y);

					float thetaP = atan2(mVelocities[i].y,mVelocities[i].x);
					float thetaQ = atan2(mVelocities[j].y,mVelocities[j].x);

					mVelocities[i].x = cos(phi)*(Qvi * cos(thetaQ - phi)) - sin(phi)*(Pvi * sin(thetaP - phi));
					mVelocities[i].y = sin(phi)*(Qvi * cos(thetaQ - phi)) + cos(phi)*(Pvi * sin(thetaP - phi));
					mVelocities[j].x = cos(phi)*(Pvi * cos(thetaP - phi)) - sin(phi)*(Qvi * sin(thetaQ - phi));
					mVelocities[j].y = sin(phi)*(Pvi * cos(thetaP - phi)) + cos(phi)*(Qvi * sin(thetaQ - phi));
				}

				Ploc -> setImpact();
				Qloc -> setImpact();
			}

			++Qloc;
        }

		++Ploc;
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

		float x = rwidth * sin(theta); 	
		float y = rwidth * cos(theta);

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
