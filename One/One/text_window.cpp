//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"text_window.h"
#include	"task.h"
#include	"error_handler.h"
#include	"text_scanner_drawing_action.h"
#include	"resources.h"
#include	"registry.h"

#include	"cpp_window.h"
#include	"application.h"
#include	"application_body.h"

//------------------------------------------------------------------------------
//	static members
//------------------------------------------------------------------------------
uInt					TextWindow::myUntitledCount = 1;
HandleTo<TextScanner>	TextWindow::myTextScanner;

//------------------------------------------------------------------------------
//	macro for default font name
//------------------------------------------------------------------------------
#define	DEFAULT_FONT_NAME	"Lucida Console"
#define	DEFAULT_FONT_SIZE	8

//--------------------------------------------------------------------------
//	tasks
//--------------------------------------------------------------------------
DEFINE_TASK (TextWindow)
{
    DO_AFTER (StdErr);
}

//--------------------------------------------------------------------------
ON_STARTUP (TextWindow)
{
	Window::RegisterWindowClass (CS_BYTEALIGNCLIENT bitor CS_OWNDC bitor CS_DBLCLKS, TextWindow::WindowProc, TEXT_WINDOW_CLASS_NAME, TEXT_WINDOW_MAIN_MENU);
	TextWindow::Startup ();
}

//--------------------------------------------------------------------------
ON_SHUTDOWN (TextWindow)
{
}

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
TextWindow::TextWindow
(
	const Text&					filename,
	uInt						width,
	uInt						height
) : ScrollingWindow (TEXT_WINDOW_CLASS_NAME, Text ("Untitled"), ScrollingWindow::SCROLL_BOTH, WS_OVERLAPPEDWINDOW bitor WS_HSCROLL bitor WS_VSCROLL, WS_EX_CLIENTEDGE, CW_USEDEFAULT, 0, 1, 1), myLines (4)
{
    InitTextWindow (width, height);
	myScanner = myTextScanner;
	OpenFile (filename);
}

//------------------------------------------------------------------------------
TextWindow::TextWindow
(
	uInt						width,
	uInt						height
) : ScrollingWindow (TEXT_WINDOW_CLASS_NAME, Text ("Untitled"), ScrollingWindow::SCROLL_BOTH, WS_OVERLAPPEDWINDOW bitor WS_HSCROLL bitor WS_VSCROLL, WS_EX_CLIENTEDGE, CW_USEDEFAULT, 0, 1, 1), myLines (4)
{
    InitTextWindow (width, height);
	myScanner = myTextScanner;
	SetEmptyUntitled ();
}

//------------------------------------------------------------------------------
TextWindow::~TextWindow
(
	void
)
{
}

//------------------------------------------------------------------------------
bool
TextWindow::OnCloseWindow
(
	void
)
{
	HDC		dc = GetDC (myWindow);
	DeleteObject (SelectObject (dc, myOldFont));
	ReleaseDC (myWindow, dc);
	return Window::OnCloseWindow ();
}

//------------------------------------------------------------------------------
void
TextWindow::Pulse
(
	void
)
{
	// if the user has clicked and dragged
	if (myMouseIsDown bitand LEFT_BUTTON)
	{
		// get the mouse position
		POINT	cursorPos;
		GetCursorPos (&cursorPos);
		ScreenToClient (myWindow, &cursorPos);

		// fetch the client rect of the window
		RECT	clientRect;
		GetClientRect (myWindow, &clientRect);

		// set up some autoscrolling values
		// XXX this should be in preferences
		sInt	autoScrollSpeedDelta = 8;
		sInt	autoScrollHorizontalSpeedDelta = myMetric.tmAveCharWidth * autoScrollSpeedDelta;
		sInt	autoScrollVerticalSpeedDelta = myMetric.tmHeight * autoScrollSpeedDelta;

		// autoscroll horizontally if needed
		bool	autoHScroll = true;
		if (cursorPos.x < 0)
		{
			sInt	autoScrollSpeed = (-cursorPos.x / autoScrollHorizontalSpeedDelta) + 1;
			for (sInt i = 0; i < autoScrollSpeed; i++)
				OnHScroll (SB_LINELEFT);
		}
		else if (cursorPos.x > clientRect.right)
		{
			sInt	autoScrollSpeed = ((cursorPos.x - clientRect.right) / autoScrollHorizontalSpeedDelta) + 1;
			for (sInt i = 0; i < autoScrollSpeed; i++)
				OnHScroll (SB_LINERIGHT);
		}
		else
			autoHScroll = false;

		// autoscroll vertically if needed
		bool	autoVScroll = true;
		if (cursorPos.y < 0)
		{
			sInt	autoScrollSpeed = (-cursorPos.y / autoScrollVerticalSpeedDelta) + 1;
			for (sInt i = 0; i < autoScrollSpeed; i++)
				OnVScroll (SB_LINEUP);
		}
		else if (cursorPos.y > clientRect.bottom)
		{
			sInt	autoScrollSpeed = ((cursorPos.y - clientRect.bottom) / autoScrollVerticalSpeedDelta) + 1;
			for (sInt i = 0; i < autoScrollSpeed; i++)
				OnVScroll (SB_LINEDOWN);
		}
		else
			autoVScroll = false;

		// if there was an autoscroll
		if (autoHScroll or autoVScroll)
		{
			// extend the selection as if the mouse were moving
			myCurrentLoc = ConvertScreenCoordsToGrid (cursorPos);
			myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);
			ExtendSelection ();
		}
	}
}

