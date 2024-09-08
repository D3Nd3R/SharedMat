#pragma once

#include "Header.hpp"
#include "Types.hpp"

#include <opencv2/core/mat.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <string>

namespace shared_cv_mat
{

class SharedSender final
{
private:
public:
    explicit SharedSender(const std::string& name, OpenMode openMode, cv::Size size, int type);

    ~SharedSender();

    bool Send(const cv::Mat& image);

private:
    boost::interprocess::managed_shared_memory OpenOrCreate(const std::string& name, OpenMode openMode) noexcept(false);

private:
    boost::interprocess::managed_shared_memory _managed_shm;
    std::string _name;
    std::string _mtx_name;
    boost::interprocess::named_mutex _mtx;

    cv::Mat _sharedImg;

    Header _localHeader;
    Header* _sharedHeader { nullptr };
};

} // namespace shared_cv_mat
