#include "Forces3.h"
#include <iostream>

void Space::addBody(Body body) {
	bodies.push_back(body);
}
void Space::calculateForces() {
    for (int i = 0; i < bodies.size(); i++) {
        Vector2f sum(0, 0);
        point first = bodies[i].getPosition();
        for (int j = 0; j < bodies.size(); j++) {
            if (i == j) continue;
            float x0, y0, K;
            point second = bodies[j].getPosition();
            x0 = second.x - first.x;
            y0 = second.y - first.y;
            K = (G * (bodies[i].getmass() * bodies[j].getmass())) / pow(x0 * x0 + y0 * y0, 1.5);
            sum.x += x0 * K;
            sum.y += y0 * K;
        }
        bodies[i].setForce(sum);
    }
}
void Space::update() {

    for (int i = 0; i < bodies.size(); i++) {

        bodies[i].calcAll();

        if (COLLIDE) {
            for (int j = 0; j < bodies.size(); j++) {
                if (i == j) continue;
                auto a = bodies[i], b = bodies[j];
                if (getLength(b.getPosition() - a.getPosition()) + 1 < a.getRadius() + b.getRadius()) {
                    std::cout << "collide" << std::endl;
                    bodies.erase(bodies.begin() + i);
                    bodies.erase(bodies.begin() + j);
                }
            }
        }
    }
}
Body Space::operator[](unsigned int index) {
    return bodies[index];
}
void Space::printAll(sf::RenderWindow* window) {
    for (Body& b : bodies) {
        auto trail = b.getTrail();
        window->draw(&trail[0], trail.size(), sf::LineStrip);
        window->draw(b);
    }
}
std::vector<Body> Space::getlist() {
    return bodies;
}
unsigned int Space::size() {
    return bodies.size();
}
Body Space::getbody(unsigned int index) {
    return bodies[index];
}