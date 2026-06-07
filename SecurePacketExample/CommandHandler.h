#pragma once
#include "Includes.h"   
#include <unordered_map>

class CommandHandler
{
public:

    using CommandFn = std::function<std::string(const std::string& args)>;

    void Register(const std::string& name, CommandFn fn)
    {
        commands_[name] = fn;
    }

    std::string Execute(const std::string& input)
    {
        auto space = input.find(' ');

        std::string cmd = (space == std::string::npos) ? input : input.substr(0, space);
        std::string args = (space == std::string::npos) ? "" : input.substr(space + 1);

        auto it = commands_.find(cmd);

        if (it != commands_.end())
            return it->second(args);

        return "Unknown command: " + cmd;
    }


    void RegisterCommands()
    {
        Register("ping",
                 [](const std::string&)
        {
            return "pong";
        });

        Register("echo",
                 [](const std::string& args)
        {
            return "ECHO Called: " + args;
        });

        Register("help",
                 [](const std::string&)
        {
            return "Available Commands: ping, echo, help";
        });
    }

private:
    std::unordered_map<std::string, CommandFn> commands_;
};