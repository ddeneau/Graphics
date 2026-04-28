#pragma once
#include <functional>
#include <map>
#include <stdexcept>

class CommandHandler {
public:
    using Command = std::function<void()>;

    CommandHandler() = default;

    void register_command(char key, Command cmd);
    bool execute(char key);
    bool has_command(char key) const;

private:
    std::map<char, Command> commands;
};