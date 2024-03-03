#include "Forces3.h"
#include <iostream>

Body::Body(point coordinates, Vector2f velocity, Vector2f acceleration, float mass, bool movable, sf::Color color) {
    this->mass = mass;
    this->setPosition(coordinates);
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->setFillColor(color);
    this->setRadius(3.0f);
    this->movable = movable;
}
void Body::setForce(Vector2f force) {
    if (movable) {
        if (MAX_FORCE != -1 && getLength(force) > MAX_FORCE) {
            std::cout << "warn force: " << getLength(force) << std::endl;

            force = getUnit(force) * MAX_FORCE;
        }
        acceleration = force / mass;
    }
}
void Body::calcVelocity() {
    velocity += acceleration;
}
void Body::calcCoordinates() {
    move(velocity);
}
void Body::calcAll() {
    this->calcVelocity();
    this->calcCoordinates();
    trail.push_back(Vertex(getPosition() + getRadius(), sf::Color(70, 70, 70)));
    for (int i = 0; i < trail.size(); i++) {
        trail[i].color.a--;
        if (trail[i].color.a <= 0)
            trail.erase(trail.begin() + i);
    }
    if (trail.size() > 255) {
        //trail.erase(trail.begin());
        std::cout << "trail overflow\n";
    }
}
float Body::getmass() {
	return mass;
}

Vector2f Body::getVelocity() {
    return velocity;
}

Vector2f Body::getAcceleration() {
    return acceleration;
}

std::vector<Vertex> Body::getTrail() {
    return trail;
}
