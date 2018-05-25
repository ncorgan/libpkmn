/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_TESTS_COMMON_H
#define PKMN_C_TESTS_COMMON_H

#include <unity.h>

#include<stdlib.h>
#include <time.h>

#define PKMN_C_TEST(test_func) \
{ \
    Unity.CurrentTestName = #test_func; \
    ++Unity.NumberOfTests; \
    if(TEST_PROTECT()) { \
        test_func(); \
    } \
    UnityConcludeTest(); \
}

#define PKMN_C_TEST_MAIN(...) \
    int main(int argc, char** argv) \
    { \
        (void)argc; \
        (void)argv; \
        srand((unsigned int)time(NULL)); \
        UnityBegin(__FILE__); \
        __VA_ARGS__; \
        return (UnityEnd()); \
    }

#endif /* PKMN_C_TESTS_COMMON_H */
