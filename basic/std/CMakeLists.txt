cmake_minimum_required(VERSION 3.27)

function(add_basic_exe target src)
    add_executable(${target} ${src} ${UTILS_FILES})
    target_link_libraries(${target} PRIVATE eventpp::eventpp)
endfunction()

add_basic_exe(CppStdSample_shared_ptr shared_ptr_sample.cpp)
