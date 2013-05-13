//
//  Particle.h
//  ParticleDemo
//
//  Created by Robert Bantin on 4/2/13.
//
//

#ifndef __ParticleDemo__Particle__
#define __ParticleDemo__Particle__

/*---------------------------------------------------------------------------
 **
 */

#include "cinder/Channel.h"
#include "cinder/Vector.h"

#include <vector>

/*---------------------------------------------------------------------------
**
*/

class Particle
{
    
public:
	explicit Particle( ci::Vec2f location, float frameRate );
    explicit Particle( ci::Vec2f location, float frameRate, float mass );
    explicit Particle( ci::Vec2f location, float frameRate, float mass, float radius );
	void update();
	void draw();
    
	inline ci::Vec2f getLocation() { return	mLoc; }
	inline ci::Vec2f getVelocity() { return mVel; }
    
    inline float getMass() { return mMass; }
    inline float getField() { return mField; }
	inline float getRadius() { return mRadius; }
    
    inline void setLocation( ci::Vec2f location ) { mLoc = location; }
    inline void setVelocity( ci::Vec2f velocity ) { mVel = velocity; }
    
    inline void setMass  ( float mass )   { mMass = mass; }
    inline void setField ( float field ) { mField = field; }
	inline void setRadius ( float radius ) { mRadius = radius; }

	inline void setImpact ( ci::Vec2f location, float ke ) { mLocImpact = location; mKe = ke; if (mImpact == 0.0f) mImpact = 4.0f; }

protected:
    
    void updateTrail();
    
    std::vector<ci::Vec2f> mPastLocations;
    
	ci::Vec2f	mLoc;
	ci::Vec2f	mVel;
    ci::Vec2f   mLocImpact;
    
    float       mMass;
    float       mField;
	float		mRadius;
    float       mDT;
	float       mImpact;
    float       mKe;
    
};
#endif /* defined(__ParticleDemo__Particle__) */
