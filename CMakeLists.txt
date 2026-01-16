cmake_minimum_required(VERSION 3.20)

project(DitherUtils)
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Fix lib includes
target_include_directories(${PROJECT_NAME} PUBLIC
    ${terminal_ctrl_SOURCE_DIR}/include
)

# Set the folder where the executable is created
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add all the source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Enable many good warnings.
# /WX and -Werror enable "warnings as errors": This means that your code won't compile if you have any warning.
# This forces you to take warnings into account, which is a good practice because warnings are here for a reason and can save you from a lot of bugs!
# If this is too strict for you, you can remove /WX and -Werror.
if(MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE /WX /W4)
else()
   target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wimplicit-fallthrough)
endif()

