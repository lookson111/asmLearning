#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

static constexpr float boxLine = 1;
float boxEdge[] = {
    boxLine, boxLine, 0,
    boxLine,-boxLine, 0,
   -boxLine,-boxLine, 0,
   -boxLine, boxLine, 0
};

float vert[] = {
    1, 1, 0,
    1,-1, 0,
   -1,-1, 0,
   -1, 1, 0
};
float xAlfa = 20;
float zAlfa = 20;
POINTFLOAT pos = {0,0};

void ShowWorld()
{
    glEnableClientState(GL_VERTEX_ARRAY);// указываем opengl рисовать массив вершин
    {
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        for (int i = -5; i < 5; i++)
            for (int j = -5; j < 5; j++) 
            {
                glPushMatrix();
                if ((i + j) % 2 == 0)
                    glColor3f(0, 0.5, 0);
                else
                    glColor3f(1, 1, 1);
                glTranslatef(i * 2, j * 2, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glPopMatrix();
            }
        
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void ShowBox() 
{
    float boxScale = 1.2;
    glEnableClientState(GL_VERTEX_ARRAY);// указываем opengl рисовать массив вершин
    {
        glVertexPointer(3, GL_FLOAT, 0, &boxEdge);
        glColor3f(0.5, 0, 0.1);
        {
            glPushMatrix();
            glTranslatef(0, 0, boxLine / 2);
            glTranslatef(boxLine, 0, 0);
            glRotatef(90, 1, 0, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
        {
            glPushMatrix();
            glTranslatef(0, 0, boxLine / 2);
            glTranslatef(0, boxLine, 0);
            glRotatef(90, 0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
        {
            glPushMatrix();
            glTranslatef(0, 0, boxLine / 2);
            glTranslatef(boxLine * 2, boxLine, 0);
            glRotatef(90, 0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
        {
            glPushMatrix();
            glTranslatef(0, 0, boxLine / 2);
            glTranslatef(boxLine, boxLine * 2, 0);
            glRotatef(90, 1, 0, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
        glColor3f(0.0, 0.5, 0.1);
        {
            glPushMatrix();
            glTranslatef(0, 0, boxLine*1.5);
            glTranslatef(boxLine, boxLine, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}
void MoveCamera()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        xAlfa = ++xAlfa > 180 ? 180 : xAlfa;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        xAlfa = --xAlfa < 0 ? 0 : xAlfa;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        zAlfa = ++zAlfa;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        zAlfa = --zAlfa;
    float angle = -zAlfa / 180 * M_PI;
    float speed = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        speed = 0.1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        speed = -0.1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        speed = 0.1;
        angle -= M_PI * 0.5;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        speed = 0.1;
        angle += M_PI * 0.5;
    }

    if (speed != 0)
    {
        pos.x += sin(angle) * speed;
        pos.y += cos(angle) * speed;
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);
}

int main()
{
    sf::ContextSettings window_settings;
    window_settings.depthBits = 24; // Request a 24-bit depth buffer
    window_settings.stencilBits = 8;  // Request a 8 bits stencil buffer
    window_settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    // create the window
    sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, window_settings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    glLoadIdentity();
    glFrustum(-1,1, -1,1, 2,80);

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            case sf::Event::Resized:
                window.setSize({ static_cast<unsigned int>(event.size.width), static_cast<unsigned int>(event.size.height) });
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    running = false;
                break;
            case sf::Event::MouseWheelScrolled:
                break;
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        {
            MoveCamera();
            ShowWorld();
            ShowBox();
            glTranslatef(3, 0, 0);
            ShowBox();
        }
        glPopMatrix();
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}