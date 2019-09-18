#include "StatusModel.hpp"
StatusModel::StatusModel(int hp, int ammo) : hp(hp), ammo(ammo) {}
void StatusModel::setHP(const int hp) { this->hp = hp; }
int StatusModel::getHP() const { return hp; }
void StatusModel::setAmmo(const int ammo) { this->ammo = ammo; }
int StatusModel::getAmmo() const { return ammo; }