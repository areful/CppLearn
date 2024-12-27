cmake_minimum_required(VERSION 3.27)
project(CppLearning)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(Main main.cpp)

# basic
add_executable(LinkedListSample basic/linked_list_sample.cpp)

# multi-thread
add_executable(ModelMultiThead multi_thread/model_multi_thread.cpp)
add_executable(ModelActor multi_thread/model_actor.cpp)
add_executable(ModelMsgPassing multi_thread/model_msg_passing.cpp)
add_executable(ModelMemoryShared multi_thread/model_memory_shared.cpp)
add_executable(ModelDataParallel multi_thread/model_data_parallel.cpp)

add_executable(MultiThread1 multi_thread/multi_thread1.cpp)
add_executable(ProducerConsumer multi_thread/producer_consumer.cpp)

add_executable(Traveler rapid/traveler.cpp)
