//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _OBJECT_TYPE_H_
#define     _OBJECT_TYPE_H_

//-----------------------------------------------------------------------------
// enumerations
//-----------------------------------------------------------------------------
enum
{
    // OBJECT TYPE is typically going to be a 16-bit value stored with the 
    // Thing class
    OT_FREE                     = 0,
    OT_CIRCLE                   = 1,
    OT_BOX                      = 1 << 1,
    OT_SHAPE_RESERVED_1         = 1 << 2,
    OT_SHAPE_RESERVED_2         = 1 << 3,
    OT_SHAPE_RESERVED_3         = 1 << 4,
    OT_SHAPE_RESERVED_4         = 1 << 5,
    OT_INVISIBLE                = 1 << 6,
    
    // masks on how we apply constraints
    OT_FIXED_POSITION           = 1 << 7,
    OT_CONSTRAINT_ONE           = 1 << 8,
    OT_CONSTRAINT_TWO           = 1 << 9,
    OT_CONSTRAINT_THREE         = 1 << 10,
    OT_CONSTRAINT_FOUR          = 1 << 11,
    OT_CONSTRAINT_FIVE          = 1 << 12,
    OT_CONSTRAINT_MASK          = OT_FIXED_POSITION bit_or OT_CONSTRAINT_ONE bit_or OT_CONSTRAINT_TWO bit_or OT_CONSTRAINT_THREE bit_or OT_CONSTRAINT_FOUR bit_or OT_CONSTRAINT_FIVE,
    
    OT_DEFAULT                  = OT_CIRCLE,
    OT_SHAPE_MASK               = OT_CIRCLE bit_or OT_BOX
};

//-----------------------------------------------------------------------------

#endif  //  _OBJECT_TYPE_H_
