#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

// Function to check if the pixel is part of the green screen
bool isGreenScreenPixel(Color pixel) {
    const int tolerance = 50; // Adjust tolerance as necessary
    return (pixel.g > 200 && pixel.r < tolerance && pixel.b < tolerance);
}

int main() {
    // File paths for the background and foreground images
    string background = "images1/backgrounds/winter.png";
    string foreground = "images1/characters/yoda.png";

    // Load background texture
    Texture backgroundTex;
    if (!backgroundTex.loadFromFile(background)) {
        cout << "Couldn't Load Image" << endl;
        exit(1);
    }

    // Load foreground texture
    Texture foregroundTex;
    if (!foregroundTex.loadFromFile(foreground)) {
        cout << "Couldn't Load Image" << endl;
        exit(1);
    }

    // Convert textures to images for pixel-level manipulation
    Image backgroundImage = backgroundTex.copyToImage();
    Image foregroundImage = foregroundTex.copyToImage();

    // Get the size of the images (both images should be the same size)
    Vector2u sz = backgroundImage.getSize();

    // Loop through each pixel of the foreground image
    for (int y = 0; y < sz.y; y++) {
        for (int x = 0; x < sz.x; x++) {
            // Get the pixel color from the foreground image
            Color currentPixel = foregroundImage.getPixel(x, y);

            // If the pixel is part of the green screen, replace it with the corresponding background pixel
            if (isGreenScreenPixel(currentPixel)) {
                Color backgroundPixel = backgroundImage.getPixel(x, y);
                foregroundImage.setPixel(x, y, backgroundPixel);
            }
        }
    }

    // By default, just show the composited image (foreground with background replacing green screen pixels)
    RenderWindow window(VideoMode(1024, 768), "Here's the output");
    Sprite sprite1;
    Texture tex1;
    tex1.loadFromImage(foregroundImage); // Use the updated foreground image with compositing
    sprite1.setTexture(tex1);

    // Main display loop
    window.clear();
    window.draw(sprite1);
    window.display();

    // Keep the window open until closed by the user
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
