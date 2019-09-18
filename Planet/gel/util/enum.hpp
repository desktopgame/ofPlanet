#pragma once
#ifndef GEL_UTIL_ENUM_HPP
#define GEL_UTIL_ENUM_HPP

#define DEF_ENUM_OPERATOR_HPP(enumTy) \
enumTy operator~ (enumTy a); \
enumTy operator| (enumTy a, enumTy b); \
enumTy operator& (enumTy a, enumTy b); \
enumTy operator^ (enumTy a, enumTy b); \
enumTy& operator|= (enumTy& a, enumTy b); \
enumTy& operator&= (enumTy& a, enumTy b); \
enumTy& operator^= (enumTy& a, enumTy b);

#define DEF_ENUM_OPERATOR_CPP(enumTy, valTy) \
enumTy operator~ (enumTy a) { return (T)~(valTy)a; } \
enumTy operator| (enumTy a, enumTy b) { return (enumTy)((valTy)a | (valTy)b); } \
enumTy operator& (enumTy a, enumTy b) { return (enumTy)((valTy)a & (valTy)b); } \
enumTy operator^ (enumTy a, enumTy b) { return (enumTy)((valTy)a ^ (valTy)b); } \
enumTy& operator|= (enumTy& a, enumTy b) { return (enumTy&)((valTy&)a |= (valTy)b); } \
enumTy& operator&= (enumTy& a, enumTy b) { return (enumTy&)((valTy&)a &= (valTy)b); } \
enumTy& operator^= (enumTy& a, enumTy b) { return (enumTy&)((valTy&)a ^= (valTy)b); }
#endif