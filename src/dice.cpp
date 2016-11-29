#include "dice.hpp"

Dice::Dice( uint64_t seed ) :
    m_seed( seed ),
    m_nth( 0 )
{
}

uint64_t Dice::seed() const
{
    return m_seed;
}

// this is indeed very salty algorithm
uint64_t Dice::sequencive( uint64_t s ) const
{
    uint64_t next = m_seed;
    const uint64_t salt = s * 1103515245 + next + 12345;
    uint64_t result = 0;

    for ( int i=0; i<8; i++ ) {
        next *= salt;
        next += salt;
        result <<= 8;
        result ^= next % 0xffff;
    }

    return result;
}

double Dice::sequencived( uint64_t s ) const
{
    return (double)sequencive( s ) / max();
}

uint64_t Dice::next()
{
    return sequencive( m_nth++ );
}

double Dice::nextd()
{
    return sequencived( m_nth++ );
}

uint64_t Dice::max() const
{
    return 0xffffffffffffffff;
}
