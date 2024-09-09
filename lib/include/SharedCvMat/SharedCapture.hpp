#pragma once

#include "SharedReceiver.hpp"

#include <opencv2/core/mat.hpp>

#include <optional>
#include <string>

namespace shared_cv_mat
{

class SharedCapture
{
public:
    /// Constructor
    /// @param maxFailedRead failed read attempts to disconnect
    explicit SharedCapture(const size_t maxFailedRead = 10) noexcept;

    /// Is connection active
    /// @return flag
    [[nodiscard]] bool isOpened() const noexcept;
    /// Open shared memory to read
    /// @param memName name of shared memory
    /// @return operation result
    [[nodiscard]] bool open(const std::string& memName) noexcept;

    /// Read new image from shared memory
    /// @param image outimage
    /// @return operation result
    [[nodiscard]] bool read(cv::OutputArray image) noexcept;

    /// Close shared memory
    void release() noexcept;

private:
    bool _isOpened { false };
    const size_t _maxFailedRead { 10 };
    std::string _memName;
    std::size_t _failedReadCounter { 0 };

    std::optional<SharedReceiver> _receiver { std::nullopt };
};

} // namespace shared_cv_mat
