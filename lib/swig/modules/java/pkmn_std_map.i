/* -----------------------------------------------------------------------------
 * pkmn_std_map.i
 *
 * SWIG typemaps for std::map
 * ----------------------------------------------------------------------------- */

%include <std_common.i>

// ------------------------------------------------------------------------
// std::map
// ------------------------------------------------------------------------

%{
#include <map>
#include <algorithm>
#include <stdexcept>
#include <vector>
%}

// exported class

namespace std {

    template<class K, class T> class map {
        // add typemaps here
      public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef K key_type;
        typedef T mapped_type;
        map();

        int size() const;
        %rename(isEmpty) empty;
        bool empty() const;
        void clear();
        %extend {
            const T& get(const K& key) throw (std::out_of_range) {
                std::map<K,T >::iterator i = self->find(key);
                if (i != self->end())
                    return i->second;
                else
                    throw std::out_of_range("key not found");
            }
            const T& put(const K& key, const T& value) {
                (*self)[key] = value;
                return (*self)[key];
            }
            T remove(const K& key) throw (std::out_of_range) {
                std::map<K,T >::iterator i = self->find(key);
                if (i != self->end()) {
                    T ret = i->second;
                    self->erase(i);
                    return ret;
                } else {
                    throw std::out_of_range("key not found");
                }
            }
            bool containsKey(const K& key) {
                std::map<K,T >::iterator i = self->find(key);
                return i != self->end();
            }
            bool containsValue(const T& value) {
                for(std::map<K,T >::iterator it = self->begin(); it != self->end(); ++it) {
                    if(it->second == value) {
                        return true;
                    }
                }
                return false;
            }
            std::vector<K> keySet() {
                std::vector<K> ret;
                for(std::map<K,T >::iterator it = self->begin(); it != self->end(); ++it) {
                    ret.push_back(it->first);
                }
                return ret;
            }
        }
    };
}
