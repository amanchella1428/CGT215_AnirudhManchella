#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath> //I looked this up and this is the C++ version of math.h from C programming that I learned in CS 159
using namespace std;

/*
With some research, here are a couple of my sources on what I did and how I used them:

1. https://en.cppreference.com/w/cpp/language/function
2. https://stackoverflow.com/questions/52250480/what-are-compound-types
3. https://www.rapidtables.com/convert/color/rgb-to-hsv.html
4.

Notes:
std::vector<T> myVec; // type T used in type std::vector<T> => compound
int* myPtr; // type int used in type int* => compound
double myVal = 10.3; // double is a fundamental type => fundamental
double& myRef = myVal; // type double is used in type double& => compound
*/



//sharpness modifier for me to debug
std::vector<std::vector<int>> createSharpenKernel(int sharpenStrength) {
    return {
        /*{  0, -1,  0},
        { -1,  5, -1},
        {  0, -1,  0}*/


        {  0, -sharpenStrength,  0},
        { -sharpenStrength,  4 + sharpenStrength, -sharpenStrength},
        {  0, -sharpenStrength,  0}
    };
}

/*
    Function: applyConvolution
    Parameters: image, kernel
    Return: result
    Notes: the basic start to using kernels and sharpens the edges because of using kernels
*/
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

/*
    Function: adjustBrightness
    Parameters: image, factor
    Return: result
    Notes: static_cast was used becasue I don't trust implicit conversions to work properly in code
*/
sf::Image adjustBrightness(const sf::Image & image, float factor) {
    int width = image.getSize().x;
    int height = image.getSize().y;

    sf::Image result;
    result.create(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            sf::Color pixel = image.getPixel(x, y);

            //Adjust each color channel based on the factor
            int newRed = std::min(std::max(static_cast<int>(pixel.r * factor), 0), 255); //red
            int newGreen = std::min(std::max(static_cast<int>(pixel.g * factor), 0), 255); //green
            int newBlue = std::min(std::max(static_cast<int>(pixel.b * factor), 0), 255); //blue

            result.setPixel(x, y, sf::Color(newRed, newGreen, newBlue));
        }
    }

    return result;
}

/*
    Function: adjustSaturationAndExposure
    Parameters: image, saturationFactor, exposureFactor
    Return: result
    Notes: RGB are really confusing like I'm learning vectors for the first time in physics
*/
sf::Image adjustSaturationAndExposure(const sf::Image& image, float saturationFactor, float exposureFactor) {
    int width = image.getSize().x;
    int height = image.getSize().y;

    sf::Image result;
    result.create(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            sf::Color pixel = image.getPixel(x, y);

            //Convert RGB to HSV, Hue, Saturation, and Value
            float r = pixel.r / 255.0f;
            float g = pixel.g / 255.0f;
            float b = pixel.b / 255.0f;
            float max = std::max({ r, g, b });
            float min = std::min({ r, g, b });
            float delta = max - min;

            float h = 0.0f; // Hue
            float s = (max == 0.0f) ? 0.0f : (delta / max); // Saturation
            float v = max; // Value (brightness)

            //h is calculated based on the differences between the RGB channels (delta) and which channel is the strongest (max)
            if (delta != 0.0f) {
                if (max == r) {
                    h = 60.0f * (fmod(((g - b) / delta), 6.0f));
                }
                else if (max == g) {
                    h = 60.0f * (((b - r) / delta) + 2.0f);
                }
                else if (max == b) {
                    h = 60.0f * (((r - g) / delta) + 4.0f);
                }
            }
            if (h < 0.0f) {
                h += 360.0f;
            }

            //Adjust saturation and exposure
            //saturation = delta:max (it's a ratio)
            s = std::min(std::max(s * saturationFactor, 0.0f), 1.0f);
            //value = max (This is mostly there so that I don't get confused on what I'm doing.
            v = std::min(std::max(v * exposureFactor, 0.0f), 1.0f);

            //Convert HSV back to RGB
            //the hue determines the section of the color wheel (5 sectors) and based on that, the RGB values are calculated from the brightness (v) and saturation (s)
            //p, q, and t are intermediate values for mixing primary and secondary colors based on the saturation (s0
            int i = static_cast<int>(h / 60.0f) % 6;
            float f = (h / 60.0f) - i;
            float p = v * (1.0f - s);
            float q = v * (1.0f - f * s);
            float t = v * (1.0f - (1.0f - f) * s);

            if (i == 0) { r = v; g = t; b = p; }
            else if (i == 1) { r = q; g = v; b = p; }
            else if (i == 2) { r = p; g = v; b = t; }
            else if (i == 3) { r = p; g = q; b = v; }
            else if (i == 4) { r = t; g = p; b = v; }
            else if (i == 5) { r = v; g = p; b = q; }

            //Set the pixel in the result image
            result.setPixel(x, y, sf::Color(
                std::min(static_cast<int>(r * 255.0f), 255),
                std::min(static_cast<int>(g * 255.0f), 255),
                std::min(static_cast<int>(b * 255.0f), 255)
            ));
        }
    }

    return result;
}

