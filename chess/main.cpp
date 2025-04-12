#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class ExplosionChess {
private:
    vector<vector<pair<int, int>>> board; // first: 棋子数量, second: 玩家编号(1或2)
    int size;
    int currentPlayer;
    bool gameOver;
    int moveCount; // 记录移动次数

    // 检查是否是角落位置
    bool isCorner(int row, int col) {
        return (row == 0 && col == 0) ||
            (row == 0 && col == size - 1) ||
            (row == size - 1 && col == 0) ||
            (row == size - 1 && col == size - 1);
    }

    // 检查是否是边缘位置
    bool isEdge(int row, int col) {
        return row == 0 || row == size - 1 || col == 0 || col == size - 1;
    }

    // 获取爆炸阈值
    int getThreshold(int row, int col) {
        if (isCorner(row, col)) return 2;
        if (isEdge(row, col)) return 3;
        return 4;
    }

    // 检查游戏是否结束
    bool checkGameOver() {
        // 只在双方都下过一手后才检查
        if (moveCount==1) return false;

        bool hasPlayer1 = false;
        bool hasPlayer2 = false;

        for (const auto& row : board) {
            for (const auto& cell : row) {
                if (cell.first > 0) {
                    if (cell.second == 1) hasPlayer1 = true;
                    else hasPlayer2 = true;
                }
            }
        }

        if (!hasPlayer1 && hasPlayer2) {
            cout << "蓝方(B)获胜！整个棋盘都是蓝方的棋子！" << endl;
            return true;
        }
        if (!hasPlayer2 && hasPlayer1) {
            cout << "红方(R)获胜！整个棋盘都是红方的棋子！" << endl;
            return true;
        }
        return false;
    }

    // 处理爆炸
    void processExplosion(int row, int col) {
        int threshold = getThreshold(row, col);
        if (board[row][col].first < threshold) return;

        int player = board[row][col].second;
        cout << (player == 1 ? "红方" : "蓝方")
            << "的位置 (" << row << ", " << col << ") 发生爆炸！" << endl;

        board[row][col].first -= threshold;
        if (board[row][col].first == 0) {
            board[row][col].second = 0;
        }

        // 向四个方向分散棋子
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;

            if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
                board[newRow][newCol].first++;
                board[newRow][newCol].second = player;
                cout << "向 (" << newRow << ", " << newCol << ") 分散一枚棋子并转换为"
                    << (player == 1 ? "红方" : "蓝方") << "的棋子" << endl;

                // 递归处理可能的连锁爆炸
                processExplosion(newRow, newCol);
            }
        }
    }

public:
    ExplosionChess(int n) : size(n), currentPlayer(1), gameOver(false),
        moveCount(0), board(n, vector<pair<int, int>>(n, { 0, 0 })) {}

    // 打印棋盘
    void printBoard() {
        cout << "   ";
        for (int i = 0; i < size; ++i) {
            cout << i << "  ";
        }
        cout << "\n  ";
        for (int i = 0; i < size; ++i) {
            cout << "---";
        }
        cout << endl;

        for (int i = 0; i < size; ++i) {
            cout << i << "| ";
            for (int j = 0; j < size; ++j) {
                if (board[i][j].first == 0) {
                    cout << ".  ";
                }
                else {
                    cout << board[i][j].first << (board[i][j].second == 1 ? "R " : "B ");
                }
            }
            cout << endl;
        }
        cout << "当前玩家: " << (currentPlayer == 1 ? "红方(R)" : "蓝方(B)") << endl;
    }

    // 添加棋子
    bool addPiece(int row, int col) {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            cout << "无效的位置！" << endl;
            return false;
        }

        if (board[row][col].first > 0 && board[row][col].second != currentPlayer) {
            cout << "该位置已被对方占据，不能放置棋子！" << endl;
            return false;
        }

        board[row][col].first++;
        board[row][col].second = currentPlayer;
        cout << (currentPlayer == 1 ? "红方" : "蓝方") << "在 (" << row << ", " << col
            << ") 放置一枚棋子，现在有 " << board[row][col].first << " 枚" << endl;

        moveCount++; // 增加移动计数

        // 处理爆炸
        processExplosion(row, col);

        return true;
    }

    // 游戏主循环
    void play() {
        cout << "爆炸棋对战游戏开始！" << endl;
        cout << "规则：" << endl;
        cout << "- 两位玩家交替放置棋子" << endl;
        cout << "- 红方(R)先手，蓝方(B)后手" << endl;
        cout << "- 角落位置(四角)：2颗棋子爆炸" << endl;
        cout << "- 边缘位置：3颗棋子爆炸" << endl;
        cout << "- 中间位置：4颗棋子爆炸" << endl;
        cout << "- 爆炸后棋子会向四周分散并转换为当前玩家的棋子" << endl;
        cout << "- 双方各放置一次棋子后才判断胜负" << endl;
        cout << "- 目标是将整个棋盘变为自己的颜色" << endl;
        cout << "输入行和列来放置棋子(例如: 2 3)，输入-1 -1结束游戏" << endl;

        while (true) {
            printBoard();

            int row, col;
            cout << "玩家" << (currentPlayer == 1 ? "红方(R)" : "蓝方(B)")
                << "，请输入要放置棋子的位置 (行 列): ";
            cin >> row >> col;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "无效输入，请输入两个数字！" << endl;
                continue;
            }

            if (row == -1 && col == -1) {
                cout << "游戏结束！" << endl;
                break;
            }

            if (addPiece(row, col)) {
                // 检查游戏是否结束（只在双方都下过一手后检查）
                gameOver = checkGameOver();
                if (gameOver) break;

                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        }

        if (gameOver) {
            printBoard();
        }
    }
};

int main() {
    int size;
    cout << "请输入棋盘大小: ";
    cin >> size;

    if (size < 2) {
        cout << "棋盘大小至少为2！" << endl;
        return 1;
    }

    ExplosionChess game(size);
    game.play();

    return 0;
}