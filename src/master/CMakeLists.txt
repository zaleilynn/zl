SET(THRIFT_SOURCE_PREFIX ${CMAKE_SOURCE_DIR}/src/proxy)

SET(MASTER_THRIFT_SOURCE
    ${THRIFT_SOURCE_PREFIX}/master/gen-cpp/master_constants.cpp
    ${THRIFT_SOURCE_PREFIX}/master/gen-cpp/Master.h
    ${THRIFT_SOURCE_PREFIX}/master/gen-cpp/master_types.h
    ${THRIFT_SOURCE_PREFIX}/master/gen-cpp/master_constants.h
    ${THRIFT_SOURCE_PREFIX}/master/gen-cpp/Master.cpp
    ${THRIFT_SOURCE_PREFIX}/master/gen-cpp/master_types.cpp)

ADD_CUSTOM_COMMAND (
    OUTPUT ${MASTER_THRIFT_SOURCE}
    COMMAND thrift --gen cpp -r -o ${CMAKE_SOURCE_DIR}/src/proxy/master
    ${CMAKE_SOURCE_DIR}/src/proxy/master/master.thrift
)

SET(WORKER_THRIFT_SOURCE
    ${THRIFT_SOURCE_PREFIX}/worker/gen-cpp/worker_constants.cpp
    ${THRIFT_SOURCE_PREFIX}/worker/gen-cpp/Worker.h
    ${THRIFT_SOURCE_PREFIX}/worker/gen-cpp/worker_types.h
    ${THRIFT_SOURCE_PREFIX}/worker/gen-cpp/worker_constants.h
    ${THRIFT_SOURCE_PREFIX}/worker/gen-cpp/Worker.cpp
    ${THRIFT_SOURCE_PREFIX}/worker/gen-cpp/worker_types.cpp)

ADD_CUSTOM_COMMAND (
    OUTPUT ${WORKER_THRIFT_SOURCE}
    COMMAND thrift --gen cpp -r -o ${CMAKE_SOURCE_DIR}/src/proxy/worker
    ${CMAKE_SOURCE_DIR}/src/proxy/worker/worker.thrift
)

SET(SRC   
    ${MASTER_THRIFT_SOURCE}
    ${WORKER_THRIFT_SOURCE} 
    config.cpp
    service.cpp
    machine.cpp
    machine_pool.cpp
    virtual_cluster.cpp
    vc_pool.cpp
    task.cpp
    task_pool.cpp 
    task_queue.cpp
    work_thread.cpp
    master.cpp
    )

ADD_EXECUTABLE(master ${SRC})
TARGET_LINK_LIBRARIES(master thrift log4cplus boost_thread classad)


INSTALL(TARGETS master RUNTIME DESTINATION bin)
