#ifndef __ParticleDemo__ImpactEngine__
#define __ParticleDemo__ImpactEngine__

#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Perlin.h"
#include "Particle.h"
#include <list>
#include <vector>

class Impact
{
public:
	explicit Impact(const ci::Vec2f& loc, const ci::Vec2f& vel, const float ke, const float framerate);
	explicit Impact(const Impact& impact);

	virtual ~Impact(void);

	void update(const ci::Perlin &perlin, const ci::Channel32f &channel);
	void draw();

	inline bool isDead() { return mIsDead; }

protected:
	void addParticles( unsigned numParticles );

protected:
	std::list<Particle> mParticles;
	ci::Vec2f			mLoc;
	ci::Vec2f			mVel;
	float				mKe;
	float				mBrightness;
	bool				mIsDead;
	float				mElapsedTime;
	float				mThreshold;
	float				mDT;
};

#endif /*__ParticleDemo__ImpactEngine__*/

