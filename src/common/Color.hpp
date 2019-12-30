#pragma once
#ifndef COMMON_COLOR_HPP
#define COMMON_COLOR_HPP
namespace planet {
struct Color {
		float r, g, b, a;
		Color(float r, float g, float b, float a);
		Color(float r, float g, float b);
};
}
#endif