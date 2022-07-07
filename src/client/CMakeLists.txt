set(target_name client)

include_directories(${Boost_INCLUDE_DIR})

add_executable(${target_name}
    main.cpp
)

target_link_libraries(${target_name}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
    boost_system
    pthread
)
