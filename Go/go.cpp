#include "stdafx.h"
#include "go.h"
#include <stdio.h>

//------------------------------------------------------------------------------
Index_Array::Index_Array (unsigned int size)
{
    member_array_size = size;
    member_index_array = new Stone*[member_array_size];
    member_index_array[0] = new Stone[member_array_size * member_array_size];
    for (unsigned int i = 1; i < member_array_size; i++)
        member_index_array[i] = member_index_array[0] + (i * member_array_size);
}

//------------------------------------------------------------------------------
Index_Array::Index_Array (const Index_Array &index_array)
{
    member_array_size = index_array.member_array_size;
    member_index_array = new Stone*[member_array_size];
    member_index_array[0] = new Stone[member_array_size * member_array_size];
    for (unsigned int i = 1; i < member_array_size; i++)
        member_index_array[i] = member_index_array[0] + (i * member_array_size);
}

//------------------------------------------------------------------------------
Index_Array::~Index_Array (void)
{
    delete member_index_array[0];
    delete member_index_array;
}

//------------------------------------------------------------------------------
unsigned int    Index_Array::Get_Size (void) const
{
    return member_array_size;
}

//------------------------------------------------------------------------------
Stone   Index_Array::Get (unsigned int x, unsigned int y) const
{
    if ((x < member_array_size) and (y < member_array_size))
        return member_index_array[y][x];
    return NO_STONE;
}

//------------------------------------------------------------------------------
Stone  *Index_Array::Get_Address (unsigned int x, unsigned int y)
{
    if ((x < member_array_size) and (y < member_array_size))
        return member_index_array[y] + x;
    return 0;
}

//------------------------------------------------------------------------------
void    Index_Array::Set (unsigned int x, unsigned int y, Stone stone)
{
    if ((x < member_array_size) and (y < member_array_size))
        member_index_array[y][x] = stone;
}

//------------------------------------------------------------------------------
void    Index_Array::Clear_To (Stone stone)
{
    for (unsigned int i = 0; i < member_array_size; i++)
        for (unsigned int j = 0; j < member_array_size; j++)
            member_index_array[i][j] = NO_STONE;
}

//------------------------------------------------------------------------------
void    Index_Array::Copy_From (const Index_Array &index_array)
{
    if (member_array_size == index_array.member_array_size)
        for (unsigned int i = 0; i < member_array_size; i++)
            for (unsigned int j = 0; j < member_array_size; j++)
                member_index_array[i][j] = index_array.member_index_array[i][j];
}

