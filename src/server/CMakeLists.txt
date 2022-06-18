set(target_name server)

include_directories(${Boost_INCLUDE_DIR})

add_executable(${target_name}
    main.cpp
    Server.cpp
)

target_link_libraries(${target_name}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
)
