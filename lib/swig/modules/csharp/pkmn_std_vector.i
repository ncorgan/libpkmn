/* -----------------------------------------------------------------------------
 * pkmn_std_vector.i
 *
 * SWIG typemaps for std::vector<T>
 * C# implementation
 * The C# wrapper is made to look and feel like a C# System.Collections.Generic.List<> collection.
 * For .NET 1 compatibility, define SWIG_DOTNET_1 when compiling the C# code; then the C# wrapper is 
 * made to look and feel like a typesafe C# System.Collections.ArrayList.
 *
 * Note that IEnumerable<> is implemented in the proxy class which is useful for using LINQ with 
 * C++ std::vector wrappers. The IList<> interface is also implemented to provide enhanced functionality
 * whenever we are confident that the required C++ operator== is available. This is the case for when 
 * T is a primitive type or a pointer. If T does define an operator==, then use the SWIG_STD_VECTOR_ENHANCED
 * macro to obtain this enhanced functionality, for example:
 *
 *   SWIG_STD_VECTOR_ENHANCED(SomeNamespace::Klass)
 *   %template(VectKlass) std::vector<SomeNamespace::Klass>;
 *
 * Warning: heavy macro usage in this file. Use swig -E to get a sane view on the real file contents!
 * ----------------------------------------------------------------------------- */

// Warning: Use the typemaps here in the expectation that the macros they are in will change name.


%include <std_common.i>

// MACRO for use within the std::vector class body
%define SWIG_STD_VECTOR_MINIMUM_INTERNAL(CSINTERFACE, CONST_REFERENCE, CTYPE...)
%typemap(csinterfaces) std::vector< CTYPE > "IDisposable, System.Collections.IEnumerable\n#if !SWIG_DOTNET_1\n    , System.Collections.Generic.CSINTERFACE<$typemap(cstype, CTYPE)>\n#endif\n";
%typemap(cscode) std::vector< CTYPE > %{
  /// <summary>Gets a value indicating whether the IList has a fixed size.</summary>
  public bool IsFixedSize {
    get {
      return false;
    }
  }

  /// <summary>Gets a value indicating whether the $csclassname is read-only.</summary>
  public bool IsReadOnly {
    get {
      return false;
    }
  }

  /// <summary>Gets or sets the element at the given index..</summary>
  /// <exception cref="System.SystemException">If index is invalid</exception>
  /// <param name="index">Index</param>
  public $typemap(cstype, CTYPE) this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  /// <summary>Gets or sets the total number of elements the internal data structure can hold without resizing.</summary>
  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < size())
        throw new ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  /// <summary>Gets the number of elements contained in the $csclassname.</summary>
  public int Count {
    get {
      return (int)size();
    }
  }

  /// <summary>Gets a value indicating whether access to the $csclassname is synchronized (thread safe).</summary>
  public bool IsSynchronized {
    get {
      return false;
    }
  }

#if SWIG_DOTNET_1
  /// <summary>Copies the elements of the $csclassname to an Array, starting at a particular Array index.</summary>
  /// <exception cref="System.ArgumentNullException">If given array is null</exception>
  /// <exception cref="System.ArgumentOutOfRangeException">If index < 0</exception>
  /// <exception cref="System.ArgumentException">If array is multidimensional</exception>
  /// <param name="array">The one-dimensional Array that is the destination of the elements copied from $csclassname. The Array must have zero-based indexing.</param>
  /// <param name="index">The zero-based index in array at which copying begins.</exception>
  public void CopyTo(System.Array array, int index)
#else
  /// <summary>Copies the elements of the $csclassname to an Array, starting at a particular Array index.</summary>
  /// <exception cref="System.ArgumentNullException">If given array is null</exception>
  /// <exception cref="System.ArgumentOutOfRangeException">If index < 0</exception>
  /// <exception cref="System.ArgumentException">If array is multidimensional</exception>
  /// <param name="array">The one-dimensional Array that is the destination of the elements copied from $csclassname. The Array must have zero-based indexing.</param>
  /// <param name="index">The zero-based index in array at which copying begins.</exception>
  public void CopyTo($typemap(cstype, CTYPE)[] array, int index)
#endif
  {
    CopyTo(0, array, index, this.Count);
  }

#if SWIG_DOTNET_1
  private void CopyTo(int index, System.Array array, int arrayIndex, int count)
#else
  private void CopyTo(int index, $typemap(cstype, CTYPE)[] array, int arrayIndex, int count)
#endif
  {
    if (array == null)
      throw new ArgumentNullException("array");
    if (index < 0)
      throw new ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new ArgumentException("Multi dimensional array.", "array");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

#if !SWIG_DOTNET_1
  System.Collections.Generic.IEnumerator<$typemap(cstype, CTYPE)> System.Collections.Generic.IEnumerable<$typemap(cstype, CTYPE)>.GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }
