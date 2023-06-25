#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <deque>
#include <vector>
#include <ctime>
#include <string>

class Game {
private:
    sf::RenderWindow window;
    sf::RectangleShape food;
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text tryAgainText;
    sf::RectangleShape border;
    std::vector<sf::RectangleShape> gridLines;
    std::deque<sf::RectangleShape> snake;
    sf::Vector2f direction;
    int score;
    bool gameOver;
    sf::SoundBuffer foodBuffer;
    sf::SoundBuffer gameOverBuffer;
    sf::Sound sound;
    sf::Music music;

    sf::RectangleShape superApple;
    sf::Clock superAppleTimer;
    bool superAppleVisible;
    sf::SoundBuffer superAppleBuffer;
    sf::Sound superAppleSound;
    bool superAppleSoundPlayed;

public:
    Game();

    void run();

private:
    void getInput();
    void update();
    void checkCollision();
    void render();
    void resetGame();
    void playFoodSound();
    void playGameOverSound();
    void playSuperAppleSound();
    void respawnFood();
    void respawnSuperApple();
};

#endif  