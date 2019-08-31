#ifndef GEL_GSYSTEM_TIMER_HPP
#define GEL_GSYSTEM_TIMER_HPP
namespace gel {
class Timer {
       public:
        explicit Timer(float length);
        void update();
        void reset();
        float progress() const;
        float progress01() const;
        bool isElapsed() const;

       private:
        float elapsed;
        float length;
};
}  // namespace gel
#endif