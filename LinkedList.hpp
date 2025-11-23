#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "List.hpp"

#include <cassert>
#include <iostream> // debug
#include <iomanip> // debug
#include <stdexcept>

template<typename T>
class LinkedList final : public IList<T> {
public:
        LinkedList();
        ~LinkedList();

        int  size() const override;
        T    get(const int POS) const override;
        void set(const int POS, const T VALUE) override;
        void insert(const int POS, const T VALUE) override;
        void remove(const int POS) override;
        T    min() const override;
        T    max() const override;
        int  find(const T VALUE) const override;
        int  rfind(const T VALUE) const override;
        // TODO: remove later
        void check() const;
        void print() const;
private:
        struct Node {
                T value;
                Node* pNext;
                Node* pPrev;
        };

        Node* _pHead;
        Node* _pTail;
        int _size;
};

//---------------------------------------------------------

// note to self: # operator for macros "stringify"
// I love text replace schenanigans
#define my_assert(x) { \
        if (!(x)) { \
                std::cout << "\n" << #x << " failed\n"; \
                std::cout << "size: " << _size << "\n"; \
                this->print(); \
                assert(x); \
        } \
}

template<typename T>
void LinkedList<T>::check() const {
        if (_size == 0) {
                my_assert(!_pHead && !_pTail);
                return;
        }
        my_assert(_pHead && _pTail);
        Node *curr = _pHead;
        for (int i = 0; i < _size; ++i) {
                my_assert(curr);
                if (i > 0) {
                        my_assert(curr->pPrev);
                        my_assert((curr->pPrev)->pNext == curr);
                }
                if (i < _size - 1) {
                        my_assert(curr->pNext);
                        my_assert((curr->pNext)->pPrev == curr);
                }
                curr = curr->pNext;
        }
        my_assert(curr == nullptr);
        return;
}

template<typename T>
void LinkedList<T>::print() const {
        Node *curr = _pHead;
        for (; curr != nullptr; curr = curr->pNext) {
                std::cout << std::setw(14) << std::left;
                std::cout << curr->pPrev;
                std::cout << " <- ";
                std::cout << std::setw(14) << std::left;
                std::cout << curr;
                std::cout << " -> ";
                std::cout << std::setw(14) << std::left;
                std::cout << curr->pNext;
                std::cout << "\n";
        }
        std::cout << "\n";
}

template<typename T>
LinkedList<T>::LinkedList() {
        // set size to zero
        // set head to be a nullptr
        // set tail to be a nullptr
        _size = 0;
        _pHead = nullptr;
        _pTail = nullptr;
}

template<typename T>
LinkedList<T>::~LinkedList() {
        // remove each node in the list
        this->check();
        if (_size == 0) {
                return;
        }

        Node *curr = _pHead;
        for (int i = 0; i < _size; i++) {
                Node *condemned = curr;
                curr = curr->pNext;
                delete condemned;
        }
        _size = 0;
        _pHead = nullptr;
        _pTail = nullptr;
        this->check();
}

template<typename T>
int LinkedList<T>::size() const {
        return _size;
}

template<typename T>
T LinkedList<T>::get(const int POS) const {
        // if POS is out of range, throw std::out_of_range exception
        if (POS < 0 || POS >= _size)
                throw std::out_of_range("LinkedList::get: POS out of range");

        // set current node to head
        // advance current node to POS
        // return value of current node
        Node *curr = _pHead;
        for (int i = 0; i < POS; i++)
                curr = curr->pNext;
        return curr->value;
}

template<typename T>
void LinkedList<T>::set(const int POS, const T VALUE) {
        // if POS is out of range, throw std::out_of_range exception
        if (POS < 0 || POS >= _size)
                throw std::out_of_range("LinkedList::set: POS out of range");

        // set current node to head
        // advance current node to POS
        // set value of current node
        Node *curr = _pHead;
        for (int i = 0; i < POS; i++)
                curr = curr->pNext;
        curr->value = VALUE;
}

