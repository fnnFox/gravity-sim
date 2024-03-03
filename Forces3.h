#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#define G 0.3f
#define COLLIDE false
#define MAX_FORCE 400.f

using namespace sf;
typedef Vector2f point;

class Body : public CircleShape {
    float mass;
    Vector2f velocity, acceleration;
    std::vector<Vertex> trail;
    bool movable;
public:
    Body(point, Vector2f, Vector2f, float, bool, sf::Color);
    void setForce(Vector2f);
    void calcVelocity();
    void calcCoordinates();
    void calcAll();
    float getmass();
    Vector2f getVelocity();
    Vector2f getAcceleration();
    std::vector<Vertex> getTrail();
};

class Space {
    std::vector<Body> bodies;
public:
    void addBody(Body);
    void calculateForces();
    void update();
    Body operator[] (unsigned int);
    Body getbody(unsigned int);
    std::vector<Body> getlist();
    unsigned int size();
    void printAll(sf::RenderWindow*);
};

Vector2f getUnit(Vector2f);
float getLength(Vector2f);
Vector2f operator+ (Vector2f, float);