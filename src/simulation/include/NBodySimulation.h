#ifndef NBODYSIMULATION_H
#define NBODYSIMULATION_H

#include <vector>

#include <armadillo>

#include "Particle.h"

class NBodySimulation
{
public:
    // Resets simulation to initial empty state
    void reset();
    // Adds a particle to the simulation
    void addParticle(const Particle &particle); 
    // Steps all particles forward in time by dt 
    void step(double dt);

    double getTime() { return t; }

    /* Returns a vector of particles in the same order they were added. */
    std::vector<Particle>& getParticles() { return particles; }
private:
    double t;
    std::vector<Particle> particles;
};

#endif /* NBODYSIMULATION_H */