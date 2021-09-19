#include "stdafx.h"
#include "Piece.h"

// class Piece

char Piece::name() const { return ' '; }

bool Piece::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (file1 == file2 && rank1 == rank2)
		return false;
	else if (!board.valid(file1, rank1) || !board.valid(file2, rank2))
		return false;
	const auto& piece = board.get(file2, rank2);
	return !piece || piece->side != side;
}

ostream& operator<<(ostream& s, const Piece& p)
{
	return p.white() ? 
		s << p.name() : s << (char)(p.name() + 32);
}

// class King : public Piece

char King::name() const { return 'K'; }

bool King::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (!Piece::legal(board, file1, rank1, file2, rank2))
		return false;
	int dfile = file2 - file1,
		drank = rank2 - rank1;
	if (max(abs(dfile), abs(drank)) != 1)
		return false;
	return true;
}

// class Queen : public Piece

char Queen::name() const { return 'Q'; }

bool Queen::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (!Piece::legal(board, file1, rank1, file2, rank2))
		return false;
	int dfile = file2 - file1,
		drank = rank2 - rank1;
	if (dfile != 0 && drank != 0 && abs(dfile) != abs(drank))
		return false;
	file1 += (file2 > file1) - (file2 < file1);
	rank1 += (rank2 > rank1) - (rank2 < rank1);
	while (file1 != file2 || rank1 != rank2)
	{
		if (board.get(file1, rank1))
			return false;
		file1 += (file2 > file1) - (file2 < file1);
		rank1 += (rank2 > rank1) - (rank2 < rank1);
	}
	return true;
}

// class Bishop : public Piece

char Bishop::name() const { return 'B'; }

bool Bishop::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (!Piece::legal(board, file1, rank1, file2, rank2))
		return false;
	int dfile = file2 - file1,
		drank = rank2 - rank1;
	if (abs(dfile) != abs(drank))
		return false;
	file1 += (file2 > file1) - (file2 < file1);
	rank1 += (rank2 > rank1) - (rank2 < rank1);
	while (file1 != file2 || rank1 != rank2)
	{
		if (board.get(file1, rank1))
			return false;
		file1 += (file2 > file1) - (file2 < file1);
		rank1 += (rank2 > rank1) - (rank2 < rank1);
	}
	return true;
}

// class Knight : public Piece

char Knight::name() const { return 'N'; }

bool Knight::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (!Piece::legal(board, file1, rank1, file2, rank2))
		return false;
	int dfile = file2 - file1,
		drank = rank2 - rank1;
	if (abs(dfile * drank) != 2)
		return false;
	return true;
}

// class Rook : public Piece

char Rook::name() const { return 'R'; }

bool Rook::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (!Piece::legal(board, file1, rank1, file2, rank2))
		return false;
	int dfile = file2 - file1,
		drank = rank2 - rank1;
	if (dfile != 0 && drank != 0)
		return false;
	file1 += (file2 > file1) - (file2 < file1);
	rank1 += (rank2 > rank1) - (rank2 < rank1);
	while (file1 != file2 || rank1 != rank2)
	{
		if (board.get(file1, rank1))
			return false;
		file1 += (file2 > file1) - (file2 < file1);
		rank1 += (rank2 > rank1) - (rank2 < rank1);
	}
	return true;
}

// class Pawn : public Piece

char Pawn::name() const { return 'P'; }

bool Pawn::legal(const Chess& board,
	int file1, int rank1,
	int file2, int rank2) const
{
	if (!Piece::legal(board, file1, rank1, file2, rank2))
		return false;
	int dfile = file2 - file1,
		drank = rank2 - rank1;
	if (dfile != 0 && abs(dfile) != 1)
		return false;
	else if (dfile == 0)
	{
		if (drank != (side ? 1 : -1) && drank != (side ? 2 : -2))
			return false;
		else if (board.get(file2, rank2))
			return false;
		else if (drank == (side ? 2 : -2))
		{
			if (rank1 != (side ? 1 : 6))
				return false;
			else if (board.get(file1, side ? rank1 + 1 : rank1 - 1))
				return false;
		}
	}
	else if (abs(dfile) == 1)
	{
		if (drank != (side ? 1 : -1))
			return false;
		else if (!board.get(file2, rank2))
			return false;
	}
	return true;
}
