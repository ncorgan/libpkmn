/* -----------------------------------------------------------------------------
 * pkmn_std_map.i
 *
 * SWIG typemaps for std::map< K, T, C >
 *
 * The C# wrapper is made to look and feel like a C# System.Collections.Generic.IDictionary<>.
 *
 * Using this wrapper is fairly simple. For example, to create a map from integers to doubles use:
 *
 *   %include <std_map.i>
 *   %template(DictIntDouble) std::map<int, double>
 *
 * Notes:
 * 1) For .NET 1 compatibility, define SWIG_DOTNET_1 when compiling the C# code. In this case
 *    the C# wrapper has only basic functionality.
 * 2) IEnumerable<> is implemented in the proxy class which is useful for using LINQ with
 *    C++ std::map wrappers.
 *
 * Warning: heavy macro usage in this file. Use swig -E to get a sane view on the real file contents!
 * ----------------------------------------------------------------------------- */

%{
#include <map>
#include <algorithm>
#include <stdexcept>
%}

/* K is the C++ key type, T is the C++ value type */
%define SWIG_STD_MAP_INTERNAL(K, T, C)

%typemap(csinterfaces) std::map< K, T, C > "IDisposable \n#if !SWIG_DOTNET_1\n    , System.Collections.Generic.IDictionary<$typemap(cstype, K), $typemap(cstype, T)>\n#endif\n";
%typemap(cscode) std::map<K, T, C > %{

  /// <summary>Gets or sets the element with the specified key.</summary>
  /// <exception cref="System.SystemException">If key is invalid</exception>
  /// <param name="key">Key</param>
  public $typemap(cstype, T) this[$typemap(cstype, K) key] {
    get {
      return getitem(key);
    }

    set {
      setitem(key, value);
    }
  }

  /// <summary>Gets the value associated with the specified key.</summary>
  /// <param name="key">The key whose value to get.</param>
  /// <param name="value">When this method returns, the value associated with the specified key,
  /// if the key is found; otherwise, the default value for the type of the value parameter.
  /// This parameter is passed uninitialized.</param>
  /// <returns>True if the $csclassname has the key, false otherwise</returns>
  public bool TryGetValue($typemap(cstype, K) key, out $typemap(cstype, T) value) {
    if (this.ContainsKey(key)) {
      value = this[key];
      return true;
    }
    value = default($typemap(cstype, T));
    return false;
  }

  /// <summary>Gets the number of elements contained in the $csclassname.</summary>
  public int Count {
    get {
      return (int)size();
    }
  }

  /// <summary>Gets a value indicating whether the $csclassname is read-only.</summary>
  public bool IsReadOnly {
    get {
      return false;
    }
  }

#if !SWIG_DOTNET_1

  /// <summary>Gets an ICollection<$typemap(cstype, K)> containing the keys of the $csclassname.</summary>
  public System.Collections.Generic.ICollection<$typemap(cstype, K)> Keys {
    get {
      System.Collections.Generic.ICollection<$typemap(cstype, K)> keys = new System.Collections.Generic.List<$typemap(cstype, K)>();
      int size = this.Count;
      if (size > 0) {
        IntPtr iter = create_iterator_begin();
        for (int i = 0; i < size; i++) {
          keys.Add(get_next_key(iter));
        }
        destroy_iterator(iter);
      }
      return keys;
    }
  }

  /// <summary>Gets an ICollection<$typemap(cstype, T)> containing the values in the $csclassname.</summary>
  public System.Collections.Generic.ICollection<$typemap(cstype, T)> Values {
    get {
      System.Collections.Generic.ICollection<$typemap(cstype, T)> vals = new System.Collections.Generic.List<$typemap(cstype, T)>();
      foreach (System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)> pair in this) {
        vals.Add(pair.Value);
      }
      return vals;
    }
  }

  /// <summary>Adds an item to the $csclassname.</summary>
  /// <exception cref="System.SystemException">If the key is already present in the $csclassname</exception>
  /// <param name="item">Key-value pair</param>
  public void Add(System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)> item) {
    Add(item.Key, item.Value);
  }

  /// <summary>Removes the first occurrence of a specific object from the $csclassname.</summary>
  /// <param name="item">Key-value pair</param>
  /// <returns>True if key-value pair is successfully removed, false otherwise</returns>
  public bool Remove(System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)> item) {
    if (Contains(item)) {
      return Remove(item.Key);
    } else {
      return false;
    }
  }

  /// <summary>Determines whether the $csclassname contains a specific value.</summary>
  /// <param name="item">Key-value pair</param>
  /// <returns>True if key-value pair is found in the $csclassname, false otherwise</returns>
  public bool Contains(System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)> item) {
    if (this[item.Key] == item.Value) {
      return true;
    } else {
      return false;
    }
  }

  /// <summary>Copies the elements of the $csclassname to an Array, starting at a particular Array index.</summary>
  /// <param name="array">The one-dimensional Array that is the destination of the elements copied from map. The Array must have zero-based indexing.</param>
  /// <param name="arrayIndex">The zero-based index in array at which copying begins.</param>
  /// <exception cref="System.ArgumentNullException">If array is null</exception>
  /// <exception cref="System.ArgumentOutOfRangeException">If arrayIndex is less than 0</exception>
  /// <exception cref="System.ArgumentException">If array is multi-dimensional</exception>
  /// <exception cref="System.ArgumentException">If number of elements to copy is too large</exception>
  public void CopyTo(System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>[] array, int arrayIndex) {
    if (array == null)
      throw new ArgumentNullException("array");
    if (arrayIndex < 0)
      throw new ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (array.Rank > 1)
      throw new ArgumentException("Multi dimensional array.", "array");
    if (arrayIndex+this.Count > array.Length)
      throw new ArgumentException("Number of elements to copy is too large.");

    System.Collections.Generic.IList<$typemap(cstype, K)> keyList = new System.Collections.Generic.List<$typemap(cstype, K)>(this.Keys);
    for (int i = 0; i < keyList.Count; i++) {
      $typemap(cstype, K) currentKey = keyList[i];
      array.SetValue(new System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>(currentKey, this[currentKey]), arrayIndex+i);
    }
  }

  System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>> System.Collections.Generic.IEnumerable<System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>>.GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

  /// <summary>Returns an enumerator that iterates through the $csclassname.</summary>
  public $csclassnameEnumerator GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

  /// <summary>An IEnumerator<KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>> implementation.</summary>
  public sealed class $csclassnameEnumerator : System.Collections.IEnumerator,
      System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>>
  {
    private $csclassname collectionRef;
    private System.Collections.Generic.IList<$typemap(cstype, K)> keyCollection;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public $csclassnameEnumerator($csclassname collection) {
      collectionRef = collection;
      keyCollection = new System.Collections.Generic.List<$typemap(cstype, K)>(collection.Keys);
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    /// <summary>Gets the current element in the $csclassname.</summary>
    /// <exception cref="System.InvalidOperationException">If enumeration has not started</exception>
    /// <exception cref="System.InvalidOperationException">If enumeration has already finished</exception>
    /// <exception cref="System.InvalidOperationException">If the collection was modified</exception>
    public System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)> Current {
      get {
        if (currentIndex == -1)
          throw new InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new InvalidOperationException("Collection modified.");
        return (System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>)currentObject;
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
        $typemap(cstype, K) currentKey = keyCollection[currentIndex];
        currentObject = new System.Collections.Generic.KeyValuePair<$typemap(cstype, K), $typemap(cstype, T)>(currentKey, collectionRef[currentKey]);
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    /// <summary>Sets the enumerator to its initial position, which is before the first element in the $csclassname.</summary>
    /// <exception cref="System.InvalidOperationException">If the collection was modified</exception>
    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new InvalidOperationException("Collection modified.");
      }
    }

    public void Dispose() {
      currentIndex = -1;
      currentObject = null;
    }
  }
