#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string.h>

//*************************** 클래스 정의 ***************************
class Stage {
public:
    virtual void onEnter() = 0; //스테이지에 입장
    virtual void update(sf::RenderWindow& window, float dt) = 0; //스테이지 화면 업데이트
    virtual void handleinput(sf::RenderWindow& window, sf::Event& event) = 0; //사용자 입력
    virtual void onExit() = 0; //스테이지 나가기
};

class Menu : public Stage {
private:
    sf::Font font;
    sf::Text text;
    sf::RectangleShape rect;

public:
    Menu();
    void onEnter() override;
    void update(sf::RenderWindow& window, float dt) override;
    void handleinput(sf::RenderWindow& window, sf::Event& event) override;
    void onExit() override;
};

class Level1 : public Stage {
private:
    const float gridSize = 40.f; //격자 크기
    const float movementSpeed = 400.f;
    const unsigned window_width = 1280;
    const unsigned window_height = 720;
    int money;

    sf::Font font;
    sf::Text moneyText;
    sf::Text moneyValue;
    sf::Vector2f center{ 640,360 }; //화면 중앙 좌표
    sf::Vector2f velocity;
    sf::RectangleShape save;
    sf::RectangleShape load;
    sf::RectangleShape maru; //마루
    sf::RectangleShape player;
    sf::RectangleShape room; //방 배경 객체
    sf::Texture texturePlayer; //텍스처 객체
    sf::Texture textureMaru; //마루 객체
    sf::Texture textureSave; //save 버튼 텍스쳐
    sf::Texture textureLoad; //load 버튼 텍스쳐
    sf::Texture textureRoom; //room 텍스쳐
    std::vector<sf::RectangleShape> lines;

public:
    Level1();
    void onEnter() override;
    void update(sf::RenderWindow& window, float dt) override;
    void handleinput(sf::RenderWindow& window, sf::Event& event) override;
    void onExit() override;
};

class Game {
public:
    sf::RenderWindow window;
    std::map<std::string, std::unique_ptr<Stage>> stages;
    Stage* currentStage;

    Game();
    void changeStage(const std::string& stageName);
    void run();
};

// ************************* onEnter(), onExit()함수 *****************************
void Menu::onEnter() {
    std::cout << "Welcome to the menu!" << std::endl;
}

void Level1::onEnter() {
    std::cout << "Welcome to the game!" << std::endl;
}

void Menu::onExit() {
    std::cout << "You are leaving the menu." << std::endl;

}

void Level1::onExit() {
    std::cout << "You are leaving the game." << std::endl;
}


// ************************** update()함수 **************************
void Menu::update(sf::RenderWindow& window, float dt) {
    window.clear(sf::Color::Blue);
    window.draw(rect);
    window.draw(text);
}

void Level1::update(sf::RenderWindow& window, float dt) {
    window.clear(sf::Color::Black);

    for (auto& line : lines) {
        sf::FloatRect playerBounds = player.getGlobalBounds();
        sf::FloatRect lineBounds = line.getGlobalBounds();
        sf::FloatRect nextPos = playerBounds;
        nextPos.left += velocity.x * dt;
        nextPos.top += velocity.y * dt;

        if (lineBounds.intersects(nextPos)) {
            velocity.x = 0.f;
            velocity.y = 0.f;
        }
    }

    player.move(velocity * dt);

    // Player가 화면 바깥으로 나가는 것을 막는 코드
    if (player.getPosition().x < 0) player.setPosition(0, player.getPosition().y);
    if (player.getPosition().y < 0) player.setPosition(player.getPosition().x, 0);
    if (player.getPosition().x > window_width - gridSize) player.setPosition(window_width - gridSize, player.getPosition().y);
    if (player.getPosition().y > window_height - gridSize) player.setPosition(player.getPosition().x, window_height - gridSize);

    for (auto& line : lines) {
        window.draw(line);
    }
    window.draw(moneyText);
    window.draw(moneyValue);
    window.draw(save);
    window.draw(load);
    window.draw(room);
    window.draw(maru);
    window.draw(player);
}

// ************************** handleinput **************************
void Menu::handleinput(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect bounds = rect.getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                std::cout << "Changing to game" << std::endl;
                onExit();
            }
        }
    }
}

void Level1::handleinput(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y = -movementSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y = movementSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -movementSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = movementSpeed;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
        money += 100;
        moneyValue.setString(std::to_string(money));

    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
        money += 1;
        moneyValue.setString(std::to_string(money));
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
        money -= 100;
        moneyValue.setString(std::to_string(money));
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
        money -= 1;
        moneyValue.setString(std::to_string(money));
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W ||
            event.key.code == sf::Keyboard::S) {
            velocity.y = 0.f;
        }
        if (event.key.code == sf::Keyboard::A ||
            event.key.code == sf::Keyboard::D) {
            velocity.x = 0.f;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect bounds = save.getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                std::cout << "save game data" << std::endl;
                std::ofstream file("game_save.txt");
                if (file.is_open()) {
                    file << money << "\n";
                    file.close();
                }
                else {
                    std::cout << "Unable to open file";
                }
            }
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect bounds = load.getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                std::cout << "load game data" << std::endl;
                std::ifstream file("game_save.txt");
                if (file.is_open()) {
                    file >> money;
                    moneyValue.setString(std::to_string(money));
                    file.close();
                }
                else {
                    std::cout << "Unable to open file";
                }
            }
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
        std::cout << "Changing to menu" << std::endl;
        onExit();
    }
}

