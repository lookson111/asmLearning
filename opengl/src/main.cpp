#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace std::literals;

int width = 1200;
int height = 600;

static constexpr int pW = 40;
static constexpr int pH = 40;
static constexpr int enemyCnt = 40;

BOOL showMask = FALSE;

float kube[] = { 0,0,0, 0,1,0, 1,1,0, 1,0,0, 0,0,1, 0,1,1, 1,1,1, 1,0,1 }; // массив вершин 
GLuint kubeInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4, 3,2,5, 6,7,3, 0,1,5, 5,4,0, // массив граней
                    1,2,6, 6,5,1, 0,3,7, 7,4,0};


struct TColor {
    float r, g, b;
};
struct TCell {
    TColor clr;
};

TCell map[pW][pH];

struct {
    float x, y, z;
    float xRot, zRot;
} camera = { 0,0,1.7, 70,-40 };

struct {
    float x, y, z;
    BOOL active;
} enemy[enemyCnt];

void Enemy_Init()
{
    for (int i = 0; i < enemyCnt; i++)
    {
        enemy[i].active = true;
        enemy[i].x = rand() % pW;
        enemy[i].y = rand() % pH;
        enemy[i].z = rand() % 5;
    }
}

void EnemyShow()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, kube);
    for (int i = 0; i < enemyCnt; i++)
    {
        if (!enemy[i].active)
            continue;
        glPushMatrix();
        glTranslatef(enemy[i].x, enemy[i].y, enemy[i].z);
        if (showMask)
            glColor3ub(255-i, 0, 0);
        else
            glColor3ub(244, 60, 43);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, kubeInd);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Camera_Apply() {
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

void PlayerMove(sf::Window* window)
{   
    if (!window->hasFocus())
        return;
    static POINT base = { 400, 300 };
    POINT cur;

    float angle = -camera.zRot / 180 * M_PI;
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
        camera.x += sin(angle) * speed;
        camera.y += cos(angle) * speed;
    }

    GetCursorPos(&cur);
    CameraRotation((base.y - cur.y) / 5.0, (base.x - cur.x) / 5.0);
    SetCursorPos(base.x, base.y);
}

void MapInit()
{
    for (int i = 0; i < pW; i++)
    {
        for (int j = 0; j < pH; j++)
        {
            float dc = (rand() % 20) * 0.01;
            map[i][j].clr.r = 0.31 + dc;
            map[i][j].clr.g = 0.5  + dc;
            map[i][j].clr.b = 0.13 + dc;
        }
    }
}

void ResizeWindow(int w, int h);

void GameMove(sf::Window* window) {
    PlayerMove(window);
}

void GameInit()
{
    glEnable(GL_DEPTH_TEST);

    MapInit();
    Enemy_Init();
    ResizeWindow(width, height);
}

void GameShow()
{
    if (showMask)
        glClearColor(0, 0, 0, 0);
    else
        glClearColor(0.6, 0.8, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    {
        Camera_Apply();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, kube);

        for (int i = 0; i < pW; i++)
        {
            for (int j = 0; j < pH; j++)
            {
                glPushMatrix();
                {
                    glTranslatef(i, j, 0);
                    if (showMask)
                        glColor3f(0, 0, 0);
                    else
                        glColor3f(map[i][j].clr.r, map[i][j].clr.g, map[i][j].clr.b);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, kubeInd);
                }
                glPopMatrix();
            }
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        EnemyShow();
    }
    glPopMatrix();
}

void PlayerShoot(sf::Window* window)
{
    showMask = TRUE;
    GameShow();
    showMask = FALSE;
    RECT rct;
    GLubyte clr[3];
    auto hwnd = window->getSystemHandle();
    GetClientRect(hwnd, &rct);
    glReadPixels(rct.right / 2.0, rct.bottom / 2.0, 1,1,
        GL_RGB, GL_UNSIGNED_BYTE, clr);
    if (clr[0] > 0)
        enemy[255-clr[0]].active = FALSE;
}


void ResizeWindow(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    float k = (float)w / (float)h;
    float sz = 0.1;
    glFrustum(-k*sz,k*sz, -sz,sz, sz * 2, 100);
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
    GameInit();
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
                PlayerShoot(&window);
                break;
            }
            case sf::Event::MouseEntered: {
                ShowCursor(FALSE);

                //PlayerMove(event.mouseMove.x, event.mouseMove.y);
            }
            case sf::Event::MouseMoved: {
                //PlayerMove(event.mouseMove.x, event.mouseMove.y);
            }
            default:
                break;
            }
        }

        // clear the buffers
        GameMove(&window);
        GameShow();


        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}