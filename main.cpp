#include "DemoRunner.hpp"

// Linux, C++20. (Изначально делал на C++17)
// Названия файлов не менял и заданные сущности не именял за исключением DemoRunner.
// Добавил проверку результатов.
// Проверял DemoRunner локально с использованием sanitizers.
// В задании не требовалось, добавил юнит тесты с кастомным test runner, закомменчены в main.
// Обычно использую Boost, в данной работе предполагаю не требовалось использовать дополнительные библиотеки.

#include "test_runner.h"
#include "Canvas.hpp"
#include "CanvasIterators.hpp"
#include "Config.hpp"
#include "GrayscalePlotter.hpp"
#include <algorithm>

using namespace plotter;

void TestRowIterator() {
    Canvas canvas(3, 1, '.');
    canvas(0, 0) = 'a';
    canvas(1, 0) = 'b';
    canvas(2, 0) = 'c';

    auto it = canvas.RowBegin(0);
    ASSERT_EQUAL(*it, 'a');
    ASSERT_EQUAL(*(it + 1), 'b');
    ASSERT_EQUAL(it[2], 'c');

    auto it_begin = it;
    auto it_end = canvas.RowEnd(0);
    ASSERT(it_begin != it_end);
    ASSERT(it_begin < it_end);
    ASSERT(it_begin + 3 == it_end);
    ASSERT_EQUAL(it_end - it_begin, 3);

    ASSERT_DOESNT_THROW(canvas.RowEnd(1));
    ASSERT_THROWS(canvas.RowBegin(2), std::invalid_argument);
    ASSERT_THROWS(Canvas::RowIterator(&canvas, 0, 4), std::invalid_argument);
}

void TestColumnIterator() {
    Canvas canvas(2, 3, '.');
    canvas(1, 0) = 'a';
    canvas(1, 1) = 'b';
    canvas(1, 2) = 'c';

    auto it = canvas.ColBegin(1);
    ASSERT_EQUAL(*it, 'a');
    ASSERT_EQUAL(*(it + 1), 'b');
    ASSERT_EQUAL(it[2], 'c');

    auto it_begin = it;
    auto it_end = canvas.ColEnd(1);
    ASSERT(it_begin != it_end);
    ASSERT(it_begin < it_end);
    ASSERT(it_begin + 3 == it_end);
    ASSERT_EQUAL(it_end - it_begin, 3);

    ASSERT_DOESNT_THROW(canvas.ColEnd(2));
    ASSERT_THROWS(canvas.ColBegin(3), std::invalid_argument);
    ASSERT_THROWS(Canvas::ColumnIterator(&canvas, 0, 4), std::invalid_argument);
}

void TestPixelIterator() {
    Canvas canvas(2, 5, 'x');
    canvas(0, 0) = '1';
    canvas(1, 1) = '4';

    auto it = canvas.begin();
    ASSERT_EQUAL(*it, '1');
    ASSERT_EQUAL(it[2], 'x');
    ASSERT_EQUAL(*(it + 3), '4');

    auto it_begin = it;
    auto it_end = canvas.end();
    ASSERT(it_begin != it_end);
    ASSERT(it_begin < it_end);
    ASSERT(it_begin + 10 == it_end);
    ASSERT_EQUAL(it_end - it_begin, 10);

    std::fill(canvas.begin(), canvas.end(), 'a');
    for (char pixel : canvas)
    {
        ASSERT_EQUAL(pixel, 'a');
    }
}

void TestGrayscalePlotter() {
    {
        std::vector<char> palette = {' ', '.', '#'};
        GrayscalePlotter plotter(5, 5, ' ', palette);

        ASSERT_EQUAL(plotter.BrightnessToChar(0.0), ' ');
        ASSERT_EQUAL(plotter.BrightnessToChar(1.0), '#');
        ASSERT_EQUAL(plotter.BrightnessToChar(0.5), '.');
        ASSERT_EQUAL(plotter.BrightnessToChar(-1.0), ' ');
        ASSERT_EQUAL(plotter.BrightnessToChar(2.0), '#');
    }

    {
        GrayscalePlotter plotter(5, 5, ' ', {});

        ASSERT_EQUAL(plotter.BrightnessToChar(0.0), ' ');
        ASSERT_EQUAL(plotter.BrightnessToChar(1.0), '@');
        ASSERT_EQUAL(plotter.BrightnessToChar(-1.0), ' ');
        ASSERT_EQUAL(plotter.BrightnessToChar(2.0), '@');
    }
}

