#include "Config.hpp"
#include "json.h"

#include <fstream>

namespace
{

constexpr const char width[] = "width";
constexpr const char height[] = "height";
constexpr const char background_char[] = "background_char";
constexpr const char plotter_type[] = "plotter_type";
constexpr const char PALETTE[] = "palette";

constexpr const char string_type[] = "string";
constexpr const char int_type[] = "int";

constexpr const char basic_plotter_type[] = "basic";
constexpr const char grayscale_plotter_type[] = "grayscale";

constexpr int default_width = 48;
constexpr int default_height = 24;
constexpr char default_background_char = ' ';
constexpr const char default_plotter_type[] = "basic";

void CheckStringItem(const json::Dict& data, const std::string& item)
{
    if (data.count(item) == 0)
    {
        throw plotter::NotFoundError(item);
    }
    if (!data.at(item).IsString())
    {
        throw plotter::WrongTypeError(item, string_type);
    }
}

void CheckIntItem(const json::Dict& data, const std::string& item)
{
    if (data.count(item) == 0)
    {
        throw plotter::NotFoundError(item);
    }
    if (!data.at(item).IsInt())
    {
        throw plotter::WrongTypeError(item, int_type);
    }
}

} // anonymous namespace

namespace plotter
{

using namespace std::literals;

// Реализуйте методы класса Config
PlotterConfig Config::LoadFromFile(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        throw std::runtime_error("Failed to open file '" + filename + "'");
    }

    PlotterConfig config;
    try
    {
        config = Config::LoadFromString(in);
    }
    catch (const ConfigParserError& e)
    {
        std::cerr << "[JSON Config Parser] Error: " << e.what() << "\n";
        throw;
    }

    if (!Config::ValidateConfig(config))
    {
        throw std::runtime_error("Parsed from file '" + filename + " config is invalide");
    }

    return config;
}

PlotterConfig Config::LoadFromString(std::istream& json_str)
{
    json::Document doc = json::Load(json_str);
    const auto& root = doc.GetRoot();
    PlotterConfig config;

    if (!root.IsDict())
    {
        throw ConfigParserError("json root is not dict");
    }
    const auto& data = root.AsDict();

    CheckIntItem(data, width);
    config.width = data.at(width).AsInt();

    CheckIntItem(data, height);
    config.height = data.at(height).AsInt();

    CheckStringItem(data, background_char);
    const auto& background_char_str = data.at(background_char).AsString();

    if (background_char_str.size() != 1)
    {
        throw ConfigParserError(background_char + " is expected to have a single char."s);
    }
    config.background_char = background_char_str[0];

    CheckStringItem(data, plotter_type);
    config.plotter_type = data.at(plotter_type).AsString();

    if (data.count(PALETTE) == 1) {
        if (!data.at(PALETTE).IsString())
        {
            throw plotter::WrongTypeError(PALETTE, string_type);
        }
        const auto& palette_str = data.at(PALETTE).AsString();
        config.palette = ParsePalette(palette_str);
    }

    return config;
}

std::vector<char> Config::ParsePalette(const std::string& palette_str)
{
    std::vector<char> palette;
    palette.reserve(palette_str.size());
    for (const char symbol : palette_str)
    {
        palette.push_back(symbol);
    }

    return palette;
}

bool Config::ValidateConfig(const PlotterConfig& config)
{
    bool ok = true;
    if (config.width < 1)
    {
        std::cerr << "width can't be less than 1, got "s << config.width << std::endl;
        ok = false;
    }

    if (config.height < 1)
    {
        std::cerr << "height can't be less than 1, got "s << config.height << std::endl;
        ok = false;
    }

    if (config.background_char == '\0')
    {
        std::cerr << "expected background char not null"s << std::endl;
        ok = false;
    }

    if (config.plotter_type != basic_plotter_type && config.plotter_type != grayscale_plotter_type)
    {
        std::cerr << "expected plotter type 'basic' or 'grayscale', got "s << config.plotter_type << std::endl;
        ok = false;
    }

    for (const char item : config.palette)
    {
        if (item == '\0')
        {
            std::cerr << "expected palette char not null"s << std::endl;
            ok = false;
        }
    }

    return (ok);
}

PlotterConfig Config::DefaultConfig()
{
    return PlotterConfig
    {
        default_width,
        default_height,
        default_background_char,
        {},
        default_plotter_type,
    };
}

} // namespace plotter
