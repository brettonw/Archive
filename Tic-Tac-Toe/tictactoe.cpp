// hiq.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <vector>
#include "tictactoe.h"
#include "assert.h"
#include "debug.h"
#include "free_list.h"
#define MAX_LOADSTRING 100

//-----------------------------------------------------------------------------
typedef unsigned char	uchar;
typedef unsigned int	uint;

//-----------------------------------------------------------------------------
const int   TTT_DIM = 3;
const int   PEG_SIZE = 12;
const int   SPACE_SIZE = 4;
const int   BOARD_SIZE = (PEG_SIZE * TTT_DIM) + (TTT_DIM - 1);
const int   CLICK_GRID_SIZE = BOARD_SIZE + SPACE_SIZE;
const int   LEVEL_LIMIT = 10;

//-----------------------------------------------------------------------------
int         gLevelChild[LEVEL_LIMIT + 2];

//-----------------------------------------------------------------------------
uchar    test_board[TTT_DIM][TTT_DIM] = 
{
{2, 1, 2},
{2, 1, 1},
{1, 2, 1}
};

//-----------------------------------------------------------------------------
uchar    win_conditions[16][TTT_DIM][TTT_DIM] = 
{
    {
        {1, 1, 1},
        {0, 0, 0},
        {0, 0, 0}
    },
    {
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    {
        {0, 0, 0},
        {0, 0, 0},
        {1, 1, 1},
    },
    {
        {1, 0, 0},
        {1, 0, 0},
        {1, 0, 0}
    },
    {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    },
    {
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1}
    },
    {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    },
    {
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0}
    },
    {
        {2, 2, 2},
        {0, 0, 0},
        {0, 0, 0}
    },
    {
        {0, 0, 0},
        {2, 2, 2},
        {0, 0, 0}
    },
    {
        {0, 0, 0},
        {0, 0, 0},
        {2, 2, 2},
    },
    {
        {2, 0, 0},
        {2, 0, 0},
        {2, 0, 0}
    },
    {
        {0, 2, 0},
        {0, 2, 0},
        {0, 2, 0}
    },
    {
        {0, 0, 2},
        {0, 0, 2},
        {0, 0, 2}
    },
    {
        {2, 0, 0},
        {0, 2, 0},
        {0, 0, 2}
    },
    {
        {0, 0, 2},
        {0, 2, 0},
        {2, 0, 0}
    },
};


//-----------------------------------------------------------------------------
struct  TTT_move
{
        char                row;
        char                column;
        
        /* void */          TTT_move (void) :
                                row (0), column (0) {}
        /* void */          TTT_move (char _row, char _column) :
                                row (_row), column (_column) {}
        /* void */          TTT_move (const TTT_move& position) :
                                row (position.row), column (position.column) {}
};

class   TTT_move_list : public std::vector<TTT_move> {};

//-----------------------------------------------------------------------------
struct  TTT_board
{
        enum
        {
            EMPTY_SPOT = 0,
            RED_PEG = 1,
            BLUE_PEG = 2
        };
    
        uint               board;
    
        /* void */          TTT_board (void);
        /* void */          TTT_board (uchar init[TTT_DIM][TTT_DIM]);
        /* void */          TTT_board (const TTT_board& _board);
        bool                IsWinner (void) const;
        bool                IsEmpty (TTT_move move) const;
        TTT_move_list       GetLegalMoveList (void) const;
        void                ApplyMove (TTT_move move, uint peg);
        void                Draw (HDC dc, RECT& rect, int children) const;

static  uint				wins[16];
static  void                InitWins (void);
};

//-----------------------------------------------------------------------------
uint     TTT_board::wins[16];

//-----------------------------------------------------------------------------
struct  TTT_node;
class   TTT_node_list : public std::vector<TTT_node*> {};

struct  TTT_node : public FreeList<TTT_node, 1024 * 128>
{
        TTT_move            move;
        TTT_board           board;
        TTT_node_list       children;
        
        /* void */          TTT_node (TTT_move _move, const TTT_board& _board) :
                                move (_move), board (_board) {}
        void                EnumerateMoves (int level = 0);
        void                StartDraw (HDC dc, RECT& rect) const;
        int                 Draw (HDC dc, RECT& rect, int draw_level, int current_level) const;
};


//-----------------------------------------------------------------------------
/* void */          
TTT_board::TTT_board (void)
{
    // init the board to dead regions
    board = EMPTY_SPOT;
}

//-----------------------------------------------------------------------------
/* void */          
TTT_board::TTT_board (uchar init[TTT_DIM][TTT_DIM])
{
    uint     shift = 0;

    // copy the init values into the board
    for (uint i = 0; i < TTT_DIM; i++)
        for (uint j = 0; j < TTT_DIM; j++)
        {
            board |= (init[i][j] << shift);
            shift += 2;
        }
}

