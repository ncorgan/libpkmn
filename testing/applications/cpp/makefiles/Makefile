#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

pkmn-cpp-app: pkmn-cpp-app.cpp
	$(CXX) -o pkmn-cpp-app `pkg-config --cflags --libs pkmn` pkmn-cpp-app.cpp

clean:
	rm -f pkmn-cpp-app

.phony: clean
