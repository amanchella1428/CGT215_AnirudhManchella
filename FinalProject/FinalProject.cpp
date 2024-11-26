#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;

//sharpness modifier for me to debug
std::vector<std::vector<int>> createSharpenKernel(int sharpenStrength) {
    return {
        {  0, -sharpenStrength,  0},
        { -sharpenStrength,  4 + sharpenStrength, -sharpenStrength},
        {  0, -sharpenStrength,  0}
    };
}

//applying the first convolution: sharpness
sf::Image applyConvolution(const sf::Image& image, const std::vector<std::vector<int>>& kernel) {
    int width = image.getSize().x;
    int height = image.getSize().y;
    int kernelSize = kernel.size();
    int kernelRadius = kernelSize / 2;

    sf::Image result;
    result.create(width, height);

    //Going through each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int redValue = 0;
            int greenValue = 0;
            int blueValue = 0;

            //Applies the kernerls for the color change
            for (int ky = -kernelRadius; ky <= kernelRadius; ++ky) {
                for (int kx = -kernelRadius; kx <= kernelRadius; ++kx) {
                    int pixelY = y + ky;
                    int pixelX = x + kx;

                    //Checks for the boundaries
                    if (pixelY >= 0 && pixelY < height && pixelX >= 0 && pixelX < width) {
                        sf::Color pixel = image.getPixel(pixelX, pixelY);

                        //Applies to kernels to each color channel
                        redValue += pixel.r * kernel[ky + kernelRadius][kx + kernelRadius];
                        greenValue += pixel.g * kernel[ky + kernelRadius][kx + kernelRadius];
                        blueValue += pixel.b * kernel[ky + kernelRadius][kx + kernelRadius];
                    }
                }
            }

            //Clamp the color values to be within the range of [0, 255]
            redValue = std::min(std::max(redValue, 0), 255);
            greenValue = std::min(std::max(greenValue, 0), 255);
            blueValue = std::min(std::max(blueValue, 0), 255);

            //Set the new pixel value in the result image
            result.setPixel(x, y, sf::Color(redValue, greenValue, blueValue));
        }
    }

    return result;
}


int main() {
    string imagePath = "C:/Users/Owner/source/repos/amanchella1428/CGT215_AnirudhManchella/FinalProject/Images/backgrounds/winter.png"; //Doesn't work unless I put the whole thing for some reason

    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) {
        cout << "Error: Could not load image from " << imagePath << endl;
        return -1;
    }

    sf::Image image = texture.copyToImage();

    //Adjusting the stregth here (dubug purposes)
    int sharpenStrength = 1;
    auto sharpenKernel = createSharpenKernel(sharpenStrength);
    sf::Image sharpenedImage = applyConvolution(image, sharpenKernel);

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Sharpened Image Display");

    sf::Sprite sprite;
    sf::Texture sharpenedTexture;
    sharpenedTexture.loadFromImage(sharpenedImage);
    sprite.setTexture(sharpenedTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
