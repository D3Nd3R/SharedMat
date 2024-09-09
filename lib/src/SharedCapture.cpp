

#include <SharedCvMat/SharedCapture.hpp>

#include <iostream>

namespace shared_cv_mat
{
SharedCapture::SharedCapture(const size_t maxFailedRead) noexcept
    : _maxFailedRead { maxFailedRead }
{
}

bool SharedCapture::isOpened() const noexcept
{
    return _isOpened && _failedReadCounter < _maxFailedRead;
}

bool SharedCapture::open(const std::string& memName) noexcept
{
    release();
    try
    {
        _receiver.emplace(memName);
        _isOpened = true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[SharedCapture::read]: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "[SharedCapture::read]: unknown exception" << std::endl;
    }

    return isOpened();
}

bool SharedCapture::read(cv::OutputArray image) noexcept
{
    if (!_isOpened || !_receiver)
        return false;

    try
    {
        const bool readOk = _receiver->read(image);

        if (readOk)
        {
            _failedReadCounter = 0;
        }
        else
        {
            ++_failedReadCounter;
        }

        return readOk;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[SharedCapture::read]: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "[SharedCapture::read]: unknown exception" << std::endl;
    }
    return false;
}

void SharedCapture::release() noexcept
{
    if (_receiver)
        _receiver.reset();

    _isOpened = false;
    _failedReadCounter = 0;
    _memName.clear();
}
} // namespace shared_cv_mat