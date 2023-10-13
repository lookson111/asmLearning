#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include "camera.h"
#include "model.h"
#include "view.h"
#include "load_data.h"
#include "GL/glu.h"

using namespace std::literals;


static int width = 1200;
static int height = 600;


float kube[] = { 0,0,0, 0,1,0, 1,1,0, 1,0,0, 0,0,1, 0,1,1, 1,1,1, 1,0,1 }; // массив вершин 
GLuint kubeInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4, 3,2,5, 6,7,3, 0,1,5, 5,4,0, // массив граней
                    1,2,6, 6,5,1, 0,3,7, 7,4,0};


void ResizeWindow(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    float k = (float)w / (float)h;
    float sz = 0.2f;
    glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 10000.0f);
}

void ObjectShow()
{
    glColor3ui(23, 142, 232);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, kube);
    glPushMatrix();
    {
        glColor3ui(234, 142, 13);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, kubeInd);
    }
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
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


void SpaceInit()
{
    glEnable(GL_DEPTH_TEST);
    ResizeWindow(width, height);
}
GLdouble rect[][3] = {5.0, 5.0, 0.0,
                       20.0, 5.0, 0.0,
                       20.0, 20.0, 0.0,
                       5.0, 20.0, 0.0,
                       6.0,10.0, 0.0,
                       7.5, 7.5, 0.0,
                       17.5, 7.5, 0.0,
                       10.0, 17.5, 0.0};
GLdouble quad1[][3] = { 
    -20,30,0, 
    -10,0,0, 
    0,30,0, 
    -10,20,0 };
void tessErrorCB(GLenum errorCode)
{
    const GLubyte *errorStr;

    errorStr = gluErrorString(errorCode);
    std::cout << "[ERROR]: " << errorStr << std::endl;
}
void GluTest() 
{
    //GLuint id = glGenLists(1);  // create a display list
    glPushMatrix();
    {            	
        GLUtesselator *tobj = gluNewTess();
        gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid (*) ( )) &glVertex3dv);
        gluTessCallback(tobj, GLU_TESS_BEGIN,  (GLvoid (*) ( )) &glBegin);
        gluTessCallback(tobj, GLU_TESS_ERROR,  (GLvoid (*)())tessErrorCB);
        gluTessCallback(tobj, GLU_TESS_END,    glEnd);
        
        gluTessBeginPolygon(tobj, NULL); 
        gluTessBeginContour(tobj); 
            gluTessVertex(tobj, rect[0], rect[0]); 
            gluTessVertex(tobj, rect[1], rect[1]); 
            gluTessVertex(tobj, rect[2], rect[2]); 
            gluTessVertex(tobj, rect[3], rect[3]);
            gluTessVertex(tobj, rect[4], rect[4]);
        gluTessEndContour(tobj); 
        gluTessBeginContour(tobj); 
            gluTessVertex(tobj, rect[5], rect[5]); 
            gluTessVertex(tobj, rect[6], rect[6]); 
            gluTessVertex(tobj, rect[7], rect[7]); 
        gluTessEndContour(tobj); 
        gluTessEndPolygon(tobj);
        //glNewList(id, GL_COMPILE);
        glColor3f(1,1,1);
        gluTessBeginPolygon(tobj, NULL);                   // with NULL data
          gluTessBeginContour(tobj);
            gluTessVertex(tobj, quad1[0], quad1[0]);
            gluTessVertex(tobj, quad1[1], quad1[1]);
            gluTessVertex(tobj, quad1[2], quad1[2]);
            gluTessVertex(tobj, quad1[3], quad1[3]);
          gluTessEndContour(tobj);
        gluTessEndPolygon(tobj);
        //glEndList();
        
        gluDeleteTess(tobj);     
           
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
    Camera camera(window);
    auto [plain, polygon] = load::LoadData("");
    model::Axes axes;
    view::ObjectsView viewer(axes, polygon, plain);


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
                if (event.key.code == sf::Keyboard::E)
                    viewer.PlainOnOff();
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
        glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Move();
        glPushMatrix();
        {
            camera.Apply();
            viewer.ShowAll();
            GluTest();
            ObjectShow();
        }
        glPopMatrix();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
