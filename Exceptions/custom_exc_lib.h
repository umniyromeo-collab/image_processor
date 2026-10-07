#pragma once

#include <string>
#include <exception>
#include <stdexcept>

class BadBMPParse final : public std::invalid_argument {
public:
    explicit BadBMPParse(const char* message) : std::invalid_argument("Parse process failed:" + std::string(message)) {
    }
    explicit BadBMPParse(const std::string& message)
        : std::invalid_argument("Parse process failed:" + std::string(message)) {
    }
};

class InvalidBMPFile final : public std::invalid_argument {
public:
    explicit InvalidBMPFile(const char* message) : std::invalid_argument("Invalid BMP file:" + std::string(message)) {
    }
    explicit InvalidBMPFile(const std::string& message) : std::invalid_argument("Invalid BMP file:" + message) {
    }
};

class InvalidFiltersStyle final : public std::invalid_argument {
public:
    explicit InvalidFiltersStyle(const char* message)
        : std::invalid_argument("Invalid Filter: " + std::string(message)) {
    }
    explicit InvalidFiltersStyle(const std::string& message) : std::invalid_argument("Invalid Filter: " + message) {
    }
};

class InvalidFilterArgs final : public std::invalid_argument {
public:
    explicit InvalidFilterArgs(const char* message)
        : std::invalid_argument("Invalid FilterArgs:" + std::string(message)) {
    }
    explicit InvalidFilterArgs(const std::string& message) : std::invalid_argument("Invalid FilterArgs:" + message) {
    }
};
