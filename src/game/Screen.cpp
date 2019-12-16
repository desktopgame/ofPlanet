#include "Screen.hpp"
namespace planet {

namespace Screen {
extern bool fullScreenMode = false;

int widthi() { return fullScreenMode ? ofGetWidth() : 1280; }

int heighti() { return fullScreenMode ? ofGetHeight() : 720; }

float widthf() { return static_cast<float>(widthi()); }

float heightf() { return static_cast<float>(heighti()); }
}  // namespace Screen
}  // namespace planet
