#pragma once

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>

namespace shared_cv_mat
{
struct Header final
{
    int activeConnect { 0 };
    cv::Size size { 0, 0 };
    int type { CV_8UC3 };
    boost::interprocess::managed_shared_memory::handle_t handle;
};
} // namespace shared_cv_mat
