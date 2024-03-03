#include <SFML/Graphics.hpp>
#include "Forces3.h"
#include <iostream>

void splash(sf::Color& color) {
    
    float time = 0.f;
    float halfDuration = 1 / 2.f;
    sf::Color oldColor = color;

    sf::Clock clock;

    while (time < 1) {
        time = clock.getElapsedTime().asSeconds();
        float ratio = pow(halfDuration,5 * time); //time * halfDuration;
        //if (ratio > 1.f)
        //    ratio = 2.f - ratio;

        sf::Color lerpedColor(
            50 * ratio,
            50 * ratio,
            50 * ratio
        );
        color = oldColor + lerpedColor;
    }
    color = oldColor;
}

int main() {

    size_t width = 1200, height = 800;
    std::string str = "Forces 3";

    unsigned int followed = 0;
    float speed = 1;
    bool follow = false;
    bool moving = false;
    bool running = true;
    sf::Vector2f oldpos;
    float zoom = 1.f;

    sf::RenderWindow window(sf::VideoMode(width, height), str);
    sf::View mainView = window.getDefaultView();

    const sf::Color background(20, 5, 35);
    sf::Color color = background;

    sf::Font font;
    sf::Text text;
    font.loadFromFile("C:/Windows/Fonts/Calibri.ttf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    Space v;
    v.addBody(Body({ 300,300 }, Vector2f(0.2, 0), Vector2f(0, 0), 15000, 1, sf::Color::Green));
    v.addBody(Body({ 300,300 - 110 - 20 }, Vector2f(7, 0), Vector2f(0, 0), 70, 1, sf::Color::Cyan));
    v.addBody(Body({ 300,300 - 110 }, Vector2f(7, 0), Vector2f(0, 0), 100, 1, sf::Color::Blue));
    v.addBody(Body({ 300,300 - 110 + 7 }, Vector2f(10.2, 0), Vector2f(0, 0), 15, 1, sf::Color::Green));

    Thread t(splash, std::ref(color));

    Thread calc([&v, &window, &speed, &running]() {
        while (window.isOpen()) {
            if (running) {
                v.calculateForces();
                v.update();
                sleep(milliseconds(10 / speed));
            }
        }
        });

    calc.launch();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {

            case sf::Event::Closed: {
                window.close();
            } break;

            case sf::Event::MouseButtonPressed: {
                if (event.key.code == Mouse::Left) {
                    oldpos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    moving = true;
                    follow = false;
                }
                else if (event.key.code == Mouse::Right) {
                    t.terminate();
                    t.launch();
                    color = background;
                }
            } break;

            case sf::Event::MouseButtonReleased: {
                if (event.key.code == Mouse::Left) {
                    moving = false;
                }
            } break;

            case sf::Event::MouseMoved: {
                if (!moving) break;
                sf::Vector2f newpos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                sf::Vector2f dpos = oldpos - newpos;
                mainView.setCenter(mainView.getCenter() + dpos);
                window.setView(mainView);
                oldpos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            } break;

            case sf::Event::MouseWheelScrolled: {
                if (event.mouseWheelScroll.delta <= -1)
                    zoom += .1f;
                else if (event.mouseWheelScroll.delta >= 1)
                    zoom = std::max(0.1f, zoom - .1f);
                mainView.setSize(window.getDefaultView().getSize());
                mainView.zoom(zoom);
                window.setView(mainView);
            } break;

            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::Space) {
                    running = !running;
                }
                else if (event.key.code >= sf::Keyboard::Numpad0 && event.key.code <= sf::Keyboard::Numpad9) {
                    if (event.key.code - sf::Keyboard::Numpad0 < v.size()) {
                        followed = event.key.code - sf::Keyboard::Numpad0;
                        follow = true;
                    }

                }
                else if (event.key.code == sf::Keyboard::Add) {
                    speed *= 2;
                }
                else if (event.key.code == sf::Keyboard::Subtract) {
                    speed /= 2;
                }

            } break;

            }
        }

        if (follow) {
            Body b = v[followed];
            mainView.setCenter(b.getPosition() + b.getRadius());
        }

        std::string status =
            (follow ? "following" : "not following") +
            std::string("\nfollowed body: ") +
            std::to_string(followed) +
            std::string("\ncoords: ") +
            std::to_string(v[followed].getPosition().x) + "\t" + std::to_string(v[followed].getPosition().y) +
            std::string("\nvelocity: ") +
            std::to_string(getLength(v[followed].getVelocity())) +
            std::string("\nsim speed: ") +
            std::to_string(speed);

        text.setString(status);

        window.clear(color);
        window.setView(mainView);
        v.printAll(&window);
        window.setView(window.getDefaultView());

        if (follow) {
            sf::Vertex velocity[]{
                sf::Vertex(window.getDefaultView().getCenter()),
                sf::Vertex(window.getDefaultView().getCenter() + v[followed].getVelocity() * 10.f)
            };
            sf::Vertex acceleration[]{
                sf::Vertex(window.getDefaultView().getCenter()),
                sf::Vertex(window.getDefaultView().getCenter() + v[followed].getAcceleration() * 100.f)
            };

            window.draw(velocity, 2, sf::Lines);
            window.draw(acceleration, 2, sf::Lines);
        }

        window.draw(text);

        window.display();
        window.setView(mainView);

    }

    return 0;
}