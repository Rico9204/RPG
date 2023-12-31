#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string.h>

//*************************** 클래스 정의 ***************************
class Phone {
public:
    int selectedFoodIndex;
    bool isPhoneEnter;
    sf::FloatRect body;
    std::vector<std::wstring> foods = { L"제육볶음", L"낙곱새", L"마라탕", L"벌꿀집 아이스크림" };
    std::vector<int> prices = { 3000, 6000, 9000, 12000 };
    std::vector<int> hunger = { 20, 40, 60, 80 };
    Phone() {
        selectedFoodIndex = -1;
        body = { 870, 200, 100, 100 };
        bool isPhoneEnter = false;
    }
    std::wstring eatFood() {
        if (selectedFoodIndex != -1) {
            std::wstring food = foods[selectedFoodIndex];
            return food;
        }
        return L"";
    }
    std::wstring getFood(int index) {
        if (index >= 0 && index < foods.size()) {
            return foods[index];
        }
        return L"";
    }
};

class Coin {
public:
    long int profit;
    int investUnit;
    int investAmount;
    int selectedCoinIndex;
    bool isCoinEnter;
    bool isChoosingCoin;
    bool isProfitHappen;
    sf::FloatRect body;
    std::wstring bitcoinToInvest;
    std::vector<std::wstring> bitcoins = { L"비트코스", L"스트라티스", L"스택스", L"스팀달러" };

    Coin() {
        profit = 0;
        investUnit = 10;
        investAmount = 10;
        selectedCoinIndex = -1;
        isProfitHappen = false;
        isCoinEnter = false;
        isChoosingCoin = false;
        body = {650, 308, 90, 102};
    }
    std::wstring getBitcoinName(int index) {
        if (index >= 0 && index < bitcoins.size()) {
            return bitcoins[index];
        }
        return L"";
    }
    void selectBitcoin(int index) {
        if (index >= 0 && index < bitcoins.size()) {
            selectedCoinIndex = index;
        }
    }
    void setInvestAmount(int amount) {
        investAmount = amount;
    }
    int getProfit() {
        return profit;
    }
    std::wstring investBitcoin(int money) {
        if (money < investAmount) {
            return L"돈이 충분하지 않습니다!";
            isProfitHappen = false;
        }
        float randomPercent =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 4) - 2;
        profit = static_cast <int> (investAmount * randomPercent * (selectedCoinIndex+1));
        money = profit - investAmount;
        if (profit > 0 ) {
            isProfitHappen = true;
            return bitcoins[selectedCoinIndex] + L"을 통해 " + std::to_wstring(profit) + L"의 수익을 냈습니다. 대단하네요!";
        }
        if (profit < investAmount ) {
            isProfitHappen = true;
            return bitcoins[selectedCoinIndex] + L"을 통해 " + std::to_wstring(profit) + L"의 손해를 봤습니다. 아쉽네요..";
        }
        if (profit == investAmount )  {
            isProfitHappen = true;
            return bitcoins[selectedCoinIndex] + L"을 통해 " + std::to_wstring(investAmount) + L"의 원금을 회수했습니다. 다행이네요";
        }
    }
};

class Ral {
public:
    int counter = 0;
    long int money; //소지금
    int clean; //청결도
    int stamina; //피로도
    int hunger; //배고픔 
    int willing; //의지
    int texPos; //텍스쳐 위치
    sf::Vector2f size = { 30, 50 };
    sf::Texture front;
    sf::Texture back;
    sf::Texture left;
    sf::Texture right;
    sf::RectangleShape player; //몸
    Ral();
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
    int fadeValue;
    bool isBathroomEnter;
    bool isBedEnter;
    bool isDoorEnter;
    bool isTrashEnter;
    bool isBooksEnter;
    bool isFadeoutEnter;
    bool isFadeinEnter;

    const float gridSize = 40.f; //격자 크기
    const float movementSpeed = 300.f;
    const unsigned window_width = 1280;
    const unsigned window_height = 720;

    sf::Clock actionTextClock;
    Phone phone;
    Coin coin;
    Ral p1;
    sf::Text actionText;
    sf::Text questionText;
    sf::Text status;
    sf::Vector2f textPos = { 1035, 10 };
    sf::Font font;
    sf::Vector2f center{ 640,360 }; //화면 중앙 좌표
    sf::Vector2f velocity;
    sf::RectangleShape save;
    sf::RectangleShape load;
    sf::RectangleShape fade;

