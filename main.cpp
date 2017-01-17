#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <chrono>

float width = 800;
float height = 800;
float fov = 90;

const float rotationSpeed = 1.0;
const float movementSpeed = 0.2;

float projectionMatrix[16] = {0};
float rotationMatrix[16] = {0};
float translationMatrix[16] = {0};
float MVPMatrix[16] = {0};

float verts[8][4] = {
        {-1, -1, -1, 1},
        {-1, 1,  -1, 1},
        {1,  1,  -1, 1},
        {1,  -1, -1, 1},
        {-1, -1, 1,  1},
        {-1, 1,  1,  1},
        {1,  1,  1,  1},
        {1,  -1, 1,  1},};

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
        {3, 7},};

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
        sf::Color(0xFF, 0xCC, 0x00),};

float vertsResult[8][4] = {0};
float rotation[3] = {0, 0, 0};
float position[3] = {0, 0, 5};

extern "C" {
int render(float* outputVerts, float* verts, int nVecs,
           float* rotation, float* position, int rotationFlag,
           float fov, float width, float height,
           float* rotationMatrix, float* translationMatrix, float* projectionMatrix, float* MVPMatrix
);
}

void rotate(float x, float y, float z);

void move(float x, float y, float z);

void setRotation(float x, float y, float z);

void setPosition(float x, float y, float z);

void rotate(float x, float y, float z) {
    rotation[0] += x * rotationSpeed;
    rotation[1] += y * rotationSpeed;
    rotation[2] += z * rotationSpeed;
}

void move(float x, float y, float z) {
    position[0] += x * movementSpeed;
    position[1] += y * movementSpeed;
    position[2] += z * movementSpeed;
}

void setRotation(float x, float y, float z) {
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
}

void setPosition(float x, float y, float z) {
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

int main() {
    sf::Text fovLabel;
    sf::Text rotationLabel;
    sf::Text positionLabel;
    sf::Text verticesLabel;
    sf::Text rotationMatrixLabel;
    sf::Text MVPMatrixLabel;
    sf::Text FPSLabel;
    sf::Font font;
    std::chrono::time_point<std::chrono::system_clock> last;

    sf::RenderWindow window(sf::VideoMode((unsigned int) width, (unsigned int) height), sf::String("ARKO_Render_x86"));
    window.setFramerateLimit(60);

    if (!font.loadFromFile("fira.ttf"))
        throw std::runtime_error("Ups, no font...");

    verticesLabel.setCharacterSize(14);
    verticesLabel.setFont(font);
    verticesLabel.setColor(sf::Color::White);
    verticesLabel.setPosition(0, 0);

    fovLabel.setCharacterSize(14);
    fovLabel.setFont(font);
    fovLabel.setColor(sf::Color::White);
    fovLabel.setPosition(0, height - 60);

    rotationMatrixLabel.setCharacterSize(14);
    rotationMatrixLabel.setFont(font);
    rotationMatrixLabel.setColor(sf::Color::White);
    rotationMatrixLabel.setPosition(0, 160);

    MVPMatrixLabel.setCharacterSize(14);
    MVPMatrixLabel.setFont(font);
    MVPMatrixLabel.setColor(sf::Color::White);
    MVPMatrixLabel.setPosition(0, 250);

    rotationLabel.setCharacterSize(14);
    rotationLabel.setFont(font);
    rotationLabel.setColor(sf::Color::White);
    rotationLabel.setPosition(0, height - 40);

    positionLabel.setCharacterSize(14);
    positionLabel.setFont(font);
    positionLabel.setColor(sf::Color::White);
    positionLabel.setPosition(0, height - 20);

    FPSLabel.setCharacterSize(14);
    FPSLabel.setFont(font);
    FPSLabel.setColor(sf::Color::White);
    FPSLabel.setPosition(width - 120, height - 20);

    last = std::chrono::system_clock::now();

    setRotation(0, 0, 0);
    setPosition(0, 0, 5);

    while (window.isOpen()) {
        sf::Event event;
        float FPS;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<float> delta = (now - last);
        FPS = 1.0f / delta.count();
        last = now;

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            fov -= 0.5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            fov += 0.5;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            setPosition(0, 0, 5);
            setRotation(0, 0, 0);
            fov = 90;
        }

        window.clear(sf::Color(0x11, 0x22, 0x44));

        render((float*) vertsResult, (float*) verts, 8, rotation, position, 0, fov, width, height,
               rotationMatrix, translationMatrix, projectionMatrix, MVPMatrix);

        for (int i = 0; i < 12; i++) {
            float* v1 = vertsResult[edges[i][0]];
            float* v2 = vertsResult[edges[i][1]];
            if (v1[2] < 0 || v2[2] < 0)
                if (v1[2] < 0 && v2[2] < 0)
                    continue;
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(v1[0], v1[1]), edgesColor[i]),
                    sf::Vertex(sf::Vector2f(v2[0], v2[1]), edgesColor[i]),
            };
            window.draw(line, 2, sf::Lines);
        }

        std::stringstream s;
        s.precision(4);
        s << std::fixed;
        s << "Position: x:" << std::setw(10) << position[0] << "  y:" << std::setw(10) << position[1] << "  z:"
          << std::setw(10) << position[2];
        positionLabel.setString(s.str());
        window.draw(positionLabel);

        s.str("");
        s << "Rotation: x:" << std::setw(10) << rotation[0] << "  y:" << std::setw(10) << rotation[1] << "  z:"
          << std::setw(10) << rotation[2];
        rotationLabel.setString(s.str());
        window.draw(rotationLabel);

        s.str("");
        s << "FOV: " << std::setw(10) << fov;
        fovLabel.setString(s.str());
        window.draw(fovLabel);


        s.str("");
        s << "FPS: " << FPS;
        FPSLabel.setString(s.str());
        window.draw(FPSLabel);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
            s.str("");
            s << "Vertices:\n";
            for (int i = 0; i < 8; ++i)
                s << "x: " << std::setw(10) << vertsResult[i][0] << " y: " << std::setw(10) << vertsResult[i][1]
                  << " z: " << std::setw(10) << vertsResult[i][2] << "\n";
            verticesLabel.setString(s.str());
            window.draw(verticesLabel);

            s.str("");
            s << "Rotation Matrix:\n";
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++)
                    s << std::setw(8) << rotationMatrix[4 * i + j] << "\t";
                s << "\n";
            }
            s << "\n";
            rotationMatrixLabel.setString(s.str());
            window.draw(rotationMatrixLabel);

            s.str("");
            s << "MVP Matrix:\n";
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++)
                    s << std::setw(8) << MVPMatrix[4 * i + j] << "\t";
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