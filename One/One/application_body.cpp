//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"application_body.h"
#include	"window.h"
#include	"windows_utility.h"
#include	"cmd_line_scanner.h"
#include	"cmd_line_scanner_action.h"
#include	"tokenizer.h"
#include	"iterator_on_list_of.h"
#include	"resizable_vector_of.h"

//------------------------------------------------------------------------------
//	static members
//------------------------------------------------------------------------------
ApplicationBody*	ApplicationBody::global = 0;

//------------------------------------------------------------------------------
//	prototypes for user supplied functions
//------------------------------------------------------------------------------
void	Main (uInt argumentCount, const ResizableVectorOf<Text>& argumentVector);

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
void
ApplicationBody::Startup
(
	uInt				pulseRate,
	const Text&			arguments
)
{
	assert (global == 0);
	new ApplicationBody (pulseRate, arguments);
}

//------------------------------------------------------------------------------
void
ApplicationBody::Shutdown
(
	void
)
{
	assert (global);
	delete global;
}

//------------------------------------------------------------------------------
bool
ApplicationBody::IsValid
(
	void
)
{
	return global ? true : false;
}

//------------------------------------------------------------------------------
sInt4
ApplicationBody::Execute
(
	void
)
{
	assert (global);
	sInt4	result = 0;
	while (MyCountWindows () > 0)
		if (PeekMessage (&(global->myCurrentMessage), 0, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage (&(global->myCurrentMessage), 0, 0, 0) != TRUE)
				result = global->myCurrentMessage.wParam;
			else
				DispatchMessage (&(global->myCurrentMessage));
		}
		else
		{
			HWND	activeWnd = GetActiveWindow ();
			if (activeWnd)
			{
				Window*	activeWindow = (Window *) GetWindowLong (activeWnd, GWL_USERDATA);
				if (activeWindow)
					activeWindow->Pulse ();
		        MsgWaitForMultipleObjects (0, 0, FALSE, global->mySleepTime, QS_ALLINPUT); 
			}
			else
			{
			    // sleep a lot longer since we are in the background
		        MsgWaitForMultipleObjects (0, 0, FALSE, INFINITE, QS_ALLINPUT); 
		    }
		}
    return result;
}

//------------------------------------------------------------------------------
void
ApplicationBody::Quit
(
	void
)
{
	assert (global);
	MyCloseAllWindows ();
}

//------------------------------------------------------------------------------
LPMSG
ApplicationBody::GetCurrentMessage
(
	void
)
{
	assert (global);
	return &(global->myCurrentMessage);
}

//------------------------------------------------------------------------------
ApplicationBody::ApplicationBody
(
	uInt				pulseRate,
	const Text&			arguments
)
{
	global = this;
	mySleepTime = 1000 / pulseRate;
	
	// tokenize the arguments
	HandleTo<Scanner>	                cmdLineScanner = new CmdLineScanner;
	Tokenizer                           tokenizer (cmdLineScanner);
	HandleTo<ListOf<PtrTo<Token> > >    tokenList = tokenizer.Tokenize (arguments);
	
	// now loop over the token list and fill a vector with the actual arguments
	IteratorOnListOf<PtrTo<Token> >     argumentIterator (tokenList.GetPtr ());
	ResizableVectorOf<Text>             argumentVector (tokenList->GetSize ());
	uInt                                argumentCount = 0;
    while (not argumentIterator.IsDone ())
    {
        PtrTo<Token>    ptrToToken = argumentIterator.GetCurrentItem ();
        switch (ptrToToken->GetType ())
        {
		    case CmdLineScannerAction::OUTPUT_IDENTIFIER:
		    case CmdLineScannerAction::OUTPUT_QUOTED_IDENTIFIER:
		    case CmdLineScannerAction::OUTPUT_UNCLOSED_QUOTED_IDENTIFER:
                    argumentVector[argumentCount++] = ptrToToken->GetText ();
                    // drop through
            default:
                break;
        }
        argumentIterator.GotoNextItem ();
    }
	
	// now call the user supplied argument handler with the vector
	Main (argumentCount, argumentVector);
}

//------------------------------------------------------------------------------
ApplicationBody::~ApplicationBody
(
	void
)
{
	global = 0;
}

//------------------------------------------------------------------------------
