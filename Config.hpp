#pragma once
#include <stdexcept>
#include <map>
#include <string>
#include <vector>

namespace plotter
{

struct PlotterConfig
{
    int width = 0;
    int height = 0;
    char background_char = '\0';
    std::vector<char> palette;
    std::string plotter_type; // "basic" или "grayscale"
};

class Config
{
public:
    static PlotterConfig LoadFromFile(const std::string& filename);
    static PlotterConfig LoadFromString(std::istream& json_str);
    static bool ValidateConfig(const PlotterConfig& config);
    static PlotterConfig DefaultConfig();

private:
    static std::vector<char> ParsePalette(const std::string& palette_str);
};

class ConfigParserError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class NotFoundError : public ConfigParserError {
public:
    explicit NotFoundError(const std::string& item) : ConfigParserError(item  + " not found") {}
};

class WrongTypeError : public ConfigParserError {
public:
    WrongTypeError(const std::string& item, const std::string& expected_type)
        : ConfigParserError(item  + " is not of type '" + expected_type + "'") {}
};

} // namespace plotter