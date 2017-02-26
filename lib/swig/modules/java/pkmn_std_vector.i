/* -----------------------------------------------------------------------------
 * std_vector.i
 *
 * LibPKMN changes: more closely matches java.util.Vector API
 * ----------------------------------------------------------------------------- */

%include <std_common.i>

%{
#include <algorithm>
#include <vector>
#include <stdexcept>
%}

namespace std {

    template<class T> class vector {
      public:
        typedef T value_type;
        typedef const value_type& const_reference;
        vector();
        vector(int n);
        int size() const;
        int capacity() const;
        %rename(ensureCapacity) reserve;
        void reserve(int minCapacity);
        %rename(setSize) resize;
        void resize(int newSize);
        %rename(isEmpty) empty;
        bool empty() const;
        void clear();
        %extend {
            bool add(const_reference e) {
                self->push_back(e);
                return true;
            }
            bool contains(const_reference e) {
                return (std::find(self->begin(), self->end(), e) != self->end());
            }
            T set(int index, T element) throw (std::out_of_range) {
                if(index >= 0 and index <= (int)self->size()) {
                    T ret = (*self)[index];
                    (*self)[index] = element;
                    return ret;
                } else {
                    throw std::out_of_range("vector index out of range");
                }
            }
            T remove(int index) throw (std::out_of_range) {
                if(index >= 0 and index <= (int)self->size()) {
                    T ret = (*self)[index];
                    self->erase(self->begin() + index);
                    return ret;
                } else {
                    throw std::out_of_range("vector index out of range");
                }
            }
            const_reference get(int index) throw (std::out_of_range) {
                if (index >= 0 && index < (int)self->size()) {
                    return (*self)[index];
                } else {
                    throw std::out_of_range("vector index out of range");
                }
            }
            const_reference firstElement() throw (std::out_of_range) {
                if(self->empty()) {
                    throw std::out_of_range("vector index out of range");
                } else {
                    return self->front();
                }
            }
            const_reference lastElement() throw (std::out_of_range) {
                if(self->empty()) {
                    throw std::out_of_range("vector index out of range");
                } else {
                    return self->back();
                }
            }
        }
    };
}
