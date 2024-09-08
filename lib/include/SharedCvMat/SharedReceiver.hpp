#pragma once
#include "Header.hpp"
#include "Types.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <string>

namespace shared_cv_mat
{

class SharedReceiver
{
public:
    SharedReceiver(const std::string& name, OpenMode openMode, cv::Size size, int type) noexcept(false);

    cv::Mat Retrieve() noexcept(false);

    ~SharedReceiver();

private:
    boost::interprocess::managed_shared_memory _msm;
    Header* _sharedHeader { nullptr };
    std::string _mtx_name;
    boost::interprocess::named_mutex _mtx;

    cv::Mat _sharedImg;
};

} // namespace shared_cv_mat
