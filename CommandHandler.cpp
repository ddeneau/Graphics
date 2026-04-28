#include "CommandHandler.h"

void CommandHandler::register_command(char key, Command cmd) {
    commands[key] = cmd;
}

bool CommandHandler::execute(char key) {
    auto it = commands.find(key);
    if (it != commands.end()) {
        it->second();
        return true;
    }
    return false;
}

bool CommandHandler::has_command(char key) const {
    return commands.find(key) != commands.end();
}