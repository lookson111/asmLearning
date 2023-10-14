#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "GL/glu.h"
#include "model.h"

namespace view {
using namespace model;

class ObjectsView {
	bool plain_on_ = false;

	const Axes& axes_;
	PolygonMash& polygon_mash_;
	const Plain& plain_;
	GLUtesselator* tobj;

	void TessInit();
	void AxesShow();
	void PolygonMashShow();
	void PlainShow();
	void ObjectShow();
	void GluTest();

public:
	ObjectsView(const Axes& axes, PolygonMash& polygon_mash,
		const Plain& plain);
	~ObjectsView();
	void ShowAll();
	void PlainOnOff();
};

} // namespace view