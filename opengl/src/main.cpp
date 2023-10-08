#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

POINTFLOAT vertices[] = {
    {0, 0},
    {1, 0},
    {1, 1},
    {0, 1}
};

float colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    1, 1, 0
};

GLuint index[] = {
    1, 2, 3, 
    3, 0, 1
};

int main()
{
    float theta = 0.0f;
    // create the window
    sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default);
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
            case sf::Event::MouseWheelScrolled:
                break;
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);

        glRotatef(2, 0, 0, 1);
        {
            glVertexPointer(2, GL_FLOAT, 0, &vertices);
            glEnableClientState(GL_VERTEX_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, &colors);
            glEnableClientState(GL_COLOR_ARRAY);

            
            //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &index);


            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
        }
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}