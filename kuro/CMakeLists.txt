cmake_minimum_required(VERSION 3.10)

set(HEADER_FILES
    include/kuro_math.h
    include/kuro_os.h
    include/kuro_gfx.h
)

if (WIN32)
    set(SOURCE_FILES
        src/winos/kuro_gfx.cpp
        src/winos/kuro_os.cpp
    )
elseif(UNIX)
    set(SOURCE_FILES
        src/linux/kuro_os.cpp
    )
endif()

add_library(kuro
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# enable C++17
# disable any compiler specifc extensions
target_compile_features(kuro PUBLIC cxx_std_17)
set_target_properties(kuro PROPERTIES
    CXX_EXTENSIONS OFF
)

# define debug macro
target_compile_definitions(kuro PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# list include directories
target_include_directories(kuro
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(kuro
    PRIVATE
        $<$<PLATFORM_ID:Windows>:UNICODE;_UNICODE>
    PUBLIC
        $<$<PLATFORM_ID:Windows>:OS_WINDOWS=1>
        $<$<PLATFORM_ID:Linux>:OS_LINUX=1>
)

# turns all warnings into errors
target_compile_options(kuro PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)