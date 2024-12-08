// In the header file (e.g., `utils.h`)
#ifndef UTILS_H
#define UTILS_H

#include "main.h"
// Function declarations
float calculateAngle(const sf::Vector2<float>&, const sf::Vector2<float>&);
sf::Vector2<float> normalize(const sf::Vector2<float>&);
void updateAirplanePosition(Airplane&, float);
void pauseForTakeOff(int seconds);
#endif // UTILS_H
