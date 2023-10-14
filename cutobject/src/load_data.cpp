#include "load_data.h"
#include <fstream>

namespace load {
	
using namespace std::literals;
namespace fs = std::filesystem;

std::optional<std::pair<model::Plain, model::PolygonMash>>
LoadData(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file)
		return std::nullopt;
	model::Plain plain;
	if (std::string str; std::getline(file, str)) {
		std::istringstream sstream(str);
		if (float A, B, C, D; sstream >> A >> B >> C >> D) {
			plain.Set(A, B, C, D);
		}
	}
	model::PolygonMash polygon_mash;
	// vertex read
	if (std::string str; std::getline(file, str)) {
		std::istringstream sstream(str);
		double x, y, z;
		while (sstream >> x >> y >> z) {
			polygon_mash.vertex.Push(x, y, z);
		}
	}
	// edges read
	model::Edge edges;
	for (std::string str; std::getline(file, str);) {
		std::istringstream sstream(str);
		int cnt;
		model::Contour* contour;
		if (sstream >> cnt) {
			if (cnt > 0) {
				if (!edges.outer.empty()) {
					polygon_mash.edges.Push(std::move(edges));
					edges = {};
				}
				edges.outer.reserve(cnt);
				contour = &edges.outer;
			}
			else {
				edges.holes.push_back({});
				edges.holes.back().reserve(-cnt);
				contour = &edges.holes.back();
			}
		}
		unsigned int vertInd;
		while (sstream >> vertInd) {
			contour->push_back(vertInd);
		}
	}
	polygon_mash.edges.Push(std::move(edges)); // insert last edges

	return { {std::move(plain), std::move(polygon_mash)} };
}

} // namespace load