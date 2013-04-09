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
#include <list>
#include <vector>

/*---------------------------------------------------------------------------
**
*/

/*---------------------------------------------------------------------------
**
*/

class ParticleController {
public:
    ParticleController();
    void update();
    void draw();
    void addParticles( unsigned numParticles );
    void removeParticles( unsigned numParticles );
    
    inline void setFrameRate ( float framerate ) { mFrameRate = framerate; }

protected:
    std::list<Particle> mParticles;
    std::vector<ci::Vec2f> mLocations;
    std::vector<ci::Vec2f> mVelocities;
    
    float mFrameRate;
};

/*---------------------------------------------------------------------------
**
*/

#endif /* defined(__ParticleDemo__ParticleController__) */
