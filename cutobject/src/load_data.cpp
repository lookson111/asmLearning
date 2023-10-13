#include "load_data.h"

namespace load {
	
using namespace std::literals;
namespace fs = std::filesystem;

std::pair<model::Plain, model::PolygonMash> LoadData(const std::filesystem::path& json_path)
{
	model::Plain plain(1,1,1,5);
	model::PolygonMash polygon_mash;
	
	
	return {std::move(plain), std::move(polygon_mash)};
}

} // namespace load