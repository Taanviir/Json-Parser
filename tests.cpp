#include "JsonParser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <json file>" << std::endl;
        return 1;
    }

    try {
        JsonParser parser(argv[1]);
        parser.parse();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
