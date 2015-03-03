//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _SOLVER_H_
#define     _SOLVER_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _COLLIDER_H_
#include    "collider.h"
#endif  //  _COLLIDER_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
// "Solver" implements a geometric constraint solver using relaxation. It uses 
// a collider object to build an open list for distance constraints
//
//-----------------------------------------------------------------------------
class Solver 
{
    public:
        Collider            m_collider;
        Word                m_corrections;
        
    public:
        /* void*/           Solver (void);
        
    public:
        void                AddThing (Thing* pThing);
        void                RemoveThing (Thing* pThing);
        void                RelaxConstraints (Word iterations = 1);
        
    private:
        void                ApplyCircleDistanceToCircleConstraint (Thing* pThingA, Thing* pThingB);
};

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern  Solver  g_solver;

//-----------------------------------------------------------------------------

#endif  //  _SOLVER_H_
