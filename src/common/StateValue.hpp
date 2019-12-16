#pragma once
#ifndef COMMON_STATEVALUE_HPP
#define COMMON_STATEVALUE_HPP
template<typename T>
class StateValue {
public:
	explicit StateValue(T value);
	explicit StateValue();

	void set(T newValue);
	T get() const;
	void detect();
	bool testIsChanged();

	T& getOldValue();
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