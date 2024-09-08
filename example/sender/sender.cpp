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
/*
 * * from http://opencvkazukima.codeplex.com/SourceControl/changeset/view/19055#interprocess/src/client.cpp
 *
 * g++ -Wall  -c  srv4.cpp
 * g++ -Wall -o "srv4" srv4.o -I /usr/include/boost  `pkg-config --libs --cflags opencv` -std=gnu++11 -lrt
 * -lboost_program_options  -pthread
 *
 * g++ -Wall  -c  srv4.cpp  -std=gnu++11
 * g++ -o  srv4 srv4.o -I /usr/local/opencv-2.4.13/include -O2 -g -Wall -L /usr/local/opencv-2.4.13/lib -lopencv_core
 * -lopencv_imgcodecs -lopencv_videoio -lopencv_highgui -lopencv_video  -lopencv_photo -lopencv_imgproc  -std=gnu++11
 * -lboost_program_options -std=gnu++11 -lrt -pthread
 *
 * LD_LIBRARY_PATH=/usr/local/opencv-2.4.13/lib/ ./srv4 --video 0 --xres 1920 --yres 1080
 */
using namespace cv;
using namespace std;
namespace po = boost::program_options;
int fps = 30;



int main()
{
    namespace ip = boost::interprocess;

    // remove existed shared memory object
    ip::shared_memory_object::remove(MEMORY_NAME);

    // Open capture device
    cv::VideoCapture capture_device(0);
    if (!capture_device.isOpened())
    {
        cerr << "cannot open camera" << endl;
        exit(1);
    }

    // and capture one frame.
    cv::Mat captured_image;
    {
        capture_device >> captured_image;
    }

    // calculate total data size of matrixes data.
    // data_size = cols * rows * sizeof(a pixel)
    const int data_size = captured_image.total() * captured_image.elemSize();

    // Reserve shared memory
    ip::managed_shared_memory msm(ip::open_or_create, MEMORY_NAME,
                                  1 * data_size + sizeof(SharedImageHeader) + 1024 /* is it enough? */);

    // make a region named "Matheader"
    // and return its pointer
    // it's size is sizeof(SharedImageHeader)
    SharedImageHeader* shared_image_header = msm.find_or_construct<SharedImageHeader>("MatHeader")();

    // make a unnamed shared memory region.
    // Its size is data_size
    const SharedImageHeader* shared_image_data_ptr;
    shared_image_data_ptr = (SharedImageHeader*)msm.allocate(data_size);

    // write the size, type and image buffPosition to the Shared Memory.
    shared_image_header->size = captured_image.size();
    shared_image_header->type = captured_image.type();
    shared_image_header->buffPosition = 0;

    // write the handler to an unnamed region to the Shared Memory
    shared_image_header->handle = msm.get_handle_from_address(shared_image_data_ptr);

    cv::Mat shared;
    shared = cv::Mat(shared_image_header->size, shared_image_header->type,
                     msm.get_address_from_handle(shared_image_header->handle));

    // Spinning until 'q' key is down
    shared_image_header->buffPosition = 1;
    while (true)
    {
        // Capture the image and show
        this_thread::sleep_for(chrono::milliseconds(5));
        capture_device.grab();
        if (shared_image_header->buffPosition == 0)
        {
            capture_device.retrieve(shared);
            // Increment the buffPosition
            shared_image_header->buffPosition = 1;
        }
        else if (shared_image_header->buffPosition == 2)
        {
            break;
        }
    }

    // Remove Shared Memory Object
    ip::shared_memory_object::remove(MEMORY_NAME);

    return 0;
}