# CMake generated Testfile for 
# Source directory: /usr/local/src/cayleyizer
# Build directory: /usr/local/src/cayleyizer/Build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test("cpp_test" "/usr/local/src/cayleyizer/Build/cayleyizer_test")
set_tests_properties("cpp_test" PROPERTIES  _BACKTRACE_TRIPLES "/usr/local/src/cayleyizer/CMakeLists.txt;230;add_test;/usr/local/src/cayleyizer/CMakeLists.txt;0;")
add_test("python_test" "/snap/cmake/1572/bin/cmake" "-E" "env" "PYTHONPATH=/usr/local/src/cayleyizer/Build/python" "/usr/bin/python3.6m" "/usr/local/src/cayleyizer/python/test.py")
set_tests_properties("python_test" PROPERTIES  _BACKTRACE_TRIPLES "/usr/local/src/cayleyizer/CMakeLists.txt;238;add_test;/usr/local/src/cayleyizer/CMakeLists.txt;0;")
