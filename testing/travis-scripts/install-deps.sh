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
sudo apt-get -y install cmake cppcheck libqt4-dev qtbase5-dev gnustep \
                        swig swig3.0 doxygen \
			mono-complete monodevelop-nunit nunit-console \
			default-jdk junit maven \
			liblua5.1-dev luarocks \
			libpython-dev libpython3-dev python3 python-pip python3-pip \
			ruby ruby-all-dev
[ $? -ne 0 ] && exit 1
if [ "$NEWCXX" = "" ]
then
    sudo apt-get -y install libboost1.55-all-dev
    [ $? -ne 0 ] && exit 1
fi
sudo pip${PYTHON_VERSION} install CppHeaderParser nose_parameterized
[ $? -ne 0 ] && exit 1
sudo luarocks install luaunit
[ $? -ne 0 ] && exit 1
sudo gem install minitest os test-unit
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
