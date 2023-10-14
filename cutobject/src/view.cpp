#include "view.h"

#include <iostream>

#include "static_data.h"

namespace view {


static void tessErrorCB(GLenum errorCode)
{
    const GLubyte* errorStr;

    errorStr = gluErrorString(errorCode);
    std::cout << "[ERROR]: " << errorStr << std::endl;
}

ObjectsView::ObjectsView(const Axes& axes, 
    PolygonMash& polygon_mash, const Plain& plain)
    : axes_(axes)
    , polygon_mash_(polygon_mash)
    , plain_(plain)
{
    tobj = gluNewTess();
    TessInit();
}
ObjectsView::~ObjectsView()
{
    gluDeleteTess(tobj);
}

void ObjectsView::TessInit() {
    gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid(*) ()) & glVertex3dv);
    gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid(*) ()) & glBegin);
    gluTessCallback(tobj, GLU_TESS_ERROR, (GLvoid(*)())tessErrorCB);
    gluTessCallback(tobj, GLU_TESS_END, glEnd);
}

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
    static constexpr int sz_point = 3;
    auto vertex = polygon_mash_.vertex.Get().data();

    glColor3f(50, 50, 1);
    for (auto& [i, edge] : polygon_mash_.edges.GeEdges()) {
        gluTessBeginPolygon(tobj, NULL);
        gluTessBeginContour(tobj);
        for (auto& outer : edge.outer) {
            auto point = &vertex[outer * sz_point];
            auto i1 = *point;
            auto i2 = *(point +1);
            auto i3 = *(point +2);
            gluTessVertex(tobj, point, point);
        }
        gluTessEndContour(tobj);
        for (auto& holes : edge.holes) {
            gluTessBeginContour(tobj);
            for (auto& holeVertex : holes) {
                auto point = &vertex[holeVertex * sz_point];
                gluTessVertex(tobj, point, point);
            }
            gluTessEndContour(tobj);
        }
        gluTessEndPolygon(tobj);
    }
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
    //GluTest();
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

void ObjectsView::GluTest()
{
    auto rect = static_data::Data::rect.data();
    auto quad1 = static_data::Data::quad1.data();
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
    glColor3f(1, 1, 1);
    gluTessBeginPolygon(tobj, NULL);                   // with NULL data
    gluTessBeginContour(tobj);
    gluTessVertex(tobj, quad1[0], quad1[0]);
    gluTessVertex(tobj, quad1[1], quad1[1]);
    gluTessVertex(tobj, quad1[2], quad1[2]);
    gluTessVertex(tobj, quad1[3], quad1[3]);
    gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);

}


} // namespace view