// ************************* 생성자 함수 ***********************
Menu::Menu() {
    if (!font.loadFromFile(".\\lib\\Maplestory Light.ttf")) { //폰트 업로드
        std::cout << "Could not load font" << std::endl;
    }

    text.setOrigin(135, 25);
    text.setFont(font); //폰트 적용
    text.setString("BaeksuGame"); //출력 문자열
    text.setCharacterSize(40); //문자 크기
    text.setFillColor(sf::Color::Red); //문자 색상
    text.setStyle(sf::Text::Bold); //폰트 스타일
    text.setPosition(640, 360); //글자 위치

    rect.setOrigin(150, 20);
    rect.setSize(sf::Vector2f(300, 40));
    rect.setPosition(sf::Vector2f(text.getPosition()));
    rect.setFillColor(sf::Color::Green);
}

Level1::Level1() {
    //플레이어
    if (!texturePlayer.loadFromFile(".\\lib\\frog.png")) {/*error...*/ } //텍스처 불러오기
    player.setTextureRect(sf::IntRect(0, 0, gridSize, gridSize)); //텍스쳐 모양 선언
    player.setSize(sf::Vector2f(gridSize, gridSize)); //플레이어 크기
    player.setTexture(&texturePlayer);
    player.setPosition(240, 240);

    //money 텍스트
    if (!font.loadFromFile(".\\lib\\Maplestory Light.ttf")) { //폰트 업로드
        std::cout << "Could not load font" << std::endl;
    }
    moneyText.setOrigin(0, 25);
    moneyText.setFont(font); //폰트 적용
    moneyText.setString("Money: "); //출력 문자열
    moneyText.setCharacterSize(20); //문자 크기
    moneyText.setStyle(sf::Text::Bold); //폰트 스타일
    moneyText.setPosition(1020, 40); //글자 위치

    //money 값
    money = 0;
    moneyValue.setOrigin(0, 25);
    moneyValue.setFont(font); //폰트 적용
    moneyValue.setString(std::to_string(money)); //출력 문자열
    moneyValue.setCharacterSize(20); //문자 크기
    moneyValue.setStyle(sf::Text::Bold); //폰트 스타일
    moneyValue.setPosition(1100, 40); //글자 위치

    //save 버튼
    if (!textureSave.loadFromFile(".\\lib\\save.png")) {/*error...*/ } //텍스처 불러오기
    save.setTextureRect(sf::IntRect(0, 0, 128, 72));
    save.setSize(sf::Vector2f(128, 72));
    save.setTexture(&textureSave);
    save.setPosition(1082, 546);

    //load 버튼
    if (!textureLoad.loadFromFile(".\\lib\\load.png")) {/*error...*/ } //텍스처 불러오기
    load.setTextureRect(sf::IntRect(0, 0, 128, 72));
    load.setSize(sf::Vector2f(128, 72));
    load.setTexture(&textureLoad);
    load.setPosition(1082, 628);


    //UI 가로선
    sf::RectangleShape line; //가로선
    line.setFillColor(sf::Color::White);
    line.setSize(sf::Vector2f(window_width, 10.f));
    line.setPosition(0.f, gridSize * 13.f);
    lines.push_back(line);

    //UI 세로선
    sf::RectangleShape line1; //세로선
    line1.setFillColor(sf::Color::White);
    line1.setSize(sf::Vector2f(10.f, window_height));
    line1.setPosition(gridSize * 25.f, 0.f);
    lines.push_back(line1);

    //방 배경
    if (!textureRoom.loadFromFile(".\\lib\\room.png")) {/*error...*/ } //텍스처 불러오기
    room.setTextureRect(sf::IntRect(0, 0, 1000, 520));
    room.setSize(sf::Vector2f(1000, 520));
    room.setTexture(&textureRoom);

    //마루 운다
    if (!textureMaru.loadFromFile(".\\lib\\maru.png")) {/*error...*/ } //텍스처 불러오기
    maru.setTextureRect(sf::IntRect(0, 0, 200, 200));
    maru.setSize(sf::Vector2f(200, 200));
    maru.setTexture(&textureMaru);
    maru.setPosition(sf::Vector2f(20, 50));
}

Game::Game() {
    
        window.create(sf::VideoMode(1280, 720), "Baeksu Simulator");
        sf::Image icon; //아이콘 객체
        if (!icon.loadFromFile(".\\lib\\ral.png"))  // 아이콘 경로 설정
        {  /* 이미지 파일 로드 실패 시 처리*/
        }
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()); //아이콘 적용
        stages["menu"] = std::make_unique<Menu>();
        stages["level1"] = std::make_unique<Level1>();
        currentStage = stages["menu"].get();
        currentStage->onEnter();
    
}

// ********************** Game() 메소드  ************************
void Game::changeStage(const std::string& stageName) {
    currentStage->onExit();
    currentStage = stages[stageName].get();
    currentStage->onEnter();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            currentStage->handleinput(window, event);

            if (event.type == sf::Event::KeyPressed) {
                // 현재 stage가 'menu'인 경우에만 'O' 키를 처리
                if (currentStage == stages["menu"].get() && event.key.code == sf::Keyboard::O) {
                    currentStage->onExit();
                    changeStage("level1");
                }
                else if (event.key.code == sf::Keyboard::P) {
                    currentStage->onExit();
                    changeStage("menu");
                }
            }
        }

        float dt = clock.restart().asSeconds();
        currentStage->update(window, dt);
        window.display();
    }
}


int main() {
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.run();
    return 0;

}
