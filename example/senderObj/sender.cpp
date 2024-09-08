#include "../common/common.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/program_options.hpp>

#include <chrono>
#include <iostream>
#include <thread>
#define NOBUG 1

#include <SharedCvMat/SharedSender.hpp>

int main()
{

    cv::VideoCapture cap;
    if (!cap.open(0))
    {
        return -1;
    }

    const cv::Size size { 1920, 1080 };
    shared_cv_mat::SharedSender sender { "mat_sender", shared_cv_mat::OpenMode::OpenOrCreate, size, CV_8UC3 };

    while (cap.isOpened())
    {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
            continue;

        cv::resize(frame, frame, size, cv::INTER_CUBIC);

        cv::imshow("Sender", frame);
        sender.Send(frame);

        if (const int c = cv::waitKey(1); c == 27)
            break;
    }

    return 0;
}