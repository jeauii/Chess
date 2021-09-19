#pragma once

#include <algorithm>
#include <iostream>
#include "Board.h"

using namespace std;

class Chess;

class Piece
{
protected:
	bool side;
public:
	Piece(bool s) : side(s) {}
	bool white() const { return side; }
	virtual char name() const;
	virtual bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const;
};

ostream& operator<<(ostream& s, const Piece& p);

class King : public Piece
{
public:
	King(bool s) : Piece(s) {}
	char name() const override;
	bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const override;
};

class Queen : public Piece
{
public:
	Queen(bool s) : Piece(s) {}
	char name() const override;
	bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const override;
};

class Bishop : public Piece
{
public:
	Bishop(bool s) : Piece(s) {}
	char name() const override;
	bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const override;
};

class Knight : public Piece
{
public:
	Knight(bool s) : Piece(s) {}
	char name() const override;
	bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const override;
};

class Rook : public Piece
{
public:
	Rook(bool s) : Piece(s) {}
	char name() const override;
	bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const override;
};

class Pawn : public Piece
{
public:
	Pawn(bool s) : Piece(s) {}
	char name() const override;
	bool legal(const Chess& board,
		int file1, int rank1,
		int file2, int rank2) const override;
};
