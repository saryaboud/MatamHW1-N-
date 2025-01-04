#pragma once

#include <functional>
#include <iostream>
#include <stdexcept>


namespace mtm {
    template<typename T>
    class SortedList {
        struct node {
            T value;
            node *next;

            explicit node(const T &data) : value(data), next(nullptr) {
            }
        };

        node *head;
        int len;

    public:
        class ConstIterator;


        ConstIterator begin() const {
            return ConstIterator(this, 0);
        }

        ConstIterator end() const {
            return ConstIterator(this, len);
        }


        SortedList() : head(nullptr), len(0) {
        }

        ~SortedList() {
            while (head != nullptr) {
                node *tmp = head;
                head = head->next;
                delete tmp;
            }
        }


        SortedList(const SortedList &other) {
            if (other.head == nullptr) {
                head = nullptr;
                len = 0;
                return;
            }
            len = other.len;
            head = new node(other.head->value);
            if (other.head->next != nullptr) {
                node *current = other.head->next;
                node *tmp = head;
                while (current != nullptr) {
                    tmp->next = new node(current->value);
                    tmp = tmp->next;
                    current = current->next;
                }
                tmp->next = nullptr;
            }
        }

        SortedList &operator=(const SortedList &other) {
            if (&other == this) return *this;
            while (head != nullptr) {
                node *tmp = head;
                head = head->next;
                delete tmp;
            }
            head = nullptr;
            len = 0;
            if (other.head == nullptr) {
                return *this;
            }
            len = other.len;
            head = new node(other.head->value);
            if (other.head->next != nullptr) {
                node *current = other.head->next;
                node *tmp = head;
                while (current != nullptr) {
                    tmp->next = new node(current->value);
                    tmp = tmp->next;
                    current = current->next;
                }
                tmp->next = nullptr;
            }
            return *this;
        }

        void insert(T value) {
            if (head == nullptr) {
                head = new node(value);
                head->next = nullptr;
                len++;
                return;
            }
            node *newnode = new node(value);
            if (value > head->value) {
                newnode->next = head;
                head = newnode;
                len++;
                return;
            }
            node *tmp = head;
            node *pre = nullptr;
            while (tmp != nullptr) {
                if (value > tmp->value) {
                    newnode->next = tmp;
                    if (pre != nullptr) {
                        pre->next = newnode;
                    }
                    len++;
                    return;
                }
                pre = tmp;
                tmp = tmp->next;
            }
            pre->next = newnode;
            newnode->next = nullptr;
            len++;
        }

        void remove(const ConstIterator &s) {
            if (s.index == 0) {
                node *tmp = head;
                this->head = this->head->next;
                delete tmp;
                len--;
            }
                int i = 0;
                node *tmp = this->head;
                while (i < s.index - 1) {
                    tmp = tmp->next;
                    i++;
                }
                if (tmp->next == nullptr)return;
                node *current = tmp->next;
                tmp->next = tmp->next->next;
                delete current;
                len--;
        }

        int length() const {
            node *tmp = this->head;
            int length = 0;
            while (tmp != nullptr) {
                length++;
                tmp = tmp->next;
            }
            return length;
        }

        SortedList filter(std::function<bool(T)> func) {
            SortedList newlist;
            node *tmp = this->head;
            while (tmp != nullptr) {
                if (func(tmp->value)) newlist.insert(tmp->value);
                tmp = tmp->next;
            }
            return newlist;
        }

        /*SortedList &filter(std::function<bool(T)> func) {
            SortedList newlist = new SortedList();
            int i=0;
            Node *tmp=head;
            while (i<length) {
                if(func(tmp->data)) {
                    newlist.insert(tmp->data);
                }
                tmp=tmp->next;
                i++;
            }
            return *newlist;
        }*/

        SortedList apply(std::function<T(T)> func) {
            SortedList newlist;
            node *tmp = this->head;
            while (tmp != nullptr) {
                newlist.insert(func(tmp->value));
                tmp = tmp->next;
            }
            return newlist;
        }


        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */
    };

    template<class T>
    class SortedList<T>::ConstIterator {
        const SortedList<T> *s;
        int index;
        friend class SortedList;

        ConstIterator(const SortedList<T> *s, int index) : s(s), index(index) {
        }

    public:
        int getindex() const {
            return index;
        }

        ConstIterator(const ConstIterator &other)
            : s(other.s), index(other.index) {
        }

        ConstIterator &operator=(const ConstIterator &other) {
            if (&other == this) return *this;
            this->s = other.s;
            this->index = other.index;
            return *this;
        }

        ~ConstIterator() = default;

        //operators
        const T &operator*() const {
            if (index >= s->len || index < 0) {
                throw std::out_of_range("out of range"); //test 3 and 6
            }
            int i = 0;
            node *tmp = s->head;
            while (i < index) {
                tmp = tmp->next;
                i++;
            }
            return tmp->value;
        }

        const ConstIterator &operator++() {
            if (index < s->len) {
                index++;
            }
            /*  if(index >= s->len) {
                  throw std::out_of_range("Iterator out of rangee");
              }*/
            return *this;
        }


        bool operator!=(const ConstIterator &other) const {
            if (other.index != index) return false;
            if (other.s != s) return false;
            return true;
        }


        /**
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. a ctor(or ctors) your implementation needs
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~ConstIterator() - destructor
         *
         * operators:
         * 5. operator* - returns the element the iterator points to
         * 6. operator++ - advances the iterator to the next element
         * 7. operator!= - returns true if the iterator points to a different element
         *
         */
    };
}
