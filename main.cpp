#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

#define LOG(MSG) std::cout << #MSG << '\n';

uint8_t pixels[512 * 512 * 4] = {0};

float rotationMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
};

float translationMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 5,
        0, 0, 0, 1
};

float projectionMatrix[16] = {
        1.000000, 0.000000, 0.000000, 0.0000000,
        0.000000, 1.000000, 0.000000, 0.0000000,
        0.000000, 0.000000, 1.000400, -0.200040,
        0.000000, 0.000000, 1.000000, 0.0000000,
};

float MVPMatrix[16] = {0};

float verts[8][4] = {
        {-1, -1, -1, 1},
        {-1, 1,  -1, 1},
        {1,  1,  -1, 1},
        {1,  -1, -1, 1},
        {-1, -1, 1,  1},
        {-1, 1,  1,  1},
        {1,  1,  1,  1},
        {1,  -1, 1,  1},
};

float vertsResult[8][4] = {0};

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

sf::Color edgesColor[12] = {
        sf::Color(0xFF, 0x55, 0xCC),
        sf::Color(0xFF, 0x55, 0xCC),
        sf::Color(0xFF, 0x55, 0xCC),
        sf::Color(0xFF, 0x55, 0xCC),
        sf::Color(0x55, 0x55, 0xCC),
        sf::Color(0x55, 0x55, 0xCC),
        sf::Color(0x55, 0x55, 0xCC),
        sf::Color(0x55, 0x55, 0xCC),
        sf::Color(0xFF, 0xCC, 0x00),
        sf::Color(0xFF, 0xCC, 0x00),
        sf::Color(0xFF, 0xCC, 0x00),
        sf::Color(0xFF, 0xCC, 0x00),
};

const float width = 512;
const float height = 512;

float rotation[3] = {0, 0, 0};
float position[3] = {0, 0, 5};
const float rotationSpeed = 0.01;
const float movementSpeed = 0.01;

extern "C" {
void multiplyMat4(float* result, float* A, float* B);
void multiplyMatVec4(float* result, float* A, float* V);
void updateRotation(float* matrix, float* rotation);
void updatePosition(float* matrix, float* position);
void normalizeVert(float* V, float width, float height);
}

void rotate(float x, float y, float z);

void move(float x, float y, float z);

void rotate(float x, float y, float z) {
    rotation[0] += x * rotationSpeed;
    rotation[1] += y * rotationSpeed;
    rotation[2] += z * rotationSpeed;
    updateRotation(rotationMatrix, rotation);
}

void move(float x, float y, float z) {
    position[0] += x * movementSpeed;
    position[1] += y * movementSpeed;
    position[2] += z * movementSpeed;
    updatePosition(translationMatrix, position);
}

void printMatrix(float* pMatrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << pMatrix[4 * i + j] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void printVec(float* pVec) {
    for (int j = 0; j < 4; j++) {
        std::cout << pVec[j] << "\t";
    }
    std::cout << "\n";
}

void render() {
    multiplyMat4(MVPMatrix, translationMatrix, rotationMatrix);
    multiplyMat4(MVPMatrix, projectionMatrix, MVPMatrix);
    for (int i = 0; i < 8; i++) {
        multiplyMatVec4(vertsResult[i], MVPMatrix, verts[i]);
        normalizeVert(vertsResult[i], width, height);
    }
}

int main() {

    sf::Image image;
    sf::Text rotationLabel;
    sf::Text positionLabel;
    sf::Font font;
    sf::RenderWindow window(sf::VideoMode((unsigned int) width, (unsigned int) height), sf::String("ARKO_Render_x86"));

    if (!font.loadFromFile("fira.ttf"))
        throw std::runtime_error("Ups, no font...");
    rotationLabel.setCharacterSize(14);
    positionLabel.setCharacterSize(14);
    rotationLabel.setFont(font);
    positionLabel.setFont(font);
    rotationLabel.setColor(sf::Color::White);
    positionLabel.setColor(sf::Color::White);
    rotationLabel.setPosition(0, 470);
    positionLabel.setPosition(0, 490);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            move(0, 1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            move(0, -1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            move(-1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            move(1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
            move(0, 0, 1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
            move(0, 0, -1);
        }

        window.clear(sf::Color(0x11, 0x22, 0x44));

        render();
        for (int i = 0; i < 12; i++) {
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(vertsResult[edges[i][0]][0], vertsResult[edges[i][0]][1]), edgesColor[i]),
                    sf::Vertex(sf::Vector2f(vertsResult[edges[i][1]][0], vertsResult[edges[i][1]][1]), edgesColor[i]),
            };
            window.draw(line, 2, sf::Lines);
        }
        positionLabel.setString(
                "Position: x " + std::to_string(position[0]) + "\ty " + std::to_string(position[1]) + "\tz " +
                std::to_string(position[2]));
        window.draw(positionLabel);
        rotationLabel.setString(
                "Rotation: x " + std::to_string(rotation[0]) + "\ty " + std::to_string(rotation[1]) + "\tz " +
                std::to_string(rotation[2]));
        window.draw(rotationLabel);
        //image.create(512, 512, pixels);
        window.display();
    }

    return 0;
}