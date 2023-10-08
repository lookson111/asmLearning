#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

POINTFLOAT* mas = nullptr;
int cnt;
float scaleY = 1;
float curX;

void DrawOs(float alfa) {
    static float d = 0.05;
    glPushMatrix();
    glRotatef(alfa, 0, 0, 1);
    {
        glBegin(GL_LINES);
        glVertex2f(-1, 0);
        glVertex2f(1, 0);
        glVertex2f(1, 0);
        glVertex2f(1 - d, 0 + d);
        glVertex2f(1, 0);
        glVertex2f(1 - d, 0 - d);
        glEnd();
    }
    glPopMatrix();
}

void Init(float start, float finish, int count)
{
    cnt = count;
    mas = new POINTFLOAT[cnt];
    float dx = (finish - start) / (cnt - 1);
    for (int i = 0; i < cnt; i++) 
    {
        mas[i].x = start;
        mas[i].y = sin(start);
        start += dx;
    }
}
void Show()
{
    float sx = 2.0 / (mas[cnt - 1].x - mas[0].x);
    float dx = (mas[cnt - 1].x + mas[0].x) * 0.5;
    glPushMatrix();
    glScalef(sx, scaleY, 1);
    glTranslatef(-dx, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < cnt; i++) 
        glVertex2f(mas[i].x, mas[i].y);
    glEnd();
    glPopMatrix();
}

void Add(float x, float y) 
{
    for (int i = 1; i < cnt; i++)
        mas[i - 1] = mas[i];
    mas[cnt - 1].x = x;
    mas[cnt - 1].y = y;
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
    curX = 10;
    Init(10, curX, 100);
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
                if ((int)event.mouseWheelScroll.delta > 0)
                    scaleY *= 1.5;
                else
                    scaleY *= 0.7;
                if (scaleY < 0.02)
                    scaleY = 0.02;
                break;
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glLineWidth(2);
        glColor3f(1, 0, 0);
        DrawOs(0);
        glColor3f(0, 1, 0);
        DrawOs(90);

        curX += 0.1;
        Add(curX, sin(curX));
        glColor3f(0, 1, 1);
        Show();
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}