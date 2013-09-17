#ifndef	_GO_
#define	_GO_

//------------------------------------------------------------------------------
// ... because msvc is not standard C++, with my own twist on the naming
//------------------------------------------------------------------------------
#define bit_or          |
#define bit_and         &
#define exclusive_or    ^
#define or              ||
#define and             &&
#define not             !

//------------------------------------------------------------------------------
// Stone type declaration
//------------------------------------------------------------------------------
enum Stone
{
    NO_STONE =			0,
    WHITE_STONE =		1,
    BLACK_STONE =		2
};

//------------------------------------------------------------------------------
// Index_Array class declaration and method definitions
//------------------------------------------------------------------------------
class Index_Array
{
    private:
        unsigned int    member_array_size;
        Stone**         member_index_array;

    public:
                        Index_Array (unsigned int size);
                        Index_Array (const Index_Array &index_array);
                       ~Index_Array (void);

        unsigned int    Get_Size (void) const;
        Stone           Get (unsigned int x, unsigned int y) const;
        Stone          *Get_Address (unsigned int x, unsigned int y);
        void            Set (unsigned int x, unsigned int y, Stone stone);
        void            Clear_To (Stone stone);
        void            Copy_From (const Index_Array &index_array);
        bool            Is_Identical_To (const Index_Array &index_array) const;
		void			Draw (HDC dc, const RECT& rect, Stone toMove, unsigned int white_prisoner_count, unsigned int black_prisoner_count) const;
		void			GetColumnRow (unsigned int& x, unsigned int& y, const RECT& rect) const;
};

//------------------------------------------------------------------------------
// Board class declaration and method definitions
//------------------------------------------------------------------------------
class Board
{
    private:
        // data members
        Index_Array         member_previous_board;
        Index_Array         member_board;
		unsigned int		white_prisoner_count;
		unsigned int		black_prisoner_count;

        // internal methods
        void                Identify_Fill (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &fill);
        bool                Identify_Group (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &fill);
		bool				Freedom_Fill (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &group, Index_Array &fill);
        bool                Group_Has_Freedom (Index_Array& board, Stone stone, unsigned int x, unsigned int y, Index_Array &group);
		unsigned int		Kill_Group (Index_Array& board, Index_Array &group);

    public:
        // constructors and destructors
                            Board (int board_size);
                            Board (const Board& board);
                           ~Board (void);

        // operations on the board
        bool                Place_Stone (Stone stone, unsigned int x, unsigned int y);
		bool				Place_Stone (Stone stone, unsigned int x, unsigned int y, const RECT& rect);
        void                Draw (HDC dc, const RECT& rect, Stone toMove) const;
		int					Score_Game (Stone stone) const;
};

//------------------------------------------------------------------------------

#endif	//_GO_