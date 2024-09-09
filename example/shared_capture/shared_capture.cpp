#include "../../lib/include/SharedCvMat/SharedReceiver.hpp"

#include <SharedCvMat/SharedCapture.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

int main()
{
    shared_cv_mat::SharedCapture capture;

    int32_t openAttempt = 100;
    while (!capture.open("mat_sender") && openAttempt > 0)
    {
        std::cout << "try open: " << openAttempt-- << std::endl;
    }

    cv::Mat image;
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
