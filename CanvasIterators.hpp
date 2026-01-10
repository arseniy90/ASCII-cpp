#pragma once
#include "Canvas.hpp"
#include <cassert>
#include <stdexcept>

namespace plotter
{

class Canvas::RowIterator
{
public:
    // Реализуйте методы класса
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = char;
    using difference_type   = std::ptrdiff_t;
    using pointer           = char*;
    using reference         = char&;

    RowIterator(Canvas* canvas, int row, int col)
        : canvas_(canvas)
        , row_(row)
        , col_(col)

    {
        if (!canvas_)
        {
            throw std::invalid_argument("Canvas cannot be null");
        }
        if (row_ < 0 || row_ > canvas_->Height())
        {
            throw std::invalid_argument("Row can't be negative or more than height");
        }
        if (col_ < 0 || col_ > canvas_->Width())
        {
            throw std::invalid_argument("Col can't be negative or more than width");
        }
    }

    reference operator*() const noexcept
    {
        return  (*canvas_)(col_, row_);
    }

    pointer operator->() const noexcept
    {
        return &(*canvas_)(col_, row_);
    }

    reference operator[](difference_type n) const noexcept
    {
        return (*canvas_)(col_ + n, row_);
    }

    RowIterator& operator++() noexcept
    {
        ++col_;
        return *this;
    }

    RowIterator operator++(int) noexcept
    {
        RowIterator tmp(*this);
        ++(*this);
        return tmp;
    }

    RowIterator& operator--() noexcept
    {
        --col_;
        return *this;
    }

    RowIterator operator--(int) noexcept
    {
        RowIterator tmp(*this);
        --(*this);
        return tmp;
    }

    RowIterator& operator+=(difference_type n) noexcept
    {
        col_ += n;
        return *this;
    }

    RowIterator& operator-=(difference_type n) noexcept
    {
        col_ -= n;
        return *this;
    }

    RowIterator operator+(difference_type n) const noexcept {
        RowIterator tmp = *this;
        tmp += n;
        return tmp;
    }

    RowIterator operator-(difference_type n) const noexcept {
        RowIterator tmp = *this;
        tmp -= n;
        return tmp;
    }

    difference_type operator-(const RowIterator& other) const noexcept
    {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && row_ == other.row_);
        return static_cast<difference_type>(col_) - static_cast<difference_type>(other.col_);
    }

    bool operator==(const RowIterator& other) const noexcept {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && row_ == other.row_);
        return col_ == other.col_;
    }

    auto operator<=>(const RowIterator& other) const noexcept {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && row_ == other.row_);
        return col_ <=> other.col_;
    }

private:
    Canvas* canvas_ = nullptr;
    int row_ = 0;
    int col_ = 0;
};


inline Canvas::RowIterator operator+(Canvas::RowIterator::difference_type n, Canvas::RowIterator it) noexcept {
    return it + n;
}

class Canvas::ColumnIterator
{
public:
    // Реализуйте методы класса
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = char;
    using difference_type   = std::ptrdiff_t;
    using pointer           = char*;
    using reference         = char&;

    ColumnIterator(Canvas* canvas, int col, int row)
    : canvas_(canvas)
    , col_(col)
    , row_(row)
    {
        if (!canvas_)
        {
            throw std::invalid_argument("Canvas cannot be null");
        }
        if (row_ < 0 || row_ > canvas_->Height())
        {
            throw std::invalid_argument("Row can't be negative or more than height");
        }
        if (col_ < 0 || col_ > canvas_->Width())
        {
            throw std::invalid_argument("Col can't be negative or more than width");
        }
    }

    reference operator*() const noexcept
    {
        return  (*canvas_)(col_, row_);
    }

    pointer operator->() const noexcept
    {
        return &(*canvas_)(col_, row_);
    }

    reference operator[](difference_type n) const noexcept
    {
        return (*canvas_)(col_, row_ + n);
    }

    ColumnIterator& operator++() noexcept
    {
        ++row_;
        return *this;
    }

    ColumnIterator operator++(int) noexcept
    {
        ColumnIterator tmp(*this);
        ++(*this);
        return tmp;
    }

    ColumnIterator& operator--() noexcept
    {
        --row_;
        return *this;
    }

