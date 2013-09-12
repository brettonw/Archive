//------------------------------------------------------------------------------
//	File:					arcball.h
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for Ken Shoemake's
//								ArcBall rotation controller
//								
//------------------------------------------------------------------------------

#include "point_2d.h"
#include "vector_2d.h"
#include "vector_3d.h"
#include "matrix_3d.h"

#ifndef ARCBALL
#define ARCBALL

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	arcball																																		//	arcball class
{																																								//	begin arcball definition
	private:																																			//	members internal to this class only
				void			ComputeArc (short, const vector_3d&, const vector_3d&) const;	//	compute the arc between two points with a given number of segments
				void			DrawArc (short numsegs) const;																//	draw the arc between the two points
				vector_3d	MapToSphere (const point_2d &pt);															//	compute the intersection pt of the arcball with a line to the point_3d
	protected:																																		//	members internal to this class hierarchy
				vector_2d	center;																												//	the center point_3d of the arcball
				real			radius;																												//	the radius of the arcball
				vector_3d	start_vec;																										//	the starting vector_3d
				vector_3d	end_vec;																											//	the ending vector_3d;
				point_2d	start_pt;																											//	the start_vec point_3d for the manipulation
				point_2d	end_pt;																												//	the end point_3d of the arc
	public:																																				//	public interface
				arcball (const point_2d&, real);																				//	normal constructor
virtual	void			Click (const point_2d&);																			//	the first point_3d in the arc
virtual	matrix_3d	Drag (const point_2d&);																				//	the second point_3d in the arc
virtual	void			DrawBackground (void) const;																	//	draw the sphere
virtual	void			DrawForeground (void) const;																	//	draw the arcball interface
};																																							//	end arcball class definition

//------------------------------------------------------------------------------

#endif //ARCBALL
