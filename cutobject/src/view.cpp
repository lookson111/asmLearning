#include "view.h"

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
}
void ObjectsView::PlainOnOff()
{
    plain_on_ = !plain_on_;
}
} // namespace view