#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include "camera.h"
#include "GL/glu.h"

using namespace std::literals;

static constexpr float dAxes = 100.0f;
static constexpr float wAxes = 0.01f;

static int width = 1200;
static int height = 600;


float kube[] = { 0,0,0, 0,1,0, 1,1,0, 1,0,0, 0,0,1, 0,1,1, 1,1,1, 1,0,1 }; // массив вершин 
GLuint kubeInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4, 3,2,5, 6,7,3, 0,1,5, 5,4,0, // массив граней
                    1,2,6, 6,5,1, 0,3,7, 7,4,0};
float axes[] = { 
    0, wAxes,-dAxes, wAxes,-wAxes,-dAxes, -wAxes,-wAxes,-dAxes, 
    0, wAxes, dAxes, wAxes,-wAxes, dAxes, -wAxes,-wAxes, dAxes};
GLuint axesInd[] = { 0,3,4, 4,1,0, 1,4,5, 5,2,1, 2,5,3, 3,0,2 }; 

float plainInSegment[] = {-1, -1, -1, 1};

struct TPoint {
    int x, y;
};

struct FPoint {
    float x, y, z;
};

struct TColor {
    GLubyte r, g, b;
};

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
    plain.push_back((-p[3]-p[1]*dAxes)/p[2]);
    //6
    plain.push_back(-dAxes);
    plain.push_back((-p[3]+ p[0] * dAxes) / p[1]);
    plain.push_back(0.0f);
    //2
    plain.push_back((-p[3] - p[2] * dAxes) / p[0]);
    plain.push_back(0.0f);
    plain.push_back(dAxes);
    //4
    plain.push_back(0.0f);
    plain.push_back(-dAxes);
    plain.push_back((-p[3] + p[1] * dAxes) / p[2]);
    //3
    plain.push_back(dAxes);
    plain.push_back((-p[3] - p[0] * dAxes) / p[1]);
    plain.push_back(0.0f);
    //5
    plain.push_back((-p[3] + p[2] * dAxes) / p[0]);
    plain.push_back(0.0f);
    plain.push_back(-dAxes);

    glEnable(GL_BLEND);
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3ub(54, 100, 234);
        glVertexPointer(3, GL_FLOAT, 0, plain.data());
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, plain.size()/3);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glDisable(GL_BLEND);
}

void SpaceInit()
{
    glEnable(GL_DEPTH_TEST);
    ResizeWindow(width, height);
}
void AxesShow()
{
    std::vector<TColor> colors = {{255,222,0}, {0,255,0}, {0,0,255}};
    std::vector<FPoint> rot = {{0,0,0}, {0,1,0}, {1,0,0}};
    glVertexPointer(3, GL_FLOAT, 0, &axes);
    glEnableClientState(GL_VERTEX_ARRAY);
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        if (i) // fix in windows
            glRotatef(90, rot[i].x, rot[i].y, rot[i].z);
        glColor3ub(colors[i].r, colors[i].g, colors[i].b);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, axesInd);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}
GLdouble rect[][3] = {5.0, 5.0, 0.0,
                       20.0, 5.0, 0.0,
                       20.0, 20.0, 0.0,
                       5.0, 20.0, 0.0,
                       10.0,50.0, 0.0,
                       7.5, 7.5, 0.0,
                       17.5, 7.5, 0.0,
                       10.0, 17.5, 0.0};
GLdouble quad1[4][3] = { {-10,30,0}, {0,0,0}, {10,30,0}, {0,20,0} };
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
	GLUquadricObj *quadObj = gluNewQuadric();
    	gluQuadricDrawStyle(quadObj, GLU_FILL);
        gluSphere(quadObj, 0.5, 10, 10);
        
        glTranslated(-2,0,0);
        gluQuadricDrawStyle(quadObj, GLU_LINE);
        gluCylinder(quadObj, 0.5, 0.75, 1, 15, 15);
        gluDeleteQuadric(quadObj);
    	
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
            //gluTessVertex(tobj, rect[4], rect[4]);
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

void SpaceShow(Camera& camera)
{
    glClearColor(0.6, 0.8, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    {
        camera.Apply();
        AxesShow();
        //PlainShow();
        GluTest();
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
    Camera camera(window);
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
        camera.Move();
        SpaceShow(camera);


        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
