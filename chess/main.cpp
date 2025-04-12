#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class ExplosionChess {
private:
    vector<vector<pair<int, int>>> board; // first: ��������, second: ��ұ��(1��2)
    int size;
    int currentPlayer;
    bool gameOver;
    int moveCount; // ��¼�ƶ�����

    // ����Ƿ��ǽ���λ��
    bool isCorner(int row, int col) {
        return (row == 0 && col == 0) ||
            (row == 0 && col == size - 1) ||
            (row == size - 1 && col == 0) ||
            (row == size - 1 && col == size - 1);
    }

    // ����Ƿ��Ǳ�Եλ��
    bool isEdge(int row, int col) {
        return row == 0 || row == size - 1 || col == 0 || col == size - 1;
    }

    // ��ȡ��ը��ֵ
    int getThreshold(int row, int col) {
        if (isCorner(row, col)) return 2;
        if (isEdge(row, col)) return 3;
        return 4;
    }

    // �����Ϸ�Ƿ����
    bool checkGameOver() {
        // ֻ��˫�����¹�һ�ֺ�ż��
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
            cout << "����(B)��ʤ���������̶������������ӣ�" << endl;
            return true;
        }
        if (!hasPlayer2 && hasPlayer1) {
            cout << "�췽(R)��ʤ���������̶��Ǻ췽�����ӣ�" << endl;
            return true;
        }
        return false;
    }

    // ����ը
    void processExplosion(int row, int col) {
        int threshold = getThreshold(row, col);
        if (board[row][col].first < threshold) return;

        int player = board[row][col].second;
        cout << (player == 1 ? "�췽" : "����")
            << "��λ�� (" << row << ", " << col << ") ������ը��" << endl;

        board[row][col].first -= threshold;
        if (board[row][col].first == 0) {
            board[row][col].second = 0;
        }

        // ���ĸ������ɢ����
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;

            if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
                board[newRow][newCol].first++;
                board[newRow][newCol].second = player;
                cout << "�� (" << newRow << ", " << newCol << ") ��ɢһö���Ӳ�ת��Ϊ"
                    << (player == 1 ? "�췽" : "����") << "������" << endl;

                // �ݹ鴦����ܵ�������ը
                processExplosion(newRow, newCol);
            }
        }
    }

public:
    ExplosionChess(int n) : size(n), currentPlayer(1), gameOver(false),
        moveCount(0), board(n, vector<pair<int, int>>(n, { 0, 0 })) {}

    // ��ӡ����
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
        cout << "��ǰ���: " << (currentPlayer == 1 ? "�췽(R)" : "����(B)") << endl;
    }

    // �������
    bool addPiece(int row, int col) {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            cout << "��Ч��λ�ã�" << endl;
            return false;
        }

        if (board[row][col].first > 0 && board[row][col].second != currentPlayer) {
            cout << "��λ���ѱ��Է�ռ�ݣ����ܷ������ӣ�" << endl;
            return false;
        }

        board[row][col].first++;
        board[row][col].second = currentPlayer;
        cout << (currentPlayer == 1 ? "�췽" : "����") << "�� (" << row << ", " << col
            << ") ����һö���ӣ������� " << board[row][col].first << " ö" << endl;

        moveCount++; // �����ƶ�����

        // ����ը
        processExplosion(row, col);

        return true;
    }

    // ��Ϸ��ѭ��
    void play() {
        cout << "��ը���ս��Ϸ��ʼ��" << endl;
        cout << "����" << endl;
        cout << "- ��λ��ҽ����������" << endl;
        cout << "- �췽(R)���֣�����(B)����" << endl;
        cout << "- ����λ��(�Ľ�)��2�����ӱ�ը" << endl;
        cout << "- ��Եλ�ã�3�����ӱ�ը" << endl;
        cout << "- �м�λ�ã�4�����ӱ�ը" << endl;
        cout << "- ��ը�����ӻ������ܷ�ɢ��ת��Ϊ��ǰ��ҵ�����" << endl;
        cout << "- ˫��������һ�����Ӻ���ж�ʤ��" << endl;
        cout << "- Ŀ���ǽ��������̱�Ϊ�Լ�����ɫ" << endl;
        cout << "�����к�������������(����: 2 3)������-1 -1������Ϸ" << endl;

        while (true) {
            printBoard();

            int row, col;
            cout << "���" << (currentPlayer == 1 ? "�췽(R)" : "����(B)")
                << "��������Ҫ�������ӵ�λ�� (�� ��): ";
            cin >> row >> col;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "��Ч���룬�������������֣�" << endl;
                continue;
            }

            if (row == -1 && col == -1) {
                cout << "��Ϸ������" << endl;
                break;
            }

            if (addPiece(row, col)) {
                // �����Ϸ�Ƿ������ֻ��˫�����¹�һ�ֺ��飩
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
    cout << "���������̴�С: ";
    cin >> size;

    if (size < 2) {
        cout << "���̴�С����Ϊ2��" << endl;
        return 1;
    }

    ExplosionChess game(size);
    game.play();

    return 0;
}