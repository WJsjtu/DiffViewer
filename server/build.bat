cmake -B ./Build -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=MINSIZEREL -DCMAKE_INSTALL_PREFIX=./Install
cmake --build ./Build --config MINSIZEREL --target install