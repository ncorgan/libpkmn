/* -----------------------------------------------------------------------------
 * std_pair.i
 *
 * LibPKMN changes: manually write get/set functions so each can be individually
 *                  Doxygenated
 * ----------------------------------------------------------------------------- */

%include <std_common.i>
%include <exception.i>

// ------------------------------------------------------------------------
// std::pair
// ------------------------------------------------------------------------

%{
#include <utility>
%}

namespace std {

  template<class T, class U> struct pair {

    pair();
    pair(T first, U second);

    %extend {
        T getFirst() {
            return self->first;
        }
        U getSecond() {
            return self->second;
        }

        void setFirst(const T &value) {
            self->first = value;
        }
        void setSecond(const U &value) {
            self->second = value;
        }
    }
  };

}
