#include "../../lib/include/SharedCvMat/SharedReceiver.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/program_options.hpp>

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    const cv::Size size { 1920, 1080 };
    shared_cv_mat::SharedReceiver receiver { "mat_sender", shared_cv_mat::OpenMode::OpenOrCreate, size, CV_8UC3 };

    while (true)
    {
        auto img = receiver.Retrieve();

        if (!img.empty())
            cv::imshow("Receiver", img);

        if (const int c = cv::waitKey(1); c == 27)
            break;
    }

    // Closes all the frames
    cv::destroyAllWindows();

    return 0;
}
