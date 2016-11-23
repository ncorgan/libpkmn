/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c/cpp_to_c.hpp"
#include "c/error_internal.hpp"

#include <pkmn.h>

#include <boost/thread/mutex.hpp>
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

class pkmn_test_exception: public std::exception {
    public:
        explicit pkmn_test_exception(const char* msg):
            msg_(msg) {}

        explicit pkmn_test_exception(const std::string &msg):
            msg_(msg) {}

        virtual ~pkmn_test_exception() throw() {}

        virtual const char* what() const throw() {
            return msg_.c_str();
        }

    protected:
        std::string msg_;
};

/*
 * Converting C++ exceptions to C error codes
 */

pkmn_error_t throw_nothing() {
    PKMN_CPP_TO_C()
}

pkmn_error_t throw_unknown() {
    PKMN_CPP_TO_C(
        throw 5;
    )
}

template <typename exception_type>
pkmn_error_t throw_exception(
    const std::string &msg
) {
    PKMN_CPP_TO_C(
        throw exception_type(msg);
    )
}

BOOST_AUTO_TEST_CASE(exception_to_error_code_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = throw_nothing();
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "None");

    error = throw_exception<std::invalid_argument>("invalid_argument");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "invalid_argument");

    error = throw_exception<std::domain_error>("domain_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_DOMAIN_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "domain_error");

    error = throw_exception<std::length_error>("length_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LENGTH_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "length_error");

    error = throw_exception<std::out_of_range>("out_of_range");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "out_of_range");

    error = throw_exception<std::logic_error>("logic_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LOGIC_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "logic_error");

    error = throw_exception<std::range_error>("range_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "range_error");

    error = throw_exception<std::overflow_error>("overflow_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OVERFLOW_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "overflow_error");

    error = throw_exception<std::underflow_error>("underflow_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNDERFLOW_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "underflow_error");

    error = throw_exception<std::runtime_error>("runtime_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "runtime_error");

    error = throw_exception<pkmn_test_exception>("std_exception");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_EXCEPTION);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "std_exception");

    error = throw_unknown();
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNKNOWN_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Unknown error");
}

/*
 * Converting C++ exceptions to C error codes (with handle)
 */

typedef struct {
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_test_handle_t;

pkmn_error_t throw_nothing_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,)
}

pkmn_error_t throw_unknown_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw 5;
    )
}

template <typename exception_type>
pkmn_error_t throw_exception_with_handle(
    const std::string &msg,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw exception_type(msg);
    )
}

BOOST_AUTO_TEST_CASE(exception_to_error_code_with_handle_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_test_handle_t test_handle;

    error = throw_nothing_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
    BOOST_CHECK_EQUAL(test_handle.last_error, "None");

    error = throw_exception_with_handle<std::invalid_argument>(
        "invalid_argument", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    BOOST_CHECK_EQUAL(test_handle.last_error, "invalid_argument");

    error = throw_exception_with_handle<std::domain_error>(
        "domain_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_DOMAIN_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "domain_error");

    error = throw_exception_with_handle<std::length_error>(
        "length_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LENGTH_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "length_error");

    error = throw_exception_with_handle<std::out_of_range>(
        "out_of_range", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);
    BOOST_CHECK_EQUAL(test_handle.last_error, "out_of_range");

    error = throw_exception_with_handle<std::logic_error>(
        "logic_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LOGIC_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "logic_error");

    error = throw_exception_with_handle<std::range_error>(
        "range_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "range_error");

    error = throw_exception_with_handle<std::overflow_error>(
        "overflow_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OVERFLOW_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "overflow_error");

    error = throw_exception_with_handle<std::underflow_error>(
        "underflow_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNDERFLOW_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "underflow_error");

    error = throw_exception_with_handle<std::runtime_error>(
        "runtime_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "runtime_error");

    error = throw_exception_with_handle<pkmn_test_exception>(
        "std_exception", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_EXCEPTION);
    BOOST_CHECK_EQUAL(test_handle.last_error, "std_exception");

    error = throw_unknown_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNKNOWN_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "Unknown error");
}
