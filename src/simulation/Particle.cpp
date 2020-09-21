
#include <iostream>

#include "Particle.h"

using arma::vec3;
using namespace std;

static const double G = 6.67408e-11;  // gravitational constant

Particle::Particle(double mass, double radius, vec3 position, vec3 velocity)
    : mass(mass), radius(radius), position(position), velocity(velocity)
{

}

bool Particle::checkCollision(const Particle &other)
{
    vec3 diff = (position - other.position);
    double seperation = arma::dot(diff, diff);  // actually seperation ^ 2

    double minSeperation = radius + other.radius;
    minSeperation = minSeperation * minSeperation;

    if (seperation < minSeperation) {
        return true;
    }

    return false;
}

void Particle::tryCollide(Particle &other)
{
    if (checkCollision(other)) {
        vec3 diff = (position - other.position);
        vec3 displacementDirection = arma::normalise(diff);
        double diffMag = arma::dot(diff, displacementDirection);
        double overlap = radius + other.radius - diffMag;

        // calculate the new velocity
        double v1 = arma::dot(velocity, displacementDirection);
        double v2 = arma::dot(other.velocity, displacementDirection);
        double p = v1 * mass + v2 * other.mass;
        double v = p / (mass + other.mass);
        velocity = velocity + (v - v1) * displacementDirection;
        other.velocity = other.velocity + (v - v2) * displacementDirection;

        // resolve overlap
        double d = overlap * other.mass / (mass + other.mass);
        displace(d * displacementDirection);
        d = overlap * mass / (mass + other.mass);
        other.displace(-d * displacementDirection);
    }
}

void Particle::interact(Particle &other, double dt)
{
    vec3 diff = (position - other.position);
    vec3 displacementDirection = arma::normalise(diff);
    double F = G * mass * other.mass / arma::dot(diff, diff);

    velocity = velocity - displacementDirection * F / mass * dt;
    other.velocity = other.velocity + displacementDirection * F / other.mass * dt;
}

void Particle::step(double t)
{
    position += velocity * t;
}

void Particle::displace(const vec3 &displacement)
{
    position += displacement;
}