#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

void ShowObj(float scale, float transX, float transY) {
    glLoadIdentity();           // восстанавливаем систему координат
    glScalef(scale, scale, 1);
    glTranslatef(transX, transY, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 0); glVertex2f(0, 0);
    glColor3f(1, 0, 0); glVertex2f(0.2, 0);
    glColor3f(0, 0, 1); glVertex2f(0.2, 0.2);
    glColor3f(0, 0, 1); glVertex2f(0.1, 0.3);
    glColor3f(0, 1, 0); glVertex2f(0, 0.2);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 1); glVertex2f(0.05, 0.05);
    glColor3f(1, 1, 1); glVertex2f(0.05, 0.15);
    glColor3f(1, 1, 1); glVertex2f(0.15, 0.15);
    glColor3f(1, 1, 1); glVertex2f(0.15, 0.05);
    glEnd();
}
void SunObj() {
    static float theta = 0.0f;
    glLoadIdentity();           // восстанавливаем систему координат
    glTranslatef(0, -0.7, 0);   // опускаем систему координат по оси y
    glRotatef(theta, 0.0f, 0.0f, 1.0f); // поворачиваем систему координат на угол
    glTranslatef(-1.5, 0, 0);    // смещаем по оси x
    glPushMatrix();
    float x, y;
    float cnt = 50;
    float l = 0.1;
    float a = M_PI * 2 / cnt;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.97, 0.8, 0);
    glVertex2f(0, 0);
    for (int i = -1; i < cnt; i++) {
        x = sin(a * i) * l;
        y = cos(a * i) * l;
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
    theta += -0.3f;
}

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
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);
        ShowObj(0.5, 0.2, 0);
        ShowObj(1, -0.2, 0);
        SunObj();
        
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}