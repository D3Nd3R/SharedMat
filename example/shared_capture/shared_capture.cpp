#include "../../lib/include/SharedCvMat/SharedReceiver.hpp"

#include <SharedCvMat/SharedCapture.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <thread>

int main()
{
    shared_cv_mat::SharedCapture capture { 100 };

    int32_t openAttempt = 10000;
    while (!capture.open("mat_sender") && openAttempt > 0)
    {
        std::cout << "try open: " << openAttempt-- << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (capture.isOpened())
        std::cout << "capture opened" << std::endl;

    cv::Mat image = cv::Mat::zeros(cv::Size(1920, 1080), CV_8UC1);
    // Show image before read to enable cv::waitKey
    cv::imshow("Receiver", image);
    while (capture.isOpened())
    {
        if (const auto flag = capture.read(image); flag && !image.empty())
            cv::imshow("Receiver", image);

        if (const int c = cv::waitKey(20); c == 27)
            break;
    }

    // Closes all the frames
    cv::destroyAllWindows();

    return 0;
}