//------------------------------------------------------------------------------
bool
TextWindow::OnPaintWindow
(
	PAINTSTRUCT&		paint
)
{
	HideCaret (myWindow);

	myDC = paint.hdc;
	SetTextColor (myDC, PALETTERGB (0, 0, 0));
	myUpdateRgn = CreateRectRgnIndirect (&paint.rcPaint);
	ExcludeClipRect (myDC, 0, 0, LEFT_BUFFER * myMetric.tmAveCharWidth, SHRT_MAX);
	TextScannerDrawingAction	action (this);
	if (myLines[0].myState == ScannerState::INVALID_STATE)
	{
		myExtents.cx = myExtents.cy = myCurrentLoc.x = myCurrentLoc.y = 0;
		myLines[0].myState = ScannerState::DEFAULT_STATE;
		myLines[0].myStartOffset = 0;
		myScanner->ScanText (myText, &action, ScannerState::DEFAULT_STATE);
		myExtents.cy = myCurrentLoc.y;
	}
	else
	{
		uInt	start = paint.rcPaint.top - myMetric.tmExternalLeading;
		uInt	currentLine = GetLineNumber (mySelectionStartOffset);
		start = (start / myMetric.tmHeight) + myVScroll.nPos;
		start = (start < currentLine) ? start : currentLine;
		start = (start < uInt (myExtents.cy)) ? start : myExtents.cy;
		myCurrentLoc.x = 0;
		myCurrentLoc.y = start;
		// need a better way to determine what length to scan, since the myLines variables are outdated immediately when a change is made
		//myScanner->ScanText (myLines[start].myString, myLines[stop].myEnd - myLines[start].myString, &action, myLines[start].myState);
		myScanner->ScanText (cString (myText) + myLines[start].myStartOffset, myText.Length () - myLines[start].myStartOffset, &action, myLines[start].myState);

		// XXX x extents never get properly updated, this is a dumb way to do it
		myExtents.cx = 0;
		for (sInt i = 0; i <= myExtents.cy; i++)
			if (myLines[i].myLength > uInt (myExtents.cx))
				myExtents.cx = myLines[i].myLength;
	}
	// update the extents
	myExtents.cy = myCurrentLoc.y;

	// update the last line (in case the last line doesn't end with a newline character)
	myLines[myExtents.cy].myLength = myText.Length () - myLines[myExtents.cy].myStartOffset;

	// update the scroll paramters
	SetScrollRange ();

	// fill the unrendered background
	WNDCLASSEX	windowClass;
	HINSTANCE	instance = Application::GetInstance ();
	windowClass.cbSize = sizeof(WNDCLASSEX);
	GetClassInfoEx (instance, GetWindowClassName (), &windowClass);
	if (windowClass.hbrBackground)
		FillRgn (myDC, myUpdateRgn, windowClass.hbrBackground);
	DeleteObject (myUpdateRgn);

	// update the cursor position and show it
	UpdateCursor ();
	ShowCaret (myWindow);

	// now make sure that we are scrolled to the selection
	if (myShouldScrollToSelection)
	{
		ScrollToSelection ();
		myShouldScrollToSelection = false;
	}

	return true;
}

//------------------------------------------------------------------------------
bool
TextWindow::OnEraseWindow
(
	HDC					dc
)
{
	RECT	clientRect;
	GetClientRect (myWindow, &clientRect);
	HRGN	newClipRegion = CreateRectRgn (0, 0, LEFT_BUFFER * myMetric.tmAveCharWidth, clientRect.bottom);
	SelectClipRgn (dc, newClipRegion);
	DeleteObject (newClipRegion);
	Window::OnEraseWindow (dc);
	SelectClipRgn (dc, 0);
	return true;
}

