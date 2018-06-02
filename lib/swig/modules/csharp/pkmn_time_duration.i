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
 * C# System.TimeSpan will be used.
 */
%typemap(cstype) pkmn::time_duration "System.TimeSpan"
%typemap(csout, excode=SWIGEXCODE) pkmn::time_duration {
    global::System.IntPtr timeDurationPtr = $imcall;$excode

    TimeDurationInternal timeDurationInternal = new TimeDurationInternal(
                                                        timeDurationPtr,
                                                        false
                                                    );

    return new System.TimeSpan(
                   0, // days
                   timeDurationInternal.Hours,
                   timeDurationInternal.Minutes,
                   timeDurationInternal.Seconds,
                   (int)System.Math.Floor(timeDurationInternal.Frames * (1000.0 / 60.0))
               );
}

%typemap(cstype) const pkmn::time_duration& "System.TimeSpan"
%typemap(csin,
         pre="
    int $csinputTotalHours = $csinput.Hours + ($csinput.Days * 24);
    TimeDurationInternal temp$csinput = new TimeDurationInternal("
    "$csinputTotalHours, $csinput.Minutes, $csinput.Seconds, (int)System.Math.Floor($csinput.Milliseconds * (60.0 / 1000.0)));
    ") const pkmn::time_duration& "$csclassname.getCPtr(temp$csinput)"


%include <pkmn/types/time_duration.hpp>
