#include "GameLogic.h"
#include <stdio.h>
#include "myio.h"

#include <time.h>

#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

GameLogic::GameLogic() {
	for (int i = 0; i < HEIGHT + 1; i++) {
		board[i] = new char[WIDTH];
		for (int j = 0; j < WIDTH; j++) {
			board[i][j] = 0;
		}
	}
	for (int j = 0; j < WIDTH; j++) {
		board[HEIGHT][j] = 1;
	}

	isRun = true;
	Block::zero.x = 6;
	Block::zero.y = 2;
}

GameLogic::~GameLogic() {
	// TODO Auto-generated destructor stub
	for (int i = 0; i < HEIGHT + 1; i++) {
		delete board[i];
	}
}

void GameLogic::start() {
	isNotGameOver=true;
	block = Block();
	next = Block();
	score = 0;
	interval = 105000;
	clearLines = 0;
	level = 1;
	nextLevel();
	refresh();
	int n;
	unsigned char key;
	clock_t refresh1 = clock();
	clock_t current = refresh1;
	clock_t blockDown = refresh1;
	while (isNotGameOver) {
		current = clock();
		if (current - refresh1 > 16000) {
			refresh1 = current;
			printBlock();
		}
		if (current - blockDown > interval) {
			moveDown();
			blockDown = current;
		}
		n = getchar();
		if (n != EOF) {
			key = n;
			switch (key) {
			case 'Q':
				isNotGameOver=false;
				this->isRun = false;
				break;
			case UP:
				transforse();
				break;
			case DOWN:
				moveDown();
				break;
			case RIGHT:
				moveRight();
				break;
			case LEFT:
				moveLeft();
				break;
			case ' ':
				drop();
				break;
			default:
				break;
			}
		}
		gotoxy(80, 24);
	}
}

void GameLogic::printBlock() {
	block.refresh();
}

void GameLogic::moveDown() {
	block.moveDown();
	if (isCollision()) {
		block.moveUp();
		//보드에 저장
		for (int i = 0; i < 4; i++) {
			board[block.current.y + block.shape[i].y][block.current.x + block.shape[i].x] = block.color;
		}
		nextTurn();
	}

}
void GameLogic::moveLeft() {
	block.moveLeft();
	if (isCollision())
		block.moveRight();
}

void GameLogic::moveRight() {
	block.moveRight();
	if (isCollision())
		block.moveLeft();
}

void GameLogic::transforse() {
	Block temp = block;
	block.transpose();
	if (isCollision()) {
		block.reversTranspose();
	} else {
		temp.clear();
		block.print();
	}
}

void GameLogic::drop() {

	while (!isCollision())
		block.moveDown();

	block.moveUp();

	nextTurn();
}

bool GameLogic::isCollision() {
	int x, y;
	for (int i = 0; i < 4; i++) {
		x = block.current.x + block.shape[i].x;
		y = block.current.y + block.shape[i].y;

		if (x >= WIDTH || y > HEIGHT || x < 0) {
			return true;
		}

		if (board[y][x]) {
			return true;
		}
	}
	return false;
}

void GameLogic::clearLine(int l) {
	char *line = board[l];
	for (int y = l; y > 0; y--) {
		board[y] = board[y - 1];
	}

	board[0] = line;
	for (int x = 0; x < WIDTH; x++) {
		board[0][x] = 0;
	}

}
void GameLogic::gameOver() {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			board[y][x] = 0;
		}
	}
	refresh();
	
	printColorString(Block::zero.x, Block::zero.y + 10, COLOR_BLACK, "gameover");
	isNotGameOver=false;

}
void GameLogic::gameClear() {

}
void GameLogic::nextTurn() {
	//보드에 저장
	for (int i = 0; i < 4; i++) {
		board[block.current.y + block.shape[i].y][block.current.x + block.shape[i].x] = block.color;
	}
	block.refresh();
	check(block);
	//새블럭 생성
	block = next;
	next = Block();
	refreshNextBlock();

	if (isCollision()) {
		gameOver();
	}
}

void GameLogic::check(Block block) {

	bool needClear = false;
	bool removeable;
	int lineCount = 0;
	int y = block.current.y + block.bottom;

	for (int i = 0; i < 4; i++) {
		removeable = true;
		for (int x = 0; x < WIDTH; x++) {
			if (!board[y][x]) {
				removeable = false;
				break;
			}
		}
		if (removeable) {
			clearLine(y);
			needClear = true;
			lineCount++;
		} else {
			y--;
		}
	}
	switch (lineCount) {
	case 1:
		score += 100;
		break;
	case 2:
		score += 300;
		break;
	case 3:
		score += 500;
		break;
	case 4:
		score += 800;
		break;
	default:
		break;
	}
	clearLines += lineCount;
	char l[4];
	sprintf(l, "%d", clearLines);
	printColorString(40, 15, 0, l);
	if (clearLines >= 20) {
		nextLevel();

	}
	if (needClear)
		refresh();

}

void GameLogic::refresh() {

	for (int i = 0; i < HEIGHT; i++) {
		gotoxy(Block::zero.x - 2, Block::zero.y + i);
		putchar('*');

		for (int x = 0; x < WIDTH; x++) {
			if (board[i][x]) {
				printColorString(x * 2 + Block::zero.x, Block::zero.y + i, board[i][x], Block::BLOCK);
			} else {
				printColorString(x * 2 + Block::zero.x, Block::zero.y + i, board[i][x], Block::CLEAR);
			}
		}
		gotoxy(Block::zero.x + WIDTH * 2, Block::zero.y + i);
		putchar('*');
	}
	gotoxy(Block::zero.x - 2, Block::zero.y + HEIGHT);
	printf("***********************");
	refreshNextBlock();
	refreshPoint();

}

void GameLogic::refreshNextBlock() {
	printColorString(30, 7, 0, "┌────────┐");
	for (int i = 0; i < 4; i++) {
		printColorString(30, 8 + i, 0, "│        │");
	}
	printColorString(30, 12, 0, "└────────┘");
	next.clear(31, 8);
	next.print(31, 8);
}

void GameLogic::refreshPoint() {
	printColorString(30, 5, 0, "SCORE");
	char score[10];
	sprintf(score, "%08d", this->score);
	printColorString(30, 6, 0, score);
}

void GameLogic::nextLevel() {
	printColorString(30, 3, 0, "LEVEL");
	char score[10];
	sprintf(score, "%2d", level);
	printColorString(30, 4, 0, score);
	clearLines = 0;
	interval -= 5000;
	if (interval <= 0) {
		gameClear();
	}
}
