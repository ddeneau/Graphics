#pragma once
#include <vector>

class KeyHistory {
private:
    std::vector<char> history;

public:
    KeyHistory() = default;

    void add(char key);
    void clear();
    const std::vector<char>& get_history() const;
    size_t size() const;
    bool is_empty() const;
};