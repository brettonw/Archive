//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _REGISTRY_H_
#define     _REGISTRY_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _SPIDER_H_
#include    "spider.h"
#endif  //  _SPIDER_H_

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
DWORD		    ReadRegistry (const TCHAR* subkey, const TCHAR* valueName, DWORD defaultValue);
const TCHAR*    ReadRegistry (const TCHAR* subkey, const TCHAR* valueName, const TCHAR* defaultValue);
bool		    WriteRegistry (const TCHAR* subkey, const TCHAR* valueName, DWORD value);
bool		    WriteRegistry (const TCHAR* subkey, const TCHAR* valueName, const TCHAR*  value);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _REGISTRY_H_

#ifdef      _REGISTRY_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _ACTION_SPLIT_H_
#include    "action_split.h"
#endif  //  _ACTION_SPLIT_H_

#ifndef     _ACTION_FLIP_H_
#include    "action_flip.h"
#endif  //  _ACTION_FLIP_H_

#ifndef     _ACTION_COMPOSITE_H_
#include    "action_composite.h"
#endif  //  _ACTION_COMPOSITE_H_

#ifndef     _RANDOM_H_
#include    "random.h"
#endif  //  _RANDOM_H_

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#define		REGISTRY_KEY_NAME		L"Software\\Clockwork 3\\Spider\\"

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
DWORD					
GetKeySize (const TCHAR* subkey, const TCHAR* valueName)
{
	DWORD       size = 0;
	HKEY        key;
	TCHAR*      keynamebuffer = new TCHAR[wcslen (REGISTRY_KEY_NAME) + wcslen (subkey) + 1];
	wsprintf (keynamebuffer, L"%s%s", REGISTRY_KEY_NAME, subkey);
	if (RegOpenKeyEx (HKEY_CURRENT_USER, keynamebuffer, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		DWORD   type;
		DWORD   tmpSize = sizeof (DWORD);
		if (RegQueryValueEx (key, valueName, 0, &type, 0, &tmpSize) == ERROR_SUCCESS)
			size = tmpSize;
		RegCloseKey (key);
	}
	delete keynamebuffer;
	return size;
}

//------------------------------------------------------------------------------
bool					
ReadKey (const TCHAR* subkey, const TCHAR* valueName, unsigned char* buffer, unsigned desiredType, unsigned desiredSize)
{
	bool	    success = false;
	HKEY	    key;
	TCHAR*      keynamebuffer = new TCHAR[wcslen (REGISTRY_KEY_NAME) + wcslen (subkey) + 1];
	wsprintf (keynamebuffer, L"%s%s", REGISTRY_KEY_NAME, subkey);
	if (RegOpenKeyEx (HKEY_CURRENT_USER, keynamebuffer, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		DWORD   type;
		DWORD	size = sizeof (DWORD);
		if (RegQueryValueEx (key, valueName, 0, &type, buffer, &size) == ERROR_SUCCESS)
			if ((type == desiredType) and (size == desiredSize))
				success = true;
		RegCloseKey (key);
	}
	delete keynamebuffer;
	return success;
}

//------------------------------------------------------------------------------
bool					
WriteKey (const TCHAR* subkey, const TCHAR* valueName, unsigned char*  buffer, unsigned type, unsigned size)
{
	HKEY	    key;
	bool	    success = false;
	TCHAR*      keynamebuffer = new TCHAR[wcslen (REGISTRY_KEY_NAME) + wcslen (subkey) + 1];
	wsprintf (keynamebuffer, L"%s%s", REGISTRY_KEY_NAME, subkey);
	if (RegCreateKeyEx (HKEY_CURRENT_USER, keynamebuffer, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &key, 0) == ERROR_SUCCESS)
	{
		if (RegSetValueEx (key, valueName, 0, type, buffer, size) == ERROR_SUCCESS)
			success = true;
		RegCloseKey (key);
	}
	delete keynamebuffer;
	return success;
}

//------------------------------------------------------------------------------
DWORD		        
ReadRegistry (const TCHAR* subkey, const TCHAR* valueName, DWORD defaultValue)
{
	int     fetchedValue;
	if (ReadKey (subkey, valueName, r_cast<unsigned char*> (&fetchedValue), REG_DWORD, sizeof (DWORD)))
		return fetchedValue;
	return defaultValue;
}

//------------------------------------------------------------------------------
const TCHAR*     
ReadRegistry (const TCHAR* subkey, const TCHAR* valueName, const TCHAR* defaultValue)
{
	unsigned        size = GetKeySize (subkey, valueName);
	const TCHAR*    returnValue = defaultValue;
	if (size)
	{
	    static TCHAR buffer[255];
	    size = min (size, 255);
		if (ReadKey (subkey, valueName, r_cast<unsigned char*> (buffer), REG_SZ, size))
		{
			buffer[size] = 0;
		    returnValue = &buffer[0];
	    }
	}
	return returnValue;
}

//------------------------------------------------------------------------------
bool		    
WriteRegistry (const TCHAR* subkey, const TCHAR* valueName, DWORD value)
{
	return WriteKey (subkey, valueName, r_cast<unsigned char*> (&value), REG_DWORD, sizeof (DWORD));
}

//------------------------------------------------------------------------------
bool		    
WriteRegistry (const TCHAR* subkey, const TCHAR* valueName, const TCHAR* value)
{
	return WriteKey (subkey, valueName, (unsigned char*) (value), REG_SZ, wcslen (value) + 1);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _REGISTRY_CPP_
