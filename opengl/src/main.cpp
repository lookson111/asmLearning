#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace std::literals;

int width = 1200;
int height = 600;

struct TButton {
    std::string_view name;
    float vert[8];
    BOOL hover;
};

float vert[] = {
    0,0,0,
    0,1,0,
    1,0,0
};

std::vector<TButton> btns = {
    {"start"sv, {0,0,  100,0,  100,30,  0,30},  FALSE}, 
    {"stop"sv,  {0,40, 100,40, 100,70,  0,70},  FALSE},
    {"quit"sv,  {0,80, 100,80, 100,110, 0,110}, FALSE}
};

void TButton_Show(const TButton& btn)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    if (btn.hover)
        glColor3f(1, 0, 0);
    else 
        glColor3f(0, 1, 0);
    glVertexPointer(2, GL_FLOAT, 0, btn.vert);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
}

BOOL PointInButton(int x, int y, TButton btn)
{
    return (x > btn.vert[0]) && (x < btn.vert[4]) &&
        (y > btn.vert[1]) && (y < btn.vert[5]);
}

void ShowMenu()
{
    glPushMatrix();
    
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    for(const auto& btn:btns)
        TButton_Show(btn);

    glPopMatrix();
}

void ResizeWindow(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    float k = (float)w / (float)h;
    glOrtho(-k, k, -1, 1, -1, 1);
}

int main()
{
    sf::ContextSettings window_settings;
    window_settings.depthBits = 24; // Request a 24-bit depth buffer
    window_settings.stencilBits = 8;  // Request a 8 bits stencil buffer
    window_settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    // create the window
    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, window_settings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    //glLoadIdentity();
    //glFrustum(-1,1, -1,1, 2,80);
    ResizeWindow(width, height);
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
            case sf::Event::Resized: {
                width = event.size.width;
                height = event.size.height;
                ResizeWindow(width, height);
                break; 
            }
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    running = false;
                break;
            case sf::Event::MouseWheelScrolled:
                break;
            case sf::Event::MouseButtonPressed: {
                for (const auto& btn : btns) {
                    if (PointInButton(event.mouseButton.x, event.mouseButton.y, btn)) 
                    {
                        std::cout << btn.name << std::endl;
                        if (btn.name == "quit"sv)
                            running = false;
                    }
                }
                break;
            }
            case sf::Event::MouseMoved: {
                for (auto& btn : btns)
                    btn.hover = PointInButton(event.mouseMove.x, event.mouseMove.y, btn);
            }
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);


        ShowMenu();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}