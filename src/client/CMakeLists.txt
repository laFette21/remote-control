set(target_name client)

add_executable(${target_name}
    Client.cpp
)

target_link_libraries(${target_name}
    ${Boost_LIBRARIES}
)
