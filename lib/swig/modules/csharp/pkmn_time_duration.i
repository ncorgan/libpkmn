/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%import <csharp/stl_macros.i>

%{
    #include <pkmn/types/time_duration.hpp>
%}

%typemap(csclassmodifiers) pkmn::time_duration "internal class";
%rename(TimeDurationInternal) pkmn::time_duration;

/*
 * This class will not be exposed through the C# DLL. Instead, the existing
 * C# System.TimeSpan will be used. This typemap will create implicit operators
 * between these two classes for internal ease-of-use.
 */
%typemap(cscode) pkmn::time_duration %{
    public static implicit operator System.TimeSpan(TimeDurationInternal timeDurationInternal)
    {
        // A frame is 1/60 of a second, so we need to convert to milliseconds.
        const double frameToMillisecondConversionFactor = (1000.0 / 60.0);
        double milliseconds = timeDurationInternal.Frames * frameToMillisecondConversionFactor;

        return new System.TimeSpan(
                       0, // days
                       timeDurationInternal.Hours,
                       timeDurationInternal.Minutes,
                       timeDurationInternal.Seconds,
                       (int)milliseconds
                   );
    }

    public static implicit operator TimeDurationInternal(System.TimeSpan csharpTimeSpan)
    {
        // A frame is 1/60 of a second, so we need to convert from milliseconds.
        const double millisecondToFrameConversionFactor = (60.0 / 1000.0);
        double frames = csharpTimeSpan.Milliseconds * millisecondToFrameConversionFactor;

        return new TimeDurationInternal(
                       csharpTimeSpan.Hours,
                       csharpTimeSpan.Minutes,
                       csharpTimeSpan.Seconds,
                       (int)frames
                   );
    }
%}

%include <pkmn/types/time_duration.hpp>
