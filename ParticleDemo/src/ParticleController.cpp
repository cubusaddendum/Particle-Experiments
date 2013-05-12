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
            
            float threshold = (Ploc -> getRadius() + Qloc -> getRadius()) * (Ploc -> getRadius() + Qloc -> getRadius());

			if (d2 <= threshold)
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

					float thetaP = atan2(mVelocities[i].y, mVelocities[i].x);
					float thetaQ = atan2(mVelocities[j].y, mVelocities[j].x);
                    
                    //! Acquire mass of each particle
                    float Pm = Ploc -> getMass();
                    float Qm = Qloc -> getMass();
                    float totalMass_1  = 1.0f / (Pm + Qm);
                    
                    //! Derived velocity on-axis
                    float Pvrx = Pvi * cos(thetaP - phi);
                    float Qvrx = Qvi * cos(thetaQ - phi);
                    
                    //! Exchange momentum
                    Vec2f Pvr = Vec2f(((Pm - Qm) * Pvrx + (Qm + Qm) * Qvrx) * totalMass_1, Pvi * sin(thetaP - phi) );
                    Vec2f Qvr = Vec2f(((Pm + Pm) * Pvrx + (Qm - Pm) * Qvrx) * totalMass_1, Qvi * sin(thetaQ - phi) );
                    
                    //! Compute new velocities of each particle
                    mVelocities[i].x = cos(phi) * Pvr.x - sin (phi) * Pvr.y;
					mVelocities[i].y = sin(phi) * Pvr.x + cos (phi) * Pvr.y;
                    mVelocities[j].x = cos(phi) * Qvr.x - sin (phi) * Qvr.y;
					mVelocities[j].y = sin(phi) * Qvr.x + cos (phi) * Qvr.y;
				}

				//Ploc -> setImpact();
				//Qloc -> setImpact();
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

        float radius = Rand::randFloat(1.0f, 3.0f);
        float mass   = 3580986219567.6f * /* 4/3 pi */ 4.18879020478638f * radius * radius * radius;
        
		mParticles.push_back( Particle( centre, mFrameRate, mass, radius ) );
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