#endif
    /// <summary>Compares two $csclassname instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if each of their key-value pairs are
    /// equal.
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

        foreach($typemap(cstype, K) key in this.Keys) {
            if(!rhs.ContainsKey(key)) {
                return false;
            }
            if(!this[key].Equals(rhs[key])) {
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
%}

  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef K key_type;
    typedef T mapped_type;

    map();
    size_type size() const;
    bool empty() const;
    %rename(Clear) clear;
    void clear();
    %extend {
      const mapped_type& getitem(const key_type& key) throw (std::out_of_range) {
        std::map< K, T, C >::iterator iter = $self->find(key);
        if (iter != $self->end())
          return iter->second;
        else
          throw std::out_of_range("key not found");
      }

      void setitem(const key_type& key, const mapped_type& x) {
        (*$self)[key] = x;
      }

      bool ContainsKey(const key_type& key) {
        std::map< K, T, C >::iterator iter = $self->find(key);
        return iter != $self->end();
      }

      void Add(const key_type& key, const mapped_type& val) throw (std::out_of_range) {
        std::map< K, T, C >::iterator iter = $self->find(key);
        if (iter != $self->end())
          throw std::out_of_range("key already exists");
        $self->insert(std::pair< K, T >(key, val));
      }

      bool Remove(const key_type& key) {
        std::map< K, T, C >::iterator iter = $self->find(key);
        if (iter != $self->end()) {
          $self->erase(iter);
          return true;
        }
        return false;
      }

      // create_iterator_begin(), get_next_key() and destroy_iterator work together to provide a collection of keys to C#
      %apply void *VOID_INT_PTR { std::map< K, T, C >::iterator *create_iterator_begin }
      %apply void *VOID_INT_PTR { std::map< K, T, C >::iterator *swigiterator }

      std::map< K, T, C >::iterator *create_iterator_begin() {
        return new std::map< K, T, C >::iterator($self->begin());
      }

      const key_type& get_next_key(std::map< K, T, C >::iterator *swigiterator) {
        (void)self; // Suppress "unused parameter" warning in SWIG code
        std::map< K, T, C >::iterator iter = *swigiterator;
        (*swigiterator)++;
        return (*iter).first;
      }

      void destroy_iterator(std::map< K, T, C >::iterator *swigiterator) {
        (void)self; // Suppress "unused parameter" warning in SWIG code
        delete swigiterator;
      }
    }


%enddef

%csmethodmodifiers std::map::ContainsKey "
/// <summary>Determines whether the $csclassname contains an element with the specified key.</summary>
public";

%csmethodmodifiers std::map::Add "
/// <summary>Adds an item to the $csclassname.</summary>
/// <exception cref=\"System.SystemException\">If the key is already present in the $csclassname</exception>
/// <param name=\"key\">Key</param>
/// <param name=\"val\">Value</param>
public";

%csmethodmodifiers std::map::Remove "
/// <summary>Removes the first occurrence of a specific object from the $csclassname.</summary>
/// <param name=\"key\">Key</param>
/// <returns>True if key-value pair is successfully removed, false otherwise</returns>
public";

%csmethodmodifiers std::map::empty "
/// <summary>Checks to see if $csclassname has no key-value pairs.</summary>
/// <returns>True if map is empty</returns>
public";

%csmethodmodifiers std::map::clear "
/// <summary>Removes all items from the $csclassname.</summary>
public";

%csmethodmodifiers std::map::size "private"
%csmethodmodifiers std::map::getitem "private"
%csmethodmodifiers std::map::setitem "private"
%csmethodmodifiers std::map::create_iterator_begin "private"
%csmethodmodifiers std::map::get_next_key "private"
%csmethodmodifiers std::map::destroy_iterator "private"

// Default implementation
namespace std {
  template<class K, class T, class C = std::less<K> > class map {
    SWIG_STD_MAP_INTERNAL(K, T, C)
  };
}
