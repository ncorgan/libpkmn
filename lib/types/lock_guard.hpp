// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba
// (C) Copyright 2018 Nicholas Corgan

#ifndef BOOST_THREAD_LOCK_GUARD_HPP
#define BOOST_THREAD_LOCK_GUARD_HPP

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

#if ! defined BOOST_THREAD_NO_CXX11_HDR_INITIALIZER_LIST
    lock_guard(std::initializer_list<boost::thread_detail::lockable_wrapper<Mutex> > l_) :
      m(*(const_cast<boost::thread_detail::lockable_wrapper<Mutex>*>(l_.begin())->m))
    {
      m.lock();
    }

    lock_guard(std::initializer_list<boost::thread_detail::lockable_adopt_wrapper<Mutex> > l_) :
      m(*(const_cast<boost::thread_detail::lockable_adopt_wrapper<Mutex>*>(l_.begin())->m))
    {
#if ! defined BOOST_THREAD_PROVIDES_NESTED_LOCKS
      BOOST_ASSERT(is_locked_by_this_thread(m));
#endif
    }

#endif
    ~lock_guard()
    {
      m.unlock();
    }
  };


#if ! defined BOOST_THREAD_NO_MAKE_LOCK_GUARD
  template <typename Lockable>
  lock_guard<Lockable> make_lock_guard(Lockable& mtx)
  {
    return { boost::thread_detail::lockable_wrapper<Lockable>(mtx) };
  }
  template <typename Lockable>
  lock_guard<Lockable> make_lock_guard(Lockable& mtx, boost::adopt_lock_t)
  {
    return { boost::thread_detail::lockable_adopt_wrapper<Lockable>(mtx) };
  }
#endif
}

#include <boost/config/abi_suffix.hpp>

#endif
