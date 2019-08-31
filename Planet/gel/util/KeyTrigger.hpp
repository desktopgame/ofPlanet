#ifndef GEL_UTIL_KEYTRIGGER_HPP
#define GEL_UTIL_KEYTRIGGER_HPP
namespace gel {
class KeyTrigger {
       public:
        explicit KeyTrigger(const int key);
        void update();
        bool isEnabled() const;

       private:
        int key;
        bool triggered;
        bool enabled;
};
}  // namespace gel
#endif  // !GEL_UTIL_KEYTRIGGER_HPP
