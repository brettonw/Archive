#ifndef	_TUPLE_H_
#define	_TUPLE_H_

//------------------------------------------------------------------
namespace Space
{
	//--------------------------------------------------------------
	// template class definition
	//--------------------------------------------------------------
	template<unsigned int dimension>
	class Tuple
	{
		public:
					float		my_component[dimension];

		public:
			static	void		Add (const Tuple& left, const Tuple& right, Tuple& result);
			static	void		Sub (const Tuple& left, const Tuple& right, Tuple& result);
			static	void		Mul (const Tuple& left, float right, Tuple& result);
			static	void		Div (const Tuple& left, float right, Tuple& result);
			static	float		Dot (const Tuple& left, const Tuple& right);
					float		operator [] (unsigned int component) const;
					float&		operator [] (unsigned int component);
	};

	//--------------------------------------------------------------
	// template class implementation
	//--------------------------------------------------------------
	template<unsigned int dimension>
	void		Tuple<dimension>::Add (const Tuple<dimension>& left, const Tuple<dimension>& right, Tuple<dimension>& result)
	{
		for (int i = 0; i < dimension; i++)
			result.my_component[i] = left.my_component[i] + right.my_component[i];
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	void		Tuple<dimension>::Sub (const Tuple<dimension>& left, const Tuple<dimension>& right, Tuple<dimension>& result)
	{
		for (int i = 0; i < dimension; i++)
			result.my_component[i] = left.my_component[i] - right.my_component[i];
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	void		Tuple<dimension>::Mul (const Tuple<dimension>& left, float right, Tuple<dimension>& result)
	{
		for (int i = 0; i < dimension; i++)
			result.my_component[i] = left.my_component[i] * right;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	void		Tuple<dimension>::Div (const Tuple<dimension>& left, float right, Tuple<dimension>& result)
	{
		float	reciprocal = 1.0f / right;
		for (int i = 0; i < dimension; i++)
			result.my_component[i] = left.my_component[i] * reciprocal;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Tuple<dimension>::Dot (const Tuple<dimension>& left, const Tuple<dimension>& right)
	{
		float	result = 0.0f;
		for (int i = 0; i < dimension; i++)
			result += left.my_component[i] * right.my_component[i];
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Tuple<dimension>::operator [] (unsigned int component) const
	{
		assert (component < dimension);
		return my_component[component];
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float&		Tuple<dimension>::operator [] (unsigned int component)
	{
		assert (component < dimension);
		return my_component[component];
	}

	//--------------------------------------------------------------
};

//------------------------------------------------------------------

#endif	//_TUPLE_H_
