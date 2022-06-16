set(target_name server)

add_executable(${target_name}
    Server.cpp
)

target_link_libraries(${target_name}
    ${Boost_LIBRARIES}
)
