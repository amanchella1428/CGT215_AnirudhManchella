#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

/*Function to check if the colour of the background is green or not.
For some reason, it's not (0,255,0), so I had a threshold option, and that worked.*/
bool isGreenScreenPixel (Color pixel) {
    const int tolerance = 50; //50 seems like a good amount. I used the adobe colour picker to see what the shades looked like.
    return (pixel.g > 200 && pixel.r < tolerance && pixel.b < tolerance);
}

int main() {
    //This file path should work for anyone who downloads the images and puts them in the proper directory.
    string background = "images1/backgrounds/winter.png";
    string foreground = "images1/characters/yoda.png";

    //Thsi is to get the background texture. If it can't get it, print a debugging statement.
    Texture backgroundTex;
    if (!backgroundTex.loadFromFile(background)) {
        cout << "Couldn't Load Image" << endl;
        exit(1);
    }

    //Same thing, but the foreground picture. I can technically add another one, but I don't wanna.
    Texture foregroundTex;
    if (!foregroundTex.loadFromFile(foreground)) {
        cout << "Couldn't Load Image" << endl;
        exit(1);
    }

    //code from the lab document, I don't really understand this part, but it works.
    Image backgroundImage = backgroundTex.copyToImage();
    Image foregroundImage = foregroundTex.copyToImage();

    Vector2u sz = backgroundImage.getSize();

    //Going through each of the pixels to see if they need to be changed or not.
    for (int y = 0; y < sz.y; y++) {
        for (int x = 0; x < sz.x; x++) {
            Color currentPixel = foregroundImage.getPixel(x, y);
            //replacing the green pixel with a non-green pixel.
            if (isGreenScreenPixel(currentPixel)) {
                Color backgroundPixel = backgroundImage.getPixel(x, y);
                foregroundImage.setPixel(x, y, backgroundPixel);
            }
        }
    }

    //The output of the image.
    RenderWindow window(VideoMode(1024, 768), "Here's the output");
    Sprite sprite1;
    Texture tex1;
    tex1.loadFromImage(foregroundImage); //this image has been updated with the greenscreen and all.
    sprite1.setTexture(tex1);

    //The main display.
    window.clear();
    window.draw(sprite1);
    window.display();

    //This code wasn't really working for me because I couldn't close the window if I used it, so I did something else.
    /*RenderWindow window(VideoMode(1024, 768), "Here's the output");
    Sprite sprite1;
    Texture tex1;
    tex1.loadFromImage(foregroundImage);
    sprite1.setTexture(tex1);
    window.clear();
    window.draw(sprite1);
    window.display();
    while (true);*/

    //Keep the window open until the user closes it. I had to look this part up on stackoverflow.
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
