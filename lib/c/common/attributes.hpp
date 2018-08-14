/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_COMMON_ATTRIBUTES_HPP
#define PKMN_C_COMMON_ATTRIBUTES_HPP

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/types/attribute_names.h>

#include <string>

namespace pkmn { namespace c {

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error get_numeric_attribute(
        const c_struct_type* p_c_struct,
        const char* p_attribute_name,
        int* p_value_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_name,
                        "p_attribute_name",
                        p_internal
                    );
            if(!error)
            {
                error = check_for_null_param<libpkmn_type>(
                            p_value_out,
                            "p_value_out",
                            p_internal
                        );
            }
            if(!error)
            {
                auto impl = [&]()
                {
                    *p_value_out = p_internal->cpp->get_numeric_attribute(
                                       p_attribute_name
                                   );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error set_numeric_attribute(
        const c_struct_type* p_c_struct,
        const char* p_attribute_name,
        int value,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_name,
                        "p_attribute_name",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    p_internal->cpp->set_numeric_attribute(
                        p_attribute_name,
                        value
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error get_string_attribute(
        const c_struct_type* p_c_struct,
        const char* p_attribute_name,
        char* p_value_out,
        size_t value_buffer_len,
        size_t* p_actual_value_len_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_name,
                        "p_attribute_name",
                        p_internal
                    );
            if(!error)
            {
                error = check_for_null_param<libpkmn_type>(
                            p_value_out,
                            "p_value_out",
                            p_internal
                        );
            }
            if(!error)
            {
                auto impl = [&]()
                {
                    pkmn::c::string_cpp_to_c(
                        p_internal->cpp->get_string_attribute(p_attribute_name),
                        p_value_out,
                        value_buffer_len,
                        p_actual_value_len_out
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error set_string_attribute(
        const c_struct_type* p_c_struct,
        const char* p_attribute_name,
        const char* p_value,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_name,
                        "p_attribute_name",
                        p_internal
                    );
            if(!error)
            {
                error = check_for_null_param<libpkmn_type>(
                            p_value,
                            "p_value",
                            p_internal
                        );
            }
            if(!error)
            {
                auto impl = [&]()
                {
                    p_internal->cpp->set_string_attribute(
                        p_attribute_name,
                        p_value
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error get_boolean_attribute(
        const c_struct_type* p_c_struct,
        const char* p_attribute_name,
        bool* p_value_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_name,
                        "p_attribute_name",
                        p_internal
                    );
            if(!error)
            {
                error = check_for_null_param<libpkmn_type>(
                            p_value_out,
                            "p_value_out",
                            p_internal
                        );
            }
            if(!error)
            {
                auto impl = [&]()
                {
                    *p_value_out = p_internal->cpp->get_boolean_attribute(
                                       p_attribute_name
                                   );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error set_boolean_attribute(
        const c_struct_type* p_c_struct,
        const char* p_attribute_name,
        bool value,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_name,
                        "p_attribute_name",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    p_internal->cpp->set_boolean_attribute(
                        p_attribute_name,
                        value
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error get_attribute_names(
        const c_struct_type* p_c_struct,
        struct pkmn_attribute_names* p_attribute_names_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            using internal_type_t = pkmn_c_internal_class_t<libpkmn_type>;

            internal_type_t* p_internal = static_cast<internal_type_t*>(
                                              p_c_struct->p_internal
                                          );

            error = check_for_null_param<libpkmn_type>(
                        p_attribute_names_out,
                        "p_attribute_names_out",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    // Make all C++ calls and operate on a second struct until we
                    // know everything succeeds before changing any user output.
                    // If this fails, we'll leak, but it's small enough to not be
                    // a concern.
                    struct pkmn_attribute_names temp_attribute_names_c =
                    {
                        // numeric_attribute_names
                        {
                            nullptr, // pp_strings
                            0ULL     // length
                        },
                        // string_attribute_names
                        {
                            nullptr, // pp_strings
                            0ULL     // length
                        },
                        // boolean_attribute_names
                        {
                            nullptr, // pp_strings
                            0ULL     // length
                        }
                    };

                    string_list_cpp_to_c(
                        p_internal->cpp->get_numeric_attribute_names(),
                        &temp_attribute_names_c.numeric_attribute_names
                    );
                    string_list_cpp_to_c(
                        p_internal->cpp->get_string_attribute_names(),
                        &temp_attribute_names_c.string_attribute_names
                    );
                    string_list_cpp_to_c(
                        p_internal->cpp->get_boolean_attribute_names(),
                        &temp_attribute_names_c.boolean_attribute_names
                    );

                    // Everything succeeded, so move it into the pointer the caller
                    // provided.
                    *p_attribute_names_out = std::move(temp_attribute_names_c);
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }
}
}

#endif /* PKMN_C_COMMON_ATTRIBUTES_HPP */
