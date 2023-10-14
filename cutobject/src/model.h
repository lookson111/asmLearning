#pragma once
#include <vector>
#include <unordered_map>

namespace model {

struct TPoint {
	int x, y;
};

struct FPoint {
	float x, y, z;
};

struct TColor {
	unsigned char r, g, b;
};

class Vertex {
public:
	using VertexType = std::vector<double>;

private:
	VertexType pointXYZ_;

public:
	void Push(double x, double y, double z);
	const VertexType& Get() const;
	VertexType& Get();
};

using Contour = std::vector<unsigned int>;

struct Edge {
	Contour outer; // outer contour
	std::vector<Contour> holes;
};

class Edges {
private:
	int counter_ = 0;
	bool started_ = false;
	std::unordered_map<int, Edge> edges_;
	std::list<Edge> temp_;
	std::list<int> to_delete_;
	using it_edges_type = decltype(edges_.begin());
	it_edges_type it_edges_;

public:
	void Push(Edge&& edge);
	const Edge* Begin();
	const Edge* Next();
	void DeleteCurrent();
	void End();
	const auto& GeEdges() const {
		return edges_;
	}
};

class Plain {
	float A_, B_, C_, D_;
	std::vector<float> plain_;
	void Init();

public:
	Plain(float A, float B, float C, float D);
	Plain();
	void Set(float A, float B, float C, float D);
	const auto& GetVertex() const {
		return plain_;
	}

};

class Axes {
	std::vector<TColor> colors_;
	std::vector<FPoint> rot_;
	const float* vertex_;
	const unsigned int* edges_ind_;

public:
	Axes();
	const auto& GetColors() const {
		return colors_;
	}
	const auto& GetRotation() const {
		return rot_;
	}
	const float* GetVertex() const {
		return vertex_;
	}
	const unsigned int* GetEdgesInd() const {
		return edges_ind_;
	}
};

struct PolygonMash {
	Vertex vertex;
	Edges edges;
};

struct Objects {
	Plain plain;
	PolygonMash polygon_mash;
};

} // namespace model