//------------------------------------------------------------------------------
bool
TextWindow::OnKeyDown
(
	sInt				virtualKey,
	uInt				data
)
{
	if (HIWORD (data) bitand KF_UP)
	{
		return true;
	}
	else
	{
		bool	shift = (GetAsyncKeyState (VK_SHIFT) != 0);
		bool	control = (GetAsyncKeyState (VK_CONTROL) != 0);
		switch (virtualKey)
		{
			case VK_PRIOR:   // page up key
 			{
				myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
				myCurrentLoc.y = (myCurrentLoc.y > sInt (myVScroll.nPage)) ? myCurrentLoc.y - myVScroll.nPage : 0;
				myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);
				if (shift)
					ExtendSelection ();
				else if (mySelectionStartOffset == mySelectionEndOffset)
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				else
					SetSelection (mySelectionStartOffset, mySelectionStartOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_NEXT:   // page down key
 			{
				myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
				myCurrentLoc.y += myVScroll.nPage;
				myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);
				if (shift)
					ExtendSelection ();
				else if (mySelectionStartOffset == mySelectionEndOffset)
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				else
					SetSelection (mySelectionEndOffset, mySelectionEndOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_LEFT:   // left arrow
 			{
				// move to the character before the cursor
				myCurrentLocOffset = (myCurrentLocOffset > 0) ? myCurrentLocOffset - 1 : 0;

				// XXX treat tabs as spaces should be an option
				// if the immediately selected character is a space, we conglomerate those
				// as if they were tab keys. That is, move up to tabSize spaces, back to
				// the last tabstop or non-space character.
				if (myText.GetChar (myCurrentLocOffset) == ' ')
				{
					// find our grid location, and compute how many spaces we can move left
					myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
					sInt	spaceCounter = myCurrentLoc.x % myIndentWidth;

					// move the selection back until we have taken that many spaces, or
					// we hit a non-space character
					for (sInt i = 0; i < spaceCounter; i++)
					{
						uInt	nextOffset = myCurrentLocOffset - 1;
						if (myText.GetChar (nextOffset) == ' ')
							myCurrentLocOffset = nextOffset;
						else
							break;
					}
				}
				else
				{
				}

				// update the selection
				if (shift)
					ExtendSelection ();
				else if (mySelectionStartOffset == mySelectionEndOffset)
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				else
					SetSelection (mySelectionStartOffset, mySelectionStartOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_RIGHT:  // right arrow
 			{
				// XXX treat tabs as spaces should be an option
				// if the immediately selected character is a space, we conglomerate those
				// as if they were tab keys. That is, move up to tabSize spaces, to
				// the next tabstop or non-space character.
				if (myText.GetChar (myCurrentLocOffset) == ' ')
				{
					// first, move right one character
					myCurrentLocOffset = (myCurrentLocOffset < myText.Length ()) ? myCurrentLocOffset + 1 : myText.Length ();

					// find our grid location, and compute how many spaces we can move right
					myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
					sInt	spaceCounter = myIndentWidth - (myCurrentLoc.x % myIndentWidth);

					// move the selection until we have taken that many spaces, or
					// we hit a non-space character
					for (sInt i = 0; (i < spaceCounter) and (myText.GetChar (myCurrentLocOffset) == ' '); i++)
						myCurrentLocOffset = (myCurrentLocOffset < myText.Length ()) ? myCurrentLocOffset + 1 : myText.Length ();
				}
				else
				{
					// move right one character
					myCurrentLocOffset = (myCurrentLocOffset < myText.Length ()) ? myCurrentLocOffset + 1 : myText.Length ();
				}

				// update the selection
				if (shift)
					ExtendSelection ();
				else if (mySelectionStartOffset == mySelectionEndOffset)
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				else
					SetSelection (mySelectionEndOffset, mySelectionEndOffset);
				ScrollToSelection ();
				return true;
			}
			
			case VK_UP:		// up arrow
 			{
				
				myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
				myCurrentLoc.y = (myCurrentLoc.y > 0) ? myCurrentLoc.y - 1 : 0;
				myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);
				if (shift)
					ExtendSelection ();
				else if (mySelectionStartOffset == mySelectionEndOffset)
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				else
					SetSelection (mySelectionStartOffset, mySelectionStartOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_DOWN:   // down arrow
 			{
				myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
				myCurrentLoc.y++;
				myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);
				if (shift)
					ExtendSelection ();
				else if (mySelectionStartOffset == mySelectionEndOffset)
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				else
					SetSelection (mySelectionEndOffset, mySelectionEndOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_HOME:	// home key
 			{
				if (control)
				{
					// control home goes to beginning of file
					myCurrentLocOffset = 0;
				}
				else
				{
					// get the line we are currently on
					uInt	line = GetLineNumber (mySelectionStartOffset);
					uInt	homeOffset = myLines[line].myStartOffset;

					// check for white space at beginning of line
					while (isspace (myText.GetChar (homeOffset)))
						homeOffset++;

					// compute the new offset
					// if we are not extending the selection, and the current offset matches up with one of the homes, and 
					// there is a selection
					bool	notExtendingSelection = (not shift);
					bool	selectionIsAtHome = (mySelectionStartOffset == homeOffset) or (mySelectionStartOffset == myLines[line].myStartOffset);
					bool	haveSelection = (mySelectionStartOffset != mySelectionEndOffset);
					if (notExtendingSelection and selectionIsAtHome and haveSelection)
					{
						// move the current location to the selection start
						myCurrentLocOffset = mySelectionStartOffset;
					}
					else
					{
						// if we are at beginning of white space, go to beginning of line, else go to beginning of white space
						myCurrentLocOffset = (myCurrentLocOffset == homeOffset) ? myLines[line].myStartOffset : homeOffset;
					}
				}
				if (shift)
					ExtendSelection ();
				else
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_END:	// end key
 			{
				if (control)
				{
					// control end goes to end of file
					myCurrentLocOffset = myText.Length ();
				}
				else
				{
					// get the line we are currently on
					uInt	line = GetLineNumber (mySelectionStartOffset);
					myCurrentLocOffset = myLines[line].GetEnd ();
				}
				if (shift)
					ExtendSelection ();
				else
					SetSelection (myCurrentLocOffset, myCurrentLocOffset);
				ScrollToSelection ();
				return true;
			}

			case VK_BACK:	// backspace key
			{
				// if there is some text to delete and there is no selection
				if ((myText.Length () > 0) and (mySelectionStartOffset == mySelectionEndOffset))
				{
					// select the character before the cursor
					mySelectionStartOffset = (mySelectionStartOffset > 0) ? mySelectionStartOffset - 1 : 0;

					// XXX treat tabs as spaces should be an option
					// if the immediately selected character is a space, we conglomerate those
					// as if they were tab keys. That is, delete up to tabSize spaces, back to
					// the last tabstop or non-space character.
					if (myText.GetChar (mySelectionStartOffset) == ' ')
					{
						// find our grid location, and compute how many spaces we can take off
						myCurrentLoc = ConvertOffsetToGrid (mySelectionStartOffset);
						sInt	spaceCounter = myCurrentLoc.x % myIndentWidth;

						// move the selection back until we have taken that many spaces, or
						// we hit a non-space character
						for (sInt i = 0; i < spaceCounter; i++)
						{
							uInt	nextOffset = mySelectionStartOffset - 1;
							if (myText.GetChar (nextOffset) == ' ')
								mySelectionStartOffset = nextOffset;
							else
								break;
						}
					}
				}

				// compute the length of the delete string, and if there is something to do
				uInt	deleteLength = mySelectionEndOffset - mySelectionStartOffset;
				if (deleteLength > 0)
				{
					// delete the text and update the selection
					myText.Delete (mySelectionStartOffset, deleteLength);
					SetSelection (mySelectionStartOffset, mySelectionStartOffset);
				}
				ScrollToSelection ();
				return true;
			}

			case VK_DELETE:	// forward delete key
			{
				if (mySelectionStartOffset == mySelectionEndOffset)
				{
				// XXX treat tabs as spaces should be an option
				// if the immediately selected character is a space, we conglomerate those
				// as if they were tab keys. That is, move up to tabSize spaces, to
				// the next tabstop or non-space character.
					if (myText.GetChar (mySelectionEndOffset) == ' ')
					{
						// first, move right one character
						mySelectionEndOffset = (mySelectionEndOffset < myText.Length ()) ? mySelectionEndOffset + 1 : myText.Length ();

						// find our grid location, and compute how many spaces we can move right
						myCurrentLoc = ConvertOffsetToGrid (mySelectionEndOffset);
						sInt	spaceCounter = myIndentWidth - (myCurrentLoc.x % myIndentWidth);

						// move the selection until we have taken that many spaces, or
						// we hit a non-space character
						for (sInt i = 0; (i < spaceCounter) and (myText.GetChar (mySelectionEndOffset) == ' '); i++)
							mySelectionEndOffset = (mySelectionEndOffset < myText.Length ()) ? mySelectionEndOffset + 1 : myText.Length ();
					}
					else
					{
						// move right one character
						mySelectionEndOffset = (mySelectionEndOffset < myText.Length ()) ? mySelectionEndOffset + 1 : myText.Length ();
					}
				}

				// update the selection
				uInt	deleteLength = mySelectionEndOffset - mySelectionStartOffset;
				if (deleteLength > 0)
				{
					myText.Delete (mySelectionStartOffset, deleteLength);
					SetSelection (mySelectionStartOffset, mySelectionStartOffset);
				}
				ScrollToSelection ();
				return true;
			}
		}
	}

	// we didn't handle the message, so do what the parent window would do
	return ScrollingWindow::OnKeyDown (virtualKey, data);
}

//------------------------------------------------------------------------------
bool
TextWindow::OnCharDown
(
	uInt				keyChar,
	sInt				data
)
{
	Character	add[128] = {'\0'};
	switch (keyChar)
	{
		// keys that should be handled in keyDown instead of charDown
		case 0x08:  // backspace 
		{
			MessageBeep(uInt (-1)); 
			// fall through to be eaten
		}
 
		// keys I won't recognize
		case '\n':  // linefeed 
		case 0x1B:  // escape 
		{
			// scroll to the current selection
			ScrollToSelection ();

			// eat the key
			return true;
		}

        case '\r':  // carriage return 
		{
			// put the CR-LF in the add buffer
			add[0] = '\r';
			add[1] = '\n';

			// check to see that there is no whitespace at the beginning
			// of the next line already
			uInt1   nextChar = (mySelectionStartOffset < myText.Length ()) ? myText.GetChar (mySelectionStartOffset) : 0;
			if ((nextChar != '\t') and (nextChar != ' '))
			{
			    // get the line we are currently on
			    uInt	line = GetLineNumber (mySelectionStartOffset);
			    uInt	offset = myLines[line].myStartOffset;
			
			    // copy leading whitespace from the current line to the add buffer,
			    for (sInt i = 0; (i < 127) and (offset + i < myText.Length ()); i++)
			    {
				    uInt1	aChar = myText.GetChar (offset + i);
				    if ((aChar == '\t') or (aChar == ' '))
					    add[i + 2] = aChar;
				    else
					    break;
			    }
			}

			// now insert the text
			InsertText (add);
			return true;
		}

        case '\t':  // tab
		{
			// XXX spaces for tabs should be an option
			myCurrentLoc = ConvertOffsetToGrid (mySelectionStartOffset);
			sInt	spaceCounter = myIndentWidth - (myCurrentLoc.x % myIndentWidth);
			for (sInt i = 0; i < spaceCounter; i++)
				add[i] = ' ';
			InsertText (add);
			return true;
		}

		default:
		{
			add[0] = Character (keyChar);
			InsertText (add);
			return true;
		}
	}
}

//------------------------------------------------------------------------------
bool
TextWindow::OnCommand
(
	sInt				command
)
{
	switch (command)
	{
		case MENU_FILE_NEW:
			{
				//	XXX this is temporary - really need a "clone" method,
				//	XXX or a way to create a window depending on the file 
				//	XXX extension
				TextWindow*	window = new TextWindow (100, 80);
				window->ShowWindow ();
			}
			return true;

		case MENU_FILE_OPEN:
			{
				OPENFILENAME	open;
				sInt1			buffer[512];
				buffer[0] = 0;
				open.lStructSize = sizeof (OPENFILENAME);
				open.hwndOwner = myWindow;
				open.hInstance = 0;
				open.lpstrFilter = "All Files\0*.*\0C++ Files\0*.C;*.CXX;*.CPP;*.H\0\0";
				open.lpstrCustomFilter = 0;
				open.nFilterIndex = 0;
				open.lpstrFile = buffer;
				open.nMaxFile = 512;
				open.lpstrFileTitle = 0;
				open.lpstrInitialDir = 0;
				open.lpstrTitle = 0;
				open.Flags = OFN_EXPLORER bitor OFN_FILEMUSTEXIST bitor OFN_HIDEREADONLY;
				open.lpstrDefExt = 0;
				open.lCustData = 0;

				if (GetOpenFileName (&open))
				{
					//	XXX this is temporary - really need a "clone" method,
					//	XXX or a way to create a window depending on the file 
					//	XXX extension
					TextWindow*	window = new TextWindow (buffer, 80, 40);
					window->ShowWindow ();
				}
			}
			return true;

		case MENU_FILE_CLOSE:
			PostMessage (myWindow, WM_CLOSE, 0, 0);
			return true;

		case MENU_FILE_SAVE:
			if (myFilename != "")
			{
				myText.ToFile (myFilename);
				return true;
			}
			// fall through to save as

		case MENU_FILE_SAVE_AS:
			{
				OPENFILENAME	open;
				Character		buffer[512];
				if (myFilename == "")
					GetWindowText (myWindow, buffer, 512);
				else
					strcpy (buffer, myFilename);
				open.lStructSize = sizeof (OPENFILENAME);
				open.hwndOwner = myWindow;
				open.hInstance = 0;
				open.lpstrFilter = "All Files\0*.*\0C++ Files\0*.C;*.CXX;*.CPP;*.H\0\0";
				open.lpstrCustomFilter = 0;
				open.nFilterIndex = 0;
				open.lpstrFile = buffer;
				open.nMaxFile = 512;
				open.lpstrFileTitle = 0;
				open.lpstrInitialDir = 0;
				open.lpstrTitle = 0;
				open.Flags = OFN_EXPLORER bitor OFN_OVERWRITEPROMPT bitor OFN_PATHMUSTEXIST;
				open.lpstrDefExt = 0;
				open.lCustData = 0;

				if (not GetSaveFileName (&open))
					return false;
				WIN32_FIND_DATA		data;
				myFilename = buffer;
				HANDLE				handle = FindFirstFile (myFilename, &data);
				if (handle != INVALID_HANDLE_VALUE)
				{
					FindClose (handle);
					myText.ToFile (myFilename);
					SetName (data.cFileName);
				}
				else
				{
					myText.ToFile (myFilename);
					handle = FindFirstFile (myFilename, &data);
					FindClose (handle);
					SetName (data.cFileName);
				}
			}
			return true;

		case MENU_FILE_PAGE_SETUP:
			{
				//	XXX	should I save this info with the file?
				PAGESETUPDLG	page;
				page.lStructSize = sizeof (PAGESETUPDLG);
				page.hwndOwner = myWindow;
				page.hDevMode = 0;
				page.hDevNames = 0;
				page.Flags = PSD_DEFAULTMINMARGINS;
				page.hInstance = 0;
				page.lCustData = 0;
				PageSetupDlg (&page);
			}
			return true;

		case MENU_FILE_PRINT:
			return true;

		case MENU_FILE_EXIT:
			ApplicationBody::Quit ();
			return true;

		case MENU_EDIT_CUT:
			{
				Copy ();
			}
			// drop through to clear

		case MENU_EDIT_CLEAR:
			{
				// delete the selected text
				uInt	selectionLength = mySelectionEndOffset - mySelectionStartOffset;
				myText.Delete (mySelectionStartOffset, selectionLength);

				// reset the selection
				SetSelection (mySelectionStartOffset, mySelectionStartOffset);

				// say that we should update the scroll location
				myShouldScrollToSelection = true;
			}
			return true;

		case MENU_EDIT_COPY:
			{
				Copy ();
			}
			return true;

		case MENU_EDIT_PASTE:
			{
				Paste ();
			}
			return true;

		case MENU_EDIT_FIND:
			{
			/*
				FINDREPLACE	find;
				FindText ();
			*/
			}
			return true;

		case MENU_EDIT_FIND_AGAIN:
			return true;

		case MENU_EDIT_REPLACE:
			{
			/*
				FINDREPLACE	find;
				ReplaceText ();
			*/
			}
			return true;

		case MENU_EDIT_SETTINGS:
			return true;

		case MENU_LANG_ENGLISH:
			{
				//	xxx	really need a subclass of TextWindow that is english only
				//	XXX	it would also be REALLY cool if these were plugins loaded from an extensions directory
				//	the idea here is to switch the window object attached to the ui window for one of the desired type
				new TextWindow (TextWindow::myTextScanner, *this);
				InvalidateRect (myWindow, 0, false);
				delete this;	//	sigh
			}
			return true;

		case MENU_LANG_C:
			{
				// switch the window to a different class
				new CPPWindow (*this);
				InvalidateRect (myWindow, 0, false);
				delete this;	//	sigh
			}
			return true;

		default:
			return false;
	}
}

//------------------------------------------------------------------------------
bool
TextWindow::OnMouseDown
(
	uInt						which,
	const POINTS&				point,
	uInt						flags
)
{
	if (which == LEFT_BUTTON)
	{
		myCurrentLoc.x = point.x;
		myCurrentLoc.y = point.y;
		myCurrentLoc = ConvertScreenCoordsToGrid (myCurrentLoc);
		myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);

		// if the user has the shift key pressed when they click
		if (flags bitand MK_SHIFT)
			ExtendSelection ();
		else
			SetSelection (myCurrentLocOffset, myCurrentLocOffset);
	}
	return Window::OnMouseDown (which, point, flags);
}

//------------------------------------------------------------------------------
bool
TextWindow::OnMouseDoubleDown
(
	uInt						which,
	const POINTS&				point,
	uInt						flags
)
{
	//	XXX temporary double click behavior
	return OnMouseDown (which, point, flags);
}

//------------------------------------------------------------------------------
bool
TextWindow::OnMouseMove
(
	const POINTS&				point,
	uInt						flags
)
{
	if (myMouseIsDown bitand LEFT_BUTTON)
	{
		myCurrentLoc.x = point.x;
		myCurrentLoc.y = point.y;
		myCurrentLoc = ConvertScreenCoordsToGrid (myCurrentLoc);
		uInt		lastLocOffset = myCurrentLocOffset;
		myCurrentLocOffset = ConvertGridToOffset (myCurrentLoc);
		if (myCurrentLocOffset != lastLocOffset)
			ExtendSelection ();
	}
	return Window::OnMouseMove (point, flags);
}

//------------------------------------------------------------------------------
bool
TextWindow::OnActivate
(
	void
)
{
	CreateCaret (myWindow, 0, 2, myMetric.tmHeight);
	SetCaretPos (myCaretLoc.x, myCaretLoc.y);
	ShowCaret (myWindow);
	return true;
}

//------------------------------------------------------------------------------
bool
TextWindow::OnDeactivate
(
	void
)
{
	DestroyCaret ();
	return true;
}

//------------------------------------------------------------------------------
bool
TextWindow::OnSizingWindow
(
	sInt						side,
	RECT*						rect
)
{
	sInt	width = (rect->right - rect->left) - myWidthDelta,
			height = (rect->bottom - rect->top) - myHeightDelta;
	width -= (LEFT_BUFFER * myMetric.tmAveCharWidth);
	height -= (myMetric.tmExternalLeading << 1);
	width -= width % myMetric.tmAveCharWidth;
	height -= height % myMetric.tmHeight;
	ScrollingWindow::SetScrollRange (width / myMetric.tmAveCharWidth, height / myMetric.tmHeight, myExtents.cx + RIGHT_BUFFER, myExtents.cy + BOTTOM_BUFFER);
	width += (LEFT_BUFFER * myMetric.tmAveCharWidth);
	height += (myMetric.tmExternalLeading << 1);
	switch (side)
	{
		case WMSZ_BOTTOM:
			rect->bottom = rect->top + height + myHeightDelta;
			break;
		case WMSZ_BOTTOMLEFT:
			rect->left = rect->right - (width + myWidthDelta);
			rect->bottom = rect->top + height + myHeightDelta;
			break;
		case WMSZ_BOTTOMRIGHT:
			rect->right = rect->left + width + myWidthDelta;
			rect->bottom = rect->top + height + myHeightDelta;
			break;
		case WMSZ_LEFT:
			rect->left = rect->right - (width + myWidthDelta);
			break;
		case WMSZ_RIGHT:
			rect->right = rect->left + width + myWidthDelta;
			break;
		case WMSZ_TOP:
			rect->top = rect->bottom - (height + myHeightDelta);
			break;
		case WMSZ_TOPLEFT:
			rect->left = rect->right - (width + myWidthDelta);
			rect->top = rect->bottom - (height + myHeightDelta);
			break;
		case WMSZ_TOPRIGHT:
			rect->right = rect->left + width + myWidthDelta;
			rect->top = rect->bottom - (height + myHeightDelta);
			break;
	}
	return true;
}

//------------------------------------------------------------------------------
void
TextWindow::SizeWindow
(
	sInt						width,
	sInt						height
)
{
	width = (width + LEFT_BUFFER) * myMetric.tmAveCharWidth;
	height = (height * myMetric.tmHeight) + (myMetric.tmExternalLeading << 1);
	Window::SizeWindow (width, height);
}

//------------------------------------------------------------------------------
void
TextWindow::SetCursor
(
	void
)
{
	::SetCursor (LoadCursor (0, IDC_IBEAM));
}

//------------------------------------------------------------------------------
Text
TextWindow::GetWindowClassName
(
	void
)
{
	return Text (TEXT_WINDOW_CLASS_NAME);
}

//------------------------------------------------------------------------------
HACCEL
TextWindow::GetWindowAccelerators
(
	void
)
{
	HINSTANCE	instance = Application::GetInstance ();
	return LoadAccelerators (instance, MAKEINTRESOURCE (TEXT_WINDOW_ACCELERATORS));
}

//------------------------------------------------------------------------------
void
TextWindow::DrawAction
(
	cString						buffer,
	uInt						length,
	uInt2						action,
	cString						nextBuffer,
	uInt2						nextState
)
{
	uInt	currentOffset = buffer - myText;
	if (action == TextScannerAction::OUTPUT_NEWLINE)
	{
		if ((currentOffset < mySelectionStartOffset) or (currentOffset >= mySelectionEndOffset))
			LineOut (PALETTERGB (255, 255, 255));
		else
			LineOut (PALETTERGB (0, 255, 255));
		if (myCurrentLoc.x > myExtents.cx)
			myExtents.cx = myCurrentLoc.x;
		myCurrentLoc.x = 0;
		myLines[myCurrentLoc.y].myLength = currentOffset - myLines[myCurrentLoc.y].myStartOffset;
		myCurrentLoc.y++;
		myLines[myCurrentLoc.y].myState = nextState;
		myLines[myCurrentLoc.y].myStartOffset = nextBuffer - myText;
	}
	else
	{
		sInt	renderLength = sInt (length);

		// compute the delta from the current offset to the beginning of the selection
		sInt	lengthToSelectionStart = mySelectionStartOffset - currentOffset;

		// if all of the string we have to render is before the start of the selection
		if (renderLength < lengthToSelectionStart)
		{
			// render the whole string unselected
			StringOut (buffer, renderLength, PALETTERGB (255, 255, 255));
			return;
		}
		// otherwise, if the selection has size
		else if (lengthToSelectionStart > 0)
		{
			// render the part up to the selection start, and update the counters
			StringOut (buffer, lengthToSelectionStart, PALETTERGB (255, 255, 255));
			buffer += lengthToSelectionStart;
			currentOffset = mySelectionStartOffset;
			renderLength -= lengthToSelectionStart;
		}

		// compute the delta from the current offset to the end of the selection
		sInt	lengthToSelectionEnd = mySelectionEndOffset - currentOffset;

		// if all that remains to be rendered is in selection
		if (renderLength < lengthToSelectionEnd)
		{
			// render the whole string selected
			StringOut (buffer, renderLength, PALETTERGB (0, 255, 255));
			return;
		}
		// otherwise, if the seleciton has size
		else if (lengthToSelectionEnd > 0)
		{
			// render the part up to the selection end, and update the counters
			StringOut (buffer, lengthToSelectionEnd, PALETTERGB (0, 255, 255));
			buffer += lengthToSelectionEnd;
			renderLength -= lengthToSelectionEnd;
		}

		// if anything is left to be rendered
		if (renderLength > 0)
		{
			// render the whole thing unselected
			StringOut (buffer, renderLength, PALETTERGB (255, 255, 255));
		}
	}
}

//------------------------------------------------------------------------------
LRESULT CALLBACK
TextWindow::WindowProc
(
	HWND						window,
	UINT						message,
	WPARAM						wParam,
	LPARAM						lParam
)
{
	TextWindow	*theWindow = (TextWindow *) GetWindowLong (window, GWL_USERDATA);
	if (theWindow)
		switch (message)
		{
			case WM_LBUTTONDBLCLK:
				if (theWindow->OnMouseDoubleDown (LEFT_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_MBUTTONDBLCLK:
				if (theWindow->OnMouseDoubleDown (MIDDLE_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_RBUTTONDBLCLK:
				if (theWindow->OnMouseDoubleDown (RIGHT_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
		}
	return ScrollingWindow::WindowProc (window, message, wParam, lParam);
}

//------------------------------------------------------------------------------
void
TextWindow::Startup
(
	void
)
{
	myTextScanner = new TextScanner;
}

//------------------------------------------------------------------------------
void
TextWindow::SetFont
(
	const Text&					fontName,
	uInt						size,
	uInt						weight
)
{
	HDC		dc = GetDC (myWindow);
	HFONT	font = SimpleCreateFont (dc, size, weight, fontName),
			oldFont = HFONT (SelectObject (dc, font));
	if (myOldFont)
		DeleteObject (oldFont);
	else
		myOldFont = oldFont;
	GetTextMetrics (dc, &myMetric);
	ReleaseDC (myWindow, dc);
	SetScrollSize (myMetric.tmAveCharWidth, myMetric.tmHeight);
}

//------------------------------------------------------------------------------
void
TextWindow::SetTabSize
(
	sInt						size
)
{
	assert (size > 1);
	myIndentWidth = size;
}

//------------------------------------------------------------------------------
sInt
TextWindow::GetTabSize
(
	void
)
{
	return myIndentWidth;
}

//------------------------------------------------------------------------------
void
TextWindow::SetText
(
	const Text&					text
)
{
	myText = text;
	myText.MakeUnique ();
	mySelectionStartOffset = mySelectionEndOffset = mySelectionOriginOffset = myCurrentLocOffset = 0;
	myCurrentLoc.x = myCurrentLoc.y = 0;
	myCaretLoc = ConvertGridToScreenCoords (myCurrentLoc);
	myExtents.cx = myExtents.cy = 0;
	SetCaretPos (myCaretLoc.x, myCaretLoc.y);
	InvalidateRect (myWindow, 0, false);
}

//------------------------------------------------------------------------------
Text
TextWindow::GetText
(
	void
) const
{
	return myText;
}

//------------------------------------------------------------------------------
void
TextWindow::SetSelection
(
	uInt						selectionStartOffset,
	uInt						selectionEndOffset
)
{
	//	XXX can I just repaint the part of the selection that changes?
	// redraw the window if there is a selection, or will be a selection
	bool	shouldInvalidate = (mySelectionStartOffset != mySelectionEndOffset) or (selectionStartOffset != selectionEndOffset);
	if (shouldInvalidate)
		InvalidateRect (myWindow, 0, false);

	// set the new selection range
	mySelectionStartOffset = mySelectionOriginOffset = selectionStartOffset;
	mySelectionEndOffset = myCurrentLocOffset = selectionEndOffset;

	// Update the cursor if we didn't invalidate the window
	if (not shouldInvalidate)
		UpdateCursor ();
}

//------------------------------------------------------------------------------
void
TextWindow::InsertText
(
	const Text&					text
)
{
	// insert the new text
	uInt	length = mySelectionEndOffset - mySelectionStartOffset;
	uInt	insertLength = text.Length ();
	myText.Delete (mySelectionStartOffset, length);
	myText.Insert (text, mySelectionStartOffset, insertLength);

	// update the selection
	uInt	newSelectionOffset = mySelectionStartOffset + insertLength;
	SetSelection (newSelectionOffset, newSelectionOffset);

	// say that we should update the scroll location
	myShouldScrollToSelection = true;

	// redraw the window
	InvalidateRect (myWindow, 0, false);
}

//------------------------------------------------------------------------------
TextWindow::TextWindow
(
	const HandleTo<TextScanner>&	scanner,
	const Text&						filename,
	uInt							width,
	uInt							height
) : ScrollingWindow (TEXT_WINDOW_CLASS_NAME, Text ("Untitled"), ScrollingWindow::SCROLL_BOTH, WS_OVERLAPPEDWINDOW bitor WS_HSCROLL bitor WS_VSCROLL, WS_EX_CLIENTEDGE, CW_USEDEFAULT, 0, 1, 1), myLines (4)
{
    InitTextWindow (width, height);
	myScanner = scanner;
	OpenFile (filename);
}

//------------------------------------------------------------------------------
TextWindow::TextWindow
(
	const HandleTo<TextScanner>&	scanner,
	const TextWindow&				textWindow
) : ScrollingWindow (textWindow), myLines (4)
{
	// clear the check on LANG_C and set the check on ENGLISH
	HMENU	hMenu = GetMenu (myWindow);
	CheckMenuItem (hMenu, MENU_LANG_ENGLISH, MF_CHECKED);
	CheckMenuItem (hMenu, MENU_LANG_C, MF_UNCHECKED);

	myScanner = scanner;
	myOldFont = textWindow.myOldFont;
	myMetric = textWindow.myMetric;
	myText = textWindow.myText;
	myTabWidth = textWindow.myTabWidth;
	myIndentWidth = textWindow.myIndentWidth;
	myExtents = textWindow.myExtents;
	myDC = textWindow.myDC;
	myUpdateRgn = textWindow.myUpdateRgn;
	myCaretLoc = textWindow.myCaretLoc;
	myCurrentLoc = textWindow.myCurrentLoc;
	mySelectionOriginOffset = textWindow.mySelectionOriginOffset;
	myCurrentLocOffset = textWindow.myCurrentLocOffset;
	mySelectionStartOffset = textWindow.mySelectionStartOffset;
	mySelectionEndOffset = textWindow.mySelectionEndOffset;
	myFilename = textWindow.myFilename;
}

//------------------------------------------------------------------------------
void
TextWindow::InitTextWindow
(
	uInt							width,
	uInt							height
)
{
	// clear the check on LANG_C and set the check on ENGLISH
	HMENU	hMenu = GetMenu (myWindow);
	CheckMenuItem (hMenu, MENU_LANG_ENGLISH, MF_CHECKED);
	CheckMenuItem (hMenu, MENU_LANG_C, MF_UNCHECKED);

	//	XXX	fetch the tab size from the preferences
	myTabWidth = Registry::Read ("Text", "TabWidth", sInt4 (DEFAULT_TAB_WIDTH));
	myIndentWidth = Registry::Read ("Text", "IndentWidth", sInt4 (DEFAULT_INDENT_WIDTH));
	HDC		dc = GetDC (myWindow);
	SetMapMode (dc, MM_TEXT);
	SetBkMode (dc, OPAQUE);
	ReleaseDC (myWindow, dc);
	myOldFont = 0;
	SetFont (DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, FW_NORMAL);
	SizeWindow (width, height);
}

//------------------------------------------------------------------------------
void
TextWindow::SetEmptyUntitled
(
	void
)
{
    // this defaults to an untitled window
	Character	untitled[32];
	sprintf (untitled, "Untitled %d", myUntitledCount++);
	SetName (untitled);
	myFilename = "";
	SetText ("");
}

//------------------------------------------------------------------------------
void
TextWindow::SetScrollRange
(
	void
)
{
	RECT	clientRect;
	GetClientRect (myWindow, &clientRect);
	sInt	width = (clientRect.right / myMetric.tmAveCharWidth) - LEFT_BUFFER,
			height = (clientRect.bottom - (myMetric.tmExternalLeading << 1)) / myMetric.tmHeight;
	ScrollingWindow::SetScrollRange (width, height, myExtents.cx + RIGHT_BUFFER, myExtents.cy + BOTTOM_BUFFER);
}

//------------------------------------------------------------------------------
void
TextWindow::StringOut
(
	cString						buffer,
	uInt						length,
	COLORREF					bgColor
)
{
	SetBkColor (myDC, bgColor);
	int		tabs = myTabWidth * myMetric.tmAveCharWidth;
	sInt	tabOrigin = (LEFT_BUFFER - myHScroll.nPos) * myMetric.tmAveCharWidth;
	POINT	screenCoord = ConvertGridToScreenCoords (myCurrentLoc);
	DWORD	size = TabbedTextOut (myDC, screenCoord.x, screenCoord.y, buffer, length, 1, &tabs, tabOrigin);
	myCurrentLoc.x += LOWORD (size) / myMetric.tmAveCharWidth;
	HRGN	drawnRgn = CreateRectRgn (screenCoord.x, screenCoord.y, screenCoord.x + LOWORD (size), screenCoord.y + HIWORD (size));
	CombineRgn (myUpdateRgn, myUpdateRgn, drawnRgn, RGN_DIFF);
	DeleteObject (drawnRgn);
}

//------------------------------------------------------------------------------
void
TextWindow::LineOut
(
	COLORREF					bgColor
)
{
	SetBkColor (myDC, bgColor);
	// we use SHRT_MAX here to cause the drawing to go to the end of the line
	int		tabs = SHRT_MAX;
	sInt	tabOrigin = (LEFT_BUFFER - myHScroll.nPos) * myMetric.tmAveCharWidth;
	POINT	screenCoord = ConvertGridToScreenCoords (myCurrentLoc);
	DWORD	size = TabbedTextOut (myDC, screenCoord.x, screenCoord.y, "\t", 1, 1, &tabs, tabOrigin);
	HRGN	drawnRgn = CreateRectRgn (screenCoord.x, screenCoord.y, screenCoord.x + LOWORD (size), screenCoord.y + HIWORD (size));
	CombineRgn (myUpdateRgn, myUpdateRgn, drawnRgn, RGN_DIFF);
	DeleteObject (drawnRgn);
}

//------------------------------------------------------------------------------
void
TextWindow::OpenFile
(
	const Text&					filename
)
{
	WIN32_FIND_DATA		data;
	HANDLE				handle = FindFirstFile (filename, &data);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindClose (handle);
		SetName (data.cFileName);
		Character   fullPathBuffer[MAX_PATH];
		String      unused;
		if (GetFullPathName (cString (filename), MAX_PATH, fullPathBuffer, &unused) < MAX_PATH)
		    myFilename = Text (fullPathBuffer);
		cerr << "Opened file:" << myFilename << endl;
		SetText (Text::FromFile (myFilename));
	}
	else
	{
        Text    message = Text (filename) + " does not exist. Would you like to create it?";
        switch (MessageBox (myWindow, message, "File not found", MB_YESNOCANCEL))
        {
            case IDNO:
            {
                SetEmptyUntitled ();
		        break;
		    }
		    
		    case IDYES:
		    {
		        // this creates the file, then tries again to open it
		        Text    emptyfile ("");
		        emptyfile.ToFile (filename);
		        OpenFile (filename);
		        break;
		    }

		    case IDCANCEL:
		    {
		        // this closes the window
		        PostMessage (myWindow, WM_CLOSE, 0, 0);
		        break;
		    }
		}
	}
}

//------------------------------------------------------------------------------
POINT
TextWindow::ConvertScreenCoordsToGrid
(
	const POINT&				point
) const
{
	POINT	grid;
	grid.x = ((point.x / myMetric.tmAveCharWidth) - LEFT_BUFFER) + myHScroll.nPos;
	grid.x += ((point.x % myMetric.tmAveCharWidth) << 1) / myMetric.tmAveCharWidth;
	grid.x = (grid.x > 0) ? grid.x : 0;
	grid.y = ((point.y - myMetric.tmExternalLeading) / myMetric.tmHeight) + myVScroll.nPos;
	grid.y = (grid.y > 0) ? grid.y : 0;
	return grid;
}

//------------------------------------------------------------------------------
POINT
TextWindow::ConvertGridToScreenCoords
(
	const POINT&				point
) const
{
	POINT	screen;
	screen.x = ((point.x + LEFT_BUFFER) - myHScroll.nPos) * myMetric.tmAveCharWidth;
	screen.y = ((point.y - myVScroll.nPos) * myMetric.tmHeight) + myMetric.tmExternalLeading;
	return screen;
}

//------------------------------------------------------------------------------
uInt
TextWindow::ConvertGridToOffset
(
	const POINT&				point
)
{
	uInt		y = (point.y > myExtents.cy) ? myExtents.cy : point.y;
	uInt		offset = myLines[y].myStartOffset;
	uInt		end = myLines[y].GetEnd ();
	sInt		x = 0,
				delta = point.x;
	while ((delta > 0) and (offset != end))
	{
		sInt	nextTabSize = myTabWidth - (x % myTabWidth);
		switch (myText.GetChar (offset))
		{
			case 0:
				break;
			case '\t':
				if ((delta > nextTabSize) or (delta > ((nextTabSize >> 1) + (nextTabSize bitand 1))))
				{
					x += nextTabSize;
					offset++;
				}
				else
					x = point.x;
				break;
			default:
				x++;
				offset++;
				break;
		}
		delta = point.x - x;
	}
	return offset;
}

//------------------------------------------------------------------------------
POINT
TextWindow::ConvertOffsetToGrid
(
	uInt						offset,
	uInt						yHint
)
{
	assert (offset <= myText.Length ());
	POINT	point;

	// find what line the offset is in
	point.y = GetLineNumber (offset, yHint);

	// find what column the offset is in
	point.x = 0;
	for (uInt i = myLines[point.y].myStartOffset; i < offset; i++)
		switch (myText.GetChar (i))
		{
			case 0:
				break;
			case '\t':
				point.x += myTabWidth - (point.x % myTabWidth);
				break;
			default:
				point.x++;
				break;
		}
	return point;
}

//------------------------------------------------------------------------------
uInt
TextWindow::GetLineNumber
(
	uInt						offset,
	uInt						yHint
)
{
	assert (offset <= myText.Length ());
	yHint = (yHint < uInt (myExtents.cy)) ? yHint : uInt(myExtents.cy);
	while ((offset > myLines[yHint].GetEnd ()) and (yHint < uInt (myExtents.cy)))
		yHint++;
	return yHint;	
}

//------------------------------------------------------------------------------
void
TextWindow::UpdateCursor
(
	void
)
{
	myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
	myCaretLoc = ConvertGridToScreenCoords (myCurrentLoc);
	SetCaretPos (myCaretLoc.x, myCaretLoc.y);
}

//------------------------------------------------------------------------------
void
TextWindow::ExtendSelection
(
	void
)
{
	if (myCurrentLocOffset < mySelectionOriginOffset)
	{
		mySelectionStartOffset = myCurrentLocOffset;
		mySelectionEndOffset = mySelectionOriginOffset;
	}
	else
	{
		mySelectionStartOffset = mySelectionOriginOffset;
		mySelectionEndOffset = myCurrentLocOffset;
	}

	// redraw the window
	InvalidateRect (myWindow, 0, false);
}

//------------------------------------------------------------------------------
void
TextWindow::ScrollToSelection
(
	sInt						xOffset
)
{
	myCurrentLoc = ConvertOffsetToGrid (myCurrentLocOffset);
	sInt	x = myCurrentLoc.x + xOffset;
	HScrollTo ((x > 0) ? x : 0);
	VScrollTo (myCurrentLoc.y);
}

//------------------------------------------------------------------------------
void
TextWindow::Copy
(
	void
)
{
	// if there is something to copy
	if (mySelectionStartOffset != mySelectionEndOffset)
	{
		// open the clipboard and check that it succeeded
		if (OpenClipboard (NULL))
		{
			// empty the clipboard
			EmptyClipboard ();
 
			// figure out how long the selection is
			sInt	selectionLength = mySelectionEndOffset - mySelectionStartOffset;

			// allocate a global handle for the text, and check that we got it
			HGLOBAL globalHandle = GlobalAlloc (GMEM_MOVEABLE, selectionLength + 1);
			if (globalHandle)
			{
				// lock the handle and check that it succeeds
				String	globalPtr = String (GlobalLock (globalHandle));
				if (globalPtr)
				{
					// copy the selected text to the buffer. 
					memcpy (globalPtr, cString (myText) + mySelectionStartOffset, selectionLength); 
					globalPtr[selectionLength] = '\0';

					// unlock the handle
					GlobalUnlock (globalHandle); 

					// place the handle on the clipboard
					SetClipboardData (CF_TEXT, globalHandle); 
				}
			}

			// close the clipboard since we're done with it
			CloseClipboard ();
		}
	}
}

//------------------------------------------------------------------------------
void
TextWindow::Paste
(
	void
)
{
	// if there is text data on the clipboard
    if (IsClipboardFormatAvailable (CF_TEXT)) 
	{
		// open the clipboard and check that it succeeded
		if (OpenClipboard (NULL))
		{
			// get the handle to the text data, and check that it succeeds
			HGLOBAL globalHandle = GetClipboardData (CF_TEXT); 
			if (globalHandle) 
			{ 
				// lock the handle, and check that is succeeds
				cString	globalPtr = cString (GlobalLock (globalHandle));
				if (globalPtr) 
				{
					// get the data and put it into the document
					Text	pasteText (globalPtr);
					InsertText (pasteText);

					// unlock the handle
					GlobalUnlock (globalHandle);
				} 
			} 
			// close the clipboard since we're done with it
			CloseClipboard (); 
		}
	}
}

//------------------------------------------------------------------------------
HFONT
TextWindow::SimpleCreateFont
(
	HDC							dc,
	sInt						size,
	uInt						weight,
	const Text&					name
)
{
	LOGFONT		logFont;
	logFont.lfHeight = MulDiv (size, GetDeviceCaps (dc, LOGPIXELSY), -72);
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = weight;
	logFont.lfItalic = false;
	logFont.lfUnderline = false;
	logFont.lfStrikeOut = false;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_RASTER_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = DEFAULT_QUALITY;
	logFont.lfPitchAndFamily = FIXED_PITCH bitor FF_MODERN;
	strcpy (logFont.lfFaceName, name);
	return CreateFontIndirect (&logFont);
}

//------------------------------------------------------------------------------
