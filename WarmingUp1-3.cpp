#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>
using namespace std;

struct Point {
    int x, y, z;
    bool valid = false;
};

Point points[10];           // 현재 리스트
Point backupPoints[10];     // f 누르기 전 스냅샷(인덱스/valid 포함)
bool sortedMode = false;    // f 토글 상태
bool hasBackup = false;    // 스냅샷 존재 여부

// --------- 유틸 ----------
double dist2(const Point& p) { // 원점까지 거리의 제곱(정렬용: sqrt 불필요)
    return 1.0 * p.x * p.x + 1.0 * p.y * p.y + 1.0 * p.z * p.z;
}

void printList() {
    for (int i = 9; i >= 0; --i) {
        if (points[i].valid) {
            cout << i << " " << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
        }
        else {
            cout << i << "\n";
        }
    }
}

int countPoints() {
    int c = 0;
    for (int i = 0; i < 10; ++i) if (points[i].valid) ++c;
    return c;
}

void invalidateSort() {
    sortedMode = false;
    hasBackup = false;
}

// --------- 명령 구현 ----------
void addTop(int x, int y, int z) {
    invalidateSort();
    for (int i = 0; i < 10; ++i) {
        if (!points[i].valid) {
            points[i] = { x, y, z, true };
            return;
        }
    }
    // 꽉 찼으면 아무 것도 안 함 (필요 시 정책 변경 가능)
    // cout << "리스트가 가득 찼습니다!\n";
}

void removeTop() {
    invalidateSort();
    for (int i = 9; i >= 0; --i) {
        if (points[i].valid) {
            points[i].valid = false;
            return;
        }
    }
}

void addBottom(int x, int y, int z) {
    invalidateSort();
    for (int i = 9; i > 0; --i) points[i] = points[i - 1];
    points[0] = { x, y, z, true };
}

void removeBottom() {
    invalidateSort();
    points[0].valid = false;
}

void shiftDown() { // 0→9, 1→0, … 9→8
    invalidateSort();
    Point t = points[0];
    for (int i = 0; i < 9; ++i) points[i] = points[i + 1];
    points[9] = t;
}

void clearAll() {
    for (int i = 0; i < 10; ++i) points[i].valid = false;
    invalidateSort();
}

void sortByDistanceToggle() {
    if (!sortedMode) {
        // 스냅샷 저장(인덱스/valid 포함)
        for (int i = 0; i < 10; ++i) backupPoints[i] = points[i];
        hasBackup = true;
        sortedMode = true;

        // 유효한 점만 모아서 거리 오름차순 정렬
        vector<Point> v;
        v.reserve(10);
        for (int i = 0; i < 10; ++i) if (points[i].valid) v.push_back(points[i]);
        sort(v.begin(), v.end(), [](const Point& a, const Point& b) {
            return dist2(a) < dist2(b);
            });

        // 인덱스 0부터 빈 칸 없이 재배치 (나머지는 비우기)
        for (int i = 0; i < 10; ++i) points[i].valid = false;
        for (int i = 0; i < (int)v.size(); ++i) points[i] = v[i];
    }
    else {
        // 스냅샷 복원: 원래 인덱스/빈칸 그대로
        if (hasBackup) {
            for (int i = 0; i < 10; ++i) points[i] = backupPoints[i];
        }
        sortedMode = false;
        hasBackup = false;
    }
}

// --------- 메인 루프 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    char cmd;
    while (1) {
        cout << "입력 예시: cmd 0 0 0" << endl << "입력: ";
        cin >> cmd;
        system("cls");
        if (cmd == 'q') break;

        if (cmd == '+') {
            int x, y, z; cin >> x >> y >> z;
            addTop(x, y, z);
        }
        else if (cmd == '-') {
            removeTop();
        }
        else if (cmd == 'e') {
            int x, y, z; cin >> x >> y >> z;
            addBottom(x, y, z);
        }
        else if (cmd == 'd') {
            removeBottom();
        }
        else if (cmd == 'a') {
            cout << countPoints() << "\n";
        }
        else if (cmd == 'b') {
            shiftDown();
        }
        else if (cmd == 'c') {
            clearAll();
        }
        else if (cmd == 'f') {
            sortByDistanceToggle();
        }

        printList();
    }
    return 0;
}
