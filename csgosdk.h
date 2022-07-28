#pragma once
#include "pch.h"
#include "csgo.hpp"
#include "csgoVector.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

typedef void* (__cdecl* tCreateInterface)(const char* name, int* pReturnCode);

void* GetInterface(tCreateInterface fn, const char* name)
{
    return (fn)(name, 0);
}

class Ent
{
public:
    union
    {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(Vector, origin, offsets::netvars::m_vecOrigin);
        DEFINE_MEMBER_N(int, clientId, 0x64);
        DEFINE_MEMBER_N(Vector, m_vecViewOffset, offsets::netvars::m_vecViewOffset);
    };
};

class EntListObj
{
public:
    Ent* ent;
    char pad_0004[12];
};

class EntList
{
public:
    EntListObj entListObjs[32];
};