#pragma once
#ifndef LUAEX_LUA_HPP
#define LUAEX_LUA_HPP
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../any/any.hpp"

struct lua_State;

namespace luaex {
/**
 * Error はLUAのエラー情報です。
 */
using Error =
    enum { E_NIL = 0,
           E_TYPE_MISSING,
           E_SYNTAX,
           E_OOM,
           E_GC,
           E_GENERIC,
           E_FATAL,
    };
/**
 * Type はLUAのデータ型です。
 */
using Type = enum { T_NUMBER = 0, T_BOOL, T_STRING };
/**
 * Variant はLUAのデータです。
 */
struct Variant {
        double number;
        bool boolean;
        std::string string;
};
/**
 * Object は型とデータのペアです。
 */
struct Object {
        explicit Object();
        Type type;
        Variant value;
};
using Function = int(struct lua_State*);
/**
 * Interpreter はLUAのラッパーです。
 */
struct Interpreter {
       public:
        explicit Interpreter();
        ~Interpreter();
        /**
         * 指定のファイルをスクリプトとして読み込みます。
         * @param file
         * @return
         */
        Error loadFile(const std::string& file);
        /**
         * 指定の文字列をスクリプトとして読み込みます。
         * @param str
         * @return
         */
        Error loadString(const std::string& str);

        /**
         * 指定の関数を呼び出します。
         * @param function
         * @param args
         * @return Types
         * @return returnOut
         * @return
         */
        Error call(const std::string& function, std::vector<Object> args,
                   std::vector<Type> returnType,
                   std::vector<Variant>& returnOut);

        /**
         * 指定の名前で関数を定義します。
         * @param name
         * @param fn
         */
        void define(const std::string& name, Function fn);

        /**
         * 指定の名前でグローバル変数を定義します。
         * @param name
         * @param value
         */
        void setGlobalNumber(const std::string& name, double value);

        /**
         * 指定の名前でグローバル変数を定義します。
         * @param name
         * @param value
         */
        void setGlobalInt(const std::string& name, int value);

        /**
         * 指定の名前でグローバル変数を定義します。
         * @param name
         * @param value
         */
        void setGlobalBool(const std::string& name, bool value);

        /**
         * 指定の名前でグローバル変数を定義します。
         * @param name
         * @param value
         */
        void setGlobalString(const std::string& name, const std::string& value);

        /**
         * 指定の名前のグローバル変数を返します。
         * @param name
         * @param value
         */
        Error getGlobalNumber(const std::string& name, double& outNumber);

        /**
         * 指定の名前のグローバル変数を返します。
         * @param name
         * @param value
         */
        Error getGlobalInt(const std::string& name, int& outInt);

        /**
         * 指定の名前のグローバル変数を返します。
         * @param name
         * @param value
         */
        Error getGlobalBool(const std::string& name, bool& outBool);

        /**
         * 指定の名前のグローバル変数を返します。
         * @param name
         * @param value
         */
        Error getGlobalString(const std::string& name, std::string& outStr);

        /**
         * 全ての変数を返します。
         * @return
         */
        std::unordered_map<std::string, Object> getAllVariables() const;

        /**
         * エラーが発生しているなら true を返します。
         * @return
         */
        bool isOccurred() const;
        /**
         * 最後に発生したエラーを返します。
         * @return
         */
        std::string getLastError() const;

       private:
        std::string getErrorString();
        Error checkStatus(int status, int ok);
        Error panic(Error error, const std::string& str);

        struct lua_State* state;
        bool occurred;
        std::string lastError;
};
/**
 * Context は、Cの関数からC++の変数にアクセスするためのクラスです。
 * Interpreter の内部で使用されます。
 */
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
}  // namespace luaex
#endif