#endif

  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

  /// <summary>Returns an enumerator that iterates through the $csclassname.</summary>
  public $csclassnameEnumerator GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

    /// <summary>Compares two $csclassname instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if their lengths are equal and if each
    /// corresponding LevelupMove is equal.
    /// </remarks>
    /// <param name="rhs">$csclassname with which to compare self</param>
    /// <returns>Whether or not $csclassname instances are equal</returns>
    public bool Equals($csclassname rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else if(this.Count != rhs.Count) {
            return false;
        }

        for(int i = 0; i < this.Count; i++) {
            if(!this[i].Equals(rhs[i])) {
                return false;
            }
        }

        return true;
    }

    /// <summary>Compares a $csclassname to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not $csclassname and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        $csclassname rhsList = rhs as $csclassname;
        if(rhsList == null) {
            return false;
        } else {
            return this.Equals(rhsList);
        }
    }

    /// <summary>Returns a hash code unique to the given object.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValues<$typemap(cstype, CTYPE)>(this)
                                       .ToHashCode();
    }

  /// <summary>An IEnumerator<$typemap(cstype, CTYPE)> implementation.</summary>
  public sealed class $csclassnameEnumerator : System.Collections.IEnumerator
#if !SWIG_DOTNET_1
    , System.Collections.Generic.IEnumerator<$typemap(cstype, CTYPE)>
#endif
  {
    private $csclassname collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public $csclassnameEnumerator($csclassname collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    /// <summary>Gets the current element in the $csclassname.</summary>
    /// <exception cref="System.InvalidOperationException">If enumeration has not started</exception>
    /// <exception cref="System.InvalidOperationException">If enumeration has already finished</exception>
    /// <exception cref="System.InvalidOperationException">If the collection was modified</exception>
    public $typemap(cstype, CTYPE) Current {
      get {
        if (currentIndex == -1)
          throw new InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new InvalidOperationException("Collection modified.");
        return ($typemap(cstype, CTYPE))currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    /// <summary>Advances the enumerator to the next element of the $csclassname.</summary>
    /// <returns>true if the enumerator was successfully advanced to the next element; false if the enumerator has passed the end of the $csclassname.</returns>
    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new InvalidOperationException("Collection modified.");
      }
    }

#if !SWIG_DOTNET_1
    public void Dispose() {
        currentIndex = -1;
        currentObject = null;
    }
#endif
  }
%}

  public:
    typedef size_t size_type;
    typedef CTYPE value_type;
    typedef CONST_REFERENCE const_reference;
    %rename(Clear) clear;
    void clear();
    %rename(Add) push_back;
    void push_back(CTYPE const& x);
    size_type size() const;
    size_type capacity() const;
    void reserve(size_type n);
    vector();
    %extend {
      vector(int capacity) throw (std::out_of_range) {
        std::vector< CTYPE >* pv = 0;
        if (capacity >= 0) {
          pv = new std::vector< CTYPE >();
          pv->reserve(capacity);
       } else {
          throw std::out_of_range("capacity");
       }
       return pv;
      }
      CTYPE getitemcopy(int index) throw (std::out_of_range) {
        if (index>=0 && index<(int)$self->size())
          return (*$self)[index];
        else
          throw std::out_of_range("index");
      }
      const_reference getitem(int index) throw (std::out_of_range) {
        if (index>=0 && index<(int)$self->size())
          return (*$self)[index];
        else
          throw std::out_of_range("index");
      }
      void setitem(int index, CTYPE const& val) throw (std::out_of_range) {
        if (index>=0 && index<(int)$self->size())
          (*$self)[index] = val;
        else
          throw std::out_of_range("index");
      }
      void Insert(int index, CTYPE const& x) throw (std::out_of_range) {
        if (index>=0 && index<(int)$self->size()+1)
          $self->insert($self->begin()+index, x);
        else
          throw std::out_of_range("index");
      }
      void RemoveAt(int index) throw (std::out_of_range) {
        if (index>=0 && index<(int)$self->size())
          $self->erase($self->begin() + index);
        else
          throw std::out_of_range("index");
      }
    }
%enddef

// Extra methods added to the collection class if operator== is defined for the class being wrapped
// The class will then implement IList<>, which adds extra functionality
%define SWIG_STD_VECTOR_EXTRA_OP_EQUALS_EQUALS(CTYPE...)
    %extend {
      bool Contains(CTYPE const& value) {
        return std::find($self->begin(), $self->end(), value) != $self->end();
      }
      int IndexOf(CTYPE const& value) {
        int index = -1;
        std::vector< CTYPE >::iterator it = std::find($self->begin(), $self->end(), value);
        if (it != $self->end())
          index = (int)(it - $self->begin());
        return index;
      }
      bool Remove(CTYPE const& value) {
        std::vector< CTYPE >::iterator it = std::find($self->begin(), $self->end(), value);
        if (it != $self->end()) {
          $self->erase(it);
      return true;
        }
        return false;
      }
    }
