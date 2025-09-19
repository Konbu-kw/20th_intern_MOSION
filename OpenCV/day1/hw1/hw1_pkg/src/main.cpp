#include "../include/hw1.hpp"

using namespace cv;
using namespace std;

HW1::HW1(const string& path) : path_img(path)
{
    //이미지 로드
    raw_img = imread(path_img, IMREAD_COLOR);

    //색상 범위 초기화
    lower_red1  = Scalar(0, 50, 50);
    upper_red1  = Scalar(10, 255, 255);
    lower_red2  = Scalar(160, 100, 100);
    upper_red2  = Scalar(180, 255, 255);

    lower_green = Scalar(35, 50, 50);
    upper_green = Scalar(85, 255, 255);

    lower_blue  = Scalar(100, 100, 100);
    upper_blue  = Scalar(130, 255, 255);
}

void HW1::bgrtohsv()
{
    cvtColor(raw_img, hsv_img, COLOR_BGR2HSV);
}

void HW1::colorfilter()
{
    Mat r_img1, r_img2;

    inRange(hsv_img, lower_red1, upper_red1, r_img1);
    inRange(hsv_img, lower_red2, upper_red2, r_img2);
    inRange(hsv_img, lower_green, upper_green, g_img);
    inRange(hsv_img, lower_blue, upper_blue, b_img);

    result_r = r_img1 + r_img2;

    imshow("raw", raw_img);
    imshow("red", result_r);
    imshow("green", g_img);
    imshow("blue", b_img);
}

void HW1::gaussianfilter()
{

    Mat gr_img, gg_img, gb_img;
    GaussianBlur(result_r, gr_img, Size(51, 51), 0);
    GaussianBlur(g_img, gg_img, Size(51, 51), 0);
    GaussianBlur(b_img, gb_img, Size(51, 51), 0);

    imshow("g_r", gr_img);
    imshow("g_g", gg_img);
    imshow("g_b", gb_img);
}

void HW1::progress()
{
    bgrtohsv();
    colorfilter();
    gaussianfilter();
    waitKey(0);
}

int main(int argc, char** argv)
{
    //이미지 파일 경로
    string img_path = "/home/mso/Desktop/20th_intern_mosion/OpenCV/day1/hw1/hw1_package/hw1.png";

    //HW1 클래스 객체 생성
    HW1 processor(img_path);

    //이미지 처리 진행
    processor.progress();

    return 0;
}
