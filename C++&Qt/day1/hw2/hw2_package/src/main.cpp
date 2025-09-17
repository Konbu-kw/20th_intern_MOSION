#include "hw2.hpp"

using namespace HW2;

int main() {
    double min, max;
    double n;   

    std::cout << "******** HW2 Point Distance Computation ********" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout <<"Please define the number of points: ";
    std::cin >> n;

    if(n <= 2 || !n || std::cin.fail()){
        std::cout << "종료합니다" << std::endl;
		std::cin.clear();
		std::cin.ignore(5, '\n');
        return -1;
    }

    std::cout << "Please define minimum of coor. value: ";
    std::cin >> min;
    if(min < 0 || !min || std::cin.fail()){
        std::cout << "종료합니다" << std::endl;
		std::cin.clear();
		std::cin.ignore(5, '\n');
        return -1;
    }

    std::cout << "Please define maximum of coor. value: ";
    std::cin >> max;

    if(max < min || !max || std::cin.fail()){
        std::cout << "종료합니다" << std::endl;
		std::cin.clear();
		std::cin.ignore(5, '\n');
        return -1;
    }

    std::cout << "Generate Random points" << std::endl;
    Coordinate compute((int)n, (int)min, (int)max);
    compute.print();

    compute.mmdistance();

    std::cout << "******** Completed ********" << std::endl;
    return 0;
}