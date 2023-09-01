#pragma once

#include "vec2.h"

#include <cassert>
#include <climits>
#include <cstdint>

#include <bit>
#include <set>
#include <vector>

enum Direction
{
    right,
    upright,
    up,
    upleft,
    left,
    downleft,
    down,
    downright,
};

class BitBoard
{
  public:
    using Position = dm::Vec2<int>;
    using Bits = std::uint64_t;

    static constexpr int board_size = 8;
    static constexpr std::size_t n_bits = 64;
    static_assert(sizeof(Bits) * CHAR_BIT == n_bits, "number of bits mismatch");

    constexpr explicit BitBoard() noexcept : BitBoard(0) {}
    constexpr explicit BitBoard(const Bits bits) noexcept : bits_(bits) {}
    constexpr explicit BitBoard(const Position& position) : BitBoard(from_position(position)) {}
    explicit BitBoard(const std::string& board);

    constexpr BitBoard(const BitBoard& other) = default;
    constexpr BitBoard& operator=(const BitBoard&) = default;

    constexpr BitBoard(BitBoard&& other) = default;
    constexpr BitBoard& operator=(BitBoard&&) = default;

    static constexpr BitBoard make_top_right() noexcept
    {
        return BitBoard{top_right};
    }
    static constexpr BitBoard make_top_left() noexcept
    {
        return BitBoard{top_left};
    }
    static constexpr BitBoard make_bottom_left() noexcept
    {
        return BitBoard{bottom_left};
    }
    static constexpr BitBoard make_bottom_right() noexcept
    {
        return BitBoard{bottom_right};
    }
    static constexpr BitBoard make_right_edge() noexcept
    {
        return BitBoard{right_edge};
    }
    static constexpr BitBoard make_top_right_edge() noexcept
    {
        return BitBoard{top_right_edge};
    }
    static constexpr BitBoard make_top_edge() noexcept
    {
        return BitBoard{top_edge};
    }
    static constexpr BitBoard make_top_left_edge() noexcept
    {
        return BitBoard{top_left_edge};
    }
    static constexpr BitBoard make_left_edge() noexcept
    {
        return BitBoard{left_edge};
    }
    static constexpr BitBoard make_bottom_left_edge() noexcept
    {
        return BitBoard{bottom_left_edge};
    }
    static constexpr BitBoard make_bottom_edge() noexcept
    {
        return BitBoard{bottom_edge};
    }
    static constexpr BitBoard make_bottom_right_edge() noexcept
    {
        return BitBoard{bottom_right_edge};
    }
    static constexpr BitBoard make_all_edge() noexcept
    {
        return BitBoard{all_edge};
    }
    static constexpr BitBoard make_positive_slope() noexcept
    {
        return BitBoard{positive_slope};
    }
    static constexpr BitBoard make_negative_slope() noexcept
    {
        return BitBoard{negative_slope};
    }
    static constexpr BitBoard make_full() noexcept
    {
        return ~(BitBoard{});
    }
    static constexpr BitBoard make_row(const size_t n) noexcept
    {
        return BitBoard::shift<Direction::down>(make_top_edge(), n);
    }
    static constexpr BitBoard make_column(const size_t n) noexcept
    {
        return BitBoard::shift<Direction::right>(make_left_edge(), n);
    }

    template <Direction D>
    [[nodiscard]] static constexpr BitBoard shift(BitBoard board, const size_t n = 1)
    {
        return board.shift_assign<D>(n);
    }
    [[nodiscard]] static BitBoard shift(BitBoard board, Direction direction, size_t n = 1);
    [[nodiscard]] static BitBoard shift(BitBoard board, Position relative_offset);

    static BitBoard neighbors_cardinal(BitBoard position) noexcept;
    static BitBoard neighbors_cardinal(const Position& position) noexcept;
    static BitBoard neighbors_diagonal(BitBoard position) noexcept;
    static BitBoard neighbors_diagonal(const Position& position) noexcept;
    static BitBoard neighbors_cardinal_and_diagonal(BitBoard position) noexcept;
    static BitBoard neighbors_cardinal_and_diagonal(const Position& position) noexcept;

    [[nodiscard]] bool test(const Position& position) const noexcept
    {
        return test_any(BitBoard{position});
    }

