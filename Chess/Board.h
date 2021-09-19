#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include "Piece.h"

using namespace std;

template<typename T, size_t S>
class Board
{
protected:
	array<array<shared_ptr<T>, S>, S> grid;
public:
	const size_t size = S;
	virtual Board& operator=(const Board& other);
	bool valid(int row, int col) const;
	shared_ptr<T> get(int row, int col) const;
	virtual void reset();
	virtual bool add(const T& piece, int row, int col);
	virtual bool remove(int row, int col);
	virtual bool move(int row1, int col1, int row2, int col2);
	virtual bool check(int& result) const;
	virtual void print() const;
};

class Piece;

class Chess : public Board<Piece, 8>
{
	bool side = true;
public:
	Chess& operator=(const Board& other) override;
	bool white() const;
	void reset() override;
	bool move(int file1, int rank1,
		int file2, int rank2) override;
	bool check(int& result) const override;
	void print() const override;
	void printMove(int file, int rank) const;
	void setup();
};
