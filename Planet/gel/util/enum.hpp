#pragma once
#ifndef GEL_UTIL_ENUM_HPP
#define GEL_UTIL_ENUM_HPP

#define DEF_ENUM_OPERATOR_HPP(enumTy)            \
        enumTy operator~(enumTy a);              \
        enumTy operator|(enumTy a, enumTy b);    \
        enumTy operator&(enumTy a, enumTy b);    \
        enumTy operator^(enumTy a, enumTy b);    \
        enumTy& operator|=(enumTy& a, enumTy b); \
        enumTy& operator&=(enumTy& a, enumTy b); \
        enumTy& operator^=(enumTy& a, enumTy b); \
        bool operator!(enumTy e);

#define DEF_ENUM_OPERATOR_CPP(enumTy, valTy)                      \
        enumTy operator~(enumTy a) { return (enumTy) ~(valTy)a; } \
        enumTy operator|(enumTy a, enumTy b) {                    \
                return (enumTy)((valTy)a | (valTy)b);             \
        }                                                         \
        enumTy operator&(enumTy a, enumTy b) {                    \
                return (enumTy)((valTy)a & (valTy)b);             \
        }                                                         \
        enumTy operator^(enumTy a, enumTy b) {                    \
                return (enumTy)((valTy)a ^ (valTy)b);             \
        }                                                         \
        enumTy& operator|=(enumTy& a, enumTy b) {                 \
                return (enumTy&)((valTy&)a |= (valTy)b);          \
        }                                                         \
        enumTy& operator&=(enumTy& a, enumTy b) {                 \
                return (enumTy&)((valTy&)a &= (valTy)b);          \
        }                                                         \
        enumTy& operator^=(enumTy& a, enumTy b) {                 \
                return (enumTy&)((valTy&)a ^= (valTy)b);          \
        }                                                         \
        bool operator!(enumTy e) { return e == static_cast<enumTy>(0); }

#endif