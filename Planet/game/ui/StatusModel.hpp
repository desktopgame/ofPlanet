#pragma once
#ifndef GAME_UI_STATUSMODEL_HPP
#define GAME_UI_STATUSMODEL_HPP

class StatusModel {
       public:
        explicit StatusModel(int hp, int ammo);

        void setHP(const int hp);
        int getHP() const;

        void setAmmo(const int ammo);
        int getAmmo() const;

       private:
        int hp;
        int ammo;
};
#endif  // !GAME_UI_STATUSMODEL_HPP
