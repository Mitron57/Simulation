#include <vector>

namespace Engine::Utils {
    void fillMissingBits(std::vector<std::uint8_t>& data,
                         std::size_t component) {
        for (std::size_t i {data.size()}; i < component; ++i) {
            data.push_back(0);
        }
    }

    std::uint64_t bitSequenceToULL(const std::vector<std::uint8_t>& data) {
        std::uint64_t result {};
        for (std::size_t i {}; i < data.size(); ++i) {
            result |= data[i] << i;
        }
        return result;
    }
}  // namespace Engine::Utils
