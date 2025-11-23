#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "List.hpp"

#include <cstring>
#include <stdexcept>

template<typename T>
class Array final : public IList<T> {
public:
        Array();
        ~Array();

        int  size() const override;
        T    get(const int POS) const override;
        void set(const int POS, const T VALUE) override;
        void insert(const int POS, const T VALUE) override;
        void remove(const int POS) override;
        T    min() const override;
        T    max() const override;
        int  find(const T VALUE) const override;
        int rfind(const T VALUE) const override;

private:
        int _size;
        T* _pArray;
};

//---------------------------------------------------------

template<typename T>
Array<T>::Array() {
        // set size to zero
        // set array to be a nullptr
        _size = 0;
        _pArray = 0;
}

template<typename T>
Array<T>::~Array() {
        // deallocate current array
        if (_pArray)
                delete[] _pArray;
        _size = 0;
}

template<typename T>
int Array<T>::size() const {
        return _size;
}

template<typename T>
T Array<T>::get(const int POS) const {
        // if POS is out of range, throw std::out_of_range exception
        if (POS < 0 || POS >= _size)
                throw std::out_of_range("Array::get: POS out of range");

        // return value at POS within array
        return _pArray[POS];
}

template<typename T>
void Array<T>::set(const int POS, const T VALUE) {
        // if POS is out of range, throw std::out_of_range exception
        if (POS < 0 || POS >= _size)
                throw std::out_of_range("Array::set: POS out of range");

        // set VALUE at POS within array
        _pArray[POS] = VALUE;
}

template<typename T>
void Array<T>::insert(const int POS, const T VALUE) {
        // if POS is before zero, clamp to zero
        // if POS is after size, clamp to size
        int bound_pos = 0;
        if (POS < 0)
                bound_pos = 0;
        else if (POS > _size)
                bound_pos = _size;
        else
                bound_pos = POS;

        // create new array of size + 1
        // copy elements 0 to POS from old array to new array
        // set element at POS in new array to VALUE
        // copy elements POS to size from old array to new array
        T *p_new_array = new T[_size + 1];
        if (_pArray) {
                // prevent copying 0 objects from nullptr (when _size == 0)
                // somehow, this is only an issue when testing linked lists
                std::memcpy(p_new_array, _pArray, bound_pos * sizeof(T));
                p_new_array[bound_pos] = VALUE;
                T *p_new_after = p_new_array + bound_pos + 1;
                T *p_old_after = _pArray + bound_pos;
                size_t num_bytes = (_size - bound_pos) * sizeof(T);
                std::memcpy(p_new_after, p_old_after, num_bytes);
        } else {
                p_new_array[bound_pos] = VALUE;
        }

        // delete old array
        // set old array to be new array
        // increment size
        if (_pArray)
                delete[] _pArray;
        _pArray = p_new_array;
        _size++;
}

template<typename T>
void Array<T>::remove(const int POS) {
        // if array is empty, throw std::out_of_range exception
        if (_size == 0 || !_pArray)
                throw std::out_of_range("Array::remove: array empty");

        // if POS is before zero, clamp to zero
        // if POS is after size, clamp to size
        int bound_pos = 0;
        if (POS < 0)
                bound_pos = 0;
        else if (POS >= _size)
                bound_pos = _size - 1;
        else
                bound_pos = POS;

        // create new array of size - 1
        // copy elements from 0 to POS from old array to  new array
        // copy elements from POS+1 to size from old array to new array
        T *p_new_array = new T[_size - 1];
        std::memcpy(p_new_array, _pArray, bound_pos * sizeof(T));
        T *p_new_after = p_new_array + bound_pos;
        T *p_old_after = _pArray + bound_pos + 1;
        size_t num_bytes = (_size - 1 - bound_pos) * sizeof(T);
        std::memcpy(p_new_after, p_old_after, num_bytes);

        // delete old array
        // set old array to be new array
        // decrement size
        if (_pArray)
                delete[] _pArray;
        _pArray = p_new_array;
        _size--;
}

template<typename T>
T Array<T>::min() const {
        // if array is empty, throw std::out_of_range exception
        if (_size == 0 || !_pArray)
                throw std::out_of_range("Array::min: array empty");

        // find minimum value within array
        T abs_min = _pArray[0];
        for (int i = 0; i < _size; ++i) {
                if (_pArray[i] < abs_min)
                        abs_min = _pArray[i];
        }

        // return min value
        return abs_min;
}

template<typename T>
T Array<T>::max() const {
        // if array is empty, throw std::out_of_range exception
        if (_size == 0 || !_pArray)
                throw std::out_of_range("Array::max: array empty");

        // find maximum value within array
        T abs_max = _pArray[0];
        for (int i = 0; i < _size; ++i) {
                if (_pArray[i] > abs_max)
                        abs_max = _pArray[i];
        }

        // return max value
        return abs_max;
}

template<typename T>
int Array<T>::find(const T VALUE) const {
        // search for first occurrence of VALUE
        // return index of first occurrence if found
        for (int i = 0; i < _size; i++) {
                if (_pArray[i] == VALUE)
                        return i;
        }

        // otherwise return -1
        return -1;
}

template<typename T>
int Array<T>::rfind(const T VALUE) const {
        // search for last occurrence of VALUE
        // return index of last occurrence if found
        for (int i = _size - 1; i >= 0; i--) {
                if (_pArray[i] == VALUE)
                        return i;
        }

        // otherwise return -1
        return -1;
}

#endif//ARRAY_HPP
