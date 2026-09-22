cmake_minimum_required(VERSION 3.27)
project(CppLearning)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

function(add_basic_exe target src)
    add_executable(${target} ${src} ${UTILS_FILES})
endfunction()

# basic
add_basic_exe(LinkedListSample linked_list_sample.cpp)

# multi-thread
add_basic_exe(ModelMultiThead model_multi_thread.cpp)
add_basic_exe(ModelActor model_actor.cpp)
add_basic_exe(ModelMsgPassing model_msg_passing.cpp)
add_basic_exe(ModelMemoryShared model_memory_shared.cpp)
add_basic_exe(ModelDataParallel model_data_parallel.cpp)

add_basic_exe(MultiThread1 multi_thread1.cpp)
add_basic_exe(ProducerConsumer producer_consumer.cpp)

# rapid
add_basic_exe(Traveler traveler.cpp)
