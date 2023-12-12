#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string.h>

//*************************** 클래스 정의 ***************************
class Ral {
public:
    int counter =0;
    int money; //소지금
    int clean; //청결도
    int tired; //피로도
    int hunger; //배고픔 
    int willing; //의지
    int texPos; //텍스쳐 위치
    sf::Vector2f size = { 30, 50 };
    sf::Texture front;
    sf::Texture back;
    sf::Texture left;
    sf::Texture right;
    sf::RectangleShape player; //몸


    Ral() {
        money = 10000;
        clean = 30;
        tired = 0;
        hunger = 50;
        willing = 20;
        if (!front.loadFromFile(".\\lib\\player_front.png")) {/*error...*/ }
        if (!back.loadFromFile(".\\lib\\player_back.png")) {/*error...*/ }
        if (!left.loadFromFile(".\\lib\\player_left.png")) {/*error...*/ }
        if (!right.loadFromFile(".\\lib\\player_right.png")) {/*error...*/ }
        player.setTextureRect(sf::IntRect(0, 0, size.x, size.y)); //텍스쳐 모양 선언
        player.setSize(sf::Vector2f(60, 100)); //플레이어 크기
        player.setTexture(&front);
        player.setPosition(sf::Vector2f(500, 250));
    }

    int getMoney() { return money; }
    int getClean() { return clean; }
    int getHunger() { return hunger; }
    int getWilling() { return willing; }
    sf::RectangleShape getPlayer() { return player; }
    sf::Vector2f getSize() { return size; }
};

class Stage {
public:
    virtual void onEnter() = 0; //스테이지에 입장
    virtual void update(sf::RenderWindow& window, float dt) = 0; //스테이지 화면 업데이트
    virtual void handleinput(sf::RenderWindow& window, sf::Event& event) = 0; //사용자 입력
    virtual void onExit() = 0; //스테이지 나가기
};

class Menu : public Stage  {
private:
    int counter = 0;
    bool isControlOpen;
    sf::Vector2f velocity;
    sf::Texture titleTex;
    sf::Texture backgroudTex;
    sf::Texture playTex;
    sf::Texture controlTex;
    sf::Texture controlWindowTex;
    sf::RectangleShape closeButton;
    sf::RectangleShape controlWindow;
    sf::RectangleShape play;
    sf::RectangleShape control;
    sf::RectangleShape title;
    sf::RectangleShape background;

public:
    Menu();
    void onEnter() override;
    void update(sf::RenderWindow& window, float dt) override;
    void handleinput(sf::RenderWindow& window, sf::Event& event) override;
    void onExit() override;
};

class Level1 : public Stage {
protected:
    const float gridSize = 40.f; //격자 크기
    const float movementSpeed = 300.f;
    const unsigned window_width = 1280;
    const unsigned window_height = 720;

    Ral p1;
    sf::Text status;
    sf::Vector2f textPos = { 1035, 10 };
    sf::Font font;
    sf::Vector2f center{ 640,360 }; //화면 중앙 좌표
    sf::Vector2f velocity;
    sf::RectangleShape save;
    sf::RectangleShape load;

    sf::RectangleShape object1; //벽
    sf::RectangleShape object2; //책장
    sf::RectangleShape object3; //컴퓨터
    sf::RectangleShape object4; //쓰레기
    sf::RectangleShape object5; //침대
    sf::RectangleShape object6; //폰

    sf::RectangleShape room; //방 배경 객체
    sf::Texture textureSave; //save 버튼 텍스쳐
    sf::Texture textureLoad; //load 버튼 텍스쳐
    sf::Texture textureRoom; //room 텍스쳐
    std::vector<sf::RectangleShape> lines;

public:
    Level1();
    void onEnter() override;
    void update(sf::RenderWindow& window, float dt) override;
    void handleinput(sf::RenderWindow& window, sf::Event& event) override;
    void printStatus(sf::RenderWindow& window, Ral player);
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
    window.clear(sf::Color::Black);
    window.draw(background);
    window.draw(title);
    window.draw(control);
    window.draw(play);

    title.move(velocity * dt);
    if (title.getPosition().y < 330) {
        velocity.y = -velocity.y;
        title.setPosition(640, 331);
    }
    else if (title.getPosition().y > 360) {
        velocity.y = -velocity.y;
        title.setPosition(640, 359); 
    }
    if (isControlOpen) {
        window.draw(controlWindow);
    }
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect boundsP = play.getGlobalBounds();
    sf::FloatRect boundsC = control.getGlobalBounds();

    if (boundsP.contains(mousePos.x, mousePos.y)) {
        play.setFillColor(sf::Color(200, 200, 200));        }
    else play.setFillColor(sf::Color::White);
    if (boundsC.contains(mousePos.x, mousePos.y)) {
        control.setFillColor(sf::Color(200, 200, 200));     }
    else control.setFillColor(sf::Color::White);

