//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_REGISTRY_H_
#define		_BW_REGISTRY_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#define		REGISTRY_KEY_NAME		"Software\\Wade\\One\\"

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	Registry
{
	public:
static	sInt4					Read (const Text& subkey, const Text& valueName, sInt4 defaultValue);
static	Text					Read (const Text& subkey, const Text& valueName, const Text& defaultValue);
static	bool					Write (const Text& subkey, const Text& valueName, sInt4 value);
static	bool					Write (const Text& subkey, const Text& valueName, const Text&  value);
	private:
static	uInt4					GetKeySize (const Text& subkey, const Text& valueName);
static	bool					ReadKey (const Text& subkey, const Text& valueName, uPointer buffer, uInt4 desiredType, uInt4 desiredSize);
static	bool					SetKey (const Text& subkey, const Text& valueName, uPointer buffer, uInt4 type, uInt4 size);
};

//------------------------------------------------------------------------------

#endif	//	_BW_REGISTRY_H_
