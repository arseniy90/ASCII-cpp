#pragma once
#include <filesystem>
#include <iostream>
#include <vector>

namespace plotter
{

class Canvas
{
public:
    static constexpr char DEFAULT_BACKGROUND = ' ';

    class RowIterator;
    class ColumnIterator;
    class PixelIterator;

    Canvas(int width, int height, char background = DEFAULT_BACKGROUND);

    Canvas(const Canvas& other) = default;
    Canvas(Canvas&& other) noexcept;
    Canvas& operator=(const Canvas& other);
    Canvas& operator=(Canvas&& other) noexcept;

    [[nodiscard]] int Width() const noexcept;
    [[nodiscard]] int Height() const noexcept;
    [[nodiscard]] int Size() const noexcept;

    char& at(int x, int y);
    [[nodiscard]] const char& at(int x, int y) const;
    char& operator()(int x, int y) noexcept;
    [[nodiscard]] const char& operator()(int x, int y) const noexcept;

    void Clear(char fill_char);
    void FillRegion(int x1, int y1, int x2, int y2, char fill_char);

    [[nodiscard]] bool InBounds(int x, int y) const noexcept;

    void Render(std::ostream& os = std::cout) const;
    void SaveToFile(const std::filesystem::path& filepath) const;
    void SaveToFile(const std::string& filename) const;

    RowIterator RowBegin(int row);
    RowIterator RowEnd(int row);
    ColumnIterator ColBegin(int col);
    ColumnIterator ColEnd(int col);
    PixelIterator begin();
    PixelIterator end();

    // Возвращает «цвет» пикселя в указанной позиции.
    // Метод требуется для PixelIterator.
    char& GetPixel(size_t pos) noexcept;
    // Константный метод. Возвращает «цвет» пикселя в указанной позиции.
    [[nodiscard]] const char& GetPixel(size_t pos) const noexcept;

private:
    int width_ = 0;
    int height_ = 0;
    char background_ = DEFAULT_BACKGROUND;
    // Добавьте контейнер для хранения данных
    std::vector<char> symbols_;

    // Обменивает значение с другим канвасом
    void Swap(Canvas& other) noexcept;
    // Обменивает значение с другим канвасом, устанавливая в нем дефолтное состояние
    void Exchange(Canvas& other) noexcept;
    // Возвращает позицию пикселя с координатами x, y
    size_t GetPixelIndex(int x, int y) const noexcept;
    // Проверяет корректность позиции пикселя
    bool IsPixelInBounds(size_t pos) const noexcept;
    // Добавляет инфо в файл перед рисунком, как в DemoPrecode
    void PrintHeader(std::ostream& os) const noexcept;
};

} // namespace plotter
