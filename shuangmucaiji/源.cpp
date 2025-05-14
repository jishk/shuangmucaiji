#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    const int DISPLAY_WIDTH = 640;
    const int DISPLAY_HEIGHT = 480;
    const int TARGET_WIDTH = 1920;
    const int TARGET_HEIGHT = 1080;

    // 初始化摄像头（Windows使用DSHOW后端）
    VideoCapture cap_left(1, CAP_DSHOW);
    VideoCapture cap_right(2, CAP_DSHOW);

    // 检查摄像头是否成功打开
    if (!cap_left.isOpened()) {
        cerr << "错误：无法打开左摄像头（设备号1）！" << endl;
        return -1;
    }
    if (!cap_right.isOpened()) {
        cerr << "错误：无法打开右摄像头（设备号2）！" << endl;
        return -1;
    }

    // 设置摄像头分辨率并检查结果
    bool ret_left_w = cap_left.set(CAP_PROP_FRAME_WIDTH, TARGET_WIDTH);
    bool ret_left_h = cap_left.set(CAP_PROP_FRAME_HEIGHT, TARGET_HEIGHT);
    if (!ret_left_w || !ret_left_h) {
        cerr << "警告：左摄像头分辨率设置失败（要求 "
            << TARGET_WIDTH << "x" << TARGET_HEIGHT << "）" << endl;
    }

    bool ret_right_w = cap_right.set(CAP_PROP_FRAME_WIDTH, TARGET_WIDTH);
    bool ret_right_h = cap_right.set(CAP_PROP_FRAME_HEIGHT, TARGET_HEIGHT);
    if (!ret_right_w || !ret_right_h) {
        cerr << "警告：右摄像头分辨率设置失败（要求 "
            << TARGET_WIDTH << "x" << TARGET_HEIGHT << "）" << endl;
    }

    // 创建可调整的显示窗口
    namedWindow("Left Camera", WINDOW_NORMAL);
    namedWindow("Right Camera", WINDOW_NORMAL);
    resizeWindow("Left Camera", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    resizeWindow("Right Camera", DISPLAY_WIDTH, DISPLAY_HEIGHT);

    Mat frame_left, frame_right;
    Mat display_left, display_right;
    int save_count = 0;

    while (true)
    {
        // 采集原始图像
        cap_left >> frame_left;
        cap_right >> frame_right;

        // 检查帧是否有效
        if (frame_left.empty() || frame_right.empty()) {
            cerr << "错误：获取摄像头帧失败！" << endl;

            // 检查摄像头连接状态
            if (!cap_left.isOpened() || !cap_right.isOpened()) {
                cerr << "错误：摄像头已断开连接，退出程序..." << endl;
                break;
            }
            continue;
        }

        // 创建显示用缩放副本
        resize(frame_left, display_left,
            Size(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0, 0, INTER_AREA);
        resize(frame_right, display_right,
            Size(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0, 0, INTER_AREA);

        // 显示缩放后的图像
        imshow("Left Camera", display_left);
        imshow("Right Camera", display_right);

        // 处理键盘事件
        int key = waitKey(1);
        if (key == 27) break; // ESC退出

        if (key == 32) {      // 空格保存图像
            string left_name = "left_" + to_string(save_count) + ".png";
            string right_name = "right_" + to_string(save_count) + ".png";

            // 检查帧有效性
            if (frame_left.empty() || frame_right.empty()) {
                cerr << "无法保存：当前帧为空！" << endl;
                continue;
            }

            // 尝试保存并检查结果
            bool saved_left = imwrite(left_name, frame_left);
            bool saved_right = imwrite(right_name, frame_right);

            if (saved_left && saved_right) {
                cout << "成功保存图像组：" << save_count << endl;
                save_count++;
            }
            else {
                cerr << "保存失败！检查以下文件路径和权限：" << endl
                    << "左: " << left_name << endl
                    << "右: " << right_name << endl;
            }
        }
    }

    // 释放资源
    cap_left.release();
    cap_right.release();
    destroyAllWindows();
    return 0;
}