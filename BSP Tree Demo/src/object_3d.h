#ifndef     _COUNTED_OBJECT_H_
#include "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

#include "ray_3d.h"

#ifndef     _OBJECT_3D_
#define     _OBJECT_3D_

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	Object_3d : public CountedObject
{
	public:
    Object_3d (void) {}
virtual	~Object_3d (void);
virtual	real		rayIntersection (const ray& r) const = 0;
};

//------------------------------------------------------------------------------

#endif  //  _OBJECT_3D_
