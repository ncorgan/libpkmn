/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/game_save.hpp>
%}

%rename(__get_time_played) get_time_played;
%rename(__set_time_played) set_time_played;

%ignore detect_type;
%include <pkmn/game_save.hpp>

%extend std::shared_ptr<pkmn::game_save> {
    %pythoncode %{
        def get_time_played(self):
            import datetime

            internal_time_played = self.__get_time_played()

            # A frame is 1/60 of a second, so we need to convert this
            # to milliseconds.
            frames_to_milliseconds = (1000.0 / 60.0)
            milliseconds = internal_time_played.frames * frames_to_milliseconds

            return datetime.timedelta(
                       hours=internal_time_played.hours,
                       minutes=internal_time_played.minutes,
                       seconds=internal_time_played.seconds,
                       milliseconds=milliseconds
                   )

        def set_time_played(self, time_played):
            if not "datetime.timedelta" in str(type(time_played)):
                raise TypeError("Expected type \"datetime.timedelta\", got {0}".format(type(time_played)))

            microseconds_to_frames = (60.0 / 1000000.0)

            hours = time_played.seconds // 3600
            minutes = (time_played.seconds // 60) % 60
            seconds = time_played.seconds % 60
            frames = int(time_played.microseconds * microseconds_to_frames)

            internal_time_played = time_duration_internal(
                                       hours, minutes, seconds, frames
                                   )
            self.__set_time_played(internal_time_played)
    %}
}
%template(game_save_sptr) std::shared_ptr<pkmn::game_save>;
