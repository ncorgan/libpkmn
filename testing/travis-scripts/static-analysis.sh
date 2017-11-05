#!/bin/bash
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env/pkmn-analysis-build
cd test-env/pkmn-analysis-build
[ $? -ne 0 ] && exit 1

echo Running CppCheck on C++ files.
find $REPO_TOPLEVEL/lib -name '*.[ch]pp' | xargs cppcheck --enable=performance,portability,warning --std=c++11 -I $REPO_TOPLEVEL/include --error-exitcode=1 --force --quiet
#[ $? -ne 0 ] && exit 1
echo Running CppCheck on C files.
find $REPO_TOPLEVEL/lib -name '*.[ch]' | xargs cppcheck --enable=performance,portability,warning --std=c99 -I $REPO_TOPLEVEL/include --error-exitcode=1 --force --quiet
#[ $? -ne 0 ] && exit 1

if [ $PYTHON_VERSION -eq 2 ]
then
    export PYTHONPATH=/usr/lib/python2.7/dist-packages:/usr/lib/python2.7/site-packages:$PYTHONPATH
    export PYTHONPATH=/usr/local/lib/python2.7/dist-packages:/usr/local/lib/python2.7/site-packages:$PYTHONPATH
    PYTHON_INCLUDE_DIR=/usr/include/python2.7
    PYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so
else
    export PYTHONPATH=/usr/lib/python3/dist-packages:/usr/lib/python3/site-packages:$PYTHONPATH
    export PYTHONPATH=/usr/local/lib/python3/dist-packages:/usr/local/lib/python3/site-packages:$PYTHONPATH
    PYTHON_INCLUDE_DIR=/usr/include/python3.4m
    PYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.4m.so
fi

echo Building LibPKMN with Clang-tidy analysis enabled.
cmake -DCMAKE_BUILD_TYPE=Release \
  -DPKMN_ENABLE_CSHARP=OFF \
  -DPKMN_ENABLE_LUA=OFF \
  -DPKMN_ENABLE_PYTHON=OFF \
  -DPKMN_ENABLE_RUBY=OFF \
  -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
  -DPYTHON_EXECUTABLE=/usr/bin/python${PYTHON_VERSION} \
  -DPKMN_USE_CLANGTIDY=ON \
  -DCLANG_TIDY_EXECUTABLE=$CLANG_TIDY_EXE \
  $REPO_TOPLEVEL
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
