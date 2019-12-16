#pragma once
#ifndef LUAEX_LUA_HPP
#define LUAEX_LUA_HPP
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include "../any/any.hpp"

struct lua_State;

namespace luaex {
using Error = enum {
	E_NIL = 0,
	E_TYPE_MISSING,
	E_SYNTAX,
	E_OOM,
	E_GC,
	E_GENERIC,
	E_FATAL,
};
using Type = enum {
	T_NUMBER = 0,
	T_BOOL,
	T_STRING
};

struct Variant {
	double number;
	bool boolean;
	std::string string;
};
struct Object {
	explicit Object();
	Type type;
	Variant value;
};
using Function = int(struct lua_State*);

struct Interpreter {
public:
	explicit Interpreter();
	~Interpreter();

	Error loadFile(const std::string& file);
	Error loadString(const std::string& str);

	Error call(const std::string& function, std::vector<Object> args, std::vector<Type> returnType, std::vector<Variant>& returnOut);
	void define(const std::string& name, Function fn);

	void setGlobalNumber(const std::string& name, double value);
	void setGlobalInt(const std::string& name, int value);
	void setGlobalBool(const std::string& name, bool value);
	void setGlobalString(const std::string& name, const std::string& value);

	Error getGlobalNumber(const std::string& name, double& outNumber);
	Error getGlobalInt(const std::string& name, int& outInt);
	Error getGlobalBool(const std::string& name, bool& outBool);
	Error getGlobalString(const std::string& name, std::string& outStr);

	std::unordered_map<std::string, Object> getAllVariables() const;

	bool isOccurred() const;
	std::string getLastError() const;
private:
	std::string getErrorString();
	Error checkStatus(int status, int ok);
	Error panic(Error error, const std::string& str);

	struct lua_State* state;
	bool occurred;
	std::string lastError;
};

class Context : public std::enable_shared_from_this<Context> {
public:
	static std::shared_ptr<Context> create();
	static std::shared_ptr<Context> current();
	void dispose();

	void select();
	void put(const std::string& name, linb::any var);
	linb::any get(const std::string& name) const;
private:
	Context();
	bool selected;
	std::unordered_map<std::string, linb::any> map;
	static std::vector<std::shared_ptr<Context> > values;
};

Object create(int i);
Object create(double d);
Object create(bool b);
Object create(const std::string& str);
}
#endif