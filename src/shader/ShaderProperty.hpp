#pragma once
#ifndef SHADER_SHADERPROPERTY_HPP
#define SHADER_SHADERPROPERTY_HPP
#include <string>
namespace planet {

class ShaderProperty {
       public:
        explicit ShaderProperty(const std::string& name, bool enabled);
        void enable();
        void disable();

        std::string name;
        bool enabled;

       private:
};
}  // namespace planet
#endif