//-----------------------------------------------------------------------------
/* void */          
TTT_board::TTT_board (const TTT_board& _board) : 
    board (_board.board)
{
}

//-----------------------------------------------------------------------------
inline
bool                
TTT_board::IsWinner (void) const
{
    // check the board for win conditions (there are only 16)
    for (uint c = 0; c < 16; c++)
        if ((board & wins[c]) == wins[c])
            return true;
    
    // not a winner
    return false;
}

//-----------------------------------------------------------------------------
inline
bool                
TTT_board::IsEmpty (TTT_move move) const
{
    uint shift = ((move.row * 3)  + move.column) * 2;
    return (((board >> shift) & 3) == EMPTY_SPOT);
}

//-----------------------------------------------------------------------------
TTT_move_list       
TTT_board::GetLegalMoveList (void) const
{
    TTT_move_list       move_list;
    
    if (not IsWinner ())
    {
        // search the board for empty spots
        for (uint i = 0; i < TTT_DIM; i++)
            for (uint j = 0; j < TTT_DIM; j++)
            {
                TTT_move    move (i, j);
                if (IsEmpty (move))
                    move_list.push_back (move);
            }
    }
    
    return move_list;
}

//-----------------------------------------------------------------------------
inline
void           
TTT_board::ApplyMove (TTT_move move, uint peg)
{
    uint shift = ((move.row * 3)  + move.column) * 2;
    board |= (peg << shift);
}

//-----------------------------------------------------------------------------
void                
TTT_board::Draw (HDC dc, RECT& rect, int children) const
{
	// check to see if this board is a winner only if it doesn't have any children
    if ((children == 0) and IsWinner ())
    {
        HBRUSH  winner_brush = CreateSolidBrush (RGB (0, 255, 0));
        RECT    board_rect;
        int     board_size = ((PEG_SIZE + 1) * TTT_DIM) + 4;
        int     left = rect.left - 2;
        int     top = rect.top - 2;
        SetRect (&board_rect, left, top, left + board_size, top + board_size);
        FillRect (dc, &board_rect, winner_brush);
        DeleteObject(reinterpret_cast<HGDIOBJ> (winner_brush));
    }

    HBRUSH  empty_brush = CreateSolidBrush (RGB (200, 200, 200));
    HBRUSH  red_brush = CreateSolidBrush (RGB (255, 0, 0));
    HBRUSH  blue_brush = CreateSolidBrush (RGB (0, 0, 255));

    uint     shift = 0;
    int     top = rect.top;
    for (int i = 0; i < TTT_DIM; i++)
    {
        int     left = rect.left;
        for (int j = 0; j < TTT_DIM; j++)
        {
            RECT    peg_rect;
            SetRect (&peg_rect, left, top, left + PEG_SIZE, top + PEG_SIZE);
            switch ((board >> shift) & 3)
            {
                case EMPTY_SPOT:
                    FillRect (dc, &peg_rect, empty_brush);
                    break;
                case RED_PEG:
                    FillRect (dc, &peg_rect, red_brush);
                    break;
                case BLUE_PEG:
                    FillRect (dc, &peg_rect, blue_brush);
                    break;
            }
            
            left += PEG_SIZE + 1;
            shift += 2;
        }
        
        top += PEG_SIZE + 1;
    }
    
    DeleteObject(reinterpret_cast<HGDIOBJ> (empty_brush));
    DeleteObject(reinterpret_cast<HGDIOBJ> (red_brush));
    DeleteObject(reinterpret_cast<HGDIOBJ> (blue_brush));
}

//-----------------------------------------------------------------------------
void                
TTT_board::InitWins (void)
{
    for (uint c = 0; c < 16; c++)
    {
        uint     shift = 0;
        
        wins[c] = 0;
        
        // copy the init values into the board
        for (uint i = 0; i < TTT_DIM; i++)
            for (uint j = 0; j < TTT_DIM; j++)
            {
                wins[c] |= (win_conditions[c][i][j] << shift);
                shift += 2;
            }
        }
}