    if (isControlOpen) {
        counter += 1;
        controlWindow.setTextureRect(sf::IntRect(0, counter / 1200 % 4 * 90, 160, 90)); //텍스쳐 모양 선언
    }
}

void Level1::update(sf::RenderWindow& window, float dt) {
    window.clear(sf::Color::Black);

    for (auto& line : lines) {
        sf::FloatRect playerBounds = p1.player.getGlobalBounds();
        sf::FloatRect lineBounds = line.getGlobalBounds();
        sf::FloatRect nextPos = playerBounds;
        nextPos.left += velocity.x * dt;
        nextPos.top += velocity.y * dt;

        if (lineBounds.intersects(nextPos)) {
            velocity.x = 0.f;
            velocity.y = 0.f;
        }
    }

    p1.player.move(velocity * dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        p1.player.setTexture(&p1.front);
        p1.texPos = p1.counter++ /30 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        p1.player.setTexture(&p1.back);
        p1.texPos = p1.counter++ / 30 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        p1.player.setTexture(&p1.left);
        p1.texPos = p1.counter++ / 30 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        p1.player.setTexture(&p1.right);
        p1.texPos = p1.counter++ / 30 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }

    // Player가 화면 바깥으로 나가는 것을 막는 코드
    if (p1.player.getPosition().x < 0) p1.player.setPosition(0, p1.player.getPosition().y);
    if (p1.player.getPosition().y < 0) p1.player.setPosition(p1.player.getPosition().x, 0);
    if (p1.player.getPosition().x > window_width - gridSize) p1.player.setPosition(window_width - gridSize, p1.player.getPosition().y);
    if (p1.player.getPosition().y > window_height - gridSize) p1.player.setPosition(p1.player.getPosition().x, window_height - gridSize);

    for (auto& line : lines) {
        window.draw(line);
    }
    printStatus(window, p1);
    window.draw(room);
    window.draw(save);
    window.draw(load);
    /*window.draw(object1);
    window.draw(object2);
    window.draw(object3);
    window.draw(object4);
    window.draw(object5);
    window.draw(object6);*/
    window.draw(p1.player);
}

// ************************** handleinput **************************
void Menu::handleinput(sf::RenderWindow& window, sf::Event& event) { 
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect bounds = play.getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                std::cout << "Changing to game" << std::endl;
                onExit();
            }
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect bounds = control.getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                std::cout << "open control window" << std::endl;
                isControlOpen = true;
            }
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect bounds = closeButton.getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                std::cout << "close control window" << std::endl;
                isControlOpen = false;
                counter = 0;
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
        p1.money += 100;

    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
        p1.money += 1;
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
        p1.money -= 100;
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
        p1.money -= 1;
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

    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W ||
            event.key.code == sf::Keyboard::S ||
            event.key.code == sf::Keyboard::A ||
            event.key.code == sf::Keyboard::D ) {
            p1.player.setTextureRect(sf::IntRect(0, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
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
                    file << "money: " << p1.money << "\n";
                    file << "tired: " << p1.tired << "\n";
                    file << "clean: " << p1.clean << "\n";
                    file << "hunger: " << p1.hunger << "\n";
                    file << "willing: " << p1.willing << "\n";
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
                    std::ifstream file("game_save.txt");
                    std::string line;
                    while (std::getline(file, line)) {
                        if (line.find("money: ") != std::string::npos) {
                            p1.money = std::stoi(line.substr(line.find("money: ") + 7));
                        }
                        else if (line.find("tired: ") != std::string::npos) {
                            p1.tired = std::stoi(line.substr(line.find("tired: ") + 7));
                        }
                        else if (line.find("clean: ") != std::string::npos) {
                            p1.clean = std::stoi(line.substr(line.find("clean: ") + 7));
                        }
                        else if (line.find("hunger: ") != std::string::npos) {
                            p1.hunger = std::stoi(line.substr(line.find("hunger: ") + 8));
                        }
                        else if (line.find("willing: ") != std::string::npos) {
                            p1.willing = std::stoi(line.substr(line.find("willing: ") + 9));
                        }
                    }
                    file.close();
                }
                else {
                    std::cout << "Unable to open file";
                }
            }
        }
    }
}

// ************************* 생성자 함수 ***********************