/*
    Function: blurBorders
    Parameters: image, borderThickness
    Return: result
    Notes: the nested for loops will actually drive me nuts
*/
sf::Image blurBorders(const sf::Image& image, int borderThickness) {
    sf::Image result = image; //Create a copy of the image to modify so that we don't mess with the original
    sf::Vector2u size = image.getSize();
    unsigned width = size.x;
    unsigned height = size.y;

    //Create a gaussian blur kernel
    std::vector<std::vector<float>> blurKernel = {
        {1.f / 16, 1.f / 8, 1.f / 16},
        {1.f / 8, 1.f / 4, 1.f / 8},
        {1.f / 16, 1.f / 8, 1.f / 16},
    };


    //The first two for loops iterate through every pixel (height then width)
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            if (x < borderThickness || x >= width - borderThickness ||
                y < borderThickness || y >= height - borderThickness) {
                //Apply blur kernel (i love math)
                float sumR = 0, sumG = 0, sumB = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int pixelX = x + kx;
                        int pixelY = y + ky;

                        //Ensure the kernel stays within bounds so that we're not mixing black (or whatever color we get) from outside the image
                        if (pixelX >= 0 && pixelX < static_cast<int>(width) &&
                            pixelY >= 0 && pixelY < static_cast<int>(height)) {
                            sf::Color pixel = image.getPixel(pixelX, pixelY);
                            float kernelValue = blurKernel[ky + 1][kx + 1];
                            
                            //Taking the RGB values and multiplying them with the corresponding weight from the blur kernel
                            sumR += pixel.r * kernelValue;
                            sumG += pixel.g * kernelValue;
                            sumB += pixel.b * kernelValue;
                        }
                    }
                }
                //Set blurred color to the result image and clamped from the range of 0 - 255
                result.setPixel(x, y, sf::Color(
                    static_cast<sf::Uint8>(sumR),
                    static_cast<sf::Uint8>(sumG),
                    static_cast<sf::Uint8>(sumB)));
            }
        }
    }

    return result;
}


void getChoices(int& choice, int& sharpnessStrength, float& brightnessFactor, float& saturationFactor, float& exposureFactor, int& borderThickness) {
    cout << "Which tool would you like to use?" << endl;
    cout << "1. Sharpness" << endl;
    cout << "2. Brightness" << endl;
    cout << "3. Saturation and Exposure" << endl;
    cout << "4. Blur" << endl;
    cout << "5. All of them" << endl;
    cout << "\nYour choice : ";
    cin >> choice;
    
    if (choice == 1 || choice == 5) {
        cout << "Please enter the sharpness level (standard is 1): " << endl;
        cin >> sharpnessStrength;
    }
    if (choice == 2 || choice == 5) {
        cout << "Please enter the brightness level (standard is 1.5): " << endl;
        cin >> brightnessFactor;
    }
    if (choice == 3 || choice == 5) {
        cout << "Please enter the saturation level (standard is 1.2): " << endl;
        cin >> saturationFactor;
        cout << "Please enter the Exposure level (standard is 1.1): " << endl;
        cin >> exposureFactor;
    }
    if (choice == 4 || choice == 5) {
        cout << "Please enter the border thickness (standard is 50): " << endl;
        cin >> borderThickness;
    }
}


int main() {
    int choice;
    int sharpnessStrength;
    float brightnessFactor;
    float saturationFactor;
    float exposureFactor;
    int borderThickness;

    
    string imagePath = "C:/Users/Owner/source/repos/amanchella1428/CGT215_AnirudhManchella/FinalProject/Images/backgrounds/winter.png";

    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) {
        cout << "Error: Could not load image from " << imagePath << endl;
        return -1;
    }

    sf::Image image = texture.copyToImage();

    //Getting the playerInput
    getChoices(choice, sharpnessStrength, brightnessFactor, saturationFactor, exposureFactor, borderThickness);


    if (choice == 1) {
        auto sharpenKernel = createSharpenKernel(sharpnessStrength);
        sf::Image sharpenedImage = applyConvolution(image, sharpenKernel);
        
        // Display the final image
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Final Image with Blur");
        sf::Texture displayTexture;
        displayTexture.loadFromImage(sharpenedImage);
        sf::Sprite sprite(displayTexture);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    if (choice == 2) {
        sf::Image brightenedImage = adjustBrightness(image, brightnessFactor);
        
        // Display the final image
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Final Image with Blur");
        sf::Texture displayTexture;
        displayTexture.loadFromImage(brightenedImage);
        sf::Sprite sprite(displayTexture);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    if (choice == 3) {
        sf::Image saturatedImage = adjustSaturationAndExposure(image, saturationFactor, exposureFactor);

        // Display the final image
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Final Image with Blur");
        sf::Texture displayTexture;
        displayTexture.loadFromImage(saturatedImage);
        sf::Sprite sprite(displayTexture);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    if (choice == 4) {
        sf::Image blurredImage = blurBorders(image, borderThickness);

        // Display the final image
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Final Image with Blur");
        sf::Texture displayTexture;
        displayTexture.loadFromImage(blurredImage);
        sf::Sprite sprite(displayTexture);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    if (choice == 5) {
        auto sharpenKernel = createSharpenKernel(sharpnessStrength);
        sf::Image sharpenedImage = applyConvolution(image, sharpenKernel);
        sf::Image brightenedImage = adjustBrightness(sharpenedImage, brightnessFactor);
        sf::Image saturatedImage = adjustSaturationAndExposure(brightenedImage, saturationFactor, exposureFactor);
        sf::Image blurredImage = blurBorders(saturatedImage, borderThickness);

        // Display the final image
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Final Image with Blur");
        sf::Texture displayTexture;
        displayTexture.loadFromImage(blurredImage);
        sf::Sprite sprite(displayTexture);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }

    //god, please work

    return 0;
}