#pragma once

#include <cstdint>

class Dice
{
private:
    uint64_t m_seed;
    uint64_t m_nth;

public:
    Dice( uint64_t seed );
    uint64_t seed() const;
    uint64_t max() const;

    // ------ returs <0, numeric_limits<uint64_t>::max()>
    uint64_t sequencive( uint64_t s ) const;
    uint64_t next();

    // ------ returs <0.0, 1.0>
    double sequencived( uint64_t s ) const;
    double nextd();

};