%enddef

// Macros for std::vector class specializations/enhancements
%define SWIG_STD_VECTOR_ENHANCED(CTYPE...)
namespace std {
  template<> class vector< CTYPE > {
    SWIG_STD_VECTOR_MINIMUM_INTERNAL(IList, %arg(CTYPE const&), %arg(CTYPE))
    SWIG_STD_VECTOR_EXTRA_OP_EQUALS_EQUALS(CTYPE)
  };
}
%enddef

// Legacy macros
%define SWIG_STD_VECTOR_SPECIALIZE(CSTYPE, CTYPE...)
#warning SWIG_STD_VECTOR_SPECIALIZE macro deprecated, please see csharp/std_vector.i and switch to SWIG_STD_VECTOR_ENHANCED
SWIG_STD_VECTOR_ENHANCED(CTYPE)
%enddef

%define SWIG_STD_VECTOR_SPECIALIZE_MINIMUM(CSTYPE, CTYPE...)
#warning SWIG_STD_VECTOR_SPECIALIZE_MINIMUM macro deprecated, it is no longer required
%enddef

%{
#include <vector>
#include <algorithm>
#include <stdexcept>
%}

%csmethodmodifiers std::vector::push_back "
/// <summary>Adds an item to the $csclassname.</summary>
public";

%csmethodmodifiers std::vector::clear "
/// <summary>Removes all items from the $csclassname.</summary>
public";

%csmethodmodifiers std::vector::Insert "
/// <summary>Inserts an item to the $csclassname at the specified index.</summary>
/// <param name=\"index\">The zero-based index at which value should be inserted.</param>
/// <param name=\"x\">The object to insert into the $csclassname.</param>
/// <exception cref=\"System.SystemException\">if index is out of range</exception>
public";

%csmethodmodifiers std::vector::RemoveAt "
/// <summary>Removes the $csclassname item at the specified index.</summary>
/// <param name=\"index\">The zero-based index of the item to remove.</param>
/// <exception cref=\"System.SystemException\">if index is out of range</exception>
public";

%csmethodmodifiers std::vector::Contains "
/// <summary>Determines whether the $csclassname contains a specific value.</summary>
public";

%csmethodmodifiers std::vector::IndexOf "
/// <summary>Determines the index of a specific item in the $csclassname.</summary>
/// <param name=\"value\">The object to locate in the $csclassname.</param>
/// <returns>The index of value if found in the list; otherwise, -1.</returns>
public";

%csmethodmodifiers std::vector::Remove "
/// <summary>Removes the first occurrence of a specific object from the $csclassname.</summary>
/// <param name=\"value\">The object to remove from the $csclassname.</param>
public";

%csmethodmodifiers std::vector::getitemcopy "private"
%csmethodmodifiers std::vector::getitem "private"
%csmethodmodifiers std::vector::setitem "private"
%csmethodmodifiers std::vector::size "private"
%csmethodmodifiers std::vector::capacity "private"
%csmethodmodifiers std::vector::reserve "private"

namespace std {
  // primary (unspecialized) class template for std::vector
  // does not require operator== to be defined
  template<class T> class vector {
    SWIG_STD_VECTOR_MINIMUM_INTERNAL(IEnumerable, T const&, T)
  };
  // specialization for pointers
  template<class T> class vector<T *> {
    SWIG_STD_VECTOR_MINIMUM_INTERNAL(IList, T *const&, T *)
    SWIG_STD_VECTOR_EXTRA_OP_EQUALS_EQUALS(T *)
  };
  // bool is specialized in the C++ standard - const_reference in particular
  template<> class vector<bool> {
    SWIG_STD_VECTOR_MINIMUM_INTERNAL(IList, bool, bool)
    SWIG_STD_VECTOR_EXTRA_OP_EQUALS_EQUALS(bool)
  };
}

// template specializations for std::vector
// these provide extra collections methods as operator== is defined
SWIG_STD_VECTOR_ENHANCED(char)
SWIG_STD_VECTOR_ENHANCED(signed char)
SWIG_STD_VECTOR_ENHANCED(unsigned char)
SWIG_STD_VECTOR_ENHANCED(short)
SWIG_STD_VECTOR_ENHANCED(unsigned short)
SWIG_STD_VECTOR_ENHANCED(int)
SWIG_STD_VECTOR_ENHANCED(unsigned int)
SWIG_STD_VECTOR_ENHANCED(long)
SWIG_STD_VECTOR_ENHANCED(unsigned long)
SWIG_STD_VECTOR_ENHANCED(long long)
SWIG_STD_VECTOR_ENHANCED(unsigned long long)
SWIG_STD_VECTOR_ENHANCED(float)
SWIG_STD_VECTOR_ENHANCED(double)

