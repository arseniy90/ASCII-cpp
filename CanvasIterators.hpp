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

    RowIterator operator+(difference_type n) noexcept
    {
         return *this += n;
    }

    RowIterator operator-(difference_type n) noexcept
    {
         return *this -= n;
    }

    difference_type operator-(const RowIterator& other) noexcept
    {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && row_ == other.row_);
        return col_ - other.col_;
    }

    friend bool operator==(const RowIterator& lhs, const RowIterator& rhs) noexcept
    {
        return lhs.canvas_ == rhs.canvas_ && lhs.row_ == rhs.row_ && lhs.col_ == rhs.col_;
    }

    friend bool operator<(const RowIterator& lhs, const RowIterator& rhs) noexcept
    {
        // Проверка консистентности
        assert(lhs.canvas_ == rhs.canvas_ && lhs.row_ == rhs.row_);
        return lhs.col_ < rhs.col_;
    }

private:
    Canvas* canvas_;
    int row_;
    int col_;
};

inline bool operator!=(const Canvas::RowIterator& lhs, const Canvas::RowIterator& rhs) noexcept
{
    return !(lhs == rhs);
}

inline bool operator<=(const Canvas::RowIterator& lhs, const Canvas::RowIterator& rhs) noexcept
{
    return !(rhs < lhs);
}

inline bool operator>(const Canvas::RowIterator& lhs, const Canvas::RowIterator& rhs) noexcept
{
    return rhs < lhs;
}

inline bool operator>=(const Canvas::RowIterator& lhs, const Canvas::RowIterator& rhs) noexcept
{
    return !(lhs < rhs);
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

    ColumnIterator operator+(difference_type n) noexcept
    {
         return *this += n;
    }

    ColumnIterator operator-(difference_type n) noexcept
    {
         return *this -= n;
    }

    difference_type operator-(const ColumnIterator& other) noexcept
    {
        // Проверка консистентности
        assert(canvas_ == other.canvas_ && col_ == other.col_);
        return row_ - other.row_;
    }

    friend bool operator==(const ColumnIterator& lhs, const ColumnIterator& rhs) noexcept
    {
        return lhs.canvas_ == rhs.canvas_ && lhs.row_ == rhs.row_ && lhs.col_ == rhs.col_;
    }

    friend bool operator<(const ColumnIterator& lhs, const ColumnIterator& rhs) noexcept
    {
        // Проверка консистентности
        assert(lhs.canvas_ == rhs.canvas_ && lhs.col_ == rhs.col_);
        return lhs.row_ < rhs.row_;
    }

private:
    Canvas* canvas_;
    int col_;
    int row_;
};

inline bool operator!=(const Canvas::ColumnIterator& lhs, const Canvas::ColumnIterator& rhs) noexcept
{
    return !(lhs == rhs);
}

inline bool operator<=(const Canvas::ColumnIterator& lhs, const Canvas::ColumnIterator& rhs) noexcept
{
    return !(rhs < lhs);
}

inline bool operator>(const Canvas::ColumnIterator& lhs, const Canvas::ColumnIterator& rhs) noexcept
{
    return rhs < lhs;
}

inline bool operator>=(const Canvas::ColumnIterator& lhs, const Canvas::ColumnIterator& rhs) noexcept
{
    return !(lhs < rhs);
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

    PixelIterator operator+(difference_type n) noexcept
    {
        return *this += n;
    }

    PixelIterator operator-(difference_type n) noexcept
    {
        return *this -= n;
    }

    difference_type operator-(const PixelIterator& other) noexcept
    {
        // Проверка консистентности
        assert(canvas_ == other.canvas_);
        return pos_ - other.pos_;
    }

    bool operator==(const PixelIterator& other) const noexcept
    {
        return canvas_ == other.canvas_ && pos_ == other.pos_;
    }

    bool operator<(const PixelIterator& other) const noexcept
    {
        return pos_ < other.pos_;
    }

private:
    Canvas* canvas_;
    size_t pos_;
};

inline bool operator!=(const Canvas::PixelIterator& lhs, const Canvas::PixelIterator& rhs) noexcept
{
    return !(lhs == rhs);
}

inline bool operator<=(const Canvas::PixelIterator& lhs, const Canvas::PixelIterator& rhs) noexcept
{
    return !(rhs < lhs);
}

inline bool operator>(const Canvas::PixelIterator& lhs, const Canvas::PixelIterator& rhs) noexcept
{
    return rhs < lhs;
}

inline bool operator>=(const Canvas::PixelIterator& lhs, const Canvas::PixelIterator& rhs) noexcept
{
    return !(lhs < rhs);
}

} // namespace plotter
