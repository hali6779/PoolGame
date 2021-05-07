#pragma once

#include <QVector2D>
#include <limits>
#include <functional>
#include <QMouseEvent>
#include <memory>
#include <type_traits>
#include <iostream>

/* filename of the config file */
constexpr char config_path[] = "/Users/bwb/Desktop/Lecture\ Slides/2018\ S1/INFO3220/3220assignment/assignment3/li-A3/config.json";

constexpr int animFrameMS = 16;
constexpr int drawFrameMS = 16;

constexpr int this_port = 55555;
constexpr int other_port = 33333;

constexpr double DOUBLEINF = std::numeric_limits<double>::max();
