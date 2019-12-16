#pragma once
#ifndef GAME_HPP
#define GAME_HPP
#include "ofApp.h"
namespace planet {

namespace Screen {
extern bool fullScreenMode;

int widthi();
int heighti();

float widthf();
float heightf();

template <typename T>
T centerX(T width);

template <typename T>
T centerY(T height);

template <typename T>
T left(T width);

template <typename T>
T right(T width);

template <typename T>
T top(T height);

template <typename T>
T bottom(T height);

template <typename T>
T divX(T gridNum);

template <typename T>
T divY(T gridNum);

template <typename T>
inline T centerX(T width) {
        return static_cast<T>((widthf() - static_cast<float>(width)) / 2.0f);
}
template <typename T>
inline T centerY(T height) {
        return static_cast<T>((heightf() - static_cast<float>(height)) / 2.0f);
}
template <typename T>
T left(T width) {
        return static_cast<T>(0);
}
template <typename T>
T right(T width) {
        return static_cast<T>(widthf() - static_cast<float>(width));
}
template <typename T>
T top(T height) {
        return static_cast<T>(0);
}
template <typename T>
T bottom(T height) {
        return static_cast<T>(heightf() - static_cast<float>(height));
}
template <typename T>
T divX(T gridNum) {
        return static_cast<T>(widthf() / static_cast<float>(gridNum));
}
template <typename T>
T divY(T gridNum) {
        return static_cast<T>(heightf() / static_cast<float>(gridNum));
}
}  // namespace Screen
}  // namespace planet
#endif