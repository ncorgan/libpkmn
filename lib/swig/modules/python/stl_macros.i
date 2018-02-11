/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#undef SWIG_EXPORT_ITERATOR_METHODS

%include <python/pkmn_std_map.i>
%include <pkmn_shared_ptr.i>
%include <std_pair.i>
%include <std_vector.i>

%define PKMN_PYTHON_MAP(cpp_key, cpp_val, python_name)
    %extend std::map<cpp_key, cpp_val> {
        %pythoncode %{
            def __eq__(self, rhs):
                if not isinstance(rhs, python_name):
                    return False
                elif self is rhs:
                    return True

                keys1 = self.keys()
                list.sort(keys1)
                keys2 = rhs.keys()
                list.sort(keys2)
                if keys1 != keys2:
                    return False

                for key in self.keys():
                    if self[key] != rhs[key]:
                        return False

                return True

            def __ne__(self, rhs):
                return not (self == rhs)
        %}
    }
    %template(python_name) std::map<cpp_key, cpp_val>;
%enddef

%define PKMN_PYTHON_PAIR(cpp_type1, cpp_type2, python_name)
    %extend std::pair<cpp_type1, cpp_type2> {
        %pythoncode %{
            def __eq__(self, rhs):
                if not isinstance(rhs, python_name):
                    return False
                elif self is rhs:
                    return True
                else:
                    return (self[0] == rhs[0] and self[1] == rhs[1])

            def __ne__(self, rhs):
                return not (self == rhs)
        %}
    }
    %template(python_name) std::pair<cpp_type1, cpp_type2>;
%enddef

%define PKMN_PYTHON_SPTR(cpp_type)
    %extend std::shared_ptr<pkmn:: ## cpp_type> {
        %pythoncode %{
            def __eq__(self, rhs):
                if not isinstance(rhs, (cpp_type ## _sptr)):
                    return False
                else:
                    return self.__sptr_eq(rhs)

            def __ne__(self, rhs):
                return not (self == rhs)
        %}
    }
    %template(cpp_type ## _sptr) std::shared_ptr<pkmn:: ## cpp_type>;
%enddef

%define PKMN_PYTHON_VECTOR(cpp_type, python_name)
    %extend std::vector<cpp_type > {
        %pythoncode %{
            def __eq__(self, rhs):
                if not isinstance(rhs, python_name):
                    return False
                elif self is rhs:
                    return True
                elif len(self) != len(rhs):
                    return False

                for i in range(len(self)):
                    if self[i] != rhs[i]:
                        return False

                return True

            def __ne__(self, rhs):
                return not (self == rhs)
        %}
    }
    %template(python_name) std::vector<cpp_type >;
%enddef
