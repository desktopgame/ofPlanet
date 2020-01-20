#pragma once
#ifndef COMMON_STATEVALUE_HPP
#define COMMON_STATEVALUE_HPP
/**
 * StateValue は、値の変更のたびに直前の値と比較して変更されたかどうかを記憶するクラスです。
 */
template<typename T>
class StateValue {
public:
	explicit StateValue(T value);
	explicit StateValue();

	/**
	 * 値を上書きします。
	 * @param newValue
	 */
	void set(T newValue);

	/**
	 * 値を返します。
	 * @return
	 */
	T get() const;

	/**
	 * 変更されているかどうかをチェックします。
	 */
	void detect();

	/**
	 * 変更されているなら true を返します。
	 * @return
	 */
	bool testIsChanged();

	/**
	 * 以前の値を返します。
	 * @return
	 */
	T& getOldValue();

	/**
	 * 現在の値を返します。
	 * @return
	 */
	T& getNewValue();
private:
	T oldValue;
	T newValue;
	bool changed;
};

template<typename T>
inline StateValue<T>::StateValue(T value) : oldValue(value), newValue(value), changed(false) {
}

template<typename T>
inline StateValue<T>::StateValue() : oldValue(), newValue(), changed(false) {
}

template<typename T>
inline void StateValue<T>::set(T newValue) {
	T temp = this->newValue;
	this->newValue = newValue;
	if (oldValue != newValue) {
		this->changed = true;
	}
	this->oldValue = temp;
}

template<typename T>
inline T StateValue<T>::get() const {
	return this->newValue;
}

template<typename T>
inline void StateValue<T>::detect() {
	if (oldValue != newValue) {
		this->changed = true;
		this->oldValue = this->newValue;
	}
}

template<typename T>
inline bool StateValue<T>::testIsChanged() {
	bool ret = changed;
	this->changed = false;
	return ret;
}

template<typename T>
inline T& StateValue<T>::getOldValue() {
	return oldValue;
}

template<typename T>
inline T& StateValue<T>::getNewValue() {
	return newValue;
}
#endif