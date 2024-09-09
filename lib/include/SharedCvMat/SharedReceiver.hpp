#pragma once
#include "Header.hpp"

#include <opencv2/core/mat.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <string>

namespace shared_cv_mat
{

class SharedReceiver
{
public:
    SharedReceiver(const std::string& name) noexcept(false);

    bool read(cv::OutputArray img) noexcept(false);

    ~SharedReceiver();

private:
    boost::interprocess::managed_shared_memory _msm;
    Header* _sharedHeader { nullptr };
    std::string _mtx_name;
    boost::interprocess::named_mutex _mtx;

    cv::Mat _sharedImg;
};

} // namespace shared_cv_mat
