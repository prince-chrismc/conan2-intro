
#include <math.h>

//========================================================================
// These are fixed constants that control the particle engine. In a
// modular world, these values should be variables...
//========================================================================

// Maximum number of particles
#define MAX_PARTICLES   3000

// Life span of a particle (in seconds)
#define LIFE_SPAN       8.f

// A new particle is born every [BIRTH_INTERVAL] second
#define BIRTH_INTERVAL (LIFE_SPAN/(float)MAX_PARTICLES)

// Particle size (meters)
#define PARTICLE_SIZE   0.7f

// Gravitational constant (m/s^2)
#define GRAVITY         9.8f

// Base initial velocity (m/s)
#define VELOCITY        8.f

// Bounce friction (1.0 = no friction, 0.0 = maximum friction)
#define FRICTION        0.75f

// "Fountain" height (m)
#define FOUNTAIN_HEIGHT 3.f

// Fountain radius (m)
#define FOUNTAIN_RADIUS 1.6f

// Minimum delta-time for particle phisics (s)
#define MIN_DELTA_T     (BIRTH_INTERVAL * 0.5f)


//========================================================================
// Particle system global variables
//========================================================================

// This structure holds all state for a single particle
typedef struct {
    float x,y,z;     // Position in space
    float vx,vy,vz;  // Velocity vector
    float r,g,b;     // Color of particle
    float life;      // Life of particle (1.0 = newborn, < 0.0 = dead)
    int   active;    // Tells if this particle is active
} PARTICLE;

// Global vectors holding all particles. We use two vectors for double
// buffering.
static PARTICLE particles[MAX_PARTICLES];

// Global variable holding the age of the youngest particle
static float min_age;

// Color of latest born particle (used for fountain lighting)
static float glow_color[4];

// Position of latest born particle (used for fountain lighting)
static float glow_pos[4];




//========================================================================
// Initialize a new particle
//========================================================================

static void init_particle(PARTICLE *p, double t)
{
    float xy_angle, velocity;

    // Start position of particle is at the fountain blow-out
    p->x = 0.f;
    p->y = 0.f;
    p->z = FOUNTAIN_HEIGHT;

    // Start velocity is up (Z)...
    p->vz = 0.7f + (0.3f / 4096.f) * (float) (rand() & 4095);

    // ...and a randomly chosen X/Y direction
    xy_angle = (2.f * (float) M_PI / 4096.f) * (float) (rand() & 4095);
    p->vx = 0.4f * (float) cos(xy_angle);
    p->vy = 0.4f * (float) sin(xy_angle);

    // Scale velocity vector according to a time-varying velocity
    velocity = VELOCITY * (0.8f + 0.1f * (float) (sin(0.5 * t) + sin(1.31 * t)));
    p->vx *= velocity;
    p->vy *= velocity;
    p->vz *= velocity;

    // Color is time-varying
    // p->r = 0.7f + 0.3f * (float) sin(0.34 * t + 0.1);
    // p->g = 0.6f + 0.4f * (float) sin(0.63 * t + 1.1);
    // p->b = 0.6f + 0.4f * (float) sin(0.91 * t + 2.1);

    // Store settings for fountain glow lighting
    glow_pos[0] = 0.4f * (float) sin(1.34 * t);
    glow_pos[1] = 0.4f * (float) sin(3.11 * t);
    glow_pos[2] = FOUNTAIN_HEIGHT + 1.f;
    glow_pos[3] = 1.f;
    glow_color[0] = p->r;
    glow_color[1] = p->g;
    glow_color[2] = p->b;
    glow_color[3] = 1.f;

    // The particle is new-born and active
    p->life = 1.f;
    p->active = 1;
}


//========================================================================
// Update a particle
//========================================================================

#define FOUNTAIN_R2 (FOUNTAIN_RADIUS+PARTICLE_SIZE/2)*(FOUNTAIN_RADIUS+PARTICLE_SIZE/2)

static void update_particle(PARTICLE *p, float dt)
{
    // If the particle is not active, we need not do anything
    if (!p->active)
        return;

    // The particle is getting older...
    p->life -= dt * (1.f / LIFE_SPAN);

    // Did the particle die?
    if (p->life <= 0.f)
    {
        p->active = 0;
        return;
    }

    // Apply gravity
    p->vz = p->vz - GRAVITY * dt;

    // Update particle position
    p->x = p->x + p->vx * dt;
    p->y = p->y + p->vy * dt;
    p->z = p->z + p->vz * dt;

    // Simple collision detection + response
    if (p->vz < 0.f)
    {
        // Particles should bounce on the fountain (with friction)
        if ((p->x * p->x + p->y * p->y) < FOUNTAIN_R2 &&
            p->z < (FOUNTAIN_HEIGHT + PARTICLE_SIZE / 2))
        {
            p->vz = -FRICTION * p->vz;
            p->z  = FOUNTAIN_HEIGHT + PARTICLE_SIZE / 2 +
                    FRICTION * (FOUNTAIN_HEIGHT +
                    PARTICLE_SIZE / 2 - p->z);
        }

        // Particles should bounce on the floor (with friction)
        else if (p->z < PARTICLE_SIZE / 2)
        {
            p->vz = -FRICTION * p->vz;
            p->z  = PARTICLE_SIZE / 2 +
                    FRICTION * (PARTICLE_SIZE / 2 - p->z);
        }
    }
}


//========================================================================
// The main frame for the particle engine. Called once per frame.
//========================================================================

static void particle_engine(double t, float dt)
{
    int i;
    float dt2;

    // Update particles (iterated several times per frame if dt is too large)
    while (dt > 0.f)
    {
        // Calculate delta time for this iteration
        dt2 = dt < MIN_DELTA_T ? dt : MIN_DELTA_T;

        for (i = 0;  i < MAX_PARTICLES;  i++)
            update_particle(&particles[i], dt2);

        min_age += dt2;

        // Should we create any new particle(s)?
        while (min_age >= BIRTH_INTERVAL)
        {
            min_age -= BIRTH_INTERVAL;

            // Find a dead particle to replace with a new one
            for (i = 0;  i < MAX_PARTICLES;  i++)
            {
                if (!particles[i].active)
                {
                    init_particle(&particles[i], t + min_age);
                    update_particle(&particles[i], min_age);
                    break;
                }
            }
        }

        dt -= dt2;
    }
}

