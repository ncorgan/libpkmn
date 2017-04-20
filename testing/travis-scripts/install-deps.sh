#!/bin/bash
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD

sudo apt-get -y update
[ $? -ne 0 ] && exit 1
sudo apt-get -y install cmake cppcheck libboost1.55-all-dev libqt4-dev qtbase5-dev \
                        libgtest-dev swig doxygen \
			mono-complete monodevelop-nunit nunit-console \
			default-jdk junit maven \
			liblua5.1-dev luarocks \
			libpython-dev libpython3-dev python3 python-pip python3-pip \
			ruby${RUBY_VERSION} ruby${RUBY_VERSION}-dev
[ $? -ne 0 ] && exit 1
sudo pip${PYTHON_VERSION} install CppHeaderParser nose_parameterized
[ $? -ne 0 ] && exit 1
sudo luarocks install luaunit
[ $? -ne 0 ] && exit 1
sudo gem install os test-unit
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
