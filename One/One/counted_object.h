//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_COUNTED_OBJECT_H_
#define		_BW_COUNTED_OBJECT_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	CountedObject
{
	public:
							CountedObject (void);
							CountedObject (const CountedObject& object);
virtual						~CountedObject (void);
		void				AddRef (void);
		uInt4				RemoveRef (void);
		uInt4				GetCount (void) const;
	private:
		uInt4				myCount;
};

//--------------------------------------------------------------------------

#endif	//	_BW_COUNTED_OBJECT_H_
