// EXECUE+ :: Image File Representation in C++
// This module handles encoded image sequences, frame data, and stream-pipeline interpretation
// within EXECUE+’s dominion-bound execution architecture.

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <map>

namespace EXECUE {

    enum class PixelFormat {
        RGB,
        RGBA,
        GRAYSCALE
    };

    struct FrameHeader {
        uint32_t width;
        uint32_t height;
        uint32_t frameIndex;
        PixelFormat format;
    };

    struct Pixel {
        uint8_t r, g, b, a;
        Pixel(uint8_t _r=0, uint8_t _g=0, uint8_t _b=0, uint8_t _a=255) : r(_r), g(_g), b(_b), a(_a) {}
    };

    struct ImageFrame {
        FrameHeader header;
        std::vector<Pixel> pixels;

        void initialize(uint32_t w, uint32_t h, PixelFormat fmt) {
            header.width = w;
            header.height = h;
            header.format = fmt;
            pixels.resize(w * h);
        }

        Pixel& at(uint32_t x, uint32_t y) {
            return pixels[y * header.width + x];
        }
    };

    class ImageFile {
    private:
        std::vector<ImageFrame> frames;
        std::string imageName;

    public:
        explicit ImageFile(const std::string& name) : imageName(name) {}

        void addFrame(const ImageFrame& frame) {
            frames.push_back(frame);
        }

        size_t getFrameCount() const {
            return frames.size();
        }

        const ImageFrame& getFrame(size_t index) const {
            if (index >= frames.size())
                throw std::out_of_range("Frame index out of bounds");
            return frames[index];
        }

        void encodeToStream(std::ostream& os) const {
            os << "EXECUE_IMAGE_START\n";
            os << "NAME: " << imageName << "\n";
            os << "FRAME_COUNT: " << frames.size() << "\n";
            for (const auto& frame : frames) {
                os << "FRAME_BEGIN\n";
                os << "WIDTH: " << frame.header.width << "\n";
                os << "HEIGHT: " << frame.header.height << "\n";
                os << "FORMAT: " << static_cast<int>(frame.header.format) << "\n";
                for (const auto& px : frame.pixels) {
                    os << (int)px.r << "," << (int)px.g << "," << (int)px.b << "," << (int)px.a << " ";
                }
                os << "\nFRAME_END\n";
            }
            os << "EXECUE_IMAGE_END\n";
        }

        void saveToFile(const std::string& path) const {
            std::ofstream file(path);
            if (!file.is_open()) throw std::runtime_error("Failed to open image file for writing");
            encodeToStream(file);
            file.close();
        }
    };

    class ImageProcessor {
    public:
        static ImageFile createTestPattern(const std::string& name, uint32_t w, uint32_t h, uint32_t count) {
            ImageFile img(name);
            for (uint32_t i = 0; i < count; ++i) {
                ImageFrame frame;
                frame.initialize(w, h, PixelFormat::RGBA);
                for (uint32_t y = 0; y < h; ++y) {
                    for (uint32_t x = 0; x < w; ++x) {
                        frame.at(x, y) = Pixel((x + i) % 256, (y + i) % 256, (x * y + i) % 256);
                    }
                }
                img.addFrame(frame);
            }
            return img;
        }
    };
}

int main() {
    using namespace EXECUE;

    auto image = ImageProcessor::createTestPattern("execue_sample", 16, 16, 3);
    image.saveToFile("execue_image_file.eximg");

    std::cout << "EXECUE+ Image File saved successfully.\n";
    return 0;
}
