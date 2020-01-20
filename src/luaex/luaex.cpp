#include "luaex.hpp"

#include <algorithm>
#include <iostream>

#include "luaimpl.hpp"

namespace luaex {
//
// Context
//
std::vector<std::shared_ptr<Context> > Context::values;
std::shared_ptr<Context> Context::create() {
        auto ret = std::shared_ptr<Context>(new Context());
        ret->selected = Context::values.empty();
        Context::values.emplace_back(ret);
        return ret;
}
std::shared_ptr<Context> Context::current() {
        auto iter = Context::values.begin();
        while (iter != Context::values.end()) {
                auto v = *iter;
                if (v->selected) {
                        return v;
                }
                ++iter;
        }
        return nullptr;
}
void Context::dispose() {
        auto self = shared_from_this();
        auto iter =
            std::remove_if(Context::values.begin(), Context::values.end(),
                           [self](std::shared_ptr<Context> ctx) -> bool {
                                   return self == ctx;
                           });
        Context::values.erase(iter, Context::values.end());
        if (this->selected && !Context::values.empty()) {
                Context::values.at(0)->select();
        }
}
void Context::select() {
        if (this->selected) {
                return;
        }
        auto iter = Context::values.begin();
        while (iter != Context::values.end()) {
                auto v = *iter;
                v->selected = false;
                ++iter;
        }
        this->selected = true;
}
void Context::put(const std::string& name, linb::any var) { map[name] = var; }
linb::any Context::get(const std::string& name) const { return map.at(name); }
Context::Context() : selected(false), map() {}
//
// Interpreter
//
Interpreter::Interpreter() : state(nullptr), occurred(false), lastError() {
        this->state = luaL_newstate();
        luaL_openlibs(state);
}

Interpreter::~Interpreter() { lua_close(state); }

Error Interpreter::loadFile(const std::string& file) {
        int status = luaL_dofile(state, file.c_str());
        return checkStatus(status, LUA_OK);
}

Error Interpreter::loadString(const std::string& str) {
        int status = luaL_loadstring(state, str.c_str());
        if (status == LUA_OK) {
                lua_pcall(state, 0, LUA_MULTRET, 0);
                return E_NIL;
        } else if (status == LUA_ERRSYNTAX) {
                return panic(E_SYNTAX, getErrorString());
        } else if (status == LUA_ERRMEM) {
                return panic(E_OOM, getErrorString());
        } else if (status == LUA_ERRGCMM) {
                return panic(E_GC, getErrorString());
        }
        return panic(E_FATAL, "status is unknown");
}

Error Interpreter::call(const std::string& function, std::vector<Object> args,
                        std::vector<Type> returnType,
                        std::vector<Variant>& returnOut) {
        int type = lua_getglobal(state, function.c_str());
        if (type == LUA_TNIL) {
                return panic(E_GENERIC, "function is not found");
        }
        for (auto& obj : args) {
                if (obj.type == T_NUMBER) {
                        lua_pushnumber(state, obj.value.number);
                } else if (obj.type == T_BOOL) {
                        lua_pushboolean(state, obj.value.boolean);
                } else if (obj.type == T_STRING) {
                        lua_pushstring(state, obj.value.string.c_str());
                }
        }
        int status = lua_pcall(state, static_cast<int>(args.size()),
                               static_cast<int>(returnType.size()), 0);
        int offset = static_cast<int>(returnType.size());
        for (int i = 0; i < static_cast<int>(returnType.size()); i++) {
                Type ty = returnType.at(i);
                Variant v;
                if (ty == T_NUMBER) {
                        v.number = luaL_checknumber(state, -offset);
                        returnOut.emplace_back(v);
                } else if (ty == T_BOOL) {
                        v.boolean = lua_toboolean(state, -offset);
                        returnOut.emplace_back(v);
                } else if (ty == T_STRING) {
                        v.string = luaL_checkstring(state, -offset);
                        returnOut.emplace_back(v);
                }
                offset--;
                lua_pop(state, 1);
        }
        return checkStatus(status, 0);
}

void Interpreter::define(const std::string& name, Function fn) {
        lua_pushcfunction(state, fn);
        lua_setglobal(state, name.c_str());
}

void Interpreter::setGlobalNumber(const std::string& name, double value) {
        lua_pushnumber(state, value);
        lua_setglobal(state, name.c_str());
}

void Interpreter::setGlobalInt(const std::string& name, int value) {
        lua_pushinteger(state, value);
        lua_setglobal(state, name.c_str());
}

void Interpreter::setGlobalBool(const std::string& name, bool value) {
        lua_pushboolean(state, value ? 1 : 0);
        lua_setglobal(state, name.c_str());
}

void Interpreter::setGlobalString(const std::string& name,
                                  const std::string& value) {
        lua_pushstring(state, value.c_str());
        lua_setglobal(state, name.c_str());
}

Error Interpreter::getGlobalNumber(const std::string& name, double& outNumber) {
        int type = lua_getglobal(state, name.c_str());
        if (type != LUA_TNUMBER) {
                return panic(E_TYPE_MISSING, "value is not number");
        }
        outNumber = luaL_checknumber(state, -1);
        lua_pop(state, 1);
        return E_NIL;
}

Error Interpreter::getGlobalInt(const std::string& name, int& outInt) {
        int type = lua_getglobal(state, name.c_str());
        if (type != LUA_TNUMBER) {
                return panic(E_TYPE_MISSING, "value is not number");
        }
        outInt = static_cast<int>(luaL_checkinteger(state, -1));
        lua_pop(state, 1);
        return E_NIL;
}

Error Interpreter::getGlobalBool(const std::string& name, bool& outBool) {
        int type = lua_getglobal(state, name.c_str());
        if (type != LUA_TBOOLEAN) {
                return panic(E_TYPE_MISSING, "value is not boolean");
        }
        outBool = lua_toboolean(state, -1) == 1;
        lua_pop(state, 1);
        return E_NIL;
}

Error Interpreter::getGlobalString(const std::string& name,
                                   std::string& outStr) {
        int type = lua_getglobal(state, name.c_str());
        if (type != LUA_TSTRING) {
                return panic(E_TYPE_MISSING, "value is not string");
        }
        outStr = luaL_checkstring(state, -1);
        lua_pop(state, 1);
        return E_NIL;
}

std::unordered_map<std::string, Object> Interpreter::getAllVariables() const {
        std::unordered_map<std::string, Object> ret;
        lua_pushglobaltable(state);
        lua_pushnil(state);
        while (lua_next(state, -2) != 0) {
                std::string key = lua_tostring(state, -2);
                int type = lua_getglobal(state, key.c_str());
                if (type == LUA_TNUMBER) {
                        ret.insert_or_assign(key,
                                             create(lua_tonumber(state, -1)));
                } else if (type == LUA_TBOOLEAN) {
                        ret.insert_or_assign(
                            key, create(lua_toboolean(state, -1) == 1));
                } else if (type == LUA_TSTRING) {
                        ret.insert_or_assign(
                            key, create(std::string(lua_tostring(state, -1))));
                }
                lua_pop(state, 1);
                lua_pop(state, 1);
        }
        lua_pop(state, 1);
        return ret;
}

bool Interpreter::isOccurred() const { return this->occurred; }

std::string Interpreter::getLastError() const { return this->lastError; }

// private
std::string Interpreter::getErrorString() {
        std::string err = lua_tostring(state, -1);
        lua_pop(state, 1);
        return err;
}

Error Interpreter::checkStatus(int status, int ok) {
        if (status == ok) {
                return E_NIL;
        }
        this->occurred = true;
        this->lastError = getErrorString();
#if _DEBUG
        std::cerr << lastError << std::endl;
#endif
        return E_GENERIC;
}
Error Interpreter::panic(Error error, const std::string& str) {
        this->occurred = true;
        this->lastError = str;
#if _DEBUG
        std::cerr << str << std::endl;
#endif
        return error;
}

Object::Object() {}
//
// Function
//
Object create(int i) {
        Object obj;
        obj.type = T_NUMBER;
        obj.value.number = i;
        return obj;
}
Object create(double d) {
        Object obj;
        obj.type = T_NUMBER;
        obj.value.number = d;
        return obj;
}
Object create(bool b) {
        Object obj;
        obj.type = T_BOOL;
        obj.value.boolean = b;
        return obj;
}
Object create(const std::string& str) {
        Object obj;
        obj.type = T_STRING;
        obj.value.string = str;
        return obj;
}
}  // namespace luaex