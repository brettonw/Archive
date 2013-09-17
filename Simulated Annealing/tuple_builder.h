#ifndef	_TUPLE_BUILDER_H_
#define	_TUPLE_BUILDER_H_

#ifndef	_TUPLE_H_
#include "tuple.h"
#endif

//--------------------------------------------------------------
namespace Space
{
	//--------------------------------------------------------------
	template<unsigned int dimension>
	class Tuple_Builder
	{
		private:
					Tuple<dimension>&	my_tuple;
					int					my_index;

		public:
					/* void */			Tuple_Builder (Tuple<dimension>& tuple, float first);
					/* void */			~Tuple_Builder (void);
					Tuple_Builder&		operator , (float next);
	};

	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */		Tuple_Builder<dimension>::Tuple_Builder (Tuple<dimension>& tuple, float first) :
	my_tuple (tuple),
	my_index(0)
	{
		my_tuple[my_index] = first;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */		Tuple_Builder<dimension>::~Tuple_Builder (void)
	{
		if (my_index == 0)
		{
			float	last = my_tuple[my_index];
			while (++my_index < dimension)
				my_tuple[my_index] = last;
		}
		else
			assert (my_index == (dimension - 1));
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Tuple_Builder<dimension>&	Tuple_Builder<dimension>::operator , (float next)
	{
		my_tuple.my_component[++my_index] = next;
		return *this;
	}

	//--------------------------------------------------------------
};

//--------------------------------------------------------------

#endif	//_TUPLE_BUILDER_H_
