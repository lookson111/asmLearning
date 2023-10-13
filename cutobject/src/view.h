#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "model.h"

namespace view {
using namespace model;
class ObjectsView {
	bool plain_on_ = true;

	const Axes& axes_;
	const PolygonMash& polygon_mash_;
	const Plain& plain_;

	void AxesShow();
	void PolygonMashShow();
	void PlainShow();

public:
	ObjectsView(const Axes& axes, const PolygonMash& polygon_mash, 
		const Plain& plain)
		: axes_(axes)
		, polygon_mash_(polygon_mash)
		, plain_(plain)
	{}
	void ShowAll();
	void PlainOnOff();
};

} // namespace view