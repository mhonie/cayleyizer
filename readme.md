cmake -S . -B build
cmake --build build

cmake --build build --target run_python_test

cmake --build build --target run_cpp_test

ctest --test-dir build --output-on-failure
