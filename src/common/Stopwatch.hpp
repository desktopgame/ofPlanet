#pragma once
#ifndef COMMON_STOPWATCH_HPP
#define COMMON_STOPWATCH_HPP
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
namespace planet {

class Stopwatch {
       public:
        explicit Stopwatch(const std::string& name);
        ~Stopwatch();

        void start();
        void stop();

        template <typename T>
        double elapsed() const;

        template <typename T>
        void log() const;

       private:
        template <typename T>
        static std::string unit();

        bool waitStart, waitStop, userCall;
        bool logged;
        std::string name;
        std::chrono::system_clock::time_point a, b;
};
template <typename T>
inline double Stopwatch::elapsed() const {
        return std::chrono::duration_cast<T>(b - a).count();
}

template <typename T>
inline void Stopwatch::log() const {
        double e = elapsed<T>();
        std::stringstream ss;
        ss << "[" << name << "] " << e << unit<T>() << std::endl;
        std::cout << ss.str();
}

template <typename T>
inline std::string Stopwatch::unit() {
        return "";
}

template <>
inline std::string Stopwatch::unit<std::chrono::microseconds>() {
        return "mcs";
}

template <>
inline std::string Stopwatch::unit<std::chrono::milliseconds>() {
        return "ms";
}

template <>
inline std::string Stopwatch::unit<std::chrono::nanoseconds>() {
        return "ns";
}

template <>
inline std::string Stopwatch::unit<std::chrono::seconds>() {
        return "s";
}
}  // namespace planet
#endif
