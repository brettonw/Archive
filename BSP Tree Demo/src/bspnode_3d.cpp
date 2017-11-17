#include "precompile.h"
#include "utility.h"
#include "BspNode_3d.h"

BspNode::BspNode (const PtrToPolygon_3d& poly)
{
	plane = poly->getPlane ();
    on = new PolygonList_3d ();
	on->addToList (poly);
}

BspNode::~BspNode (void)
{
}

void	BspNode::insert (PtrToPolygonList_3d& list, hclass keep)
{
    PtrToPolygonList_3d	inside (new PolygonList_3d ());
    PtrToPolygonList_3d outside (new PolygonList_3d ());
    PtrToPolygon_3d		poly, inp, outp;
	for (poly = list->pop (); poly.getPtr (); poly = list->pop ())
	{
		hclass	sgn = Split (poly, plane, inp, outp);
		if (sgn == HC_ON)
			on->addToList (poly);
		else
		{
			if ((sgn == HC_IN) || (sgn == HC_SPANNING))
				inside->addToList (inp);
			if ((sgn == HC_OUT) || (sgn == HC_SPANNING))
				outside->addToList (outp);
		}
	}
	if (!inside->empty ())
		in.insert (inside, keep, HC_IN);
	if (!outside->empty ())
		out.insert (outside, keep, HC_OUT);
}

void	BspNode::push (const PtrToPolygon_3d& poly, PtrToPolygonList_3d& result, hclass keep)
{
    PtrToPolygonList_3d	inside (new PolygonList_3d ());
    PtrToPolygonList_3d outside (new PolygonList_3d ());
    PtrToPolygon_3d	inp, outp;
	hclass	sgn = Split (poly, plane, inp, outp);
	if (sgn == HC_ON)
		result->addToList (poly);
	else
	{
		if ((sgn == HC_IN) || (sgn == HC_SPANNING))
			in.push (inp, result, keep, HC_IN);
		if ((sgn == HC_OUT) || (sgn == HC_SPANNING))
			out.push (outp, result, keep, HC_OUT);
	}
}

void	BspNode::push (PtrToPolygonList_3d& list, PtrToPolygonList_3d& result, hclass keep)
{
    PtrToPolygonList_3d	inside (new PolygonList_3d ());
    PtrToPolygonList_3d outside (new PolygonList_3d ());
    PtrToPolygon_3d		poly, inp, outp;
	for (poly = list->pop (); poly.getPtr (); poly = list->pop ())
	{
		hclass	sgn = Split (poly, plane, inp, outp);
		if (sgn == HC_ON)
			result->addToList (poly);
		else
		{
			if ((sgn == HC_IN) || (sgn == HC_SPANNING))
				inside->addToList (inp);
			if ((sgn == HC_OUT) || (sgn == HC_SPANNING))
				outside->addToList (outp);
		}
	}
	if (!inside->empty ())
		in.push (inside, result, keep, HC_IN);
	if (!outside->empty ())
		out.push (outside, result, keep, HC_OUT);
}

void	BspNode::reduce (void)
{
    PtrToPolygonList_3d	results (new PolygonList_3d ());
    PtrToPolygonList_3d boundary (new PolygonList_3d ());
    for (PtrToPolygon_3d poly = on->pop (); poly.getPtr (); poly = on->pop ())
	{
		if (FABS (poly->getPlane ()[W] + plane[W]) > EPSILON)
		{
			in.push (poly, results, HC_IN, HC_IN);
			out.push (results, boundary, HC_OUT, HC_OUT);
		}
		else
		{
			out.push (poly, results, HC_OUT, HC_OUT);
			in.push (results, boundary, HC_IN, HC_IN);
		}
	}
	on = boundary;
	in.reduce ();
	out.reduce ();
}

void	BspNode::draw (const point_3d &eye) const
{
	real	sgn = eye | plane;
	if (sgn < R(0.0))
	{
		out.draw (eye);
		on->draw ();
		in.draw (eye);
	}
	else
	{
		in.draw (eye);
		on->draw ();
		out.draw (eye);
	}
}

bool	BspNode::rayIntersection (const ray& r, PtrToPolygon_3d& poly_hit, point_3d& ipt) const
{
	real	dist = r.Origin () | plane,
				costheta = r.Direction () | plane;
	if (dist > EPSILON)
	{
		if (out.rayIntersection (r, poly_hit, ipt)) return TRUE;
		if (costheta < -EPSILON)
		{
			if (planeSearch (r, poly_hit, ipt)) return TRUE;
			return in.rayIntersection (r, poly_hit, ipt);
		}
	}
	else if (dist < -EPSILON)
	{
		if (in.rayIntersection (r, poly_hit, ipt)) return TRUE;
		if (costheta > EPSILON)
		{
			if (planeSearch (r, poly_hit, ipt)) return TRUE;
			return out.rayIntersection (r, poly_hit, ipt);
		}
	}
	else
	{
		if (costheta < -EPSILON)
			return in.rayIntersection (r, poly_hit, ipt);
		else if (costheta > EPSILON)
			return out.rayIntersection (r, poly_hit, ipt);
	}
	return FALSE;
}

bool	BspNode::planeSearch (const ray& r, PtrToPolygon_3d& poly_hit, point_3d& ipt) const
{
	real	dist = plane.rayIntersection (r);
	if (dist > R(0.0))
	{
		ipt = r.IntersectionPoint (dist);
		if (on->getBoundingBox ().Contains (ipt))
		{
            /*
			iterator	entry (on);
			for (polyptr poly = entry (); poly (); poly = entry ())
				if (poly->Contains (ipt))
				{
					poly_hit = poly;
					return TRUE;
				}
                */
		}
	}
	return FALSE;
}
