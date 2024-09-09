#include "Utils.hpp"

namespace shared_cv_mat::details
{
const std::string& MutexSuffix() noexcept
{
    static std::string suffix { "_mutex" };
    return suffix;
}
} // namespace shared_cv_mat::details
