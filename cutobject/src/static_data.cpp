#include "static_data.h"

namespace static_data {

constexpr float Data::dAxes = 100.0f;
constexpr float Data::wAxes = 0.01f;
constexpr std::array<float, 18> Data::axes = {
    0.0f, wAxes,-dAxes, wAxes,-wAxes,-dAxes, -wAxes,-wAxes,-dAxes,
    0.0f, wAxes, dAxes, wAxes,-wAxes, dAxes, -wAxes,-wAxes, dAxes };

constexpr std::array<unsigned int, 24> Data::axesInd = {
    0,3,4, 4,1,0,
    1,4,5, 5,2,1,
    2,5,3, 3,0,2 };

std::array<double[3], 8> Data::rect = {
    5.0, 5.0, 0.0,
    20.0, 5.0, 0.0,
    20.0, 20.0, 0.0,
    5.0, 20.0, 0.0,
    6.0,10.0, 0.0,
    7.5, 7.5, 0.0,
    17.5, 7.5, 0.0,
    10.0, 17.5, 0.0 };

std::array<double[3], 4> Data::quad1 = {
    -20,30,0,
    -10,0,0,
    0,30,0,
    -10,20,0 };

constexpr std::array<float, 24> Data::kube = {
    0.0f,0,0, 0,1,0,
    1,1,0, 1,0,0,
    0,0,1, 0,1,1,
    1,1,1, 1,0,1 }; // массив вершин 
constexpr std::array<unsigned int, 36> Data::kubeInd = { (unsigned int)
    0,1,2, 2,3,0,
    4,5,6, 6,7,4,
    3,2,5, 6,7,3,
    0,1,5, 5,4,0, // массив граней
    1,2,6, 6,5,1,
    0,3,7, 7,4,0 };

}