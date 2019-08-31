#ifndef GEL_UTIL_MOUSETRIGGER_HPP
#define GEL_UTIL_MOUSETRIGGER_HPP
#include "../gli.hpp"

namespace gel {
class MouseTrigger {
       public:
        explicit MouseTrigger(const GLenum mode);
        void update();
        bool isEnabled() const;

       private:
        GLenum mode;
        bool triggered;
        bool enabled;
};
}  // namespace gel
#endif