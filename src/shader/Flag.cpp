#include "Flag.hpp"

#include <stdexcept>
namespace planet {
Flag::Flag(bool enabled) : enabled(enabled) {}
void Flag::enable() { this->enabled = true; }
void Flag::disable() { this->enabled = false; }
void Flag::check(bool require, const std::string& message) const {
        if (!(enabled == require)) {
                throw std::logic_error(message);
        }
}
bool Flag::isEnabled() const { return enabled; }
bool Flag::operator*() const noexcept { return enabled; }
}  // namespace planet