template<typename T>
void LinkedList<T>::insert(const int POS, const T VALUE) {
        this->check();
        // if POS is before zero, clamp to zero
        // if POS is after size, clamp to size
        int bound_pos = 0;
        if (POS < 0)
                bound_pos = 0;
        else if (POS > _size)
                bound_pos = _size;
        else
                bound_pos = POS;

        // create pointer to new node
        // set value to be VALUE
        // set previous pointer to nullptr
        // set next pointer to nullptr
        Node *p_new_node = new Node;
        p_new_node->value = VALUE;
        p_new_node->pNext = nullptr;
        p_new_node->pPrev = nullptr;

        if (_size == 0) {
                // if list is currently empty
                // set head and tail to new node
                _pHead = p_new_node;
                _pTail = p_new_node;
        } else if (bound_pos == 0) {
                // otherwise
                // if position is before head
                // set new node next to be head
                // set head previous to be new node
                // set head to be new node
                p_new_node->pNext = _pHead;
                _pHead->pPrev = p_new_node;
                _pHead = p_new_node;
        } else if (bound_pos == _size) {
                // else if position is after tail
                // set new node previous to be tail
                // set tail next to be new node
                // set tail to be new node
                p_new_node->pPrev = _pTail;
                _pTail->pNext = p_new_node;
                _pTail = p_new_node;
        } else {
                // otherwise
                // set current node to head
                // advance current node to pos
                // link new node into current list
                // link current list to new node
                Node *curr = _pHead;
                for (int i = 0; i < bound_pos; i++)
                        curr = curr->pNext;
                Node *before = curr->pPrev;
                p_new_node->pPrev = before;
                p_new_node->pNext = curr;
                curr->pPrev = p_new_node;
                before->pNext = p_new_node;
        }
        // increment size
        _size++;
        this->check();
}

template<typename T>
void LinkedList<T>::remove(const int POS) {
        this->check();
        // if array is empty, throw std::out_of_range exception
        // if POS is before zero, clamp to zero
        // if POS is after size, clamp to size
        if (_size == 0)
                throw std::out_of_range("LinkedList::remove: graph is empty");
        int bound_pos = 0;
        if (POS < 0)
                bound_pos = 0;
        else if (POS >= _size)
                bound_pos = _size - 1;
        else
                bound_pos = POS;

        Node *condemned;
        if (_size == 1) {
                // if list has one element
                // set node to delete to head
                // set head and tail to be nullptr
                condemned = _pHead;
                _pHead = nullptr;
                _pTail = nullptr;
        } else if (bound_pos == 0) {
                // otherwise
                // if deleting head
                // set node to delete to head
                // advance head to next
                // set head previous to nullptr
                condemned = _pHead;
                _pHead = _pHead->pNext;
                _pHead->pPrev = nullptr;
        } else if (bound_pos == _size - 1) {
                // else if deleting tail
                // set node to delete to tail
                // step tail to previous
                // set tail next to nullptr
                condemned = _pTail;
                _pTail = _pTail->pPrev;
                _pTail->pNext = nullptr;
        } else {
                // otherwise
                // set current node to head
                // advance current node to space before position
                // set node to delete to the current node's next node
                // unlink node to delete
                Node *curr = _pHead;
                for (int i = 0; i < bound_pos; i++)
                        curr = curr->pNext;
                condemned = curr;
                Node *before = curr->pPrev;
                Node *after = curr->pNext;
                before->pNext = after;
                after->pPrev = before;
        }

        // delete node
        // decrement size
        if (condemned)
                delete condemned;
        _size--;
        this->check();
}

template<typename T>
T LinkedList<T>::min() const {
        // if list is empty, throw std::out_of_range exception
        if (_size == 0)
                throw std::out_of_range("LinkedList::min: graph is empty");

        // find minimum value within list
        // return min value
        T abs_min = _pHead->value;
        for (Node *curr = _pHead; curr != nullptr; curr = curr->pNext) {
                if (curr->value < abs_min)
                        abs_min = curr->value;
        }
        return abs_min;
}

template<typename T>
T LinkedList<T>::max() const {
        // if list is empty, throw std::out_of_range exception
        if (_size == 0)
                throw std::out_of_range("LinkedList::max: graph is empty");

        // find maxiumum value within list
        // return max value
        T abs_max = _pHead->value;
        for (Node *curr = _pHead; curr != nullptr; curr = curr->pNext) {
                if (curr->value > abs_max)
                        abs_max = curr->value;
        }
        return abs_max;
}

template<typename T>
int LinkedList<T>::find(const T VALUE) const {
        // search for first occurrence of VALUE
        // hint: start at the beginning and go forward

        // return index of first occurrence if found
        Node *curr = _pHead;
        for (int i = 0; i < _size; i++) {
                if (curr->value == VALUE)
                        return i;
                curr = curr->pNext;
        }

        // otherwise return -1
        return -1;
}

template<typename T>
int LinkedList<T>::rfind(const T VALUE) const {
        // search for last occurrence of VALUE
        // hint: start at the end and go backward

        // return index of last occurrence if found
        Node *curr = _pTail;
        for (int i = _size - 1; i >= 0; i--) {
                if (curr->value == VALUE)
                        return i;
                curr = curr->pPrev;
        }

        // otherwise return -1
        return -1;
}

#endif //LINKED_LIST_HPP
