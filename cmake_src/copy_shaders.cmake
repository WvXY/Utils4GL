# copy_shaders.cmake

function(copy_shaders)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
    file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION ${CMAKE_BINARY_DIR})
    message(STATUS "Moved shaders directory to: ${CMAKE_BINARY_DIR}/shaders")
endfunction()