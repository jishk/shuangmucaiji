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

    // ��ʼ������ͷ��Windowsʹ��DSHOW��ˣ�
    VideoCapture cap_left(1, CAP_DSHOW);
    VideoCapture cap_right(2, CAP_DSHOW);

    // �������ͷ�Ƿ�ɹ���
    if (!cap_left.isOpened()) {
        cerr << "�����޷���������ͷ���豸��1����" << endl;
        return -1;
    }
    if (!cap_right.isOpened()) {
        cerr << "�����޷���������ͷ���豸��2����" << endl;
        return -1;
    }

    // ��������ͷ�ֱ��ʲ������
    bool ret_left_w = cap_left.set(CAP_PROP_FRAME_WIDTH, TARGET_WIDTH);
    bool ret_left_h = cap_left.set(CAP_PROP_FRAME_HEIGHT, TARGET_HEIGHT);
    if (!ret_left_w || !ret_left_h) {
        cerr << "���棺������ͷ�ֱ�������ʧ�ܣ�Ҫ�� "
            << TARGET_WIDTH << "x" << TARGET_HEIGHT << "��" << endl;
    }

    bool ret_right_w = cap_right.set(CAP_PROP_FRAME_WIDTH, TARGET_WIDTH);
    bool ret_right_h = cap_right.set(CAP_PROP_FRAME_HEIGHT, TARGET_HEIGHT);
    if (!ret_right_w || !ret_right_h) {
        cerr << "���棺������ͷ�ֱ�������ʧ�ܣ�Ҫ�� "
            << TARGET_WIDTH << "x" << TARGET_HEIGHT << "��" << endl;
    }

    // �����ɵ�������ʾ����
    namedWindow("Left Camera", WINDOW_NORMAL);
    namedWindow("Right Camera", WINDOW_NORMAL);
    resizeWindow("Left Camera", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    resizeWindow("Right Camera", DISPLAY_WIDTH, DISPLAY_HEIGHT);

    Mat frame_left, frame_right;
    Mat display_left, display_right;
    int save_count = 0;

    while (true)
    {
        // �ɼ�ԭʼͼ��
        cap_left >> frame_left;
        cap_right >> frame_right;

        // ���֡�Ƿ���Ч
        if (frame_left.empty() || frame_right.empty()) {
            cerr << "���󣺻�ȡ����ͷ֡ʧ�ܣ�" << endl;

            // �������ͷ����״̬
            if (!cap_left.isOpened() || !cap_right.isOpened()) {
                cerr << "��������ͷ�ѶϿ����ӣ��˳�����..." << endl;
                break;
            }
            continue;
        }

        // ������ʾ�����Ÿ���
        resize(frame_left, display_left,
            Size(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0, 0, INTER_AREA);
        resize(frame_right, display_right,
            Size(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0, 0, INTER_AREA);

        // ��ʾ���ź��ͼ��
        imshow("Left Camera", display_left);
        imshow("Right Camera", display_right);

        // ��������¼�
        int key = waitKey(1);
        if (key == 27) break; // ESC�˳�

        if (key == 32) {      // �ո񱣴�ͼ��
            string left_name = "left_" + to_string(save_count) + ".png";
            string right_name = "right_" + to_string(save_count) + ".png";

            // ���֡��Ч��
            if (frame_left.empty() || frame_right.empty()) {
                cerr << "�޷����棺��ǰ֡Ϊ�գ�" << endl;
                continue;
            }

            // ���Ա��沢�����
            bool saved_left = imwrite(left_name, frame_left);
            bool saved_right = imwrite(right_name, frame_right);

            if (saved_left && saved_right) {
                cout << "�ɹ�����ͼ���飺" << save_count << endl;
                save_count++;
            }
            else {
                cerr << "����ʧ�ܣ���������ļ�·����Ȩ�ޣ�" << endl
                    << "��: " << left_name << endl
                    << "��: " << right_name << endl;
            }
        }
    }

    // �ͷ���Դ
    cap_left.release();
    cap_right.release();
    destroyAllWindows();
    return 0;
}