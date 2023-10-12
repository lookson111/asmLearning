#include "model.h"


static constexpr float dAxes = 100.0f;
static constexpr float wAxes = 0.01f;

float axes[] = {
    0, wAxes,-dAxes, wAxes,-wAxes,-dAxes, -wAxes,-wAxes,-dAxes,
    0, wAxes, dAxes, wAxes,-wAxes, dAxes, -wAxes,-wAxes, dAxes };
GLuint axesInd[] = { 0,3,4, 4,1,0, 1,4,5, 5,2,1, 2,5,3, 3,0,2 };

namespace model {

void Vertex::Push(float x, float y, float z)
{
	pointXYZ_.insert(pointXYZ_.end(), {x, y, z});
}

const std::vector<float>& Vertex::Get()
{
	return pointXYZ_;
}

void Edges::Push(Edge&& edge)
{
	if (started_)
		temp_.push_back(std::move(edge));
	else
		edges_.insert({ counter_++, std::move(edge) });
}

void Edges::Begin()
{
	started_ = true;
	it_edges_ = edges_.begin();
}

const Edge& Edges::Next()
{
	return (it_edges_++)->second;
}

void Edges::DeleteCurrent()
{
	to_delete_.push_back(it_edges_->first);
}

void Edges::End()
{
	started_ = false;
	auto it_temp = temp_.begin();
	for (int i : to_delete_)
	{
		std::swap(edges_[i], *it_temp);
		it_temp = temp_.erase(it_temp);
	}
	to_delete_.clear();
	for (auto& edge : temp_) {
		Push(std::move(edge));
	}
	temp_.clear();
}


Plain::Plain(float A, float B, float C, float D)
	: A_(A), B_(B), C_(C), D_(D)
{
}

void Plain::Show()
{
    std::vector<float> plain;
    //1
    plain.push_back(0.0f);
    plain.push_back(dAxes);
    plain.push_back((-D_ - B_ * dAxes) / C_);
    //6
    plain.push_back(-dAxes);
    plain.push_back((-D_ + A_ * dAxes) / B_);
    plain.push_back(0.0f);
    //2
    plain.push_back((-D_ - C_ * dAxes) / A_);
    plain.push_back(0.0f);
    plain.push_back(dAxes);
    //4
    plain.push_back(0.0f);
    plain.push_back(-dAxes);
    plain.push_back((-D_ + B_ * dAxes) / C_);
    //3
    plain.push_back(dAxes);
    plain.push_back((-D_ - A_ * dAxes) / B_);
    plain.push_back(0.0f);
    //5
    plain.push_back((-D_ + C_ * dAxes) / A_);
    plain.push_back(0.0f);
    plain.push_back(-dAxes);

    glEnable(GL_BLEND);
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3ub(54, 100, 234);
        glVertexPointer(3, GL_FLOAT, 0, plain.data());
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)plain.size() / 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glDisable(GL_BLEND);
}

void Axes::Show()
{
    std::vector<TColor> colors = { {255,222,0}, {0,255,0}, {0,0,255} };
    std::vector<FPoint> rot = { {0,0,0}, {0,1,0}, {1,0,0} };
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

} // namespace load