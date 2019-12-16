#pragma once
#ifndef INPUT_BUTTONSTATE_HPP
#define INPUT_BUTTONSTATE_HPP
namespace planet {

enum class ButtonState : unsigned int { None, Press, Release, Held };
}
#endif