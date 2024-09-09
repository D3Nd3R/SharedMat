
#include <SharedCvMat/SharedSender.hpp>
#include <Utils.hpp>

#include <mutex>

namespace shared_cv_mat
{
SharedSender::SharedSender(const std::string& name, OpenMode openMode, cv::Size size, int type)
    : _name { name }
    , _mtx_name { _name + details::MutexSuffix() }
    , _mtx { boost::interprocess::open_or_create, _mtx_name.c_str() }
{
    boost::interprocess::shared_memory_object::remove(_name.c_str());

    _localHeader.size = size;
    _localHeader.type = type;
    _localHeader.activeConnect = 0;

    _sharedImg = cv::Mat::zeros(size, type);

    _managed_shm = OpenOrCreate(name, openMode);

    std::lock_guard lock(_mtx);
    _sharedHeader = _managed_shm.find_or_construct<Header>("Header")();

    const int data_size = _sharedImg.total() * _sharedImg.elemSize();
    const Header* shared_image_data_ptr { (Header*)_managed_shm.allocate(data_size) };

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
    std::lock_guard lock(_mtx);
    if (image.type() != _localHeader.type || image.size() != _localHeader.size)
        return false;

    image.copyTo(_sharedImg);
    _sharedHeader->newDataReady = 1;
    return true;
}

boost::interprocess::managed_shared_memory SharedSender::OpenOrCreate(const std::string& name, OpenMode) noexcept(false)
{
    const int data_size = _sharedImg.total() * _sharedImg.elemSize();
    return boost::interprocess::managed_shared_memory(boost::interprocess::create_only, name.c_str(),
                                                      1 * data_size + sizeof(Header) + 1024);
}
} // namespace shared_cv_mat