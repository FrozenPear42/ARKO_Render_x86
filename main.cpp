#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#define LOG(MSG)    std::cout << #MSG << '\n';

sf::Uint8 pixels[512 * 512 * 4] = {0};

double rotationMatrix[16] = {0};

double translationMatrix[16] = {0};

double projectionMatrix[16] = {1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1,
};

double MVPMatrix[16] = {0};

double verts[8][4] = {
        {-1, -1, -1, 1},
        {-1, 1,  -1, 1},
        {1,  1,  -1, 1},
        {1,  -1, -1, 1},
        {-1, -1, 1,  1},
        {-1, 1,  1,  1},
        {1,  1,  1,  1},
        {1,  -1, 1,  1},
};

double vertsResult[8][4] = {0};

uint8_t edges[12][2] = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
};

double rotation[3] = {0};
double position[3] = {0};
const double rotationSpeed = 0.1;
const double movementSpeed = 0.1;

void rotate(double x, double y, double z);

void move(double x, double y, double z);

void multiplyMat4(double* result, double* A, double* B);

void multiplyMatVec4(double* result, double* A, double* V);


void rotate(double x, double y, double z) {
    rotation[0] += x * rotationSpeed;
    rotation[1] += y * rotationSpeed;
    rotation[2] += z * rotationSpeed;
    std::cout << rotation[0] << " " << rotation[1] << " " << rotation[2] << '\n';
}

void move(double x, double y, double z) {
    position[0] += x * movementSpeed;
    position[1] += y * movementSpeed;
    position[2] += z * movementSpeed;
    std::cout << position[0] << " " << position[1] << " " << position[2] << '\n';
}

int main() {
    sf::Image image;
    sf::RenderWindow window(sf::VideoMode(512, 512), sf::String('a'));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /* rotation */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rotate(1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            rotate(-1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rotate(0, -1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rotate(0, 1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            rotate(0, 0, 1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            rotate(0, 0, -1);
        }

        /* position */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
            move(0, 1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            move(0, -1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
            move(-1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
            move(1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
            move(0, 0, 1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            move(0, 0, -1);
        }


        window.clear(sf::Color(0xAA, 0xBB, 0xCC));
//        multiplyMat4(MVPMatrix, rotationMatrix, translationMatrix);
        image.create(512, 512, pixels);
        window.display();

    }

    return 0;
}