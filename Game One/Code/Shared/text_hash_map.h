//--------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _TEXT_HASH_MAP_H_
#define     _TEXT_HASH_MAP_H_

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#ifndef     _TEXT_H_
#include    "text.h"
#endif  //  _TEXT_H_

//--------------------------------------------------------------------------
// hash traits for text
//--------------------------------------------------------------------------
class HashCompareText
{
    public:
        enum
        {
            bucket_size = 4,
            min_buckets = 8
        };
        
    public:
        size_t operator () (const Text& key) const
        {
            cString text = key.GetText ();
            uInt    value = 0x55555555;
            while (*text)
                value = (value << 1) xor *text++;
            return value;
        }

        bool operator () (const Text& key1, const Text& key2) const
        {
            return key1 < key2;
        }
};

//--------------------------------------------------------------------------
// class definitions
//--------------------------------------------------------------------------
template<class aType>
class TextHashMap : public std::hash_map<Text, aType, HashCompareText> {};

//--------------------------------------------------------------------------

#endif  //  _TEXT_HASH_MAP_H_
