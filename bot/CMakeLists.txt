cmake_minimum_required(VERSION 3.20)
project(bot)

set(CMAKE_CXX_STANDARD 98)

add_executable(bot
        Bot.cpp
        Bot.hpp
        startBot.cpp)
