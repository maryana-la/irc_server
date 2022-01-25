
project(ft_irc)

set(CMAKE_CXX_FLAGS " -Wall -Wextra -Werror -std=c++98 -pedantic")
set(CMAKE_CXX_STANDARD 98)
add_executable(ft_irc 
        main.cpp
        Server.cpp
        Command.cpp
        Utils.cpp
        registrationCMD.cpp
        Channel.cpp
        Client.cpp
        modeCMD.cpp
        serverCommands.cpp
        channelCMD.cpp
        privmsgCMD.cpp
        Client.hpp Channel.hpp Utils.hpp ServerUtils.cpp extraCMD.cpp)


add_executable(bot
        bot/StartBot.cpp
        bot/Bot.cpp
        )