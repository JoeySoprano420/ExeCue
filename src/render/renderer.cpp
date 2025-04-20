#include <iostream>
#include <string>

namespace execue {
    void renderResult(const std::string& output) {
        std::cout << "[Rendered Execue Output] >>> " << output << std::endl;
    }

    void renderError(const std::string& err) {
        std::cerr << "[Execue ERROR] !!! " << err << std::endl;
    }
}
