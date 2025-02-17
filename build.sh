: ${CMAKE_DIR:=cmake-build}

cmake -D CMAKE_BUILD_TYPE=Release -S . -B $CMAKE_DIR
cmake --build $CMAKE_DIR