// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba
// (C) Copyright 2018 Nicholas Corgan

/*
 * These classes are adopted from <boost/thread/lockable_adapter.hpp> and
 * <boost/thread/lock_guard.hpp>.
 */

#ifndef PKMN_TYPES_MUTEX_HELPERS_HPP
#define PKMN_TYPES_MUTEX_HELPERS_HPP

#include <boost/thread/detail/config.hpp>
#include <boost/thread/detail/delete.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/detail/lockable_wrapper.hpp>
#include <boost/thread/lock_options.hpp>
#if ! defined BOOST_THREAD_PROVIDES_NESTED_LOCKS
#include <boost/thread/is_locked_by_this_thread.hpp>
#include <boost/assert.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace pkmn
{
  // Copied from Boost since lock() and unlock() weren't const in earlier
  // versions, causing a compiler error when instantiating in a copy
  // constructor.
  template <typename BasicLockable>
  class basic_lockable_adapter
  {
  public:
    typedef BasicLockable mutex_type;

  protected:
    mutex_type& lockable() const
    {
      return lockable_;
    }
    mutable mutex_type lockable_; /*< mutable so that it can be modified by const functions >*/
  public:

    BOOST_THREAD_NO_COPYABLE( basic_lockable_adapter) /*< no copyable >*/

    basic_lockable_adapter()
    {}

    void lock() const
    {
      lockable().lock();
    }
    void unlock() const
    {
      lockable().unlock();
    }

  };

  template <typename Mutex>
  class lock_guard
  {
  private:
    const Mutex& m;

  public:
    typedef Mutex mutex_type;
    BOOST_THREAD_NO_COPYABLE( lock_guard )

    // Modification from Boost: take in const reference, since lock() is const
    explicit lock_guard(const Mutex& m_) :
      m(m_)
    {
      m.lock();
    }

    lock_guard(Mutex& m_, boost::adopt_lock_t) :
      m(m_)
    {
#if ! defined BOOST_THREAD_PROVIDES_NESTED_LOCKS
      BOOST_ASSERT(is_locked_by_this_thread(m));
#endif
    }

    ~lock_guard()
    {
      m.unlock();
    }
  };
}

#include <boost/config/abi_suffix.hpp>

#endif
