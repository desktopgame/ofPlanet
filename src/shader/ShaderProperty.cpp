#include "ShaderProperty.hpp"
namespace planet {

ShaderProperty::ShaderProperty(const std::string& name, bool enabled)
    : name(name), enabled(enabled) {}

void ShaderProperty::enable() { this->enabled = true; }

void ShaderProperty::disable() { this->enabled = false; }
}  // namespace planet
