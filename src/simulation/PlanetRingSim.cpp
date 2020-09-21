
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PlanetRingSim.h"
#include "ShaderProgram.h"

using namespace std;
using arma::vec3;

#define PLANET_INDEX        0
#define MOON_START_INDEX    1

/******************** Helper Functions ********************/

static void writeVertex(vector<float> &buf, const vec3 &vertex, const vec3 &color)
{
    buf.push_back(vertex[0]);
    buf.push_back(vertex[1]);
    buf.push_back(vertex[2]);
    buf.push_back(color[0]);
    buf.push_back(color[1]);
    buf.push_back(color[2]);
}

static vec3 sphericalToCartesian(double theta, double phi, double radius)
{
    vec3 res = {cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)};
    res = radius * res;

    return res;
}

static vector<float> calcSphereVertices(unsigned int thetaSteps, unsigned int phiSteps)
{
    double thetaStep = 2 * M_PI / thetaSteps;
    double phiStep = M_PI / phiSteps;
    vector<float> sphereVertices;
    vec3 color = {1.0f, 1.0f, 1.0f};

    for (int i = 0; i < thetaSteps; ++i) {
        for (int j = 0; j < phiSteps; ++j) {
            double theta = i * thetaStep;
            double nextTheta = (i + 1) * thetaStep;
            double phi = j * phiStep;
            double nextPhi = (j + 1) * phiStep;

            vec3 topLeft = sphericalToCartesian(theta, phi, 1.0);
            vec3 topRight = sphericalToCartesian(nextTheta, phi, 1.0);
            vec3 bottomLeft = sphericalToCartesian(theta, nextPhi, 1.0);
            vec3 bottomRight = sphericalToCartesian(nextTheta, nextPhi, 1.0);

            // first triangle
            writeVertex(sphereVertices, topLeft, {1.0f, 0.0f, 0.0f});
            writeVertex(sphereVertices, bottomLeft, {0.0f, 1.0f, 0.0f});
            writeVertex(sphereVertices, bottomRight, {0.0f, 0.0f, 1.0f});
            // second triangle
            writeVertex(sphereVertices, topLeft, {1.0f, 0.0f, 0.0f});
            writeVertex(sphereVertices, bottomRight, {0.0f, 1.0f, 0.0f});
            writeVertex(sphereVertices, topRight, {0.0f, 0.0f, 1.0f});
        }
    }

    // TODO: remove this
    #ifdef COMMENTED_OUT
    for (int i = 0; i < 200; i = i + 6) {
        if (i % 18 == 0) {
            cout << "**** New Triangle ****\n";
        }
        cout << "(" << sphereVertices[i] << ", " << sphereVertices[i+1] << ", " << sphereVertices[i+2] << "), ";
        cout << sphereVertices[i+3] << ", " << sphereVertices[i+4] << ", " << sphereVertices[i+5] << ")" << endl;
    }
    #endif

    return sphereVertices;
}

/******************** Class Methods ********************/

PlanetRingSim::PlanetRingSim(unsigned int nMoonParticles, GLFWwindow *window, ShaderProgram &program) 
    : shaderProgram(program), camera(0, 90, -0.5, 0.5, 40000e3, program), window(window)
{
    glGenBuffers(1, &particleBuffer);
    glGenBuffers(1, &sphereBuffer);

    sphereVertices = calcSphereVertices(20, 10);
    glBindBuffer(GL_ARRAY_BUFFER, sphereBuffer);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(sphereVertices[0]), sphereVertices.data(), GL_STATIC_DRAW);

    reset(nMoonParticles);
}

PlanetRingSim::~PlanetRingSim()
{
    glDeleteBuffers(1, &particleBuffer);
    glDeleteBuffers(1, &sphereBuffer);
}

void PlanetRingSim::reset(unsigned int nMoonParticles)
{
    this->nMoonParticles = nMoonParticles;
    
    // create planet
    const double planetMass = 5.97e24;
    const double planetRadius = 6.378e6;
    nBodySim.addParticle(Particle(planetMass, planetRadius, vec3({0.0, 0.0, 0.0}), vec3({0.0, 0.0, 0.0})));
    particleColors.push_back(vec3()); // dummy element

    // create moon
    for (int i = 0; i < 200; ++i) {
        double x = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
        double y = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
        double z = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
        nBodySim.addParticle(Particle(100, 0.01, vec3({x, y, z}), vec3({0.0, 0.0, 0.0})));
        /* TODO: assign color here */
        particleColors.push_back(vec3({1.0f, 1.0f, 1.0f}));
    }

    #ifdef COMMENTED_OUT
    // TODO: add better moon generation (HCP)
    const double moonRadius = 1737e3;
    const double moonMass = 7.34e22;
    double moonVolume = M_PI * moonRadius * moonRadius;
    double particleVolume = moonVolume * 0.74; // multiply by sphere close pack ratio
    particleVolume /= nMoonParticles;
    double particleRadius = sqrt(particleVolume / M_PI);
    double particleMass = moonMass / nMoonParticles;
    double F_gravity = 6.67e-11 * moonMass * 
    for (int i = 0; i < nMoonParticles; ++i) {
        nBodySim.addParticle(Particle(particleMass, particleVolume, ))
    }
    #endif
}

void PlanetRingSim::draw()
{
    /* update camera */
    camera.update(window, shaderProgram);

    glm::mat4 model;
    GLint modelLocation;
    const vector<Particle>& particles = nBodySim.getParticles();

    
    // draw planet
    glBindBuffer(GL_ARRAY_BUFFER, sphereBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    modelLocation = glGetUniformLocation(shaderProgram.ID(), "model");
    const Particle &particle = particles[PLANET_INDEX];
    float r = particle.getRadius();
    const vec3 &pos = particle.getPosition();
    model = glm::mat4(1.0f);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(pos[0], pos[1], pos[2]));
    model = glm::scale(model, glm::vec3(r, r, r));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size() / VERTEX_ELEMENT_FLOAT_COUNT);

    // draw moon particles
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    model = glm::mat4(1.0f);
    modelLocation = glGetUniformLocation(shaderProgram.ID(), "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    pointVertices.clear();
    for (int i = MOON_START_INDEX; i < particles.size(); ++i) {
        if (particles[i].isAlive()) {
            const vec3 &pos = particles[i].getPosition();
            pointVertices.push_back(pos[0]);
            pointVertices.push_back(pos[1]);
            pointVertices.push_back(pos[2]);
            const vec3 &color = particleColors[i];
            pointVertices.push_back(color[0]);
            pointVertices.push_back(color[1]);
            pointVertices.push_back(color[2]);
        }
    }
    glBufferData(GL_ARRAY_BUFFER, pointVertices.size() * sizeof(pointVertices[0]), pointVertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, pointVertices.size() / VERTEX_ELEMENT_FLOAT_COUNT);
    

    #ifdef COMMENTED_OUT
    float vertices[] = {
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 1.0f,     0.0f, 1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    model = glm::mat4(1.0f);
    modelLocation = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    pointVertices.clear();
    for (int i = 0; i < sizeof(vertices) / sizeof(float); ++i) {
        pointVertices.push_back(vertices[i]);
    }
    glBufferData(GL_ARRAY_BUFFER, pointVertices.size() * sizeof(pointVertices[0]), pointVertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, pointVertices.size() / VERTEX_ELEMENT_FLOAT_COUNT);
    #endif
}

void PlanetRingSim::step(double dt)
{
    nBodySim.step(dt);
}