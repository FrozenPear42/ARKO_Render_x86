#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Shape.hpp>

#define LOG(MSG) std::cout << #MSG << '\n';

uint8_t pixels[512 * 512 * 4] = {0};

float rotationMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
};

float translationMatrix[16] = {
        20, 0, 0, 100,
        0, 20, 0, 100,
        0, 0, 20, 100,
        0, 0, 0, 1,
};

float projectionMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
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

float rotation[3] = {0};
float position[3] = {0};
const float rotationSpeed = 0.1;
const float movementSpeed = 0.1;

extern "C" {
void multiplyMat4(float* result, float* A, float* B);
void multiplyMatVec4(float* result, float* A, float* V);
}

void rotate(float x, float y, float z);

void move(float x, float y, float z);

void rotate(float x, float y, float z) {
    rotation[0] += x * rotationSpeed;
    rotation[1] += y * rotationSpeed;
    rotation[2] += z * rotationSpeed;
    std::cout << rotation[0] << " " << rotation[1] << " " << rotation[2] << '\n';
}

void move(float x, float y, float z) {
    position[0] += x * movementSpeed;
    position[1] += y * movementSpeed;
    position[2] += z * movementSpeed;
    std::cout << position[0] << " " << position[1] << " " << position[2] << '\n';
}

void render() {
    multiplyMat4(MVPMatrix, rotationMatrix, translationMatrix);
    multiplyMat4(MVPMatrix, projectionMatrix, MVPMatrix);
    for (int i = 0; i < 8; i++)
        multiplyMatVec4(vertsResult[i], MVPMatrix, verts[i]);
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

int main() {
    sf::Image image;
    sf::RenderWindow window(sf::VideoMode(512, 512), sf::String('a'));

    printMatrix(MVPMatrix);
    printMatrix(rotationMatrix);
    printMatrix(translationMatrix);

    multiplyMat4(MVPMatrix, rotationMatrix, translationMatrix);

    printMatrix(MVPMatrix);
    printMatrix(rotationMatrix);
    printMatrix(translationMatrix);

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

        window.clear(sf::Color(0x33, 0x44, 0xAA));

        render();
        for (int i = 0; i < 12; i++) {
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(vertsResult[edges[i][0]][0], vertsResult[edges[i][0]][1]), sf::Color::White),
                    sf::Vertex(sf::Vector2f(vertsResult[edges[i][1]][0], vertsResult[edges[i][1]][1]), sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }
        //image.create(512, 512, pixels);
        window.display();
    }

    return 0;
}