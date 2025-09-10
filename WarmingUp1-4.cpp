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
const int MAX_TURNS = 20; // 총 시도 횟수 제한

struct Cell {
    char value;     // 실제 문자
    bool revealed;  // 현재 공개 상태
    bool matched;   // 맞춘 상태
};

vector<vector<Cell>> board(boardSize, vector<Cell>(boardSize));
int score = 0;
int turns = 0;

// 보드 초기화
void initBoard() {
    score = 0;
    turns = 0;
    vector<char> letters;

    // 12개 소문자 알파벳 2개씩 = 24개
    for (char c = 'a'; c < 'a' + 12; c++) {
        letters.push_back(c);
        letters.push_back(c);
    }
    // 조커 추가
    letters.push_back('J');

    // 섞기
    srand((unsigned)time(NULL));
    random_shuffle(letters.begin(), letters.end());

    // 보드에 배치
    int idx = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j].value = letters[idx++];
            board[i][j].revealed = false;
            board[i][j].matched = false;
        }
    }
}

// 보드 출력
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

// 힌트 (전체 공개 후 3초 뒤 원상복귀)
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

// 좌표 입력 (예: 1 a → (0,0))
bool getInput(int& row, int& col) {
    string colInput;
    cout << "행(1~5) 열(a~e) 입력: ";
    cin >> row >> colInput;
    if (row < 1 || row > 5 || colInput[0] < 'a' || colInput[0] > 'e') {
        cout << "잘못된 입력입니다.\n";
        return false;
    }
    row -= 1;
    col = colInput[0] - 'a';
    return true;
}

// 두 칸 선택 및 처리
void selectCells() {
    int r1, c1, r2, c2;
    if (!getInput(r1, c1)) return;
    if (board[r1][c1].matched) { cout << "이미 맞춘 칸입니다.\n"; return; }
    board[r1][c1].revealed = true;
    printBoard();

    if (!getInput(r2, c2)) { board[r1][c1].revealed = false; return; }
    if ((r1 == r2 && c1 == c2) || board[r2][c2].matched) {
        cout << "잘못된 두 번째 선택입니다.\n";
        board[r1][c1].revealed = false;
        return;
    }
    board[r2][c2].revealed = true;
    printBoard();

    turns++;

    char v1 = board[r1][c1].value;
    char v2 = board[r2][c2].value;

    if (v1 == v2 || v1 == 'J' || v2 == 'J') {
        cout << "매치 성공!\n";
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
        cout << "매치 실패...\n";
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
        cout << "명령어: (r=리셋, h=힌트, q=종료, Enter=두 칸 선택)"<<endl;
        cin.ignore(); // 버퍼에 남아있는 '\n' 하나만 버림
        rewind(stdin);

        cmd = cin.peek();
        if (cmd == 'r') {
            cin.get();
            initBoard();
            cout << "게임 리셋!\n";
        }
        else if (cmd == 'h') {
            cin.get();
            showHint();
        }
        else if (cmd == 'q') {
            cin.get();
            cout << "게임 종료!\n";
            break;
        }
        else {
            selectCells();
        }

        if (turns >= MAX_TURNS) {
            cout << "횟수 초과! 최종 점수: " << score << "\n";
            break;
        }
    }
    return 0;
}
