#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

using namespace std::literals;

static constexpr float dAxes = 10.0f;

static int width = 1200;
static int height = 600;

static constexpr GLfloat axesLine[]{
    dAxes, 0, 0,
   -dAxes, 0, 0,
    0, dAxes, 0,
    0,-dAxes, 0,
    0, 0, dAxes,
    0, 0,-dAxes
};
static constexpr GLfloat axesColor[]{
    1, 0, 0,
    1, 0, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 1,
    0, 0, 1
};

float kube[] = { 0,0,0, 0,1,0, 1,1,0, 1,0,0, 0,0,1, 0,1,1, 1,1,1, 1,0,1 }; // массив вершин 
GLuint kubeInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4, 3,2,5, 6,7,3, 0,1,5, 5,4,0, // массив граней
                    1,2,6, 6,5,1, 0,3,7, 7,4,0};

float plainInSegment[] = {-100, -100, -100, 3};

struct TPoint {
    int x, y;
};

struct TColor {
    float r, g, b;
};
struct TCell {
    TColor clr;
};

struct {
    float x, y, z;
    float xRot, zRot;
} camera = { 0,0,1.7, 70,-40 };

struct Object {
    std::vector<float> xyz;
    float x, y, z;
};

void ResizeWindow(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    float k = (float)w / (float)h;
    float sz = 0.2;
    glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 10000);
}


void CameraApply() {
    glRotatef(-camera.xRot, 1, 0, 0);
    glRotatef(-camera.zRot, 0, 0, 1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}

void CameraRotation(float xAngle, float zAngle)
{
    camera.zRot += zAngle;
    if (camera.zRot < 0)
        camera.zRot += 360;
    if (camera.zRot > 360)
        camera.zRot -= 360;
    camera.xRot += xAngle;
    if (camera.xRot < 0)
        camera.xRot = 0;
    if (camera.xRot > 180)
        camera.xRot = 180;
}

void CameraMove(sf::Window& window)
{
    if (!window.hasFocus())
        return;
    static sf::Vector2i base = { width / 2, height / 2 };
    static constexpr float baseSpeed = 0.1;

    float angle = -camera.zRot / 180 * M_PI;
    float speed = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        speed = baseSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        speed = -baseSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        speed = baseSpeed;
        angle -= M_PI * 0.5;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        speed = baseSpeed;
        angle += M_PI * 0.5;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        camera.z += baseSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        camera.z -= baseSpeed;
    if (speed != 0)
    {
        camera.x += sin(angle) * speed;
        camera.y += cos(angle) * speed;
    }
    sf::Vector2i cur = sf::Mouse::getPosition(window);
    CameraRotation((base.y - cur.y) / 15.0, (base.x - cur.x) / 15.0);
    sf::Mouse::setPosition(base, window);
}

void ObjectInit()
{

}

void ObjectShow()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, kube);
    for (int i = 0; i < 1/*objectcnt*/; i++)
    {
        glPushMatrix();
        glColor3ub(244, 60, 43);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, kubeInd);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}


void PlainShow()
{
    auto p = plainInSegment;
    std::vector<float> plain;
    //1
    plain.push_back(0.0f);
    plain.push_back(dAxes);
    plain.push_back((-p[4]-p[1]*dAxes)/p[2]);
    //6
    plain.push_back(-dAxes);
    plain.push_back((-p[4]+ p[0] * dAxes) / p[1]);
    plain.push_back(0.0f);
    //2
    plain.push_back((-p[4] - p[2] * dAxes) / p[0]);
    plain.push_back(0.0f);
    plain.push_back(dAxes);
    //4
    plain.push_back(0.0f);
    plain.push_back(-dAxes);
    plain.push_back((-p[4] + p[1] * dAxes) / p[2]);
    //3
    plain.push_back(dAxes);
    plain.push_back((-p[4] - p[0] * dAxes) / p[1]);
    plain.push_back(0.0f);
    //5
    plain.push_back((-p[4] + p[2] * dAxes) / p[0]);
    plain.push_back(0.0f);
    plain.push_back(-dAxes);

    glEnable(GL_BLEND);
    {
        glBlendFunc(GL_ONE, GL_ONE);
        glColor3ub(54, 76, 234);
        glVertexPointer(3, GL_FLOAT, 0, plain.data());
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glDisable(GL_BLEND);
}

void SpaceInit()
{
    glEnable(GL_DEPTH_TEST);
    PlainShow();
    ObjectInit();
    ResizeWindow(width, height);
}
void CoordinateAxesShow()
{
    glLineWidth(2);
    glVertexPointer(3, GL_FLOAT, 0, &axesLine);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, &axesColor);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_LINES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void SpaceShow()
{
    glClearColor(0.6, 0.8, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    {
        CameraApply();
        CoordinateAxesShow();
        PlainShow();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, kube);
        glPushMatrix();
        {
            glColor3ui(234, 142, 13);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, kubeInd);
        }
        glPopMatrix();
        glDisableClientState(GL_VERTEX_ARRAY);
        ObjectShow();
    }
    glPopMatrix();
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
    window.setMouseCursorGrabbed(true);
    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    SpaceInit();
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
                break;
            }
            case sf::Event::MouseEntered: {
                //window.setMouseCursorVisible(false);
            }
            case sf::Event::MouseMoved: {
                //std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
                //std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
            }
            default:
                break;
            }
        }

        // clear the buffers
        CameraMove(window);
        SpaceShow();


        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
