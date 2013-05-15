//
//  ParticleController.h
//  ParticleDemo
//
//  Created by Robert Bantin on 4/2/13.
//
//

#ifndef __ParticleDemo__ParticleController__
#define __ParticleDemo__ParticleController__

/*---------------------------------------------------------------------------
**
*/

#include "Particle.h"
#include "cinder/Perlin.h"
#include <list>
#include <vector>
#include <map>

/*---------------------------------------------------------------------------
**
*/

/*---------------------------------------------------------------------------
**
*/

class ParticleController {
public:
    ParticleController();
    void update(const cinder::Perlin &perlin, const cinder::Channel32f &channel);
    void draw();
    void addParticles( unsigned numParticles );
    void removeParticles( unsigned numParticles );
    
    inline void setFrameRate ( float framerate ) { mFrameRate = framerate; }

protected:
    std::list<Particle> mParticles;
    std::vector<ci::Vec2f> mLocations;
    std::vector<ci::Vec2f> mVelocities;
	std::multimap<unsigned, unsigned> mCollisions;
    
    float mFrameRate;
};

/*---------------------------------------------------------------------------
**
*/

#endif /* defined(__ParticleDemo__ParticleController__) */
