set(target_name server)

include_directories(${Boost_INCLUDE_DIR})

add_executable(${target_name}
    main.cpp
    Server.cpp
    Stream.cpp
)

target_include_directories(${target_name} PUBLIC
    ${Pylon_INCLUDE_DIRS}
)

target_link_libraries(${target_name}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
    ${Pylon_LIBRARIES}
    boost_system
    pthread
)
