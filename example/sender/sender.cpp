
#include <SharedCvMat/SharedSender.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
    cv::VideoCapture cap;
    if (!cap.open(0))
    {
        return -1;
    }

    const cv::Size size { 1920, 1080 };
    shared_cv_mat::SharedSender sender { "shared_mat_sender", size, CV_8UC3 };

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