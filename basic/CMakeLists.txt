cmake_minimum_required(VERSION 3.27)
project(CppLearning)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# basic
add_executable(LinkedListSample linked_list_sample.cpp)

# multi-thread
add_executable(ModelMultiThead model_multi_thread.cpp)
add_executable(ModelActor model_actor.cpp)
add_executable(ModelMsgPassing model_msg_passing.cpp)
add_executable(ModelMemoryShared model_memory_shared.cpp)
add_executable(ModelDataParallel model_data_parallel.cpp)

add_executable(MultiThread1 multi_thread1.cpp)
add_executable(ProducerConsumer producer_consumer.cpp)

# rapid
add_executable(Traveler traveler.cpp)
