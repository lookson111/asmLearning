#include "view.h"

#include <iostream>
#include "GL/glu.h"

#include "static_data.h"

namespace view {
void ObjectsView::AxesShow()
{
    glVertexPointer(3, GL_FLOAT, 0, axes_.GetVertex());
    glEnableClientState(GL_VERTEX_ARRAY);
    auto& r = axes_.GetRotation();
    auto& c = axes_.GetColors();
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        if (i) // fix in windows
            glRotatef(90, r[i].x, r[i].y, r[i].z);
        glColor3ub(c[i].r, c[i].g, c[i].b);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, axes_.GetEdgesInd());
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}
void ObjectsView::PolygonMashShow()
{
}
void ObjectsView::PlainShow()
{
    auto& vertex = plain_.GetVertex();
    glEnable(GL_BLEND);
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3ub(54, 100, 234);
        glVertexPointer(3, GL_FLOAT, 0, vertex.data());
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)vertex.size() / 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glDisable(GL_BLEND);
}
void ObjectsView::ShowAll()
{
    if (plain_on_)
        PlainShow();
    PolygonMashShow();
    AxesShow();
    GluTest();
    ObjectShow();
}
void ObjectsView::PlainOnOff()
{
    plain_on_ = !plain_on_;
}


void ObjectsView::ObjectShow()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, static_data::Data::kube.data());
    for (int i = 0; i < 1/*objectcnt*/; i++)
    {
        glPushMatrix();
        glColor3ub(244, 60, 43);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, static_data::Data::kubeInd.data());
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

static void tessErrorCB(GLenum errorCode)
{
    const GLubyte* errorStr;

    errorStr = gluErrorString(errorCode);
    std::cout << "[ERROR]: " << errorStr << std::endl;
}

void ObjectsView::GluTest()
{
    auto rect = static_data::Data::rect.data();
    auto quad1 = static_data::Data::quad1.data();
    //GLuint id = glGenLists(1);  // create a display list
    glPushMatrix();
    {
        GLUtesselator* tobj = gluNewTess();
        gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid(*) ()) & glVertex3dv);
        gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid(*) ()) & glBegin);
        gluTessCallback(tobj, GLU_TESS_ERROR, (GLvoid(*)())tessErrorCB);
        gluTessCallback(tobj, GLU_TESS_END, glEnd);

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
        glColor3f(1, 1, 1);
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


} // namespace view