//------------------------------------------------------------------------------
bool    Index_Array::Is_Identical_To (const Index_Array &index_array) const
{
    if (member_array_size == index_array.member_array_size)
	{
        for (unsigned int i = 0; i < member_array_size; i++)
            for (unsigned int j = 0; j < member_array_size; j++)
                if (member_index_array[i][j] != index_array.member_index_array[i][j])
					return false;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void    Index_Array::Draw (HDC dc, const RECT& rect, Stone toMove, unsigned int white_prisoner_count, unsigned int black_prisoner_count) const
{
	unsigned int	array_size = member_array_size + 1,
					spacing = array_size,
					height = rect.bottom - rect.top,
					width = rect.right - rect.left,
					square_size = (height > width) ? width : height,
					space_size = square_size / spacing,
					stone_size = (space_size / 8) * 7,
					half_stone_size = stone_size / 2,
					quarter_stone_size = half_stone_size / 2;
	RECT			boardRect;
	boardRect.left = boardRect.top = space_size;
	boardRect.right = boardRect.bottom = member_array_size * space_size;

	// erase the window
	FillRect (dc, &rect, static_cast<HBRUSH> (GetStockObject (WHITE_BRUSH)));

	// draw the grid
	HPEN	grid_pen = CreatePen (PS_SOLID, 6, 0x00dddddd),
			old_pen = static_cast<HPEN> (SelectObject (dc, grid_pen));
	for (unsigned int i = 0; i < member_array_size; i++)
	{
		int	loc = space_size * (i + 1);
		MoveToEx (dc, loc, boardRect.top, 0);
		LineTo (dc, loc, boardRect.bottom);
		MoveToEx (dc, boardRect.left, loc, 0);
		LineTo (dc, boardRect.right, loc);
	}
	SelectObject (dc, old_pen);

	// draw the stones on the grid
	HBRUSH	white_stone_color = CreateSolidBrush (0x00cccccc),
			white_stone_highlight_color = CreateSolidBrush (0x00ffffff),
			black_stone_color = CreateSolidBrush (0x00000000),
			black_stone_highlight_color = CreateSolidBrush (0x00999999),
			old_brush;
	HPEN	empty_pen = CreatePen (PS_NULL, 0, 0);
    for (unsigned int i = 0; i < member_array_size; i++)
    {
		int		x = space_size * (i + 1);
        for (unsigned int j = 0; j < member_array_size; j++)
		{
			int		y = space_size * (j + 1);
            switch (member_index_array[i][j])
            {
                case NO_STONE:		
					break;
                case WHITE_STONE:
					old_brush = static_cast<HBRUSH> (SelectObject (dc, white_stone_color));
					Ellipse (dc, x - half_stone_size, y - half_stone_size, x + half_stone_size, y + half_stone_size);
					old_pen = static_cast<HPEN> (SelectObject (dc, empty_pen));
					SelectObject (dc, white_stone_highlight_color);
					Ellipse (dc, x, y - quarter_stone_size, x + quarter_stone_size, y);
					SelectObject (dc, old_brush);
					SelectObject (dc, old_pen);
					break;
                case BLACK_STONE:
					old_brush = static_cast<HBRUSH> (SelectObject (dc, black_stone_color));
					Ellipse (dc, x - half_stone_size, y - half_stone_size, x + half_stone_size, y + half_stone_size);
					old_pen = static_cast<HPEN> (SelectObject (dc, empty_pen));
					SelectObject (dc, black_stone_highlight_color);
					Ellipse (dc, x, y - quarter_stone_size, x + quarter_stone_size, y);
					SelectObject (dc, old_brush);
					SelectObject (dc, old_pen);
					break;
            }
		}
    }
	DeleteObject (empty_pen);

	// say who is to move
	char			move_string_buffer[3][256];
	unsigned int	x,
					real_height = width - half_stone_size,
					y = real_height + ((height - real_height) / 2);	// assuming the window is taller than it is wide

	stone_size = (stone_size * 3) / 2;
	half_stone_size = stone_size / 2;
	quarter_stone_size = half_stone_size / 2;

	SIZE			textSize;
	HFONT			textFont = CreateFont (48, 0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET bit_or OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH bit_or FF_ROMAN, 0),
					oldFont = static_cast<HFONT> (SelectObject (dc, textFont));
	int				textLength0 = sprintf (move_string_buffer[0], "'Esc' to pass"),
					textLength1 = sprintf (move_string_buffer[1], "White: %d", white_prisoner_count),
					textLength2 = sprintf (move_string_buffer[2], "Black: %d", black_prisoner_count);
	GetTextExtentPoint32 (dc, move_string_buffer[0], textLength0, &textSize);
	textSize.cx += stone_size * 2;
	x = ((width - textSize.cx) / 2) + half_stone_size;

    switch (toMove)
    {
        case WHITE_STONE:
			old_brush = static_cast<HBRUSH> (SelectObject (dc, white_stone_color));
			Ellipse (dc, x - half_stone_size, y - half_stone_size, x + half_stone_size, y + half_stone_size);
			old_pen = static_cast<HPEN> (SelectObject (dc, empty_pen));
			SelectObject (dc, white_stone_highlight_color);
			Ellipse (dc, x, y - quarter_stone_size, x + quarter_stone_size, y);
			SelectObject (dc, old_brush);
			SelectObject (dc, old_pen);
			break;
        case BLACK_STONE:
			old_brush = static_cast<HBRUSH> (SelectObject (dc, black_stone_color));
			Ellipse (dc, x - half_stone_size, y - half_stone_size, x + half_stone_size, y + half_stone_size);
			old_pen = static_cast<HPEN> (SelectObject (dc, empty_pen));
			SelectObject (dc, black_stone_highlight_color);
			Ellipse (dc, x, y - quarter_stone_size, x + quarter_stone_size, y);
			SelectObject (dc, old_brush);
			SelectObject (dc, old_pen);
			break;
    }

	TextOut (dc, x + stone_size, y - 70, move_string_buffer[0], textLength0);
	TextOut (dc, x + stone_size, y - 20, move_string_buffer[1], textLength1);
	TextOut (dc, x + stone_size, y + 30, move_string_buffer[2], textLength2);
	SelectObject (dc, oldFont);
	DeleteObject (textFont);
	DeleteObject (white_stone_color);
	DeleteObject (white_stone_highlight_color);
	DeleteObject (black_stone_color);
	DeleteObject (black_stone_highlight_color);
}

//------------------------------------------------------------------------------
void	Index_Array::GetColumnRow (unsigned int& x, unsigned int& y, const RECT& rect) const
{
	unsigned int	array_size = member_array_size + 1,
					spacing = array_size,
					height = rect.bottom - rect.top,
					width = rect.right - rect.left,
					square_size = (height > width) ? width : height,
					space_size = square_size / spacing,
					half_space_size = space_size / 2;
	x = (x < half_space_size) ? 0xffffffff : ((x - half_space_size) / space_size);
	y = (y < half_space_size) ? 0xffffffff : ((y - half_space_size) / space_size);
}

//------------------------------------------------------------------------------
void    Board::Identify_Fill (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &fill)
{
	Stone	*cell = fill.Get_Address (x, y);
	if (*cell == NO_STONE)
	{
		// record the fill
		*cell = stone;

		// look at the neighbors
		if ((cell = board.Get_Address (x - 1, y)) and (*cell == stone))
			Identify_Fill (board, stone, x - 1, y, fill);
		if ((cell = board.Get_Address (x + 1, y)) and (*cell == stone))
			Identify_Fill (board, stone, x + 1, y, fill);
		if ((cell = board.Get_Address (x, y - 1)) and (*cell == stone))
			Identify_Fill (board, stone, x, y - 1, fill);
		if ((cell = board.Get_Address (x, y + 1)) and (*cell == stone))
			Identify_Fill (board, stone, x, y + 1, fill);
	}
}

//------------------------------------------------------------------------------
bool    Board::Identify_Group (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &fill)
{
	Stone	*cell = board.Get_Address (x, y);
	if (cell and (*cell == stone))
	{
		Identify_Fill (board, stone, x, y, fill);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
#define CHECK_NEIGHBOR(x, y)																		\
	if (cell = group.Get_Address (x, y))															\
		if (*cell == stone)																			\
		{																							\
			if (Freedom_Fill (board, stone, x, y, group, fill))										\
				return true;																		\
		}																							\
		else if ((*cell == NO_STONE) and (*(cell = board.Get_Address (x, y)) == NO_STONE))			\
			return true

//------------------------------------------------------------------------------
bool    Board::Freedom_Fill (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &group, Index_Array &fill)
{
	Stone	*cell = fill.Get_Address (x, y);
	if (*cell == NO_STONE)
	{
		// record the fill
		*cell = stone;

		// look at the neighbors
		CHECK_NEIGHBOR (x - 1, y);
		CHECK_NEIGHBOR (x + 1, y);
		CHECK_NEIGHBOR (x, y - 1);
		CHECK_NEIGHBOR (x, y + 1);
	}
	return false;
}

//------------------------------------------------------------------------------
bool    Board::Group_Has_Freedom (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &group)
{
	Index_Array	fill (group);
	fill.Clear_To (NO_STONE);
	return Freedom_Fill (board, stone, x, y, group, fill);
}

//------------------------------------------------------------------------------
unsigned int	Board::Kill_Group (Index_Array& board, Index_Array &group)
{
	unsigned int	count = 0;
    for (unsigned int i = 0; i < board.Get_Size (); i++)
        for (unsigned int j = 0; j < board.Get_Size (); j++)
		{
			Stone	stone = group.Get (i, j);
			if ((stone != NO_STONE) and (stone == board.Get (i, j)))
			{
				count++;
				board.Set (i, j, NO_STONE);
			}
		}
	return count;
}

//------------------------------------------------------------------------------
Board::Board (int board_size) : 
    member_board (board_size),
	member_previous_board (board_size),
	white_prisoner_count (0),
	black_prisoner_count (0)
{
    member_previous_board.Clear_To (NO_STONE);
    member_board.Clear_To (NO_STONE);
}

//------------------------------------------------------------------------------
Board::Board (const Board& board) :
    member_board (board.member_board),
    member_previous_board (board.member_previous_board),
	white_prisoner_count (board.white_prisoner_count),
	black_prisoner_count (board.black_prisoner_count)
{
    member_previous_board.Copy_From (board.member_previous_board);
    member_board.Copy_From (board.member_board);
}

//------------------------------------------------------------------------------
Board::~Board (void)
{
}

//------------------------------------------------------------------------------
bool    Board::Place_Stone (Stone stone, unsigned int x, unsigned int y)
{
	Index_Array		board (member_board);
	board.Copy_From (member_board);
    Stone *cell = board.Get_Address (x, y);
    if (cell and (*cell == NO_STONE))
    {
		unsigned int	prisoner_count = 0;
        // set the cell to the stone color
        *cell = stone;

        // identify neighbor groups of the opposite color
        // process kills first
        Stone			opposite_stone = (stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE;
        Index_Array		group_left (board.Get_Size ()),
						group_right (board.Get_Size ()),
						group_up (board.Get_Size ()),
						group_down (board.Get_Size ());

		group_left.Clear_To (NO_STONE);
		if (Identify_Group (board, opposite_stone, x - 1, y, group_left))
			if (not Group_Has_Freedom (board, opposite_stone, x - 1, y, group_left))
				prisoner_count += Kill_Group (board, group_left);

		group_right.Clear_To (NO_STONE);
		if (Identify_Group (board, opposite_stone, x + 1, y, group_right))
			if (not Group_Has_Freedom (board, opposite_stone, x + 1, y, group_right))
				prisoner_count += Kill_Group (board, group_right);

		group_up.Clear_To (NO_STONE);
		if (Identify_Group (board, opposite_stone, x, y - 1, group_up))
			if (not Group_Has_Freedom (board, opposite_stone, x, y - 1, group_up))
				prisoner_count += Kill_Group (board, group_up);

		group_down.Clear_To (NO_STONE);
		if (Identify_Group (board, opposite_stone, x, y + 1, group_down))
			if (not Group_Has_Freedom (board, opposite_stone, x, y + 1, group_down))
				prisoner_count += Kill_Group (board, group_down);

        Index_Array fill (board.Get_Size ());
		fill.Clear_To (NO_STONE);
		Identify_Group (board, stone, x, y, fill);

        // now check to see if the move was legal
        if ((Group_Has_Freedom (board, stone, x, y, fill)) and (not board.Is_Identical_To (member_previous_board)))
		{
			if (stone == WHITE_STONE)
				white_prisoner_count += prisoner_count;
			else
				black_prisoner_count += prisoner_count;
			member_previous_board.Copy_From (member_board);
			member_board.Copy_From (board);
			return true;
		}
    }
    return false;
}

//------------------------------------------------------------------------------
bool    Board::Place_Stone (Stone stone, unsigned int x, unsigned int y, const RECT& rect)
{
	member_board.GetColumnRow (x, y, rect);
    return Place_Stone (stone, y, x);
}

//------------------------------------------------------------------------------
void    Board::Draw (HDC dc, const RECT& rect, Stone toMove) const
{
	member_board.Draw (dc, rect, toMove, white_prisoner_count, black_prisoner_count);
}

//------------------------------------------------------------------------------
int     Board::Score_Game (Stone stone) const
{
	return (stone == BLACK_STONE) ? (black_prisoner_count - white_prisoner_count) : (white_prisoner_count - black_prisoner_count);
}

/*
//------------------------------------------------------------------------------
// a little black rain cloud, of course...
//------------------------------------------------------------------------------
int main (int argument_count, char** argument_array)
{
    Board   board (10);
	Stone	stone = BLACK_STONE;
	bool	keep_playing = true;
	int		pass_count = 0;

	while (keep_playing and (pass_count < 2))
	{
		board.Draw ();
		char	column,
				row;
		cout << ((stone == BLACK_STONE) ? "Black" : "White") << " to move." << endl;
		cout << "Input column and row, 'z z' to pass, 'x x' to exit: ";
		cin >> column >> row;
		cout << endl;
		column = tolower (column);
		switch (column)
		{
			case 'z':
				pass_count++;
				stone = (stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE;
				break;
			case 'x':
				keep_playing = false;
				break;
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': 
			case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': 
			case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': 
			case 'v': case 'w':
				pass_count = 0;
				column -= 'a';
				row -= 'a';
				if (board.Place_Stone (stone, column, row))
				{
					stone = (stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE;
					break;
				}
			default:
				cout << "Sorry, that's not a legal move, please try again." << endl << endl;
				break;
		}	
	}
	int		result = board.Score_Game ();
	cout << ((result > 0) ? "White wins!" : (result < 0) ? "Black wins!" : "Tie game!") << endl;

    return 0;
}

//------------------------------------------------------------------------------
*/