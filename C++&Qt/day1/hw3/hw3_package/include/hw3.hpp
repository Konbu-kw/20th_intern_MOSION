#include <iostream>
#include <cstdlib>
#include <ctime>

namespace HW3 {
    class Monster {
    public:
        int HP = 50;
        int x = 5, y = 4;
        int M_dmg = 33;
        Monster();
        Monster(int x, int y, int HP){
            this -> HP = HP;
            this -> x = x;
            this -> y = y;
            this -> M_dmg = 33;
        }


        int Be_Attacked(int dmg) {
            HP -= dmg;
            if (HP < 0) HP = 0;
            return HP;
        }

        void MoveRandom() {
            //위 아래 좌우 랜덤 움직임
            int d = rand() % 4;
            switch (d) {
                case 0: 
                    if (y < 4) y++; 
                    break;
                case 1: 
                    if (y > 0) y--; 
                    break;
                case 2: 
                    if (x > 0) x--; 
                    break;
                case 3:    
                    if (x < 4) x++; 
                    break;
            }
        }
    };


    class Player {
    public:
        int HP = 100;
        int MP = 15;
        int x = 0, y = 0;
        int P_dmg = 10;
        Player();
        Player(int x, int y){
            this -> HP = 100;
            this -> MP = 15;
            this -> x = x;
            this -> y = y;
            this -> P_dmg = 10;
        }

        void Attack(Monster &target) {
            if (MP <= 0) {
                std::cout << "MP 부족 !" << std::endl;
                HP = 0;
                return;
            }
            MP--;

            if (x == target.x && y == target.y) {
                //동일 위치면 공격 유효 아니면 MP 낭비
                target.Be_Attacked(P_dmg);
                std::cout << "공격 성공 ! Monster HP: ";
                std::cout << target.HP << std::endl;

                //30% 확률 반격
                if ((rand() % 100) < 30) {
                    HP -= target.M_dmg;
                    std::cout << "공격 받음 ! Player HP: ";
                    std::cout << HP << std::endl;
                }
                //공격 이후 랜덤 움직임
                target.MoveRandom();
                //몬스터 위치 공개 코드 - 디버깅용
                std::cout << "몬스터가 (";
                std::cout << target.x;
                std::cout << ",";
                std::cout << target.y;
                std::cout << ")로 도망 !" << std::endl;

            } else {
                //동일 위치에 있지 않은 상태에서 공격 시도 -> MP만 소비
                std::cout << "공격 실패 !" << std::endl;
            }
        }

        void Show_status() {
            //상태 표시 S
            std::cout << "HP: ";
            std::cout << HP << std::endl;
            std::cout << "MP: ";
            std::cout << MP << std::endl;
            std::cout << "Position:";
            std::cout << x;
            std::cout << ",";
            std::cout << y << std::endl;
        }

        void X_move(int move) {
            //좌우 움직임 R, L
            if (x + move >= 0 && x + move <= 5) {
                //지도 범위 제한
                x += move;
            }
        }

        void Y_move(int move) {
            //위 아래 움직임
            if (y + move >= 0 && y + move <= 5) {
                //지도 범위 제한
                y += move;
            }
        }
    };
}
