
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "NBodySimulation.h"

using namespace std;
using arma::vec3;

void NBodySimulation::reset()
{
    particles.clear();
}

void NBodySimulation::addParticle(const Particle &particle)
{
    particles.push_back(particle);
}

void NBodySimulation::step(double dt)
{
    // simple O(n^2) n-body algorithm
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = i + 1; j < particles.size(); ++j) {
            // update velocities due to interaction particles[i] and particles[j]
            particles[i].interact(particles[j], dt);
        }
        // update particles[i] position
        particles[i].step(dt);

        vec3 pos = particles[i].getPosition();
    }

    // handle collisions
    for (int i = 0; i < particles.size() - 1; ++i) {
        for (int j = i + 1; j < particles.size(); ++j) {
            particles[i].tryCollide(particles[j]);
        }
    }
}