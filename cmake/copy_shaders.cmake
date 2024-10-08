# copy_shaders.cmake

function(copy_shaders)

    if(EXISTS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
        file(REMOVE_RECURSE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
    endif()

    # No need to create the directory, file(COPY) will do it without 'sudo' permissions
    # file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

    file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION ${CMAKE_BINARY_DIR})

    message(STATUS "Moved shaders directory to: ${CMAKE_BINARY_DIR}/shaders")

endfunction()