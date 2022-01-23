
project(ft_irc)

#set(CMAKE_CXX_FLAGS " -Wall -Wextra -Werror -std=c++98 -pedantic")

add_executable(ft_irc 
        main.cpp
        Server.cpp
        Command.cpp
        Utils.cpp
        registrationCMD.cpp
        Channel.cpp
        Client.cpp
        modeCommand.cpp
        serverCommands.cpp)
