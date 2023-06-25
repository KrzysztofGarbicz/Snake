#include "game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Snake Game"), direction(20, 0), score(0), gameOver(false) {
    srand(time(NULL));

    // £adowanie muzyki
    if (!music.openFromFile("sounds/music.wav")) {
        std::cerr << "B³¹d ³adowania muzyki." << std::endl;
    }
    music.setLoop(true); // Ustawienie zapêtlenia muzyki

    // Ustawianie jedzenia
    food.setSize(sf::Vector2f(20, 20));
    food.setFillColor(sf::Color::Red);
    respawnFood();

    // Ustawianie wê¿a
    sf::RectangleShape head;
    head.setSize(sf::Vector2f(20, 20));
    head.setFillColor(sf::Color::Green);
    head.setPosition((window.getSize().x) / 2, (window.getSize().y) / 2);
    snake.push_front(head);

    // Ustawianie tekstu wyniku
    font.loadFromFile("arial.ttf");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    // Ustawianie tekstu "Game Over"
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 25);
    gameOverText.setString("Game Over");

    // Ustawianie tekstu "Naciœnij Enter, aby spróbowaæ ponownie"
    tryAgainText.setFont(font);
    tryAgainText.setCharacterSize(20);
    tryAgainText.setFillColor(sf::Color::Red);
    tryAgainText.setPosition(window.getSize().x / 2 - 120, window.getSize().y / 2 + 30);
    tryAgainText.setString("Press Enter to try again");

    // Ustawianie siatki
    sf::RectangleShape line;
    line.setFillColor(sf::Color(64, 64, 64)); // szary kolor
    for (int i = 0; i <= window.getSize().x; i += 20) {
        line.setSize(sf::Vector2f(1, window.getSize().y - 10));
        line.setPosition(i, 0);
        gridLines.push_back(line);
    }
    for (int i = 0; i <= window.getSize().y; i += 20) {
        line.setSize(sf::Vector2f(window.getSize().x - 10, 1));
        line.setPosition(0, i);
        gridLines.push_back(line);
    }

    // Ustawianie super jab³ka
    superApple.setSize(sf::Vector2f(20, 20));
    superApple.setPosition(-100, -100); // Poza ekranem na pocz¹tku

    // £adowanie dŸwiêków
    if (!foodBuffer.loadFromFile("sounds/eat.wav")) {
        // Obs³uga b³êdu ³adowania dŸwiêku jab³ka
        std::cerr << "B³¹d ³adowania dŸwiêku jedzenia." << std::endl;
    }
    if (!gameOverBuffer.loadFromFile("sounds/game_over.wav")) {
        // Obs³uga b³êdu ³adowania dŸwiêku koñca gry
        std::cerr << "B³¹d ³adowania dŸwiêku koñca gry." << std::endl;
    }
    // £adowanie dŸwiêku super jab³ka
    if (!superAppleBuffer.loadFromFile("sounds/super_apple.wav")) {
        std::cerr << "B³¹d ³adowania dŸwiêku super jab³ka." << std::endl;
    }
}

void Game::run() {
    music.play(); // Rozpoczêcie odtwarzania muzyki
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                gameOver = false;
                resetGame();
            }
        }
        else {
            getInput();
            update();
            checkCollision();
        }
        // Sprawdzanie czasu trwania super jab³ka
        if (superAppleVisible && superAppleTimer.getElapsedTime().asSeconds() >= 10) {
            superAppleVisible = false;
            superApple.setPosition(-100, -100); // Przeniesienie poza ekran
        }

        // Animacja zmiany koloru super jab³ka
        if (superAppleVisible) {
            float time = superAppleTimer.getElapsedTime().asSeconds();
            int alpha = 255 * (1 - std::cos(time * 3.14)); // Interpolacja liniowa z wartoœci 0 do 255
            sf::Color color = sf::Color(255, 0, 255, alpha); // Fioletowy z zmieniaj¹cym siê alpha
            superApple.setFillColor(color);
        }

        render();
        sf::sleep(sf::milliseconds(100));
    }
    music.stop(); // Zatrzymanie muzyki po zakoñczeniu gry
}