void TestCanvas() {
    Canvas c(3, 2, '.');
    ASSERT_EQUAL(c.Width(), 3);
    ASSERT_EQUAL(c.Height(), 2);
    ASSERT_EQUAL(c.Size(), 6);

    c(1, 1) = 'x';
    ASSERT_EQUAL(c.at(1, 1), 'x');

    ASSERT(c.InBounds(0, 0));
    ASSERT(c.InBounds(2, 1));
    ASSERT(!c.InBounds(0, 2));
    ASSERT(!c.InBounds(3, 0));
    ASSERT(!c.InBounds(-1, 0));
    ASSERT(!c.InBounds(0, -1));

    c.FillRegion(-2, -2, 1, 1, 'x');
    ASSERT_EQUAL(c(0,0), 'x');
    ASSERT_EQUAL(c(1,1), 'x');
    ASSERT_EQUAL(c(2,1), '.');

    ASSERT_THROWS(c.FillRegion(3,3,1,1,'x'), std::runtime_error);

    ASSERT_THROWS(Canvas(0, 1), std::invalid_argument);
    ASSERT_THROWS(Canvas(1, 0), std::invalid_argument);
    ASSERT_THROWS(Canvas(1, 1, '\0'), std::invalid_argument);

}

void TestConfigParsing() {
    {
        std::string json = R"({
            "width": 10,
            "height": 5,
            "background_char": "@",
            "plotter_type": "grayscale",
            "palette": " .#"
        })";

        std::stringstream ss(json);
        auto cfg = Config::LoadFromString(ss);

        ASSERT_EQUAL(cfg.width, 10);
        ASSERT_EQUAL(cfg.height, 5);
        ASSERT_EQUAL(cfg.background_char, '@');
        ASSERT_EQUAL(cfg.plotter_type, "grayscale");
        ASSERT_EQUAL(cfg.palette.size(), 3u);
    }

    {
        auto cfg = Config::DefaultConfig();
        ASSERT(Config::ValidateConfig(cfg));
    }

    {
        std::string json = R"({"height": 10, "background_char": " ", "plotter_type": "grayscale"})";
        std::stringstream ss(json);
        ASSERT_THROWS(Config::LoadFromString(ss), ConfigParserError);
    }

    {
        std::string json = R"({"width": "100", "height": 50})";
        std::stringstream ss(json);
        ASSERT_THROWS(Config::LoadFromString(ss), ConfigParserError);
    }

    {
        PlotterConfig cfg;
        cfg.width = 10;
        cfg.height = 10;
        cfg.background_char = '\0';
         cfg.plotter_type = "basic";
        ASSERT(!Config::ValidateConfig(cfg));
    }

    {
        PlotterConfig cfg;
        cfg.width = 10;
        cfg.height = 10;
        cfg.background_char = 'a';
        cfg.plotter_type = "unknown";
        ASSERT(!Config::ValidateConfig(cfg));
    }
}

void TestFileSystem() {
    {
        Canvas canvas(2, 2, '#');
        std::string path = "./test_output_results_dir/sub_folder/frame.txt";

        std::filesystem::remove_all("test_output_results_dir");

        ASSERT_DOESNT_THROW(canvas.SaveToFile(path));
        ASSERT(std::filesystem::exists(path));

        std::filesystem::remove_all("test_output_results_dir");
    }

    {
        Canvas canvas(2, 2, '#');
        std::filesystem::path path("./test_output_results_dir/sub_folder/frame.txt");

        std::filesystem::remove_all("test_output_results_dir");

        ASSERT_DOESNT_THROW(canvas.SaveToFile(path));
        ASSERT(std::filesystem::exists(path));

        std::filesystem::remove_all("test_output_results_dir");
    }
}

int main() {
    // TestRunner tr;

    // RUN_TEST(tr, TestRowIterator);
    // RUN_TEST(tr, TestColumnIterator);
    // RUN_TEST(tr, TestPixelIterator);
    // RUN_TEST(tr, TestGrayscalePlotter);
    // RUN_TEST(tr, TestCanvas);
    // RUN_TEST(tr, TestConfigParsing);
    // RUN_TEST(tr, TestFileSystem);

    DemoRunner::RunAllDemos();
}