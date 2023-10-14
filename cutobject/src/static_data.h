#pragma once

#include <array>

namespace static_data {

struct Data
{
    static const float dAxes;
    static const float wAxes;
    static const std::array<float, 18> axes;
    static const std::array<unsigned int, 24> axesInd;

    static std::array<double[3], 8> rect;
    static std::array<double[3], 4> quad1;

    static const std::array<float, 24> kube; // массив вершин 
    static const std::array<unsigned int, 36> kubeInd;
};

}