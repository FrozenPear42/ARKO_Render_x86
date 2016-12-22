#include <iostream>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iomanip>

const float width = 512;
const float height = 512;
const float fov = 90;
const float near = 0.1;
const float far = 500;

uint8_t pixels[(int) width * (int) height * 4] = {0};

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


float rotation[3] = {0, 0, 0};
float position[3] = {0, 0, 5};
const float rotationSpeed = 0.05;
const float movementSpeed = 0.005;

extern "C" {
void multiplyMat4(float* result, float* A, float* B);
void multiplyMatVec4(float* result, float* A, float* V);
void updateRotation(float* matrix, float* rotation);
void updatePosition(float* matrix, float* position);
void normalizeVert(float* V, float width, float height);
}

void rotate(float x, float y, float z);

void move(float x, float y, float z);

void setRotation(float x, float y, float z);

void setPosition(float x, float y, float z);


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

void setRotation(float x, float y, float z) {
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
    updateRotation(rotationMatrix, rotation);
}

void setPosition(float x, float y, float z) {
    position[0] = x;
    position[1] = y;
    position[2] = z;
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
    sf::Text verticesLabel;
    sf::Text MVPMatrixLabel;
    sf::Font font;
    sf::RenderWindow window(sf::VideoMode((unsigned int) width, (unsigned int) height), sf::String("ARKO_Render_x86"));

    if (!font.loadFromFile("fira.ttf"))
        throw std::runtime_error("Ups, no font...");

    verticesLabel.setCharacterSize(14);
    verticesLabel.setFont(font);
    verticesLabel.setColor(sf::Color::White);
    verticesLabel.setPosition(0, 0);

    MVPMatrixLabel.setCharacterSize(14);
    MVPMatrixLabel.setFont(font);
    MVPMatrixLabel.setColor(sf::Color::White);
    MVPMatrixLabel.setPosition(0, 160);

    rotationLabel.setCharacterSize(14);
    rotationLabel.setFont(font);
    rotationLabel.setColor(sf::Color::White);
    rotationLabel.setPosition(0, height - 40);

    positionLabel.setCharacterSize(14);
    positionLabel.setFont(font);
    positionLabel.setColor(sf::Color::White);
    positionLabel.setPosition(0, height - 20);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /* rotation */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rotate(-1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            rotate(1, 0, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rotate(0, 1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rotate(0, -1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            rotate(0, 0, -1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            rotate(0, 0, 1);
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
            move(0, 0, 1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
            move(0, 0, -1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            setPosition(0, 0, 5);
            setRotation(0, 0, 0);
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

        std::stringstream s;
        s.precision(4);
        s << std::fixed;
        s << "Position: x:" << std::setw(10) << position[0] << "  y:" << std::setw(10) << position[1] << "  z:"
          << std::setw(10) << position[2];
        positionLabel.setString(s.str());
        s.str("");
        s << "Rotation: x:" << std::setw(10) << rotation[0] << "  y:" << std::setw(10) << rotation[1] << "  z:"
          << std::setw(10) << rotation[2];
        rotationLabel.setString(s.str());
        window.draw(positionLabel);
        window.draw(rotationLabel);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
            s.str("");
            s << "Vertices:\n";
            for (int i = 0; i < 8; ++i)
                s << "x: " << std::setw(10) << vertsResult[i][0] << " y: " << std::setw(10) << vertsResult[i][1]
                  << " z: " << std::setw(10) << vertsResult[i][2] << "\n";
            verticesLabel.setString(s.str());
            window.draw(verticesLabel);
            s.str("");
            s << "MVP Matrix:\n";
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    s << std::setw(8) << MVPMatrix[4 * i + j] << "\t";
                }
                s << "\n";
            }
            s << "\n";
            MVPMatrixLabel.setString(s.str());
            window.draw(MVPMatrixLabel);
        }

        window.display();
    }

    return 0;
}