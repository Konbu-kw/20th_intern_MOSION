#ifndef HW1_HPP
#define HW1_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class HW1
{
public:
    //사진 경로
    HW1(const string& path);

    //이미지 처리 실행
    void progress();

private:
    //사진 경로
    string path_img;
    
    //기본 이미지
    Mat raw_img;
    //HSV 이미지
    Mat hsv_img;

    //색상 범위
    Scalar lower_red1, upper_red1;
    Scalar lower_red2, upper_red2;
    Scalar lower_green, upper_green;
    Scalar lower_blue, upper_blue;

    //매서드 함수
    void bgrtohsv();
    void colorfilter();
    void gaussianfilter();

    //colortfilter시 사용
    Mat result_r;
    Mat g_img;
    Mat b_img;
};

#endif
