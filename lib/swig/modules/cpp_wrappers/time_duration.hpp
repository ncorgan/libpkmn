/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_TIME_DURATION_HPP
#define CPP_WRAPPERS_TIME_DURATION_HPP

#include "private_exports.hpp"

#include <pkmn/game_save.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    class game_save_time_played
    {
        public:
            explicit game_save_time_played(
                const std::shared_ptr<pkmn::game_save>& internal
            ): _internal(internal)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                _cached_time_duration = _internal->get_time_played();
                _generation = pkmn::priv::game_enum_to_generation(
                                  _internal->get_game()
                              );
            }

            inline int get_hours() const
            {
                return _cached_time_duration.hours;
            }

            inline void set_hours(int hours)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                pkmn::time_duration time_played(_cached_time_duration);
                time_played.hours = hours;

                _internal->set_time_played(time_played);

                // Only cache the value if we know it's valid.
                _cached_time_duration.hours = hours;
            }

            inline int get_minutes() const
            {
                return _cached_time_duration.minutes;
            }

            inline void set_minutes(int minutes)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                pkmn::time_duration time_played(_cached_time_duration);
                time_played.minutes = minutes;

                _internal->set_time_played(time_played);

                // Only cache the value if we know it's valid.
                _cached_time_duration.minutes = minutes;
            }

            inline int get_seconds() const
            {
                return _cached_time_duration.seconds;
            }

            inline void set_seconds(int seconds)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                pkmn::time_duration time_played(_cached_time_duration);
                time_played.seconds = seconds;

                _internal->set_time_played(time_played);

                // Only cache the value if we know it's valid.
                _cached_time_duration.seconds = seconds;
            }

            inline int get_frames() const
            {
                return _cached_time_duration.frames;
            }

            inline void set_frames(int frames)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                // Generation I doesn't have frames, but there's no error,
                // so just ignore it.
                if(_generation > 1)
                {
                    pkmn::time_duration time_played(_cached_time_duration);
                    time_played.frames = frames;

                    _internal->set_time_played(time_played);

                    // Only cache the value if we know it's valid.
                    _cached_time_duration.frames = frames;
                }
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return uintmax_t(_internal.get());
            }
#else
            inline bool operator==(const game_save_time_played& rhs) const
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return (_internal == rhs._internal);
            }

            inline bool operator!=(const game_save_time_played& rhs) const
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const std::shared_ptr<pkmn::game_save>& _internal;

            pkmn::time_duration _cached_time_duration;
            int _generation;
    };

}}

#endif /* CPP_WRAPPERS_TIME_DURATION_HPP */
