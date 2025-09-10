#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <thread>
#include <chrono>
#include <Windows.h>
using namespace std;

const int boardSize = 5;
const int MAX_TURNS = 20; // �� �õ� Ƚ�� ����

struct Cell {
    char value;     // ���� ����
    bool revealed;  // ���� ���� ����
    bool matched;   // ���� ����
};

vector<vector<Cell>> board(boardSize, vector<Cell>(boardSize));
int score = 0;
int turns = 0;

// ���� �ʱ�ȭ
void initBoard() {
    score = 0;
    turns = 0;
    vector<char> letters;

    // 12�� �ҹ��� ���ĺ� 2���� = 24��
    for (char c = 'a'; c < 'a' + 12; c++) {
        letters.push_back(c);
        letters.push_back(c);
    }
    // ��Ŀ �߰�
    letters.push_back('J');

    // ����
    srand((unsigned)time(NULL));
    random_shuffle(letters.begin(), letters.end());

    // ���忡 ��ġ
    int idx = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j].value = letters[idx++];
            board[i][j].revealed = false;
            board[i][j].matched = false;
        }
    }
}

// ���� ���
void printBoard() {
    cout << "    ";
    for (int j = 0; j < boardSize; j++)
        cout << (char)('a' + j) << "   ";
    cout << endl;

    for (int i = 0; i < boardSize; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j].matched) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), int(board[i][j].value) - 'a');
                cout << (char)toupper(board[i][j].value) << "   ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (board[i][j].revealed) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), int(board[i][j].value) - 'a');
                cout << board[i][j].value << "   ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else {
                cout << "*   ";
            }
        }
        cout << endl;
    }
    cout << "\nScore: " << score << "  Turns: " << turns << "/" << MAX_TURNS << "\n\n";
}

// ��Ʈ (��ü ���� �� 3�� �� ���󺹱�)
void showHint() {
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            board[i][j].revealed = true;
    printBoard();
    this_thread::sleep_for(chrono::seconds(3));
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (!board[i][j].matched) board[i][j].revealed = false;
}

// ��ǥ �Է� (��: 1 a �� (0,0))
bool getInput(int& row, int& col) {
    string colInput;
    cout << "��(1~5) ��(a~e) �Է�: ";
    cin >> row >> colInput;
    if (row < 1 || row > 5 || colInput[0] < 'a' || colInput[0] > 'e') {
        cout << "�߸��� �Է��Դϴ�.\n";
        return false;
    }
    row -= 1;
    col = colInput[0] - 'a';
    return true;
}

// �� ĭ ���� �� ó��
void selectCells() {
    int r1, c1, r2, c2;
    if (!getInput(r1, c1)) return;
    if (board[r1][c1].matched) { cout << "�̹� ���� ĭ�Դϴ�.\n"; return; }
    board[r1][c1].revealed = true;
    printBoard();

    if (!getInput(r2, c2)) { board[r1][c1].revealed = false; return; }
    if ((r1 == r2 && c1 == c2) || board[r2][c2].matched) {
        cout << "�߸��� �� ��° �����Դϴ�.\n";
        board[r1][c1].revealed = false;
        return;
    }
    board[r2][c2].revealed = true;
    printBoard();

    turns++;

    char v1 = board[r1][c1].value;
    char v2 = board[r2][c2].value;

    if (v1 == v2 || v1 == 'J' || v2 == 'J') {
        cout << "��ġ ����!\n";
        if (v1 == 'J' || v2 == 'J') {
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j].value == v1 || board[i][j].value == v2) board[i][j].matched = true;
                }
            }
        }
        board[r1][c1].matched = true;
        board[r2][c2].matched = true;
        score += 10;
    }
    else {
        cout << "��ġ ����...\n";
        this_thread::sleep_for(chrono::seconds(2));
        board[r1][c1].revealed = false;
        board[r2][c2].revealed = false;
        score -= 2;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    initBoard();

    char cmd;
    while (true) {
        printBoard();
        cout << "��ɾ�: (r=����, h=��Ʈ, q=����, Enter=�� ĭ ����)"<<endl;
        cin.ignore(); // ���ۿ� �����ִ� '\n' �ϳ��� ����
        rewind(stdin);

        cmd = cin.peek();
        if (cmd == 'r') {
            cin.get();
            initBoard();
            cout << "���� ����!\n";
        }
        else if (cmd == 'h') {
            cin.get();
            showHint();
        }
        else if (cmd == 'q') {
            cin.get();
            cout << "���� ����!\n";
            break;
        }
        else {
            selectCells();
        }

        if (turns >= MAX_TURNS) {
            cout << "Ƚ�� �ʰ�! ���� ����: " << score << "\n";
            break;
        }
    }
    return 0;
}
