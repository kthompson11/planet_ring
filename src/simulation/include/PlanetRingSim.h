#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <armadillo>

#include "Particle.h"
#include "NBodySimulation.h"

class PlanetRingSim
{
public:
    PlanetRingSim(unsigned int nMoonParticles, GLuint shaderID);
    ~PlanetRingSim();

    /* Steps the simulation by on time interval */
    void step(double dt);

    /* Draws the simulation. Viewport must already be set up. */
    void draw();

    void reset(unsigned int nParticles);

private:
    NBodySimulation nBodySim;

    int nMoonParticles;
    double t = 0;
    std::vector<arma::vec3> particleColors;

    GLuint shaderID;
    GLuint particleBuffer = 0;
    GLuint sphereBuffer = 0;
    std::vector<float> pointVertices;
    std::vector<float> sphereVertices;
};

#endif /* SIMULATION_H */