    sf::RectangleShape object1; //벽
    sf::RectangleShape object2; //책장
    sf::RectangleShape object3; //컴퓨터
    sf::RectangleShape object4; //쓰레기
    sf::RectangleShape object5; //침대
    sf::RectangleShape object6; //폰

    sf::FloatRect trashRect;
    sf::FloatRect bathroomRect; 
    sf::FloatRect bedRect;
    sf::FloatRect doorRect;
    sf::FloatRect booksRect;

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
    
    if (coin.investUnit < 1) coin.investUnit = 1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        p1.player.setTexture(&p1.front);
        p1.texPos = p1.counter++ / 160 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        p1.player.setTexture(&p1.back);
        p1.texPos = p1.counter++ / 160 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        p1.player.setTexture(&p1.left);
        p1.texPos = p1.counter++ / 160 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        p1.player.setTexture(&p1.right);
        p1.texPos = p1.counter++ / 160 % 12;
        p1.texPos = p1.texPos * 30;
        p1.player.setTextureRect(sf::IntRect(p1.texPos % 360, 0, p1.size.x, p1.size.y)); //텍스쳐 모양 선언
    }

    // Player가 화면 바깥으로 나가는 것을 막는 코드
    if (p1.player.getPosition().x < 0) p1.player.setPosition(0, p1.player.getPosition().y);
    if (p1.player.getPosition().y < 0) p1.player.setPosition(p1.player.getPosition().x, 0);
    if (p1.player.getPosition().x > window_width - gridSize) p1.player.setPosition(window_width - gridSize, p1.player.getPosition().y);
    if (p1.player.getPosition().y > window_height - gridSize) p1.player.setPosition(p1.player.getPosition().x, window_height - gridSize);

    if ((actionText.getString() != "" && actionTextClock.getElapsedTime().asSeconds() > 3.f) || coin.isCoinEnter) {
        actionText.setString("");
    }

    if (isFadeoutEnter) {
        fadeValue += 1;
        fade.setFillColor(sf::Color(0, 0, 0, fadeValue));
    }
    if (isFadeinEnter) {
        fadeValue -= 1;
        fade.setFillColor(sf::Color(0, 0, 0, fadeValue));
    }
    if (fadeValue >= 255) {
        isFadeoutEnter = false;
        isFadeinEnter = true;
    }
    if (fadeValue <= 0) {
        isFadeinEnter = false;
    }

    


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
    window.draw(actionText);
    window.draw(questionText);
    window.draw(fade);
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
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y = movementSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -movementSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = movementSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (coin.body.contains(p1.player.getPosition()) && coin.isCoinEnter == false && questionText.getString() == L"") {
            coin.isCoinEnter = true;
            questionText.setString(L"투자할 비트코인을 고르자\n1: 비트코스\n2: 스트라티스\n3: 스택스\n4: 스팀달러");
        }
        if (bathroomRect.contains(p1.player.getPosition()) && isBathroomEnter == false && questionText.getString() == L"") {
            if (actionTextClock.getElapsedTime().asSeconds() < 3) actionText.setString(L"");
            isBathroomEnter = true;
            questionText.setString(L"몸을 씻을까? Y/N");
        }
        if (bedRect.contains(p1.player.getPosition()) && isBedEnter == false && questionText.getString() == L"") {
            if (actionTextClock.getElapsedTime().asSeconds() < 3) actionText.setString(L"");
            isBedEnter = true;
            questionText.setString(L"지금 잠을 잘까? Y/N");
        }
        if (phone.body.contains(p1.player.getPosition()) && phone.isPhoneEnter == false && questionText.getString() == L"") {
            if (actionTextClock.getElapsedTime().asSeconds() < 3) actionText.setString(L"");
            phone.isPhoneEnter = true;
            questionText.setString(L"폰으로 배달을 시킬까? Y/N");
        }
        if (doorRect.contains(p1.player.getPosition()) && isDoorEnter == false && questionText.getString() == L"") {
            if (actionTextClock.getElapsedTime().asSeconds() < 3) actionText.setString(L"");
            isDoorEnter = true;
            questionText.setString(L"편의점 알바하러 갈까? Y/N");
        }
        if (booksRect.contains(p1.player.getPosition()) && isBooksEnter == false && questionText.getString() == L"") {
            if (actionTextClock.getElapsedTime().asSeconds() < 3) actionText.setString(L"");
            isBooksEnter = true;
            actionText.setString(L"책이다....\n읽고 싶지는 않다");
            actionTextClock.restart();
            isBooksEnter = false;
        }
        if (trashRect.contains(p1.player.getPosition()) && isTrashEnter == false && questionText.getString() == L"") {
            if (actionTextClock.getElapsedTime().asSeconds() < 3) actionText.setString(L"");
            isTrashEnter = true;
            actionText.setString(L"쓰레기들이다\n배달 시켜먹고 남은 쓰레기, 맥주캔 등이 나뒹굴고 있다..\n치우기에는 너무 귀찮은 것 같다.");
            actionTextClock.restart();
            isTrashEnter = false;
        }
    }

    else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 ||
        event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Num4) {
        if (coin.isCoinEnter) {
            coin.selectedCoinIndex = event.key.code - sf::Keyboard::Num1;
            coin.selectBitcoin(coin.selectedCoinIndex);
            coin.isCoinEnter = false;
            coin.isChoosingCoin = true;
        }
        else if (phone.isPhoneEnter && questionText.getString() == L"뭘 먹을까?\n1:제육볶음\n2:낙곱새\n3:마라탕\n4:벌꿀집 아이스크림") {
            phone.selectedFoodIndex = event.key.code - sf::Keyboard::Num1;
            std::wstring food = phone.eatFood();
            if (p1.money > phone.prices[phone.selectedFoodIndex]) {
                p1.money -= phone.prices[phone.selectedFoodIndex];
                p1.hunger += phone.hunger[phone.selectedFoodIndex];
                p1.stamina -= 5;
                questionText.setString(L"");
                actionText.setString(std::to_wstring(phone.prices[phone.selectedFoodIndex]) + 
                    L"원으로 " + food + L"를 먹었습니다" + L"\n" + food + L"를 통해 배고픔을 " + 
                    std::to_wstring(phone.hunger[phone.selectedFoodIndex]) + L"만큼 채웠습니다");
                actionTextClock.restart();
                phone.isPhoneEnter = false;
            }
            else {
                questionText.setString(L"");
                actionText.setString(L"돈이 부족합니다!");
                actionTextClock.restart();
                phone.isPhoneEnter = false;
            }
        }
    }
    else if (coin.isChoosingCoin && !coin.isCoinEnter) {
        if (event.key.code == sf::Keyboard::Up) {
            coin.investAmount += coin.investUnit;
        }
        else if (event.key.code == sf::Keyboard::Down) {
            coin.investAmount -= coin.investUnit;
            if (coin.investAmount < 0) coin.investAmount = 0;
        }
        else if (event.key.code == sf::Keyboard::Right) {
            coin.investUnit *= 10;
            if (coin.investUnit > 10000000000) coin.investUnit /= 10;
        }
        else if (event.key.code == sf::Keyboard::Left) {
            coin.investUnit /= 10;
            if (coin.investUnit < 0) coin.investUnit = 1;
        }
        questionText.setString("L");
        questionText.setString(L"투자하려면 방향키 키보드를 통해 금액을 조정후 엔터를 누르세요\n상, 하: 현재 자리수 1씩 증가\n좌, 우: 최고 자리수 증가\n" + std::to_wstring(coin.investAmount) + L" 만큼 투자하기");
    }
    
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Enter && coin.isChoosingCoin && coin.investAmount > 0) {
            questionText.setString("");
            coin.setInvestAmount(coin.investAmount);
            std::wstring result = coin.investBitcoin(p1.money);
            actionText.setString(result);
            if (coin.isProfitHappen) {
                p1.willing -= 5;
                p1.stamina -= 5;
                p1.hunger -= 5;
                coin.isProfitHappen = false;
            }
            coin.isChoosingCoin = false;
            coin.investAmount = 10; 
            coin.investUnit = 10; 
            p1.money += coin.getProfit();
            actionTextClock.restart();
        }
        if(event.key.code == sf::Keyboard::Y){
            if (isBathroomEnter) {
                if (p1.clean < 100) {
                    p1.clean += 40;
                    p1.stamina -= 5;
                    p1.hunger -= 5;
                    questionText.setString(L"");
                    actionText.setString(L"몸을 깨끗이 씻었습니다!");
                    if (p1.clean >= 100) p1.clean = 100;
                    actionTextClock.restart();
                    isBathroomEnter = false;
                }
                else if (p1.clean == 100) {
                    questionText.setString(L"");
                    actionText.setString(L"지금은 씻을 필요가 없을 것 같다");
                    actionTextClock.restart();
                    isBathroomEnter = false;
                }
            }
            else if (isBedEnter) {
                if (p1.stamina <= 20) {
                    p1.stamina += 80;
                    p1.hunger -= 60;
                    p1.clean -= 40;
                    questionText.setString(L"");
                    actionText.setString(L"자고 일어났습니다\n개운하네요");
                    isFadeoutEnter = true;
                    actionTextClock.restart();
                    isBedEnter = false;
                }
                else {
                    questionText.setString(L"");
                    actionText.setString(L"아직은 잠이 안온다...");
                    actionTextClock.restart();
                    isBedEnter = false;
                }   
            }
            else if (phone.isPhoneEnter) {
                if (p1.hunger < 100) {
                    questionText.setString(L"뭘 먹을까?\n1:제육볶음\n2:낙곱새\n3:마라탕\n4:벌꿀집 아이스크림");
                }
                else {
                    questionText.setString(L"");
                    actionText.setString(L"아직은 배가 안고프다...");
                    actionTextClock.restart();
                    phone.isPhoneEnter = false;
                }
            }
            else if (isDoorEnter){
                isFadeoutEnter = true;
                if (p1.stamina > 60) {
                    p1.stamina -= 60;
                    p1.hunger -= 60;
                    p1.clean -= 30;
                    p1.money += 57720;
                    questionText.setString(L"");
                    actionText.setString(L"알바를 마치고 집에 돌아왔다....\n지친다 지쳐");
                    actionTextClock.restart();
                    isDoorEnter = false;
                }
                else {
                    questionText.setString(L"");
                    actionText.setString(L"알바에 갈 힘이 없다.. 그냥 쉴래....");
                    actionTextClock.restart();
                    isDoorEnter = false;
                }
            }

        }
        if (event.key.code == sf::Keyboard::N) {
            if (isBathroomEnter || isBedEnter || phone.isPhoneEnter || isDoorEnter) {
                questionText.setString(L"");
                isBathroomEnter = false;
                isBedEnter = false;
                phone.isPhoneEnter = false;
                isDoorEnter = false;
            }
        }
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
            p1.counter = 0;
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
                    file << "stamina: " << p1.stamina << "\n";
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
                        else if (line.find("stamina: ") != std::string::npos) {
                            p1.stamina = std::stoi(line.substr(line.find("stamina: ") + 9));
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
    fadeValue = 0;
    isBedEnter = false;
    isDoorEnter = false;
    isBooksEnter = false;
    isTrashEnter = false;
    isFadeinEnter = false;
    isFadeoutEnter = false;
    isBathroomEnter = false;

    bathroomRect = { 560, 0, 50, 100 };
    trashRect = { 0, 300 , 220, 200 };
    booksRect = { 230,80,265, 80 };
    bedRect = { 670, 0, 300, 250 };
    doorRect = {0, 0, 100, 100};

    fade.setSize(sf::Vector2f(1000, 520));
    fade.setFillColor(sf::Color(0, 0, 0, fadeValue));

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

    actionText.setFont(font);
    actionText.setCharacterSize(24);
    actionText.setFillColor(sf::Color::White);
    actionText.setStyle(sf::Text::Bold);
    actionText.setPosition(10, lines[0].getPosition().y + lines[0].getGlobalBounds().height + 10);

    questionText.setFont(font);
    questionText.setCharacterSize(24);
    questionText.setFillColor(sf::Color::White);
    questionText.setStyle(sf::Text::Bold);
    questionText.setPosition(10, lines[0].getPosition().y + lines[0].getGlobalBounds().height + 10);


}

Game::Game() {
    window.create(sf::VideoMode(1280, 720), "Baeksu Simulator");
    sf::Image icon; //아이콘 객체
    if (!icon.loadFromFile(".\\lib\\icon.png"))  // 아이콘 경로 설정
    {  /* 이미지 파일 로드 실패 시 처리*/   }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()); //아이콘 적용
    stages["menu"] = std::make_unique<Menu>();
    stages["level1"] = std::make_unique<Level1>();
    currentStage = stages["menu"].get();
    currentStage->onEnter();

}

Ral::Ral() {
    money = 10000;
    clean = 50;
    stamina = 100;
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
    status.setString(L"스테미너: " + std::to_wstring(p1.stamina));
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