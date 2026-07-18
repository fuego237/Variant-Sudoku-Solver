#pragma once

struct Position {
    int row;
    int column;

    bool operator==(const Position& p) const
    {
        return row == p.row && column == p.column;
    }

    bool related(const Position& p) const
    {
        return row == p.row || column == p.column || (row/3 == p.row/3 && column/3 == p.column/3);
    }
};

namespace std {
    template <>
    struct hash<Position> {
        size_t operator()(const Position& p) const noexcept {
            size_t h1 = hash<int>{}(p.row);
            size_t h2 = hash<int>{}(p.column);
            
            return h1 ^ (h2 << 1); 
        }
    };
}