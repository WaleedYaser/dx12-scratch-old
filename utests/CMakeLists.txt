cmake_minimum_required(VERSION 3.10)

add_executable(utests utests_math.cpp)

# turns all warnings into errors
target_compile_options(utests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

target_link_libraries(utests PRIVATE doctest kuro)