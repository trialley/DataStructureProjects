#pragma once
// node type used by winner tree

#ifndef player_
#define player_
template<typename T>
struct player {
    T value;

    operator int () const { return value; }
};

#endif

