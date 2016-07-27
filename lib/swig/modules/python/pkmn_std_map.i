/*
  Maps
*/

%fragment("StdMapCommonTraits","header",fragment="StdSequenceTraits")
{
  namespace swig {
    template <class ValueType>
    struct from_key_oper 
    {
      typedef const ValueType& argument_type;
      typedef  PyObject *result_type;
      result_type operator()(argument_type v) const
      {
    return swig::from(v.first);
      }
    };

    template <class ValueType>
    struct from_value_oper 
    {
      typedef const ValueType& argument_type;
      typedef  PyObject *result_type;
      result_type operator()(argument_type v) const
      {
    return swig::from(v.second);
      }
    };

    template<class OutIterator, class FromOper, class ValueType = typename OutIterator::value_type>
    struct SwigPyMapIterator_T : SwigPyIteratorClosed_T<OutIterator, ValueType, FromOper>
    {
      SwigPyMapIterator_T(OutIterator curr, OutIterator first, OutIterator last, PyObject *seq)
    : SwigPyIteratorClosed_T<OutIterator,ValueType,FromOper>(curr, first, last, seq)
      {
      }
    };


    template<class OutIterator,
         class FromOper = from_key_oper<typename OutIterator::value_type> >
    struct SwigPyMapKeyIterator_T : SwigPyMapIterator_T<OutIterator, FromOper>
    {
      SwigPyMapKeyIterator_T(OutIterator curr, OutIterator first, OutIterator last, PyObject *seq)
    : SwigPyMapIterator_T<OutIterator, FromOper>(curr, first, last, seq)
      {
      }
    };

    template<typename OutIter>
    inline SwigPyIterator*
    make_output_key_iterator(const OutIter& current, const OutIter& begin, const OutIter& end, PyObject *seq = 0)
    {
      return new SwigPyMapKeyIterator_T<OutIter>(current, begin, end, seq);
    }

    template<class OutIterator,
         class FromOper = from_value_oper<typename OutIterator::value_type> >
    struct SwigPyMapValueITerator_T : SwigPyMapIterator_T<OutIterator, FromOper>
    {
      SwigPyMapValueITerator_T(OutIterator curr, OutIterator first, OutIterator last, PyObject *seq)
    : SwigPyMapIterator_T<OutIterator, FromOper>(curr, first, last, seq)
      {
      }
    };
    

    template<typename OutIter>
    inline SwigPyIterator*
    make_output_value_iterator(const OutIter& current, const OutIter& begin, const OutIter& end, PyObject *seq = 0)
    {
      return new SwigPyMapValueITerator_T<OutIter>(current, begin, end, seq);
    }
  }
}

%fragment("StdMapTraits","header",fragment="StdMapCommonTraits")
{
  namespace swig {
    template <class SwigPySeq, class K, class T, class Compare, class Alloc >
    inline void
    assign(const SwigPySeq& swigpyseq, std::map<K,T,Compare,Alloc > *map) {
      typedef typename std::map<K,T,Compare,Alloc >::value_type value_type;
      typename SwigPySeq::const_iterator it = swigpyseq.begin();
      for (;it != swigpyseq.end(); ++it) {
    map->insert(value_type(it->first, it->second));
      }
    }

    template <class K, class T, class Compare, class Alloc>
    struct traits_asptr<std::map<K,T,Compare,Alloc > >  {
      typedef std::map<K,T,Compare,Alloc > map_type;
      static int asptr(PyObject *obj, map_type **val) {
    int res = SWIG_ERROR;
    SWIG_PYTHON_THREAD_BEGIN_BLOCK;
    if (PyDict_Check(obj)) {
      SwigVar_PyObject items = PyObject_CallMethod(obj,(char *)"items",NULL);
%#if PY_VERSION_HEX >= 0x03000000
          /* In Python 3.x the ".items()" method returns a dict_items object */
          items = PySequence_Fast(items, ".items() didn't return a sequence!");
%#endif
      res = traits_asptr_stdseq<map_type, std::pair<K, T> >::asptr(items, val);
    } else {
      map_type *p;
      res = SWIG_ConvertPtr(obj,(void**)&p,swig::type_info<map_type>(),0);
      if (SWIG_IsOK(res) && val)  *val = p;
    }
    SWIG_PYTHON_THREAD_END_BLOCK;
    return res;
      }      
    };
      
    template <class K, class T, class Compare, class Alloc >
    struct traits_from<std::map<K,T,Compare,Alloc > >  {
      typedef std::map<K,T,Compare,Alloc > map_type;
      typedef typename map_type::const_iterator const_iterator;
      typedef typename map_type::size_type size_type;

      static PyObject *asdict(const map_type& map) {
    SWIG_PYTHON_THREAD_BEGIN_BLOCK;
    size_type size = map.size();
    int pysize = (size <= (size_type) INT_MAX) ? (int) size : -1;
    if (pysize < 0) {
      PyErr_SetString(PyExc_OverflowError,
              "map size not valid in python");
      SWIG_PYTHON_THREAD_END_BLOCK;
      return NULL;
    }
    PyObject *obj = PyDict_New();
    for (const_iterator i= map.begin(); i!= map.end(); ++i) {
      swig::SwigVar_PyObject key = swig::from(i->first);
      swig::SwigVar_PyObject val = swig::from(i->second);
      PyDict_SetItem(obj, key, val);
    }
    SWIG_PYTHON_THREAD_END_BLOCK;
    return obj;
      }
                
      static PyObject *from(const map_type& map) {
    swig_type_info *desc = swig::type_info<map_type>();
    if (desc && desc->clientdata) {
      return SWIG_InternalNewPointerObj(new map_type(map), desc, SWIG_POINTER_OWN);
    } else {
      return asdict(map);
    }
      }
    };
  }
}

