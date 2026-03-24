#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstring>
#include <stdexcept>

template<typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int length;

    void resize(int new_capacity) {
        T* new_data = new T[new_capacity];
        for (int i = 0; i < length; i++) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    DynamicArray(int initial_capacity = 16) : capacity(initial_capacity), length(0) {
        data = new T[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (length >= capacity) {
            resize(capacity * 2);
        }
        data[length++] = value;
    }

    void pop_back() {
        if (length > 0) {
            length--;
        }
    }

    T& operator[](int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    int size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void clear() {
        length = 0;
    }

    T* begin() {
        return data;
    }

    T* end() {
        return data + length;
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data + length;
    }
};

#endif // DYNAMIC_ARRAY_H