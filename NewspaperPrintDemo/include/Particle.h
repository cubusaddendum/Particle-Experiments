#pragma once
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"

#include <vector>

using namespace ci;
using namespace cinder;

class Particle {
public:
	Particle();
	Particle( ci::Vec2f );
	void update( const float& dT, unsigned mode );
    void update( const cinder::Channel32f& channel );
	void draw();
	
	Vec2f	mLoc;
	Vec2f	mDir;
	float	mVel;
    Color   mColor;
    float   mScale;
	float	mRadius;
};