%define %swig_map_common(Map...)
  %swig_sequence_iterator(Map);
  %swig_container_methods(Map)

#if defined(SWIGPYTHON_BUILTIN)
  %feature("python:slot", "mp_length", functype="lenfunc") __len__;
  %feature("python:slot", "mp_subscript", functype="binaryfunc") __getitem__;
  %feature("python:slot", "tp_iter", functype="getiterfunc") key_iterator;

  %extend {
    %newobject iterkeys(PyObject **PYTHON_SELF);
    swig::SwigPyIterator* iterkeys(PyObject **PYTHON_SELF) {
      return swig::make_output_key_iterator(self->begin(), self->begin(), self->end(), *PYTHON_SELF);
    }
      
    %newobject itervalues(PyObject **PYTHON_SELF);
    swig::SwigPyIterator* itervalues(PyObject **PYTHON_SELF) {
      return swig::make_output_value_iterator(self->begin(), self->begin(), self->end(), *PYTHON_SELF);
    }

    %newobject iteritems(PyObject **PYTHON_SELF);
    swig::SwigPyIterator* iteritems(PyObject **PYTHON_SELF) {
      return swig::make_output_iterator(self->begin(), self->begin(), self->end(), *PYTHON_SELF);
    }
  }

#else
  %extend {
    %pythoncode %{def __iter__(self):
    return self.key_iterator()%}
    %pythoncode %{def iterkeys(self):
    return self.key_iterator()%}
    %pythoncode %{def itervalues(self):
    return self.value_iterator()%}
    %pythoncode %{def iteritems(self):
    return self.iterator()%}
  }
#endif

  %extend {
    mapped_type const & __getitem__(const key_type& key) throw (std::out_of_range) {
      Map::const_iterator i = self->find(key);
      if (i != self->end())
    return i->second;
      else
    throw std::out_of_range("key not found");
    }

    void __delitem__(const key_type& key) throw (std::out_of_range) {
      Map::iterator i = self->find(key);
      if (i != self->end())
    self->erase(i);
      else
    throw std::out_of_range("key not found");
    }
    
    bool has_key(const key_type& key) const {
      Map::const_iterator i = self->find(key);
      return i != self->end();
    }
    
    PyObject* keys() {
      Map::size_type size = self->size();
      int pysize = (size <= (Map::size_type) INT_MAX) ? (int) size : -1;
      SWIG_PYTHON_THREAD_BEGIN_BLOCK;
      if (pysize < 0) {
    PyErr_SetString(PyExc_OverflowError,
            "map size not valid in python");
    SWIG_PYTHON_THREAD_END_BLOCK;
    return NULL;
      }
      PyObject* keyList = PyList_New(pysize);
      Map::const_iterator i = self->begin();
      for (int j = 0; j < pysize; ++i, ++j) {
    PyList_SET_ITEM(keyList, j, swig::from(i->first));
      }
      SWIG_PYTHON_THREAD_END_BLOCK;
      return keyList;
    }
    
    PyObject* values() {
      Map::size_type size = self->size();
      int pysize = (size <= (Map::size_type) INT_MAX) ? (int) size : -1;
      SWIG_PYTHON_THREAD_BEGIN_BLOCK;
      if (pysize < 0) {
    PyErr_SetString(PyExc_OverflowError,
            "map size not valid in python");
    SWIG_PYTHON_THREAD_END_BLOCK;
    return NULL;
      }
      PyObject* valList = PyList_New(pysize);
      Map::const_iterator i = self->begin();
      for (int j = 0; j < pysize; ++i, ++j) {
    PyList_SET_ITEM(valList, j, swig::from(i->second));
      }
      SWIG_PYTHON_THREAD_END_BLOCK;
      return valList;
    }
    
    PyObject* items() {
      Map::size_type size = self->size();
      int pysize = (size <= (Map::size_type) INT_MAX) ? (int) size : -1;
      SWIG_PYTHON_THREAD_BEGIN_BLOCK;
      if (pysize < 0) {
    PyErr_SetString(PyExc_OverflowError,
            "map size not valid in python");
    SWIG_PYTHON_THREAD_END_BLOCK;
    return NULL;
      }    
      PyObject* itemList = PyList_New(pysize);
      Map::const_iterator i = self->begin();
      for (int j = 0; j < pysize; ++i, ++j) {
    PyList_SET_ITEM(itemList, j, swig::from(*i));
      }
      SWIG_PYTHON_THREAD_END_BLOCK;
      return itemList;
    }
    
    // Python 2.2 methods
    bool __contains__(const key_type& key) {
      return self->find(key) != self->end();
    }

    %newobject key_iterator(PyObject **PYTHON_SELF);
    swig::SwigPyIterator* key_iterator(PyObject **PYTHON_SELF) {
      return swig::make_output_key_iterator(self->begin(), self->begin(), self->end(), *PYTHON_SELF);
    }

    %newobject value_iterator(PyObject **PYTHON_SELF);
    swig::SwigPyIterator* value_iterator(PyObject **PYTHON_SELF) {
      return swig::make_output_value_iterator(self->begin(), self->begin(), self->end(), *PYTHON_SELF);
    }
  }

