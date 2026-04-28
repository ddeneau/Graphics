#include "KeyHistory.h"

void KeyHistory::add(char key) {
    history.push_back(key);
}

void KeyHistory::clear() {
    history.clear();
}

const std::vector<char>& KeyHistory::get_history() const {
    return history;
}

size_t KeyHistory::size() const {
    return history.size();
}

bool KeyHistory::is_empty() const {
    return history.empty();
}