void Game::getInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction.y != 20) direction = { 0, -20 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction.y != -20) direction = { 0, 20 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction.x != 20) direction = { -20, 0 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction.x != -20) direction = { 20, 0 };
}

void Game::update() {
    sf::RectangleShape head = snake.front();
    head.move(direction);
    snake.push_front(head);

    if (head.getGlobalBounds().intersects(food.getGlobalBounds())) {
        // Aktualizacja punktacji
        score++;

        // Sprawdzanie czy ma siê pojawiæ super jab³ko
        int chance = rand() % 10; // Losowanie liczby od 0 do 9
        if (chance == 0) { // 10% szansy na pojawienie siê super jab³ka
            respawnSuperApple();
        }

        playFoodSound();
        respawnFood();
    }
    else {
        snake.pop_back();
    }
    // Sprawdzanie kolizji ze super jab³kiem
    if (superAppleVisible && head.getGlobalBounds().intersects(superApple.getGlobalBounds())) {
        // Zebranie super jab³ka
        score += 5;
        superAppleVisible = false;
        superApple.setPosition(-100, -100); // Przeniesienie poza ekran

        // Odtwarzanie dŸwiêku super jab³ka
        playSuperAppleSound();
    }

    scoreText.setString("Score: " + std::to_string(score));
}
//Pojawienie siê nowego jab³ka
void Game::respawnFood() {
    int x = (rand() % ((window.getSize().x) / 20)) * 20;
    int y = (rand() % ((window.getSize().y) / 20)) * 20;
    food.setPosition(x, y);
}

void Game::respawnSuperApple() {
    // Przywracanie super jab³ka na planszê

    // Ustawianie pozycji super jab³ka
    int x = (rand() % ((window.getSize().x) / 20)) * 20;
    int y = (rand() % ((window.getSize().y) / 20)) * 20;
    superApple.setPosition(x, y);

    // Resetowanie timera
    superAppleTimer.restart();

    // Ustawianie widocznoœci super jab³ka
    superAppleVisible = true;
}
//Sprawdzanie kolizji
void Game::checkCollision() {
    sf::RectangleShape& head = snake.front();
    if (head.getPosition().x < 5 || head.getPosition().y < 5 || head.getPosition().x >= window.getSize().x - 25 || head.getPosition().y >= window.getSize().y - 25) {
        playGameOverSound();
        gameOver = true;
    }

    auto it = snake.begin();
    it++;
    for (; it != snake.end(); it++) {
        if (it->getGlobalBounds().intersects(head.getGlobalBounds())) {
            playGameOverSound();
            gameOver = true;
        }
    }
}

void Game::render() {
    window.clear();
    for (auto& line : gridLines) {
        window.draw(line);
    }
    for (auto& part : snake) {
        window.draw(part);
    }
    window.draw(food);

    window.draw(scoreText);

    if (gameOver) {
        window.draw(gameOverText);
        window.draw(tryAgainText);
    }

    if (superAppleVisible) {
        window.draw(superApple);
    }

    window.display();
}

void Game::resetGame() {
    snake.clear();
    sf::RectangleShape head;
    head.setSize(sf::Vector2f(20, 20));
    head.setFillColor(sf::Color::Green);
    head.setPosition((window.getSize().x) / 2, (window.getSize().y) / 2);
    snake.push_front(head);

    direction = { 20, 0 };
    score = 0;
    respawnFood();
}

void Game::playFoodSound() {
    sound.setBuffer(foodBuffer);
    sound.play();
}

void Game::playGameOverSound() {
    sound.setBuffer(gameOverBuffer);
    sound.play();
}

void Game::playSuperAppleSound() {
    superAppleSound.setBuffer(superAppleBuffer);
    superAppleSound.play();
}
