/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_JAVA_INIT(libname)
    %pragma(java) jniclasscode=%{
        static {
            try {
                System.loadLibrary(libname);
            } catch (UnsatisfiedLinkError e) {
                System.err.println("Failed to load " + libname + " with error: " + e);
                System.exit(1);
            }
        }
    %}

    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    //%include <pkmn_javadocs.i>
    %include <java_CamelCase.i>
    //%include <java/equality.i>
    %include <java/stl_macros.i>
%enddef
