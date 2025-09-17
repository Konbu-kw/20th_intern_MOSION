#include "hw3.hpp"

using namespace HW3;

int main() {
    srand((unsigned int)time(NULL));


    //초기 상태 초기화
    Player player(0, 0);
    Monster monster(5, 4, 50);

    char control;

    while (true) {
        if (player.HP <= 0) {
            std::cout << "Player Die !" << std::endl;
            break;
        }
        if (monster.HP <= 0) {
            std::cout << "Moster Die !" << std::endl;
            break;
        }

        std::cout << "Type Command(A/U/D/R/L/S): ";
        char input[6];
        std::cin >> input;
        control = input[0];
        std::cout << std::endl;

        //대소문자 관계없이 입력
        switch (control) {
            case 'A': case 'a':
                player.Attack(monster);
                break;
            case 'U': case 'u':
                player.Y_move(1);
                std::cout << "Y Position 1 moved!" << std::endl;
                break;
            case 'D': case 'd':
                player.Y_move(-1);
                std::cout << "Y Position -1 moved!" << std::endl;
                break;
            case 'R': case 'r':
                player.X_move(1);
                std::cout << "X Position 1 moved!" << std::endl;
                break;
            case 'L': case 'l':
                player.X_move(-1);
                std::cout << "X Position -1 moved!" << std::endl;
                break;
            case 'S': case 's':
                player.Show_status();
                break;
            default:
                std::cout << "Invaild Input" << std::endl;
        }
    }

    return 0;
}
