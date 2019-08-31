#ifndef GAME_INPUT_MOUSESCROLL_HPP
#define GAME_INPUT_MOUSESCROLL_HPP
class MouseScroll {
       public:
        explicit MouseScroll(double factor);
        void update();
        void reset();
        double getScrollX() const;
        double getScrollY() const;

       private:
        double oldX;
        double oldY;
        double diffX;
        double diffY;
        double factor;
};
#endif