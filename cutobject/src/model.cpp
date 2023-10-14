#include "model.h"
#include "static_data.h"

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
    auto& dAxes = static_data::Data::dAxes;
    //1
    plain_.push_back(0.0f);
    plain_.push_back(dAxes);
    plain_.push_back((-D_ - B_ * dAxes) / C_);
    //6
    plain_.push_back(-dAxes);
    plain_.push_back((-D_ + A_ * dAxes) / B_);
    plain_.push_back(0.0f);
    //2
    plain_.push_back((-D_ - C_ * dAxes) / A_);
    plain_.push_back(0.0f);
    plain_.push_back(dAxes);
    //4
    plain_.push_back(0.0f);
    plain_.push_back(-dAxes);
    plain_.push_back((-D_ + B_ * dAxes) / C_);
    //3
    plain_.push_back(dAxes);
    plain_.push_back((-D_ - A_ * dAxes) / B_);
    plain_.push_back(0.0f);
    //5
    plain_.push_back((-D_ + C_ * dAxes) / A_);
    plain_.push_back(0.0f);
    plain_.push_back(-dAxes);
}

Axes::Axes()
{
    colors_ = { {255,222,0}, {0,255,0}, {0,0,255} };
    rot_ = { {0,0,0}, {0,1,0}, {1,0,0} };
    vertex_ = static_data::Data::axes.data();
    edges_ind_ = static_data::Data::axesInd.data();
}


} // namespace load