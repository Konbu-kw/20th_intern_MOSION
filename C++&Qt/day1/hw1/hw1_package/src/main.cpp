#include "hw1.hpp"

using namespace HW1;

int main() {
    //단일 객체 생성
    hw1 element;

    element.count();
    //예외처리
    if (!element.check_cin()) {
        std::cout << "종료합니다" << std::endl;
        return 0;
    }
    //동적할당 -> 요소 삽입 -> 연산 -> 출력
    element.mallocing();
    element.insert_element();
    element.calculate();
    element.result();

    return 0;
}