Menu::Menu() {
    isControlOpen = false;
    velocity = { 0, -20 };

    if (!backgroudTex.loadFromFile(".\\lib\\background.png")) {/*error...*/ } //텍스처 불러오기
    background.setTextureRect(sf::IntRect(0, 0, 1280, 720));
    background.setTexture(&backgroudTex);
    background.setSize(sf::Vector2f(1280, 720));

    if (!titleTex.loadFromFile(".\\lib\\title.png")) {/*error...*/ } //텍스처 불러오기
    title.setTextureRect(sf::IntRect(0, 0, 650, 650));
    title.setTexture(&titleTex);
    title.setOrigin(325, 325);
    title.setSize(sf::Vector2f(650, 650));
    title.setPosition(640, 350);

    if (!playTex.loadFromFile(".\\lib\\play.png")) {/*error...*/ } //텍스처 불러오기
    play.setTextureRect(sf::IntRect(0, 0, 50, 30));
    play.setTexture(&playTex);
    play.setOrigin(25, 15);
    play.setSize(sf::Vector2f(150, 90));
    play.setPosition(490, 600);

    if (!controlTex.loadFromFile(".\\lib\\control.png")) {/*error...*/ } //텍스처 불러오기
    control.setTextureRect(sf::IntRect(0, 0, 50, 30));
    control.setTexture(&controlTex);
    control.setOrigin(25, 15);
    control.setSize(sf::Vector2f(150, 90));
    control.setPosition(690, 600);

    if (!controlWindowTex.loadFromFile(".\\lib\\controlWindow.png")) {/*error...*/ } //텍스처 불러오기
    controlWindow.setTextureRect(sf::IntRect(0, 0, 160, 90));
    controlWindow.setTexture(&controlWindowTex);
    controlWindow.setOrigin(320, 180);
    controlWindow.setSize(sf::Vector2f(640, 360));
    controlWindow.setPosition(640, 360);

    closeButton.setSize(sf::Vector2f(10, 10));
    closeButton.setPosition(950, 180);
}

Level1::Level1() {
    if (!font.loadFromFile(".\\lib\\Maplestory Light.ttf")) { //폰트 업로드
        std::cout << "Could not load font" << std::endl;
    }

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

    //이동불가 영역들
    object1.setFillColor(sf::Color::Red);
    object1.setSize(sf::Vector2f(1000, 75));
    object1.setPosition(0, 0);
    lines.push_back(object1);

    object2.setFillColor(sf::Color::Red);
    object2.setSize(sf::Vector2f(261, 85));
    object2.setPosition(233, 30);
    lines.push_back(object2);

    object3.setFillColor(sf::Color::Red);
    object3.setSize(sf::Vector2f(280, 110));
    object3.setPosition(590, 410);
    lines.push_back(object3);

    object4.setFillColor(sf::Color::Red);
    object4.setSize(sf::Vector2f(175, 105));
    object4.setPosition(0, 415);
    lines.push_back(object4);

    object5.setFillColor(sf::Color::Red);
    object5.setSize(sf::Vector2f(250, 50));
    object5.setPosition(747, 150);
    lines.push_back(object5);

    object6.setFillColor(sf::Color::Red);
    object6.setSize(sf::Vector2f(45, 70));
    object6.setPosition(955, 200);
    lines.push_back(object6);


    //방 배경
    if (!textureRoom.loadFromFile(".\\lib\\room.png")) {/*error...*/ } //텍스처 불러오기
    room.setTextureRect(sf::IntRect(0, 0, 1000, 560));
    room.setSize(sf::Vector2f(1000, 520));
    room.setTexture(&textureRoom);

}

Game::Game() {

    window.create(sf::VideoMode(1280, 720), "Baeksu Simulator");
    sf::Image icon; //아이콘 객체
    if (!icon.loadFromFile(".\\lib\\icon.png"))  // 아이콘 경로 설정
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

            if (event.type == sf::Event::MouseButtonPressed) {
                if (currentStage == stages["menu"].get() &&  event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::FloatRect bounds = { 465, 585, 150, 90 };
                    if (bounds.contains(mousePos.x, mousePos.y)) {
                        std::cout << "Changing to game" << std::endl;
                        changeStage("level1");
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (currentStage == stages["level1"].get() && event.key.code == sf::Keyboard::Escape) {
                    changeStage("menu");
                }
            }
                
        }

        float dt = clock.restart().asSeconds();
        currentStage->update(window, dt);
        window.display();
    }
}

//************************* Level1 메소드 *********************************
void Level1::printStatus(sf::RenderWindow& window, Ral player) {
    status.setFont(font); //폰트 적용
    status.setStyle(sf::Text::Bold); //폰트 스타일
    status.setString(L"== 상태창 =="); //출력 문자열
    status.setCharacterSize(40); //문자 크기
    status.setPosition(textPos); //글자 위치
    window.draw(status);

    status.setCharacterSize(25);
    status.setPosition(sf::Vector2f(textPos.x, textPos.y + 80));
    status.setString(L"소지금: " + std::to_wstring(p1.money));
    window.draw(status);

    status.setPosition(sf::Vector2f(textPos.x, textPos.y + 160));
    status.setString(L"피로도: " + std::to_wstring(p1.tired));
    window.draw(status);

    status.setPosition(sf::Vector2f(textPos.x, textPos.y + 240));
    status.setString(L"청결도: " + std::to_wstring(p1.clean));
    window.draw(status);

    status.setPosition(sf::Vector2f(textPos.x, textPos.y + 320));
    status.setString(L"배고픔: " + std::to_wstring(p1.hunger));
    window.draw(status);

    status.setPosition(sf::Vector2f(textPos.x, textPos.y + 400));
    status.setString(L"의지: " + std::to_wstring(p1.willing));
    window.draw(status);

}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.run();
    return 0;
}