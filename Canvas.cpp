#include "Canvas.hpp"
#include "CanvasIterators.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace plotter
{

namespace fs = std::filesystem;

// Реализуйте методы класса Canvas в этом файле
Canvas::Canvas(int width, int height, char background /*= DEFAULT_BACKGROUND */)
    : width_(width)
    , height_(height)
    , background_(background)
{

    if (width_ < 0 || height_ < 0)
    {
        throw std::invalid_argument("Width and height can't be negative");
    }

    if (background_ == '\0')
    {
        throw std::invalid_argument("Background can't be set to null");
    }

    // Проверка переполнения Size
    if (width_ != 0 && std::numeric_limits<int>::max() / width < height)
    {
        throw std::invalid_argument("Width and height are too big.");
    }

    symbols_.assign(width_ * height_, background_);
}

Canvas::Canvas(Canvas&& other) noexcept
{
    if (other.symbols_.empty())
    {
        // Проверка консистентности
        assert(other.width_ == 0 && other.height_ == 0);
        std::swap(background_, other.background_);
        return;
    }

    Swap(other);
}

Canvas& Canvas::operator=(const Canvas& other)
{
    // copy and swap
    if (this == &other)
    {
        return *this;
    }

    if (other.symbols_.empty())
    {
        // Проверка консистентности
        assert(other.width_ == 0 && other.height_ == 0);
        symbols_.clear();
        width_ = other.width_;
        height_ = other.height_;
        background_ = other.background_;
    }
    else
    {
        auto copy(other);
        Swap(copy);
    }

    return *this;
}

Canvas& Canvas::operator=(Canvas&& other) noexcept
{
    if (this != &other)
    {
        Exchange(other);
    }

    return *this;
}

int Canvas::Width() const noexcept
{
    return width_;
}

int Canvas::Height() const noexcept
{
    return height_;
}

int Canvas::Size() const noexcept
{
    return width_ * height_;
}

char& Canvas::at(int x, int y)
{
    const size_t idx = GetPixelIndex(x, y);
    return symbols_.at(idx);
}

const char& Canvas::at(int x, int y) const
{
    const size_t idx = GetPixelIndex(x, y);
    return symbols_.at(idx);
}

char& Canvas::operator()(int x, int y) noexcept
{
    const size_t idx = GetPixelIndex(x, y);
    assert(IsPixelInBounds(idx));
    return symbols_[idx];
}

const char& Canvas::operator()(int x, int y) const noexcept
{
    const size_t idx = GetPixelIndex(x, y);
    assert(IsPixelInBounds(idx));
    return symbols_[idx];
}

void Canvas::Clear(char fill_char)
{
    symbols_.assign(width_ * height_, fill_char);
}

void Canvas::FillRegion(int x1, int y1, int x2, int y2, char fill_char) {
    if ((x1 > x2) || (y1 > y2))
    {
        throw std::runtime_error("Incorrect fill region");
    }

    int left   = std::max(0, x1);
    int right  = std::min(width_ - 1, x2);
    int top    = std::max(0, y1);
    int bottom = std::min(height_ - 1, y2);

    for (int i = top; i < height_ && i <= bottom; ++i)
    {
        for (int j = left; j < width_ && j <= right; ++j)
        {
            const size_t idx = GetPixelIndex(j, i);
            symbols_[idx] = fill_char;
        }
    }
}

bool Canvas::InBounds(int x, int y) const noexcept
{
    return (x >= 0) && (x < width_) && (y >= 0) && (y < height_);
}

void Canvas::Render(std::ostream& os /* = std::cout */) const
{
    for (int i = 0; i < height_; ++i)
    {
        for (int j = 0; j < width_; ++j)
        {
            const size_t idx = GetPixelIndex(j, i);
            os << symbols_[idx];
        }
        os << '\n';
    }
    os.flush();
}

void Canvas::SaveToFile(const fs::path& filepath) const
{
    if (filepath.empty())
    {
        throw std::runtime_error("Filepath is empty");
    }

    const auto fullpath = fs::absolute(filepath);
    const auto parent_path = fullpath.parent_path();
    fs::create_directories(parent_path);
    std::ofstream out(fullpath);

    if (!out)
    {
        throw std::runtime_error("Failed to create file '" + fullpath.string() + "'\n");
    }

    PrintHeader(out);
    Render(out);
    out.close();
}

void Canvas::SaveToFile(const std::string& filename) const
{
    SaveToFile(fs::path(filename));
}

Canvas::RowIterator Canvas::RowBegin(int row)
{
    return RowIterator(this, row, 0);
}

Canvas::RowIterator Canvas::RowEnd(int row)
{
    return RowIterator(this, row, width_);
}

Canvas::ColumnIterator Canvas::ColBegin(int col)
{
    return ColumnIterator(this, 0, col);
}

Canvas::ColumnIterator Canvas::ColEnd(int col)
{
    return ColumnIterator(this, height_, col);
}

Canvas::PixelIterator Canvas::begin()
{
    return PixelIterator(this, 0);
}

Canvas::PixelIterator Canvas::end()
{
    return PixelIterator(this, Size());
}

char& Canvas::GetPixel(size_t pos) noexcept
{
    assert(IsPixelInBounds(pos));
    return symbols_[pos];
}

const char& Canvas::GetPixel(size_t pos) const noexcept
{
    assert(IsPixelInBounds(pos));
    return symbols_[pos];
}

void Canvas::Swap(Canvas& other) noexcept
{
    symbols_.swap(other.symbols_);
    std::swap(width_, other.width_);
    std::swap(height_, other.height_);
    std::swap(background_, other.background_);
}

void Canvas::Exchange(Canvas& other) noexcept
{
    symbols_ = std::exchange(other.symbols_, {});
    width_ = std::exchange(other.width_, 0);
    height_ = std::exchange(other.height_, 0);
    background_ = std::exchange(other.background_, DEFAULT_BACKGROUND);
}

size_t Canvas::GetPixelIndex(int x, int y) const noexcept
{
    return static_cast<size_t>(y * width_ + x);
}

bool Canvas::IsPixelInBounds(size_t pos) const noexcept
{
    return pos < symbols_.size();
}

void Canvas::PrintHeader(std::ostream& os) const noexcept
{
    os << "Canvas " << width_ << 'x' << height_ << '\n'
        << "Background: '" << background_ << "'\n"
        << "Content:" << '\n';
}

} // namespace plotter
