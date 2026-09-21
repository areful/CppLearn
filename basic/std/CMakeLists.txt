cmake_minimum_required(VERSION 3.27)

function(add_basic_exe target src)
    add_executable(${target} ${src} ${UTILS_FILES})
    target_link_libraries(${target} PRIVATE eventpp::eventpp)
endfunction()

add_basic_exe(CppStdSample_shared_ptr shared_ptr_sample.cpp)
add_basic_exe(CppStdSample_make_shared make_shared_sample.cpp)
add_basic_exe(CppStdSample_reset reset_sample.cpp)
add_basic_exe(CppStdSample_weak_ptr weak_ptr_sample.cpp)
add_basic_exe(CppStdSample_lock_expired1 lock_expired_sample.cpp)
add_basic_exe(CppStdSample_lock_expired2 lock_expired_sample2.cpp)
add_basic_exe(CppStdSample_lock_expired3 lock_expired_sample3.cpp)
