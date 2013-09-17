#ifndef	_POINT_H_
#define	_POINT_H_

#ifndef _VECTOR_H_
#include "vector.h"
#endif

//------------------------------------------------------------------
namespace Space
{
	//--------------------------------------------------------------
	// template class definition
	//--------------------------------------------------------------
	template<unsigned int dimension>
	class Point
	{
		private:
					Tuple<dimension>			my_tuple;

		public:
					/* void */					Point (void) {}
					/* void */					Point (const Point<dimension>& point);

					Point<dimension>&			operator = (const Point<dimension>& point);
					Tuple_Builder<dimension>	operator = (float first);

					Point<dimension>			operator + (const Vector<dimension>& vector) const;
					Point<dimension>			operator - (const Vector<dimension>& vector) const;
					Point<dimension>&			operator += (const Vector<dimension>& vector);
					Point<dimension>&			operator -= (const Vector<dimension>& vector);

					Vector<dimension>			operator - (const Point<dimension>& point) const;

					float						operator [] (unsigned int component) const;
					float&						operator [] (unsigned int component);
	};

	//--------------------------------------------------------------
	// Orthographic projection into n-1 dimensional space by
	// dropping the last component
	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension - 1>	Orthographic_Project_Vector (const Point<dimension>& point)
	{
		return Point<dimension - 1> (*(reinterpret_cast<Vector<dimension - 1>*> (&point)));
	}

	//--------------------------------------------------------------
	// Point and Vector conversion routines
	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&	Point_To_Vector (Point<dimension>& point)
	{
		return *(reinterpret_cast<Vector<dimension>*> (&point));
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>&	Vector_To_Point (Vector<dimension>& vector)
	{
		return *(reinterpret_cast<Point<dimension>*> (&vector));
	}

	//--------------------------------------------------------------
	// template class implementation
	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */	Point<dimension>::Point (const Point<dimension>& point)
	{
		my_tuple = point.my_tuple;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>&		Point<dimension>::operator = (const Point<dimension>& point)
	{
		my_tuple = point.my_tuple;
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Tuple_Builder<dimension>	Point<dimension>::operator = (float first)
	{
		return Tuple_Builder<dimension> (my_tuple, first);
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>		Point<dimension>::operator + (const Vector<dimension>& vector) const
	{
		Point<dimension>	result;
		Tuple<dimension>::Add (my_tuple, vector.my_tuple, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>		Point<dimension>::operator - (const Vector<dimension>& vector) const
	{
		Point<dimension>	result;
		Tuple<dimension>::Sub (my_tuple, vector.my_tuple, result);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>&		Point<dimension>::operator += (const Vector<dimension>& vector)
	{
		Tuple<dimension>::Add (my_tuple, vector.my_tuple, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>&		Point<dimension>::operator -= (const Vector<dimension>& vector)
	{
		Tuple<dimension>::Sub (my_tuple, vector.my_tuple, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Point<dimension>::operator - (const Point<dimension>& point) const
	{
		// XXX hmmm, I don't like this private access thing...
		Vector<dimension>	result;
		Tuple<dimension>::Sub (my_tuple, point.my_tuple, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Point<dimension>::operator [] (unsigned int component) const
	{
		return my_tuple[component];
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float&		Point<dimension>::operator [] (unsigned int component)
	{
		return my_tuple[component];
	}

	//--------------------------------------------------------------
};

//------------------------------------------------------------------

#endif	//_POINT_H_
