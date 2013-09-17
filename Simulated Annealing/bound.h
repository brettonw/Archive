#ifndef	_BOUND_H_
#define	_BOUND_H_

#ifndef _POINT_H_
#include "point.h"
#endif

//------------------------------------------------------------------
namespace Space
{
	//--------------------------------------------------------------
	// template class definition
	//--------------------------------------------------------------
	template<unsigned int dimension>
	class Bound
	{
		private:
					Point<dimension>	my_min;
					Point<dimension>	my_max;

		public:
					/* void */			Bound (void);
					/* void */			Bound (const Bound<dimension>& bound);
					/* void */			Bound (const Point<dimension>& min, const Point<dimension>& max);

					Bound<dimension>&	operator = (const Bound<dimension>& bound);

					Bound<dimension>&	operator += (const Bound<dimension>& bound);
					Bound<dimension>&	operator += (const Point<dimension>& point);

					bool				Contains (const Bound<dimension>& bound) const;
					bool				Contains (const Point<dimension>& point) const;

					bool				Bound_Is_Defined_By (const Bound<dimension>& bound) const;
					bool				Bound_Is_Defined_By (const Point<dimension>& point) const;

					Vector<dimension>	Get_Diagonal_Vector (void) const;

					void				Reset (void);

					Point<dimension>	Get_Min (void) const;
					Point<dimension>	Get_Max (void) const;
	};

	//--------------------------------------------------------------
	// template class implementation
	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */			Bound<dimension>::Bound (void)
	{
		my_min = +1.0e6f;
		my_max = -1.0e6f;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */			Bound<dimension>::Bound (const Bound<dimension>& bound) :
	my_min (bound.my_min),
	my_max (bound.my_max)
	{
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */			Bound<dimension>::Bound (const Point<dimension>& min, const Point<dimension>& max) :
	my_min (min),
	my_max (max)
	{
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Bound<dimension>&	Bound<dimension>::operator = (const Bound<dimension>& bound)
	{
		my_min = bound.my_min;
		my_max = bound.my_max;
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Bound<dimension>&	Bound<dimension>::operator += (const Bound<dimension>& bound)
	{
		for (unsigned int i = 0; i < dimension; i++)
		{
			if (bound.my_min[i] < my_min[i])
				my_min[i] = bound.my_min[i];
			if (bound.my_max[i] > my_max[i])
				my_max[i] = bound.my_max[i];
		}
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Bound<dimension>&	Bound<dimension>::operator += (const Point<dimension>& point)
	{
		for (unsigned int i = 0; i < dimension; i++)
		{
			if (point[i] < my_min[i])
				my_min[i] = point[i];
			if (point[i] > my_max[i])
				my_max[i] = point[i];
		}
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	bool				Bound<dimension>::Contains (const Bound<dimension>& bound) const
	{
		for (unsigned int i = 0; i < dimension; i++)
			if ((bound.my_min[i] < my_min[i]) || (bound.my_max[i] > my_max[i]))
				return false;
		return true;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	bool				Bound<dimension>::Contains (const Point<dimension>& point) const
	{
		for (unsigned int i = 0; i < dimension; i++)
			if ((point[i] < my_min[i]) || (point[i] > my_max[i]))
				return false;
		return true;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	bool				Bound<dimension>::Bound_Is_Defined_By (const Bound<dimension>& bound) const
	{
		for (unsigned int i = 0; i < dimension; i++)
			if ((bound.my_min[i] == my_min[i]) || (bound.my_max[i] == my_max[i]))
				return true;
		return false;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	bool				Bound<dimension>::Bound_Is_Defined_By (const Point<dimension>& point) const
	{
		for (unsigned int i = 0; i < dimension; i++)
			if ((point[i] == my_min[i]) || (point[i] == my_max[i]))
				return true;
		return false;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>	Bound<dimension>::Get_Diagonal_Vector (void) const
	{
		return my_max - my_min;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	void	Bound<dimension>::Reset (void)
	{
		my_min = +1.0e6f;
		my_max = -1.0e6f;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>	Bound<dimension>::Get_Min (void) const
	{
		return my_min;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Point<dimension>	Bound<dimension>::Get_Max (void) const
	{
		return my_max;
	}

	//--------------------------------------------------------------
};

//------------------------------------------------------------------

#endif	//_BOUND_H_
