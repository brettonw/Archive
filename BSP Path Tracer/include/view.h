//------------------------------------------------------------------------------
//	File:					view.h
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a view of space
//
//------------------------------------------------------------------------------

#include	"matrix_3d.h"
#include	"point_2d.h"
#include	"polyptr_3d.h"
#include	"camera_3d.h"

#ifndef VIEW
#define VIEW

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	view																																			//	view class
{																																								//	begin view definition
	private:																																			//	private interface
				point_2d	dctovdc (real x, real y) const;																//	map floating point screen coordinates to virtual device coordinates
	protected:																																		//	protected interface
				camera		cam;																													//	camera
				int				width, height;																								//	width and height of the image
				real			xsize, ysize, aspect;																					//	parameters for mapping
				int			current_rays;
				real			total_power;
	public:																																				//	public interface
				view (int width, int height);																						//	constructor
				~view (void);																														//	destructor
static			point_3d	Phong (real x, real y);																				//	return a vector from a phong distribution
				void		BuildLightList (void);
				polyptr		GetLight (real& s);
				void			TraceFirstParticle (const ray &trace, real power);									//	trace a particle
				void			TraceSubsequentParticle (const ray &trace, real power);									//	trace a particle
				void			TraceParticles (void);																				//	trace particles from all the light sources
				real			TraceFirstRay (const ray &trace);															//	trace a ray from the eyepoint
				real			TraceSecondRay (const point_3d& origin_pt, const polyptr& poly);														//	trace a ray from the intersection point
				real			TraceLightSource (const point_3d& origin_pt, const vector_3d& normal);																//	trace a ray from the intersection point
				void			TraceScene (char *filename);																	//	ray trace the scene
};																																							//	end view class definition

//------------------------------------------------------------------------------

#endif //VIEW
