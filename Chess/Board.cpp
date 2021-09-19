#include "stdafx.h"
#include "Board.h"

// template<typename T, size_t S> class Board

template<typename T, size_t S>
Board<T, S>& Board<T, S>::operator=(const Board& other)
{
	grid = other.grid;
	return *this;
}

template<typename T, size_t S>
bool Board<T, S>::valid(int row, int col) const
{
	return row >= 0 && row < size &&
		col >= 0 && col < size;
}

template<typename T, size_t S>
shared_ptr<T> Board<T, S>::get(int row, int col) const
{
	return valid(row, col) ?
		grid[row][col] : nullptr;
}

template<typename T, size_t S>
void Board<T, S>::reset()
{
	grid = array<array<shared_ptr<T>, S>, S>();
}

template<typename T, size_t S>
bool Board<T, S>::add(const T& piece, int row, int col)
{
	if (!valid(row, col))
		return false;
	else if (grid[row][col])
		return false;
	grid[row][col] = make_shared<T>(piece);
	return true;
}

template<typename T, size_t S>
bool Board<T, S>::remove(int row, int col)
{
	if (!valid(row, col))
		return false;
	else if (!grid[row][col])
		return false;
	grid[row][col].reset();
	return true;
}

template<typename T, size_t S>
bool Board<T, S>::move(int row1, int col1, int row2, int col2)
{
	if (!valid(row1, col1) || !valid(row2, col2))
		return false;
	else if (!grid[row1][col1])
		return false;
	grid[row2][col2] = grid[row1][col1];
	grid[row1][col1].reset();
	return true;
}

template<typename T, size_t S>
bool Board<T, S>::check(int& result) const
{
	result = 0;
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			if (!grid[i][j])
				return true;
		}
	}
	return false;
}

template<typename T, size_t S>
void Board<T, S>::print() const
{
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			if (!grid[i][j])
				cout << ' ';
			else
				cout << *grid[i][j];
			cout << ' ';
		}
		cout << endl;
	}
}

template class Board<Piece, 8>;

// class Chess : public Board<Piece, 8>

Chess& Chess::operator=(const Board& board)
{
	auto other = dynamic_cast<const Chess&>(board);
	Board::operator=(board);
	side = other.side;
	return *this;
}

bool Chess::white() const { return side; }

bool Chess::move(int file1, int rank1, int file2, int rank2)
{
	if (!valid(file1, rank1))
		return false;
	else if (!grid[file1][rank1])
		return false;
	else if (grid[file1][rank1]->white() != side)
		return false;
	else if (!grid[file1][rank1]->legal(*this, file1, rank1, file2, rank2))
		return false;
	Board::move(file1, rank1, file2, rank2);
	if (grid[file2][rank2]->name() == 'P')
	{
		if (side && rank2 == 7)
			grid[file2][rank2] = make_shared<Queen>(true);
		else if (!side && rank2 == 0)
			grid[file2][rank2] = make_shared<Queen>(false);
	}
	side = !side;
	return true;
}

bool Chess::check(int& result) const
{
	result = 0;
	bool white = false, black = false,
		draw = true;
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			if (grid[i][j])
			{
				if (grid[i][j]->name() == 'K')
					(grid[i][j]->white() ? white : black) = true;
				else
					draw = false;
			}
		}
	}
	if (white && black)
		return draw;
	else if (white)
		result = 1;
	else if (black)
		result = -1;
	return true;
}

void Chess::print() const
{
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			if (!grid[j][size - i - 1])
				cout << ' ';
			else
				cout << *grid[j][size - i - 1];
			cout << ' ';
		}
		cout << endl;
	}
}

void Chess::printMove(int file, int rank) const
{
	if (!valid(file, rank))
		return;
	else if (!grid[file][rank])
		return;
	if (!side)
		cout << grid[file][rank]->name();
	else
		cout << grid[file][rank]->name() + 32;
	cout << file + 97 << rank + 49 << endl;
}

void Chess::reset()
{
	grid[0][0] = make_shared<Rook>(true); grid[0][7] = make_shared<Rook>(false);
	grid[1][0] = make_shared<Knight>(true); grid[1][7] = make_shared<Knight>(false);
	grid[2][0] = make_shared<Bishop>(true); grid[2][7] = make_shared<Bishop>(false);
	grid[3][0] = make_shared<Queen>(true); grid[3][7] = make_shared<Queen>(false);
	grid[4][0] = make_shared<King>(true); grid[4][7] = make_shared<King>(false);
	grid[5][0] = make_shared<Bishop>(true); grid[5][7] = make_shared<Bishop>(false);
	grid[6][0] = make_shared<Knight>(true); grid[6][7] = make_shared<Knight>(false);
	grid[7][0] = make_shared<Rook>(true); grid[7][7] = make_shared<Rook>(false);
	grid[0][1] = make_shared<Pawn>(true); grid[0][6] = make_shared<Pawn>(false);
	grid[1][1] = make_shared<Pawn>(true); grid[1][6] = make_shared<Pawn>(false);
	grid[2][1] = make_shared<Pawn>(true); grid[2][6] = make_shared<Pawn>(false);
	grid[3][1] = make_shared<Pawn>(true); grid[3][6] = make_shared<Pawn>(false);
	grid[4][1] = make_shared<Pawn>(true); grid[4][6] = make_shared<Pawn>(false);
	grid[5][1] = make_shared<Pawn>(true); grid[5][6] = make_shared<Pawn>(false);
	grid[6][1] = make_shared<Pawn>(true); grid[6][6] = make_shared<Pawn>(false);
	grid[7][1] = make_shared<Pawn>(true); grid[7][6] = make_shared<Pawn>(false);
	side = true;
}

void Chess::setup()
{
	grid[3][0] = make_shared<Queen>(true);
	grid[4][0] = make_shared<King>(true); grid[4][7] = make_shared<King>(false);
	side = true;
}
