//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"registry.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
sInt4
Registry::Read
(
	const Text&			subkey,
	const Text&			valueName,
	sInt4				defaultValue
)
{
	sInt4	fetchedValue;
	if (ReadKey (subkey, valueName, uPointer (&fetchedValue), REG_DWORD, sizeof (sInt4)))
		return fetchedValue;
	return defaultValue;
}

//------------------------------------------------------------------------------
Text
Registry::Read
(
	const Text&			subkey,
	const Text&			valueName,
	const Text&			defaultValue
)
{
	uInt4	size = GetKeySize (subkey, valueName);
	Text	returnValue = defaultValue;
	if (size)
	{
		String	buffer = new sInt1[size];
		if (ReadKey (subkey, valueName, uPointer (buffer), REG_SZ, size))
			returnValue = Text (buffer);
		delete[] buffer;
	}
	return returnValue;
}

//------------------------------------------------------------------------------
bool
Registry::Write
(
	const Text&			subkey,
	const Text&			valueName,
	sInt4				value
)
{
	return SetKey (subkey, valueName, uPointer (&value), REG_DWORD, sizeof (sInt4));
}

//------------------------------------------------------------------------------
bool
Registry::Write
(
	const Text&			subkey,
	const Text&			valueName,
	const Text&			value
)
{
	return SetKey (subkey, valueName, uPointer (value.GetText ()), REG_SZ, value.Length () + 1);
}

//------------------------------------------------------------------------------
uInt4
Registry::GetKeySize
(
	const Text&			subkey,
	const Text&			valueName
)
{
	uInt4	size = 0;
	HKEY	myKey;
	if (RegOpenKeyEx (HKEY_CURRENT_USER, REGISTRY_KEY_NAME + subkey, 0, KEY_READ, &myKey) == ERROR_SUCCESS)
	{
		uInt4	type,
				tmpSize = sizeof (uInt4);
		if (RegQueryValueEx (myKey, valueName, 0, &type, 0, &tmpSize) == ERROR_SUCCESS)
			size = tmpSize;
		RegCloseKey (myKey);
	}
	return size;
}

//------------------------------------------------------------------------------
bool
Registry::ReadKey
(
	const Text&			subkey,
	const Text&			valueName,
	uPointer			buffer,
	uInt4				desiredType,
	uInt4				desiredSize
)
{
	bool	success = false;
	HKEY	myKey;
	if (RegOpenKeyEx (HKEY_CURRENT_USER, REGISTRY_KEY_NAME + subkey, 0, KEY_READ, &myKey) == ERROR_SUCCESS)
	{
		uInt4	type,
				size = sizeof (uInt4);
		if (RegQueryValueEx (myKey, valueName, 0, &type, buffer, &size) == ERROR_SUCCESS)
			if ((type == desiredType) and (size == desiredSize))
				success = true;
		RegCloseKey (myKey);
	}
	return success;
}

//------------------------------------------------------------------------------
bool
Registry::SetKey
(
	const Text&			subkey,
	const Text&			valueName,
	uPointer			buffer,
	uInt4				type,
	uInt4				size
)
{
	HKEY	myKey;
	uInt4	result;
	bool	success = false;
	if (RegCreateKeyEx (HKEY_CURRENT_USER, REGISTRY_KEY_NAME + subkey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &myKey, &result) == ERROR_SUCCESS)
	{
		if (RegSetValueEx (myKey, valueName, 0, type, buffer, size) == ERROR_SUCCESS)
			success = true;
		RegCloseKey (myKey);
	}
	return success;
}

//------------------------------------------------------------------------------
