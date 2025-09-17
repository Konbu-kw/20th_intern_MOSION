#include <iostream>
#include <cstdlib>
#include <ctime>

namespace HW2{

//structure 구조체 활용
struct coor {
    int x, y;
};

class Coordinate {
private:
    coor* p;
    int p_number;

public:
    Coordinate(int n, int min, int max);
    ~Coordinate();

    double distance(coor& p1, coor& p2);
    void mmdistance();
    void print();
};

//뉴튼-랩슨법으로 sqrt
double newtonSqrt(double n) {
    if (n < 0) return -1;
    double t = n;
    for (int i = 0; i < 16; i++) {
        if (t < 1.0) break;
        t = (t * t + n) / (2.0 * t);
    }
    return t;
}

//생성자
Coordinate::Coordinate(int n, int min, int max) {
    p_number = n;
    p = new coor[p_number];
    srand((unsigned int)time(NULL));
    for (int i = 0; i < p_number; i++) {
        //좌표는 정수형
        p[i].x = min + rand() % (max - min + 1);
        p[i].y = min + rand() % (max - min + 1);
    }
}

//소멸자
Coordinate::~Coordinate() {
    delete[] p;
}

//거리 계산
double Coordinate::distance(coor& p1, coor& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return newtonSqrt(dx * dx + dy * dy);
}

//모든 점 출력
void Coordinate::print() {
    for (int i = 0; i < p_number; i++) {
        std::cout << "Point ";
        std::cout << i+1;
        std::cout << ": nX=";
        std::cout << p[i].x;
        std::cout << " , nY=";
        std::cout << p[i].y << std::endl;
    }
}

//최소, 최대 거리 계산
void Coordinate::mmdistance() {
    if (p_number < 2) {
        std::cout << "Lack Point Number" << std::endl;
        return;
    }

    double min_distance = distance(p[0], p[1]);
    double max_distance = min_distance;
    coor min_coor1 = p[0], min_coor2 = p[1];
    coor max_coor1 = p[0], max_coor2 = p[1];

    for (int i = 0; i < p_number; i++) {
        for (int j = i + 1; j < p_number; j++) {
            double d = distance(p[i], p[j]);
            if (d < min_distance) {
                min_distance = d;
                min_coor1 = p[i];
                min_coor2 = p[j];
            }
            if (d > max_distance) {
                max_distance = d;
                max_coor1 = p[i];
                max_coor2 = p[j];
            }
        }
    }

    //결과 출력
    std::cout << "-------- Result --------" << std::endl;
    std::cout << "MinDist=";
    std::cout << min_distance << std::endl;
    std::cout << "Pair of Min Coor.(x,y): P1(";
    std::cout << min_coor1.x;
    std::cout << ",";
    std::cout << min_coor1.y;
    std::cout << ") & P2(";
    std::cout << min_coor2.x;
    std::cout << ",";
    std::cout << min_coor2.y;
    std::cout << ")" << std::endl;

    std::cout << "MaxDist=";
    std::cout << max_distance;
    std::cout << std::endl;
    std::cout << "Pair of Max Coor.(x,y): P1(";
    std::cout << max_coor1.x;
    std::cout << ",";
    std::cout << max_coor1.y;
    std::cout << ") & P2(";
    std::cout << max_coor2.x;
    std::cout << ",";
    std::cout << max_coor2.y;
    std::cout << ")" << std::endl;
}
}