    [[nodiscard]] bool test_any(const BitBoard other) const noexcept
    {
        return !(*this & other).empty();
    }

    [[nodiscard]] bool test_all(const BitBoard other) const noexcept
    {
        return (*this & other) == other;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return bits_ == 0U;
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept
    {
        return std::popcount(bits_);
    }

    [[nodiscard]] constexpr bool has_single_position() const noexcept
    {
        return std::has_single_bit(bits_);
    }

    BitBoard& set(const BitBoard other) noexcept
    {
        bits_ |= other.bits_;
        return *this;
    }

    BitBoard& set(const Position& position)
    {
        return set(BitBoard{position});
    }

    BitBoard& clear(const BitBoard other) noexcept
    {
        *this &= ~other;
        return *this;
    }

    BitBoard& clear(const Position& position)
    {
        return clear(BitBoard{position});
    }

    BitBoard& clear_all() noexcept
    {
        bits_ = 0U;
        return *this;
    }

    template <Direction D>
    [[nodiscard]] bool on_edge() const noexcept;

    [[nodiscard]] bool on_edge(Direction direction) const noexcept;

    [[nodiscard]] bool on_any_edge() const noexcept;

    template <Direction D>
    constexpr BitBoard& shift_assign(size_t n = 1) noexcept;

    BitBoard& shift_assign(Direction direction, size_t n = 1) noexcept;

    BitBoard& shift_assign(Position relative_offset) noexcept;

    template <Direction D>
    BitBoard& dilate() noexcept
    {
        return *this |= BitBoard::shift<D>(*this);
    }

    template <Direction D>
    BitBoard& dilate(const size_t n) noexcept
    {
        for (size_t i = 0; i < n; i++) {
            dilate<D>();
        }
        return *this;
    }

    BitBoard& dilate(Direction direction, size_t n = 1) noexcept;

    [[nodiscard]] constexpr unsigned long long to_ullong() const
    {
        return bits_;
    }
    [[nodiscard]] constexpr Position to_position() const;
    [[nodiscard]] std::vector<Position> to_position_vector() const noexcept;
    [[nodiscard]] std::vector<BitBoard> to_bitboard_vector() const noexcept;
    [[nodiscard]] std::set<Position> to_position_set() const noexcept;

    [[nodiscard]] std::string to_string() const noexcept;

    constexpr BitBoard& operator<<=(size_t n)
    {
        bits_ <<= n;
        return *this;
    }
    [[nodiscard]] constexpr BitBoard operator<<(size_t n) const
    {
        return BitBoard{*this} <<= n;
    }
    constexpr BitBoard& operator>>=(size_t n)
    {
        bits_ >>= n;
        return *this;
    }
    [[nodiscard]] constexpr BitBoard operator>>(size_t n) const
    {
        return BitBoard{*this} >>= n;
    }
    constexpr BitBoard& operator|=(const BitBoard other)
    {
        bits_ |= other.bits_;
        return *this;
    }
    [[nodiscard]] constexpr BitBoard operator|(const BitBoard other) const
    {
        return BitBoard{*this} |= other;
    }
    constexpr BitBoard& operator&=(const BitBoard other)
    {
        bits_ &= other.bits_;
        return *this;
    }
    [[nodiscard]] constexpr BitBoard operator&(const BitBoard other) const
    {
        return BitBoard{*this} &= other;
    }
    constexpr BitBoard& operator^=(const BitBoard other)
    {
        bits_ ^= other.bits_;
        return *this;
    }
    [[nodiscard]] constexpr BitBoard operator^(const BitBoard other) const
    {
        return BitBoard{bits_ ^ other.bits_};
    }
    [[nodiscard]] constexpr BitBoard operator~() const
    {
        return BitBoard{~bits_};
    }

    [[nodiscard]] constexpr friend bool operator==(const BitBoard lhs, const BitBoard rhs)
    {
        return lhs.bits_ == rhs.bits_;
    }
    [[nodiscard]] constexpr friend bool operator!=(const BitBoard lhs, const BitBoard rhs)
    {
        return !(lhs == rhs);
    }
    [[nodiscard]] constexpr friend bool operator<(const BitBoard lhs, const BitBoard rhs)
    {
        return lhs.bits_ < rhs.bits_;
    }
    [[nodiscard]] constexpr friend bool operator>(const BitBoard lhs, const BitBoard rhs)
    {
        return rhs < lhs;
    }
    [[nodiscard]] constexpr friend bool operator<=(const BitBoard lhs, const BitBoard rhs)
    {
        return !(lhs > rhs);
    }
    [[nodiscard]] constexpr friend bool operator>=(const BitBoard lhs, const BitBoard rhs)
    {
        return !(lhs < rhs);
    }

  private:
    Bits bits_;

    static constexpr Bits top_right = 0b00000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    static constexpr Bits top_left = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    static constexpr Bits bottom_left = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000000;
    static constexpr Bits bottom_right = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
    static constexpr Bits top_edge = 0b11111111'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    static constexpr Bits bottom_edge = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;
    static constexpr Bits left_edge = 0b10000000'10000000'10000000'10000000'10000000'10000000'10000000'10000000;
    static constexpr Bits right_edge = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;
    static constexpr Bits top_right_edge = top_edge | right_edge;
    static constexpr Bits top_left_edge = top_edge | left_edge;
    static constexpr Bits bottom_right_edge = bottom_edge | right_edge;
    static constexpr Bits bottom_left_edge = bottom_edge | left_edge;
    static constexpr Bits all_edge = right_edge | top_edge | left_edge | bottom_edge;
    static constexpr Bits negative_slope = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;
    static constexpr Bits positive_slope = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

    inline static constexpr BitBoard from_index(std::size_t index);
    inline static constexpr BitBoard from_position(const Position& position);
    inline static constexpr Position index_to_position(std::size_t index) noexcept;
    inline static constexpr std::size_t position_to_index(const Position& position) noexcept;

    friend void swap(BitBoard& lhs, BitBoard& rhs)
    {
        std::swap(lhs.bits_, rhs.bits_);
    }
};

constexpr BitBoard BitBoard::from_index(const std::size_t index)
{
    if (index >= n_bits) {
        throw std::invalid_argument("position outside of board");
    }
    return BitBoard::make_top_left() >> index;
}

constexpr BitBoard BitBoard::from_position(const Position& position)
{
    if (position.x() < 0 || position.x() >= board_size || position.y() < 0 || position.y() >= board_size) {
        throw std::invalid_argument("position outside of board");
    }
    return from_index(position_to_index(position));
}

constexpr BitBoard::Position BitBoard::to_position() const
{
    assert(has_single_position());
    return index_to_position(std::countl_zero(bits_));
}

constexpr std::size_t BitBoard::position_to_index(const Position& position) noexcept
{
    return position.x() * board_size + position.y();
}

constexpr BitBoard::Position BitBoard::index_to_position(const std::size_t index) noexcept
{
    using T = Position::dimension_type;
    return {static_cast<T>(index / board_size), static_cast<T>(index % board_size)};
}

template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::up>(const size_t n) noexcept
{
    bits_ <<= (board_size * n);
    return *this;
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::down>(const size_t n) noexcept
{
    bits_ >>= (board_size * n);
    return *this;
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::left>(const size_t n) noexcept
{
    Bits wall{0};
    for (size_t i = 0; i < n; i++) {
        wall |= (right_edge << i);
    }
    bits_ <<= n;
    bits_ &= ~wall;
    return *this;
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::right>(const size_t n) noexcept
{
    Bits wall{0};
    for (size_t i = 0; i < n; i++) {
        wall |= (left_edge >> i);
    }
    bits_ >>= n;
    bits_ &= ~wall;
    return *this;
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::upright>(const size_t n) noexcept
{
    return shift_assign<Direction::up>(n).shift_assign<Direction::right>(n);
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::upleft>(const size_t n) noexcept
{
    return shift_assign<Direction::up>(n).shift_assign<Direction::left>(n);
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::downright>(const size_t n) noexcept
{
    return shift_assign<Direction::down>(n).shift_assign<Direction::right>(n);
}
template <>
constexpr BitBoard& BitBoard::shift_assign<Direction::downleft>(const size_t n) noexcept
{
    return shift_assign<Direction::down>(n).shift_assign<Direction::left>(n);
}
