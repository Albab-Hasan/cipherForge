add_library(cipherforge STATIC)

target_include_directories(cipherforge PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_subdirectory(symmetric)
