//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TOKEN_WINDOW_H_
#define		_BW_TOKEN_WINDOW_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCROLLING_WINDOW_H_
#include	"scrolling_window.h"
#endif	//	_BW_SCROLLING_WINDOW_H_

#ifndef		_BW_TEXT_SCANNER_H_
#include	"text_scanner.h"
#endif	//	_BW_TEXT_SCANNER_H_

#ifndef		_BW_LINE_STATE_H_
#include	"line_state.h"
#endif	//	_BW_LINE_STATE_H_

#ifndef		_BW_HANDLE_TO_H_
#include	"handle_to.h"
#endif	//	_BW_HANDLE_TO_H_

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#define		TEXT_WINDOW_CLASS_NAME		"TextWindowClass"

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	TextWindow : public ScrollingWindow
{
	public:
		TextWindow (uInt width, uInt height);
		TextWindow (const Text& filename, uInt width, uInt height);
virtual	~TextWindow (void);
virtual	bool					OnCloseWindow (void);
virtual	void					Pulse (void);
virtual	bool					OnPaintWindow (PAINTSTRUCT& paint);
virtual	bool					OnEraseWindow (HDC dc);
virtual	bool					OnKeyDown (sInt virtualKeyCode, uInt data);
virtual	bool					OnCharDown (uInt keyChar, sInt data);
virtual	bool					OnCommand (sInt command);
virtual	bool					OnMouseDown (uInt which, const POINTS& point, uInt flags);
virtual	bool					OnMouseDoubleDown (uInt which, const POINTS& point, uInt flags);
virtual	void					OnMouseMove (const POINTS& point, uInt flags);
virtual	bool					OnActivate (void);
virtual	bool					OnDeactivate (void);
virtual	bool					OnSizingWindow (sInt code, RECT* rect);
virtual	void					SizeWindow (sInt width, sInt height);
virtual	void					SetCursor (void);
virtual	Text					GetWindowClassName (void);
virtual	HACCEL					GetWindowAccelerators (void);
virtual	void					DrawAction (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);

static	LRESULT CALLBACK		WindowProc (HWND window, UINT message, WPARAM wParam, LPARAM lParam);
static	void					Startup (void);

		// items related to the text editing "control"
		void					SetFont (const Text& fontName, uInt size, uInt weight = FW_NORMAL);
		void					SetTabSize (sInt size);
		sInt					GetTabSize (void);
		void					SetText (const Text& text);
		Text					GetText (void) const;
		void					SetSelection (uInt selectionStartOffset, uInt selectionEndOffset);
		uInt                    GetSelectionStart (void) const;
		uInt                    GetSelectionEnd (void) const;
		void					InsertText (const Text& text);

	public:
		const enum
		{
			LEFT_BUFFER = 3,
			RIGHT_BUFFER = 2,
			BOTTOM_BUFFER = 2,
			DEFAULT_INDENT_WIDTH = 4,
			DEFAULT_TAB_WIDTH = 8
		};

	protected:
		TextWindow (const HandleTo<TextScanner>& scanner, const Text& filename, uInt width, uInt height);
		TextWindow (const HandleTo<TextScanner>& scanner, const TextWindow& textWindow);
		void                            InitTextWindow (uInt width, uInt height);
		void                            SetEmptyUntitled (void);
		void					        InitScrollbars (void);
		void					        SetScrollRange (void);
		void					        StringOut (cString buffer, uInt length, COLORREF bgColor);
		void					        LineOut (COLORREF bgColor);
		void					        OpenFile (const Text& filename);
		POINT					        ConvertScreenCoordsToGrid (const POINT& point) const;
		POINT					        ConvertGridToScreenCoords (const POINT& point) const;
		uInt					        ConvertGridToOffset (const POINT& point);
		POINT					        ConvertOffsetToGrid (uInt offset, uInt yHint = 0);
		uInt					        GetLineNumber (uInt offset, uInt yHint = 0);
		void					        UpdateCursor (void);
		void					        ExtendSelection (void);
		void					        ScrollToSelection (sInt xOffset = 0);
		void					        Copy (void);
		void					        Paste (void);
static	HFONT					        SimpleCreateFont (HDC dc, sInt size, uInt weight, const Text& name);

	protected:
		HFONT					        myOldFont;
		TEXTMETRIC				        myMetric;
		HandleTo<TextScanner>		    myScanner;
		Text					        myText;
		sInt					        myIndentWidth;
		sInt                            myTabWidth;
		SIZE					        myExtents;
		HDC						        myDC;
		HRGN					        myUpdateRgn;
		POINT					        myCaretLoc;
		POINT					        myCurrentLoc;

		uInt					        mySelectionOriginOffset;
		uInt					        myCurrentLocOffset;
		uInt					        mySelectionStartOffset;
		uInt					        mySelectionEndOffset;

		ResizableVectorOf<LineState>    myLines;
		Text					        myFilename;

		bool					        myShouldScrollToSelection;

static	uInt					        myUntitledCount;
static	HandleTo<TextScanner>		    myTextScanner;
};

//------------------------------------------------------------------------------

#endif	//	_BW_TOKEN_WINDOW_H_
