// Chess.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <algorithm>
#include <memory>
#include <array>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include "Board.h"

using namespace std;

class Node
{
	const Chess board;
	bool state;
	vector<shared_ptr<Node>> children;
	int visits = 0;
	double value = 0;
public:
	Node(const Chess& b) : board(b)
	{
		int result = 0;
		state = board.check(result);
	}
	void show(Chess& display) const
	{
		display = board;
	}
	shared_ptr<Node> get(size_t index) const
	{
		return index < children.size() ? 
			children[index] : nullptr;
	}
	size_t find() const
	{
		size_t index = 0;
		for (int i = 1; i < children.size(); ++i)
		{
			if (children[i]->visits > children[index]->visits)
				index = i;
		}
		return index;
	}
	shared_ptr<Node> select(double C) const
	{
		shared_ptr<Node> child = children[0];
		for (int i = 1; i < children.size(); ++i)
		{
			if (children[i]->UCB(visits, C) > 
				child->UCB(visits, C))
			{
				child = children[i];
			}
		}
		return child;
	}
	int simulate(int maxDepth) const
	{
		Chess newBoard = board;
		int depth = 0;
		int result = 0;
		while (!newBoard.check(result))
		{
			int file1, rank1, file2, rank2;
			do
			{
				file2 = rand() % 8; rank2 = rand() % 8;
				do
				{
					file1 = rand() % 8; rank1 = rand() % 8;
				} while (!newBoard.get(file1, rank1));
			} while (!newBoard.move(file1, rank1, file2, rank2));
			if (++depth == maxDepth)
				return result;
		}
		return result;
	}
	void expand()
	{
		Chess newBoard = board;
		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				for (int k = 0; k < 8; ++k)
				{
					for (int l = 0; l < 8; ++l)
					{
						if (newBoard.move(i, j, k, l))
						{
							children.push_back(make_shared<Node>(newBoard));
							newBoard = board;
						}
					}
				}
			}
		}
	}
	int update(double C = 1.0)
	{
		int rollout = 0;
		if (!children.empty())
			rollout = select(C)->update();
		else if (state)
			board.check(rollout);
		else if (visits == 0)
			rollout = simulate(128);
		else
		{
			expand();
			rollout = children.front()->update();
		}
		if (rollout != 0)
			value += board.white() != (rollout > 0) ? 1 : -1;
		++visits;
		return rollout;
	}
	void print() const
	{
		board.print();
		cout << "visits = " << visits << ", "
			<< "mean = " << mean() << ", "
			<< "size = " << children.size() << endl;
		cout << endl;
		for (int i = 0; i < children.size(); ++i)
		{
			cout << "child[" << i << "]: " << endl;
			children[i]->board.print();
			cout << "visits = " << children[i]->visits << ", "
				<< "mean = " << children[i]->mean() << ", "
				<< "UCB1 = " << children[i]->UCB(visits, 1.0) << endl;
			cout << endl;
		}
	}
private:
	double mean() const { return value / visits; }
	double UCB(int parent, double C) const
	{
		return visits == 0 ? INFINITY :
			mean() + C * sqrt(log(parent) / visits);
	}
};


class Player
{
	shared_ptr<Node> root;
public:
	void show(Chess& board)
	{
		root->show(board);
	}
	void look(const Chess& board)
	{
		root = make_shared<Node>(board);
		root->expand();
	}
	void think(int visits, double C)
	{
		for (int i = 0; i < visits; ++i)
		{
			try { root->update(C); }
			catch (const exception&) { break; }
		}
	}
	void think(time_t seconds, double C)
	{
		auto t = time(NULL);
		while (time(NULL) - t < seconds)
		{
			try { root->update(C); }
			catch (const exception&) { break; }
		}
	}
	void advance(size_t index)
	{
		root = root->get(index);
	}
	size_t advance()
	{
		size_t index = root->find();
		root = root->get(index);
		return index;
	}
	void print() const
	{
		root->print();
	}
};

int test()
{
	Chess board;
	board.reset();
	int moves = 1;
	int result = 0;
	bool side = true;
	while (!board.check(result))
	{
		system("cls");
		cout << moves << ".";
		if (!side) cout << "...";
		cout << endl;
		board.print();
		int file1, rank1,
			file2, rank2;
		do
		{
			file1 = rand() % 8; rank1 = rand() % 8;
			file2 = rand() % 8; rank2 = rand() % 8;
		} while (!board.move(file1, rank1, file2, rank2));
		if (!side) ++moves;
		side = !side;
		cout << endl;
		board.print();
		system("pause");
	}
	cout << result << endl;
	return result;
}

template <typename T>
int test(T limit1, T limit2)
{
	Player white, black;
	Chess board;
	board.reset();
	white.look(board);
	black.look(board);
	int moves = 1;
	int result = 0;
	bool side = true;
	while (!board.check(result))
	{
		system("cls");
		white.show(board);
		cout << moves << ".";
		if (!side) cout << "...";
		cout << endl;
		board.print();
		(side ? white : black).think(
			side ? limit1 : limit2, 1.0);
		cout << "................" << endl;
		(side ? white : black).print();
		cout << "................" << endl;
		(side ? black : white).advance(
			(side ? white : black).advance());
		if (!side) ++moves;
		side = !side;
		black.show(board);
		cout << endl;
		board.print();
		//system("pause");
	}
	cout << result << endl;
	return result;
} 

int main()
{
	test((time_t)60, (time_t)30);
}
