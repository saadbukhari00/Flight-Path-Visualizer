// In the corresponding `.cpp` file (e.g., `utils.cpp`)
#include "utils.h"


float calculateAngle(const sf::Vector2f& start, const sf::Vector2f& target) 
{
    return atan2(target.y - start.y, target.x - start.x) * 180.f / 3.14159f;
}

void pauseForTakeOff(int seconds) 
{
    this_thread::sleep_for(chrono::seconds(seconds));
    
}
// Function to normalize a vector (make its magnitude = 1)
sf::Vector2f normalize(const sf::Vector2f& vector) 
{
    float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
    return magnitude == 0 ? sf::Vector2f(0, 0) : sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
}

// Update airplane position based on direction and speed
void updateAirplanePosition(Airplane& airplane, float deltaTime) 
{
    if (airplane.isMoving) 
    {
        sf::Vector2f direction = normalize(airplane.targetPosition - airplane.shape.getPosition());
        airplane.shape.move(direction * airplane.speed * deltaTime);

        // Check if the airplane has reached or is very close to the target position
        sf::Vector2f currentPosition = airplane.shape.getPosition();
        if (fabs(currentPosition.x - airplane.targetPosition.x) < 1.f &&
            fabs(currentPosition.y - airplane.targetPosition.y) < 1.f) 
            {
            airplane.shape.setPosition(airplane.targetPosition);
            airplane.isMoving = false;
        }
    }
}