%enddef

%define %swig_map_methods(Map...)
  %swig_map_common(Map)

#if defined(SWIGPYTHON_BUILTIN)
  %feature("python:slot", "mp_ass_subscript", functype="objobjargproc") __setitem__;
#endif

  %extend {
    // This will be called through the mp_ass_subscript slot to delete an entry.
    void __setitem__(const key_type& key) {
      self->erase(key);
    }

    void __setitem__(const key_type& key, const mapped_type& x) throw (std::out_of_range) {
      (*self)[key] = x;
    }

    PyObject* asdict() {
      return swig::traits_from< Map >::asdict(*self);
    }
  }


%enddef

//
// std::map
//

%include <std_pair.i>
%include <std_container.i>

%define %std_map_methods_common(map...)
  %std_container_methods(map);

  size_type erase(const key_type& x);
  size_type count(const key_type& x) const;
%enddef

%define %std_map_methods(map...)
  %std_map_methods_common(map);
%enddef


// ------------------------------------------------------------------------
// std::map
// 
// const declarations are used to guess the intent of the function being
// exported; therefore, the following rationale is applied:
// 
//   -- f(std::map<T>), f(const std::map<T>&):
//      the parameter being read-only, either a sequence or a
//      previously wrapped std::map<T> can be passed.
//   -- f(std::map<T>&), f(std::map<T>*):
//      the parameter may be modified; therefore, only a wrapped std::map
//      can be passed.
//   -- std::map<T> f(), const std::map<T>& f():
//      the map is returned by copy; therefore, a sequence of T:s 
//      is returned which is most easily used in other functions
//   -- std::map<T>& f(), std::map<T>* f():
//      the map is returned by reference; therefore, a wrapped std::map
//      is returned
//   -- const std::map<T>* f(), f(const std::map<T>*):
//      for consistency, they expect and return a plain map pointer.
// ------------------------------------------------------------------------

%{
#include <map>
%}

// exported class

namespace std {

  template<class _Key, class _Tp, class _Compare = std::less<_Key >,
       class _Alloc = allocator<std::pair<const _Key, _Tp > > >
  class map {
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Key key_type;
    typedef _Tp mapped_type;
    typedef std::pair<const _Key, _Tp> value_type;

    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _Alloc allocator_type;

    %traits_swigtype(_Key);
    %traits_swigtype(_Tp);        

    %fragment(SWIG_Traits_frag(std::pair< _Key, _Tp >), "header",
          fragment=SWIG_Traits_frag(_Key),
          fragment=SWIG_Traits_frag(_Tp),
          fragment="StdPairTraits") {
      namespace swig {
    template <>  struct traits<std::pair< _Key, _Tp > > {
      typedef pointer_category category;
      static const char* type_name() {
        return "std::pair<" #_Key "," #_Tp " >";
      }
    };
      }
    }

    %fragment(SWIG_Traits_frag(std::map<_Key, _Tp, _Compare, _Alloc >), "header",
          fragment=SWIG_Traits_frag(std::pair<_Key, _Tp >),
          fragment="StdMapTraits") {
      namespace swig {
    template <>  struct traits<std::map<_Key, _Tp, _Compare, _Alloc > > {
      typedef pointer_category category;
      static const char* type_name() {
        return "std::map<" #_Key "," #_Tp "," #_Compare "," #_Alloc " >";
      }
    };
      }
    }

    %typemap_traits_ptr(SWIG_TYPECHECK_MAP, std::map<_Key, _Tp, _Compare, _Alloc >);

    map( const _Compare& );

#ifdef %swig_map_methods
    // Add swig/language extra methods
    %swig_map_methods(std::map<_Key, _Tp, _Compare, _Alloc >);
#endif
  
    %std_map_methods(map);
  };

}
