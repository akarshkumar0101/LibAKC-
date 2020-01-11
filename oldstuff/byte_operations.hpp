//
//  byte_operations.hpp
//  LibAKC++
//
//  Created by Akarsh Kumar on 4/11/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#pragma once

#include <iostream>

uint16_t reverseByteOrder16(uint16_t a)
{
    a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
    return a;
}

uint32_t reverseByteOrder32(uint32_t a)
{
    a = ((a & 0x000000FF) << 24) |
    ((a & 0x0000FF00) <<  8) |
    ((a & 0x00FF0000) >>  8) |
    ((a & 0xFF000000) >> 24);
    return a;
}

uint64_t reverseByteOrder64(uint64_t a)
{
    a = ((a & 0x00000000000000FFULL) << 56) |
    ((a & 0x000000000000FF00ULL) << 40) |
    ((a & 0x0000000000FF0000ULL) << 24) |
    ((a & 0x00000000FF000000ULL) <<  8) |
    ((a & 0x000000FF00000000ULL) >>  8) |
    ((a & 0x0000FF0000000000ULL) >> 24) |
    ((a & 0x00FF000000000000ULL) >> 40) |
    ((a & 0xFF00000000000000ULL) >> 56);
    return a;
}

