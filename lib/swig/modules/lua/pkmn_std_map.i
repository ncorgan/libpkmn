/* -----------------------------------------------------------------------------
 * std_map.i
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
        map(const map<K,T> &);

        unsigned int size() const;
        bool empty() const;
        void clear();
        %extend {
            const T& __getitem__(const K& key) throw (std::out_of_range) {
                std::map<K,T >::iterator i = self->find(key);
                if (i != self->end())
                    return i->second;
                else
                    throw std::out_of_range("key not found");
            }
            void __setitem__(const K& key, const T& val) {
                (*self)[key] = val;
            }
            int __len(void*) {
                return int(self->size());
            }
            const T& get(const K& key) throw (std::out_of_range) {
                std::map<K,T >::iterator i = self->find(key);
                if (i != self->end())
                    return i->second;
                else
                    throw std::out_of_range("key not found");
            }
            void set(const K& key, const T& x) {
                (*self)[key] = x;
            }
            void del(const K& key) throw (std::out_of_range) {
                std::map<K,T >::iterator i = self->find(key);
                if (i != self->end())
                    self->erase(i);
                else
                    throw std::out_of_range("key not found");
            }
            bool has_key(const K& key) {
                std::map<K,T >::iterator i = self->find(key);
                return i != self->end();
            }
            std::vector<K> keys() {
                std::vector<K> ret;
                for(auto iter = self->begin(); iter != self->end(); ++iter) {
                    ret.emplace_back(iter->first);
                }
                return ret;
            }
            bool __eq__(const map<K,T>& rhs) {
                return (*self) == rhs;
            }
            bool __ne__(const map<K,T>& rhs) {
                return (*self) != rhs;
            }
        }
    };
}
