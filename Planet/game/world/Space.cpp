#include "Space.hpp"
Space::Space()
    : posX(nullptr),
      negX(nullptr),
      posY(nullptr),
      negY(nullptr),
      posZ(nullptr),
      negZ(nullptr) {}

bool Space::isHoled() const {
        return this->posX == nullptr && this->posY == nullptr &&
               this->posZ == nullptr && this->negX == nullptr &&
               this->negY == nullptr && this->negZ == nullptr;
}
bool Space::isFilled() const {
        return this->posX != nullptr && this->posY != nullptr &&
               this->posZ != nullptr && this->negX != nullptr &&
               this->negY != nullptr && this->negZ != nullptr;
}