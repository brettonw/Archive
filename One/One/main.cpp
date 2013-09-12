//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"cpp_window.h"
#include	"resizable_vector_of.h"
#include	"text.h"

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void
Main
(
    uInt                            argumentCount,
	const ResizableVectorOf<Text>&  argumentVector
)
{
    // start with no windows opened
    TextWindow* window;
    bool        windowOpened = false;
    
    // loop over all of the tokens in the argument list
    for (uInt i = 0; i < argumentCount; i++)
    {
        Text    argument = argumentVector[i];
        cString name = cString (argument);
                    
	    // hack to find extension and file type
	    // XXX would be nice to have a file type that knew names and extensions
	    // XXX then we register file types to window creation routines...
	    sInt j;
	    for (j = argument.Length (); j > 0; j--)
			if (name[j - 1] == '.')
			    break;
	    cString	extension = name + j;
	    bool	isCPP = (_stricmp ("cpp", extension) == 0) or (_stricmp ("c", extension) == 0) or (_stricmp ("hpp", extension) == 0) or (_stricmp ("h", extension) == 0);

	    // open a window
	    if (isCPP)
			window = new CPPWindow (name, 80, 40);
	    else
			window = new TextWindow (name, 80, 40);
	    window->ShowWindow ();

        // increment the window count
		windowOpened = true;
    }
    
    // if no windows were opened
	if (not windowOpened)
	{
	    // open a blank, untitled text window
		window = new TextWindow (80, 40);
		window->ShowWindow ();
	}
}

//------------------------------------------------------------------------------
