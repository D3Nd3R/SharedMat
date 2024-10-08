
#include <SharedCvMat/SharedSender.hpp>
#include <Utils.hpp>

#include <mutex>

namespace shared_cv_mat
{
SharedSender::SharedSender(const std::string& name, cv::Size size, int type)
    : _name { name }
    , _mtx_name { _name + details::MutexSuffix() }
    , _mtxDelete { _mtx_name }
{
    boost::interprocess::permissions unrestricted_permissions;
    unrestricted_permissions.set_unrestricted();

    const mode_t old_umask = umask(0);
    _mtx.emplace(boost::interprocess::open_or_create, _mtx_name.c_str(), unrestricted_permissions);
    umask(old_umask);
    boost::interprocess::shared_memory_object::remove(_name.c_str());

    _localHeader.size = size;
    _localHeader.type = type;
    _localHeader.activeConnect = 0;

    _sharedImg = cv::Mat::zeros(size, type);

    const int data_size = _sharedImg.total() * _sharedImg.elemSize();
    _managed_shm =
        boost::interprocess::managed_shared_memory(boost::interprocess::create_only, name.c_str(),
                                                   1 * data_size + sizeof(Header) + 1024, 0, unrestricted_permissions);

    std::lock_guard lock(*_mtx);
    _sharedHeader = _managed_shm.find_or_construct<Header>("Header")();

    const Header* shared_image_data_ptr { static_cast<Header*>(_managed_shm.allocate(data_size)) };

    _localHeader.handle = _managed_shm.get_handle_from_address(shared_image_data_ptr);

    *_sharedHeader = _localHeader;

    _sharedImg =
        cv::Mat(_localHeader.size, _localHeader.type, _managed_shm.get_address_from_handle(_localHeader.handle));
}

SharedSender::~SharedSender()
{
    boost::interprocess::shared_memory_object::remove(_name.c_str());
    boost::interprocess::named_mutex::remove(_mtx_name.c_str());
}

bool SharedSender::Send(const cv::Mat& image)
{
    std::lock_guard lock(*_mtx);
    if (image.type() != _localHeader.type || image.size() != _localHeader.size)
        return false;

    image.copyTo(_sharedImg);
    _sharedHeader->newDataReady = 1;
    return true;
}
} // namespace shared_cv_mat