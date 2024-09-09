#include "../../lib/include/SharedCvMat/SharedReceiver.hpp"


#include <opencv2/highgui/highgui.hpp>


int main()
{
    const cv::Size size { 1920, 1080 };
    shared_cv_mat::SharedReceiver receiver { "mat_sender", shared_cv_mat::OpenMode::OpenOrCreate, size, CV_8UC3 };

    cv::Mat image;
    while (true)
    {
        if (const auto flag = receiver.Retrieve(image); flag && !image.empty())
            cv::imshow("Receiver", image);

        if (const int c = cv::waitKey(1); c == 27)
            break;
    }

    // Closes all the frames
    cv::destroyAllWindows();

    return 0;
}
