#pragma once

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>

#include <chrono>

namespace shared_cv_mat
{
struct Header final
{
    int activeConnect { 0 };
    cv::Size size { 0, 0 };
    int type { CV_8UC3 };
    std::chrono::milliseconds timestamp { std::chrono::milliseconds::zero() };
    boost::interprocess::managed_shared_memory::handle_t handle;
};
} // namespace shared_cv_mat
