//
// Created by d3nd3r on 9/8/24.
//

#include <SharedCvMat/SharedReceiver.hpp>

#include <mutex>

namespace shared_cv_mat
{
SharedReceiver::SharedReceiver(const std::string& name, OpenMode, cv::Size, int)
    : _mtx_name(name + "_mutex")
    , _mtx { boost::interprocess::open_or_create, _mtx_name.c_str() }
{
    std::lock_guard lock { _mtx };
    _msm = boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create, name.c_str(), 10000 * 1024);
    _sharedHeader = _msm.find<Header>("Header").first;
    _sharedImg = cv::Mat(_sharedHeader->size, _sharedHeader->type, _msm.get_address_from_handle(_sharedHeader->handle));

    _sharedHeader->activeConnect = 1;
}

cv::Mat SharedReceiver::Retrieve()
{
    cv::Mat localImg;
    {
        std::lock_guard lock { _mtx };
        if (_sharedHeader && _sharedHeader->newDataReady)
        {
            _sharedHeader->newDataReady = 0;
            _sharedImg.copyTo(localImg);
        }
    }
    return localImg;
}

SharedReceiver::~SharedReceiver()
{
    try
    {
        std::lock_guard lock{_mtx};
        if (_sharedHeader)
            _sharedHeader->activeConnect = 0;
    }
    catch (...)
    {
    }
}

} // namespace shared_cv_mat