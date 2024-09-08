#pragma once

#include <opencv2/core/core.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

typedef struct{
    cv::Size  size;
    int       type;
    int       buffPosition;
    boost::interprocess::managed_shared_memory::handle_t handle;
} SharedImageHeader;

const char *MEMORY_NAME = "MySharedMemory";

