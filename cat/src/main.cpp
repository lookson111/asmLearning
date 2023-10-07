#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

int main()
{
    float theta = 0.0f;
    // create the window
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

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
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.0f, 0.0f, 0.2f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // draw...
        glPushMatrix();
        glRotatef(theta, 1.0f, 0.0f, 1.0f);

        glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(1.0f, 1.0f);

        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(1.0f, 1.0f);

        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, 0.0f, 1.0f);

        glEnd();

        glPopMatrix();
        theta += 1.0f;

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}