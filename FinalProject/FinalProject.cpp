#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

//A custom clamp function because I don't know how to update to c++ 18 or higher
template <typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

//Using Kernals to get the image itself
Image applyConvolution(const Image& image, const vector<vector<int>>& kernel) {
    Vector2u sz = image.getSize();
    unsigned int width = sz.x, height = sz.y;

    int kernelSize = kernel.size();
    int kernelRadius = kernelSize / 2;

    //Creating a new image for the final result.
    Image result;
    result.create(width, height);

    //Iterating over each pixel in a nested for loop for rows and columns
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int newValue = 0;

            //Applying the kernals (I think my math is correct since I learned about this in MA 265 and 266)
            for (int ky = -kernelRadius; ky <= kernelRadius; ++ky) {
                for (int kx = -kernelRadius; kx <= kernelRadius; ++kx) {
                    int pixelX = x + kx;
                    int pixelY = y + ky;

                    //Checking the boundaries and applying color
                    if (pixelX >= 0 && pixelX < static_cast<int>(width) && pixelY >= 0 && pixelY < static_cast<int>(height)) {
                        Color pixelColor = image.getPixel(pixelX, pixelY);
                        int gray = pixelColor.r; //Assuming grayscale (RGB stuff)
                        newValue += gray * kernel[ky + kernelRadius][kx + kernelRadius];
                    }
                }
            }

            //Calling the custom clamp function
            newValue = clamp(newValue, 0, 255);
            result.setPixel(x, y, Color(newValue, newValue, newValue)); //eliminating the greyscale
        }
    }

    return result;
}

int main() {
    string imagePath = "C:/Users/Owner/source/repos/amanchella1428/CGT215_AnirudhManchella/FinalProject/Images/backgrounds/winter.png"; //fails to load without absolute path for some reason

    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) {
        cout << "Error: Could not load image from " << imagePath << endl;
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Scaled Image Display"); //Fixed window size to compare images, we can make this custom later with variables

    sf::Sprite sprite;
    sprite.setTexture(texture);

    //scaling the sprite to fit into the window (stretches the image if we expand it out)
    sf::Vector2u imageSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / imageSize.x;
    float scaleY = static_cast<float>(windowSize.y) / imageSize.y;
    float scale = std::min(scaleX, scaleY);
    sprite.setScale(scale, scale);

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

//This was really hard please go easy on my grade