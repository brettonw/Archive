//------------------------------------------------------------------------------
//	File:					distributor.h
//	Date:					9/22/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a sample distributor
//
//------------------------------------------------------------------------------

#include "utility.h"
#include "control.h"

#ifndef	DISTRIBUTOR
#define	DISTRIBUTOR

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	distributor																																				//	ray class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	#if STRATIFIED_SAMPLING
		real	*values;
		real	step;
		int		index;
	#endif
	public:																																				//	members available externally
		real	Sample (void);
	#if STRATIFIED_SAMPLING
		distributor (void);
		~distributor (void);
		void	Shuffle (void);
	#endif
};																																							//	end

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
#if STRATIFIED_SAMPLING
inline	distributor::distributor (void)
{																																								//	begin
	values = new real[STRATIFIED_SAMPLE_SIZE + 1];
	step = R(1.0) / real (STRATIFIED_SAMPLE_SIZE);
	values[0] = R(0.0);
	for (int i = 1; i <= STRATIFIED_SAMPLE_SIZE; i++)
		values[i] = values[i - 1] + step;
	Shuffle ();
	index = 0;
}																																								//	end

//------------------------------------------------------------------------------
inline	distributor::~distributor (void)
{																																								//	begin
	delete[] values;
}																																								//	end

//------------------------------------------------------------------------------
inline	void	distributor::Shuffle (void)
{																																								//	begin
	for (int i = 1; i <= STRATIFIED_SAMPLE_SIZE; i++)
	{
		int	newi = int (UnitRandom () * real (STRATIFIED_SAMPLE_SIZE));
		real	tmp = values[i];
		values[i] = values[newi];
		values[newi] = tmp;
	}
}																																								//	end
#endif
//------------------------------------------------------------------------------
inline	real	distributor::Sample (void)
{																																								//	begin
	#if STRATIFIED_SAMPLING
	real	sample = values[index++];
	if (index > STRATIFIED_SAMPLE_SIZE)
	{
		Shuffle ();
		index = 0;
	}
	#if JITTERED_SAMPLING
	return sample + (UnitRandom () * step);
	#else
	return sample;
	#endif
	#else
	return UnitRandom ();
	#endif
}																																								//	end

//------------------------------------------------------------------------------

#endif	//DISTRIBUTOR
