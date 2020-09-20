#ifndef PARTICLE_H
#define PARTICLE_H

#include <armadillo>

class Particle
{
public:
    Particle(double mass, double radius, arma::vec3 position, arma::vec3 velocity);

    /**
     * Checks for a collision with another particle.
     * @param other The other particle to check for collision with.
     * 
     * @return true if particles are colliding; false otherwise
     */
    bool checkCollision(const Particle &other);
    // Resolve collisions with another particle if needed
    void tryCollide(Particle &other);
    // Interact with another particle for a time period dt
    void interact(Particle &other, double dt);
    // Moves particle forward in time by t
    void step(double t);

    // Displace particle in space
    void displace(const arma::vec3 &displacement);
    void setPosition(const arma::vec3 &position) { this->position = position; }
    void setVelocity(const arma::vec3 &velocity) { this->velocity = velocity; }

    bool isAlive() const { return alive; }
    const arma::vec3& getPosition() const { return position; }
    const arma::vec3& getVelocity() const { return velocity; }
    const double getRadius() const { return radius; }
    const double getMass() const { return mass; }

private:
    bool alive = true;  // is particle still being simulated?
    double mass;
    double radius;
    arma::vec3 position;  // position
    arma::vec3 velocity;  // velocity
};

#endif /* PARTICLE_H */