//-----------------------------------------------------------------------------
int gEnumCounter = 0;
int gMoveCounter = 0;
int gMoveCounterMax = 0;
void                
TTT_node::EnumerateMoves (int level)
{
    if (level < LEVEL_LIMIT)
    {
        // get the legal move list
        TTT_move_list           move_list = board.GetLegalMoveList ();
        
        gEnumCounter++;
        gMoveCounter += int (move_list.size ());
        if (int (move_list.size ()) > gMoveCounterMax)
            gMoveCounterMax = int (move_list.size ());
            
       // char    output[128];
       // sprintf (output, "Level (%d): %d moves (%d total)\n", level, int (move_list.size ()), gMoveCounter);
       // OutputDebugString (output);

        // generate new nodes for each move in the move list
        TTT_move_list::iterator iter = move_list.begin ();
        TTT_move_list::iterator end = move_list.end ();
        while (iter != end)
        {
            TTT_move&   move = *iter++;
            TTT_node*   node = NewCall TTT_node (move, board);
            children.push_back (node);
            node->board.ApplyMove (move, (level & 1) + 1);
            node->EnumerateMoves (level + 1);
        }
        
        // nice if we could keep a "hash" of the board to limit duplication
    }
}

//-----------------------------------------------------------------------------
void                
TTT_node::StartDraw (HDC dc, RECT& rect) const
{
    //board.Draw (dc, rect, int (children.size ()));
    for (int i = 0; i < LEVEL_LIMIT; i++)
    {
        Draw (dc, rect, i, 0);
        rect.top += BOARD_SIZE + SPACE_SIZE;
    }
}

//-----------------------------------------------------------------------------
int                
TTT_node::Draw (HDC dc, RECT& rect, int draw_level, int current_level) const
{
    if ((current_level < draw_level) and (rect.left < rect.right))
    {
        RECT    drawRect = rect;
        int     level_child = gLevelChild[current_level];
        if ((level_child < -1) or (children.size () == 0))
        {
            // draw no children
            return 0;
        }
        else 
        {
            if (level_child >= int (children.size ()))
            {
                level_child = -1;
                for (int i = current_level; i < LEVEL_LIMIT; i++)
                    gLevelChild[i] = -1;
            }
            if (level_child == -1)
            {
                TTT_node_list::const_iterator   iter = children.begin ();
                TTT_node_list::const_iterator   end = children.end ();
                if (iter != end)
                {
                    const TTT_node* node = *iter++;
                    drawRect.left += node->Draw (dc, drawRect, draw_level, current_level + 1);
                }
                while (iter != end)
                {
                    drawRect.left += SPACE_SIZE;
                    const TTT_node* node = *iter++;
                    drawRect.left += node->Draw (dc, drawRect, draw_level, current_level + 1);
                }
                
                MoveToEx (dc, drawRect.left + (SPACE_SIZE / 2) + (SPACE_SIZE & 0x01), drawRect.top - 1, 0);
                LineTo (dc, drawRect.left + (SPACE_SIZE / 2) + (SPACE_SIZE & 0x01), drawRect.top + BOARD_SIZE + 1);
            }
            else
            {
                const TTT_node* node = children.at (level_child);
                drawRect.left += node->Draw (dc, drawRect, draw_level, current_level + 1);
            }

            return (drawRect.left - rect.left);
        }
    }
    else
    {
        board.Draw (dc, rect, int (children.size ()));
        return BOARD_SIZE;
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//TTT_board   gBoard (test_board);
TTT_board   gBoard;
TTT_move    gMove;
TTT_node    gBaseNode (gMove, gBoard);


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HIQ, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	
	//gLevelChild[0] = -1;
	for (int i = 0; i < LEVEL_LIMIT; i++)
	    gLevelChild[i] = -1;

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

    TTT_board::InitWins ();
    gBaseNode.EnumerateMoves ();
    InvalidateRect (0, 0, TRUE);
    //int gEnumCounter = 0;
    //int gMoveCounter = 0;
    //int gMoveCounterMax = 0;
    char    output[256];
    sprintf (output, "Moves (%d), nodes (%d), average branch (%d), max branch (%d)\n", gMoveCounter, gEnumCounter, gMoveCounter / gEnumCounter, gMoveCounterMax);
    OutputDebugString (output);

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_HIQ);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_HIQ);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_HIQ;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT        drawRect;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		GetClientRect (hWnd, &drawRect);
		InflateRect (&drawRect, -SPACE_SIZE, -SPACE_SIZE);
		gBaseNode.StartDraw (hdc, drawRect);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	    {
            int yPos = (HIWORD(lParam) / CLICK_GRID_SIZE) - 1;
            if ((yPos == 0) or ((yPos > 0) and (gLevelChild[yPos - 1] != -1)))
            {
                if (gLevelChild[yPos] == -1)
                {
                    int xPos = LOWORD(lParam) / CLICK_GRID_SIZE;
                    gLevelChild[yPos] = xPos;
                    while (yPos < LEVEL_LIMIT)
                        gLevelChild[++yPos] = -1;
                }
                else
                {
                    gLevelChild[yPos] = -1;
                    while (yPos < LEVEL_LIMIT)
                        gLevelChild[++yPos] = -1;
                }
                InvalidateRect (0, 0, TRUE);
            }
        }
	    break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
