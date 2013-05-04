#pragma once
#include "Particle.h"
#include <list>

class ParticleController {
public:
	ParticleController();
	void update();
	void draw();
    void addParticle( int xi, int yi );
	void removeParticles( int amt );
    
    inline void setFramerate( float framerate ) { mDT = 1.0f/framerate; };
    inline void setChannel( Channel32f& channel ) { mChannel = channel; };
    inline void setMode( unsigned mode ) { mMode = mode; };
	
protected:
	std::list<Particle>	mParticles;
    
    unsigned mXRes;
    unsigned mYRes;
    float mDT;
    Channel32f mChannel;
    unsigned mMode;
};