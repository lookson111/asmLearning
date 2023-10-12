#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

namespace model {

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

class Vertex {
private:
	std::vector<float> pointXYZ_;

public:
	void Push(float x, float y, float z);
	const std::vector<float>& Get();
};
using Edge = std::vector<int>;

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
	void Begin();
	const Edge& Next();
	void DeleteCurrent();
	void End();
};

class Plain {
	float A_, B_, C_, D_;
public:
	Plain(float A, float B, float C, float D);
	void Show();

};

class Axes {
public:
	static void Show();
};

} // namespace model