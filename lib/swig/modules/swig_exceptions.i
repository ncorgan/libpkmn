/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This file makes using SWIG exception handling easier, providing the following
 * macros to catch all default C++ exceptions, any custom exceptions, and (with a
 * compile flag) boost::exception and its subclasses.
 *
 * The following macros are exposed:
 *
 * SWIG_CATCH_DEFAULT - A standard try-catch that catches std::exception and its
 *                      subclasses. Use this if you are not using any custom
 *                      std::exception subclasses.
 *
 * The following macros should be used when you add a custom exception.
 *
 * SWIG_TRY              - Starts a SWIG try/catch statement.
 *
 * SWIG_CUSTOM_EXCEPTION - Adds a catch statement for a custom exception. Specify
 *                         the C++ error name and SWIG exception enum to which it
 *                         corresponds.
 *
 *                         Valid exception enums:
 *                          * SWIG_UnknownError
 *                          * SWIG_IOError
 *                          * SWIG_RuntimeError
 *                          * SWIG_IndexError
 *                          * SWIG_TypeError
 *                          * SWIG_DivisionByZero
 *                          * SWIG_OverflowError
 *                          * SWIG_SyntaxError
 *                          * SWIG_ValueError
 *                          * SWIG_SystemError
 *                          * SWIG_AttributeError
 *                          * SWIG_MemoryError
 *                          * SWIG_NullReferenceError
 *
 * SWIG_CATCH            - Catches all standard C++ exceptions, as well as unknown
 *                         errors.
 *
 * The following code snippet shows how to use these together.
 *
 * SWIG_TRY
 * SWIG_CUSTOM_EXCEPTION(my_first_exception, SWIG_RuntimeError)
 * SWIG_CUSTOM_EXCEPTION(my_other_exception, SWIG_ValueError)
 * SWIG_CATCH
 *
 * Executing SWIG with the compile flag -DSWIG_WRAP_BOOST_EXCEPTIONS will allow
 * SWIG_CATCH and SWIG_CATCH_DEFAULT to catch boost::exception and any subclasses.
 */

%include <exception.i>

%{
#ifdef SWIG_WRAP_BOOST_EXCEPTIONS
    #include <boost/exception/all.hpp>
#endif

    #include <stdexcept>
%}

#ifdef SWIG_WRAP_BOOST_EXCEPTIONS
%define _SWIG_BOOST_EXCEPTION
    catch (const boost::exception &e) {
        SWIG_exception(
            SWIG_RuntimeError,
            boost::diagnostic_information(e).c_str()
        );
    }
%enddef
#else
#define _SWIG_BOOST_EXCEPTION
#endif

%define SWIG_TRY
    %exception {
        try { $action }
%enddef

%define SWIG_CATCH
        _SWIG_BOOST_EXCEPTION
        SWIG_CATCH_STDEXCEPT
        catch (...) {
            SWIG_exception(
                SWIG_UnknownError,
                "Unknown error"
            );
        }
    }
%enddef

%define SWIG_CATCH_DEFAULT
    SWIG_TRY
    SWIG_CATCH
%enddef

%define SWIG_CUSTOM_EXCEPTION(name, err_type)
    catch (const name &e) {
        SWIG_exception(
            err_type, e.what()
        );
    }
%enddef
