#include <iostream>

namespace HW1{

class hw1 {
private:
    int n;              //원소 개수
    double check_n;     //입력 확인용 예외처리용
    int* arr;           //동적 배열
    int max;            //최대값
    int min;            //최소값
    int sum;            //합
    double avg;         //평균

public:
    //생성자
    hw1() {
        n = 0;
        check_n = 0.0;
        arr = 0;
        max = 0;
        min = 0;
        sum = 0;
        avg = 0.0;
    }
    //소멸자
    ~hw1() {
        delete arr;
    }
    //원소 개수
    void count() {
        std::cout << "몇 개의 원소를 할당하시겠습니까? : " << std::endl;
        std::cin >> check_n;
    }
    //입력 확인용 예외처리(일반 정수입력, 실수 입력, 음수 입력, 문자 입력)
    bool check_cin() {
        if (std::cin.fail()) {
            return false;
        }
        if (check_n <= 1 || !check_n) {
            return false;
        }
        n = (int)(check_n);
        return true;
    }
    //동적할당
    void mallocing() {
        arr = new int[n];
    }
    //순서대로 값 대입
    void insert_element() {
        for (int i = 0; i < n; i) {
            std::cout << "정수형 데이터 입력: ";
            double check_arr;
            std::cin >> check_arr;
            if(std::cin.fail() || check_arr != (int)check_arr){
                std::cout << "잘못된 입력" << std::endl;
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;

            }
            else{
                arr[i] = (int)check_arr;
                i++;
            }
        }
    }
    //연산
    void calculate() {
        sum = 0;
        max = arr[0];
        min = arr[0];

        for (int i = 0; i < n; i++) {
            sum += arr[i];
            if (arr[i] > max) max = arr[i];
            if (arr[i] < min) min = arr[i];
        }

        avg = (double)(sum) / n;
    }
    //출력
    void result(){
        std::cout << "최대값: ";
        std::cout << max << std::endl;
        std::cout << "최소값: ";
        std::cout << min << std::endl;
        std::cout << "전체합: ";
        std::cout << sum << std::endl;
        std::cout << "평균: ";
        std::cout << avg << std::endl;
    }
};
}