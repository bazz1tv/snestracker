
#ifndef __EFFECTS_H__
#define __EFFECTS_H__

#include "Globals.h"

extern SDL_Surface *pix;
extern SDL_Texture *pix_tex;

/// The Particle class
class cMParticle : public cMAngleSprite
{
public:
	cMParticle( double x, double y, double nangle = 0, double nspeed = 0 );
	~cMParticle( void );

	void Update( void );

	void Draw( SDL_Renderer *renderer );

	void HandleCollision( int direction );

	/// The Color and Alpha
	double red,green,blue,alpha;

	/// The fade-out speed
	double fadeoutspeed;

	/// The Direction Randomness
	double Randomness;

	/// Does the Particle collides and has gravity
	bool active;

};

/// The Particle Emitter class
class cMParticleEmitter
{
public:
	cMParticleEmitter( double x, double y );
	~cMParticleEmitter( void );

	void InitParticles( unsigned int numParticles );

	void Update( void );

	void Draw( SDL_Renderer *renderer );

	/// The Position
	double posx,posy;
	
	/// The Color and Alpha
	Uint8 red,green,blue,alpha;

	/// The fade-out speed
	double fadeoutspeed;
	
	/// The Direction
	double angle,speed;

	/// The Direction Randomness
	double Randomness;
	
	/// If all Particles faded out
	bool visible;
	
	/// The Particles
	cMParticle **Particles;
	unsigned int ParticleCount;

};

/// The dynamic Particle Emittter Array
extern cMParticleEmitter **ParticleEmitter;
extern unsigned int ParticleEmitterCount;

/** Adds an Particle Emitter
 * Power : Amount of Particles
 * Randomness : Amount of Random Movement
 *			-----	-----
 * if Fixed Direction is set all spawned Particles
 * will have those direction when spawned
 */
void AddParticleEmitter( double x, double y, double nSpeed, Uint8 nred, Uint8 ngreen, Uint8 nblue,
						double nfadeoutspeed, unsigned int power, double nRandomness, double fixedDirection = -1 );

/// Updates all Particle Emitter
void UpdateParticleEmitter( void );

/// Draws all Particle Emitter
void DrawParticleEmitter( void );

/// Delete's every Particle Emitter
void DeleteAllParticleEmitter( void );

#endif