    ColumnIterator operator--(int) noexcept
    {
        ColumnIterator tmp(*this);
        --(*this);
        return tmp;
    }

    ColumnIterator& operator+=(difference_type n) noexcept
    {
        row_ += n;
        return *this;
    }

    ColumnIterator& operator-=(difference_type n) noexcept
    {
        row_ -= n;
        return *this;
    }

    ColumnIterator operator+(difference_type n) const noexcept
    {
        ColumnIterator tmp = *this;
        tmp += n;
        return tmp;
    }

    ColumnIterator operator-(difference_type n) const noexcept
    {
        ColumnIterator tmp = *this;
        tmp -= n;
        return tmp;
    }

    difference_type operator-(const ColumnIterator& other) const noexcept
    {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && col_ == other.col_);
        return static_cast<difference_type>(row_) - static_cast<difference_type>(other.row_);
    }

    bool operator==(const ColumnIterator& other) const noexcept {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && col_ == other.col_);
        return row_ == other.row_;
    }

    auto operator<=>(const ColumnIterator& other) const noexcept {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && col_ == other.col_);
        return row_ <=> other.row_;
    }

private:
    Canvas* canvas_ = nullptr;
    int col_ = 0;
    int row_ = 0;
};

inline Canvas::ColumnIterator operator+(Canvas::ColumnIterator::difference_type n, Canvas::ColumnIterator it) noexcept {
    return it + n;
}

class Canvas::PixelIterator
{
public:
    // Реализуйте методы класса
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = char;
    using difference_type   = std::ptrdiff_t;
    using pointer           = char*;
    using reference         = char&;

    explicit PixelIterator(Canvas* canvas, size_t pos = 0)
        : canvas_(canvas), pos_(pos)
    {
        if (!canvas_)
        {
            throw std::invalid_argument("Canvas cannot be null");
        }
        if (pos > static_cast<size_t>(canvas_->Size()))
        {
            throw std::invalid_argument("Pos can't be negative or more than size");
        }
    }

    reference operator*() const noexcept
    {
        return canvas_->GetPixel(pos_);
    }

    pointer operator->() const noexcept
    {
        return &canvas_->GetPixel(pos_);
    }

    reference operator[](difference_type n) const noexcept
    {
        return canvas_->GetPixel(pos_ + n);
    }

    PixelIterator& operator++() noexcept
    {
        ++pos_;
        return *this;
    }

    PixelIterator operator++(int) noexcept
    {
        PixelIterator tmp(*this);
        ++(*this);
        return tmp;
    }

    PixelIterator& operator--() noexcept
    {
        --pos_;
        return *this;
    }

    PixelIterator operator--(int) noexcept
    {
        PixelIterator tmp(*this);
        --(*this);
        return tmp;
    }

    PixelIterator& operator+=(difference_type n) noexcept
    {
        pos_ += n;
        return *this;
    }

    PixelIterator& operator-=(difference_type n) noexcept
    {
        pos_ -= n;
        return *this;
    }

    PixelIterator operator+(difference_type n) const noexcept
    {
        PixelIterator tmp = *this;
        tmp += n;
        return tmp;
    }

    PixelIterator operator-(difference_type n) const noexcept
    {
        PixelIterator tmp = *this;
        tmp -= n;
        return tmp;
    }

    difference_type operator-(const PixelIterator& other) const noexcept
    {
        // Проверка консистентности
        assert(canvas_ == other.canvas_);
        return static_cast<difference_type>(pos_) - static_cast<difference_type>(other.pos_);
    }

    bool operator==(const PixelIterator& other) const noexcept {
        // Проверка консистентности
        assert(canvas_ == other.canvas_);
        return pos_ == other.pos_;
    }

    auto operator<=>(const PixelIterator& other) const noexcept {
        // Проверка консистентности
        assert(canvas_ == other.canvas_);
        return pos_ <=> other.pos_;
    }

private:
    Canvas* canvas_;
    size_t pos_;
};

inline Canvas::PixelIterator operator+(Canvas::PixelIterator::difference_type n, Canvas::PixelIterator it) noexcept {
    return it + n;
}

} // namespace plotter
