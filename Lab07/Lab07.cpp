#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

int main() {
    //MAKING THE WORLD
        // Create our window and world with gravity 0,1
        RenderWindow window(VideoMode(800, 600), "Bounce");
        World world(Vector2f(0, 1));

        // Create the ball
        PhysicsCircle ball;
        ball.setCenter(Vector2f(400, 300));
        ball.setRadius(20);
        world.AddPhysicsBody(ball);

        // Create the floor
        PhysicsRectangle floor;
        floor.setSize(Vector2f(800, 20));
        floor.setCenter(Vector2f(400, 590));
        floor.setStatic(true);
        world.AddPhysicsBody(floor);

        // Left wall
        PhysicsRectangle leftWall;
        leftWall.setSize(Vector2f(20, 600)); 
        leftWall.setCenter(Vector2f(10, 300)); 
        leftWall.setStatic(true); 
        world.AddPhysicsBody(leftWall);

        // Right wall
        PhysicsRectangle rightWall;
        rightWall.setSize(Vector2f(20, 600)); 
        rightWall.setCenter(Vector2f(790, 300)); 
        rightWall.setStatic(true); 
        world.AddPhysicsBody(rightWall);

        // Ceiling
        PhysicsRectangle ceiling;
        ceiling.setSize(Vector2f(800, 20)); 
        ceiling.setCenter(Vector2f(400, 10)); 
        ceiling.setStatic(true); 
        world.AddPhysicsBody(ceiling);

        // Center obstacle
        PhysicsRectangle centerBox;
        centerBox.setSize(Vector2f(100, 100)); 
        centerBox.setCenter(Vector2f(400, 300));
        centerBox.setStatic(true); 
        world.AddPhysicsBody(centerBox);

    //Getting the thudCount
    int thudCount(0);
    floor.onCollision = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount + 1 << endl; //added the + 1 for better readability in case the user, player, or someone doesn't realize programming starts from 0 and not 1.
        thudCount++;
        };
    leftWall.onCollision = floor.onCollision;
    rightWall.onCollision = floor.onCollision;
    ceiling.onCollision = floor.onCollision;
    //It's still detecting the center box for the thuds (around 7 or 8) and I don't know why. Maybe I should add a time delay like I did for the bang?


    //Getting the bangCount
    int bangCount(0);
    Clock clock;
    //The code was treating the spawning as a collision of the main box, so I added a time delay. Was that efficient? probably not, but it worked.
    float delayTime = 2.0f;
    centerBox.onCollision = [&bangCount, &clock, delayTime](PhysicsBodyCollisionResult result) {
        // Start counting bangs after 2 seconds
        if (clock.getElapsedTime().asSeconds() > delayTime) {
            cout << "bang " << bangCount + 1 << endl; //added the + 1 for better readability in case the user, player, or someone doesn't realize programming starts from 0 and not 1.
            bangCount++;
            if (bangCount >= 3) {
                exit(0);
            }
        }
        };

    //I couldn't see it happen, so I slowed down the ball.
    ball.applyImpulse(Vector2f(0.45f, 0.5f));
    //The ball kept slipping through the walls and whatnot, so I added a check to make sure that it actually follows the bounce physics.
    const int MAX_DELTA_TIME_MS = 30; 
    Time lastTime(clock.getElapsedTime());

    while (window.isOpen()) {
        Time currentTime = clock.getElapsedTime();
        Time deltaTime(currentTime - lastTime);
        int deltaTimeMS = deltaTime.asMilliseconds();

        if (deltaTimeMS > MAX_DELTA_TIME_MS) {
            deltaTimeMS = MAX_DELTA_TIME_MS;
        }

        //Here we update the physics like bouncing.
        if (deltaTimeMS > 0) {
            world.UpdatePhysics(deltaTimeMS); 
            lastTime = currentTime;
        }

        //Clear, draw, and display the output with all of the logic done.
        window.clear(Color(0, 0, 0));
        window.draw(ball);
        window.draw(floor);
        window.draw(leftWall);
        window.draw(rightWall);
        window.draw(ceiling);
        window.draw(centerBox);
        window.display();
    }
}
