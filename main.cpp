#include <SFML/Graphics.hpp>
#include <iostream>

void UI() {
}

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(1280, 720), "BaekSu"); //1280 x 720 화면, 창 이름은 백수

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f center(windowSize.x / 2.f, windowSize.y / 2.f); //화면 중앙 좌표

    sf::RectangleShape rec(sf::Vector2f(1220, 660)); //메인 창 외각 흰 사각형
    rec.setOrigin(610, 330);
    rec.setPosition(center);
    rec.setFillColor(sf::Color::White);

    sf::RectangleShape rec2(sf::Vector2f(1200, 640)); //메인 창 내각 검정 사각형
    rec2.setOrigin(600, 320);
    rec2.setPosition(center);
    rec2.setFillColor(sf::Color::Black);

    sf::RectangleShape rec3(sf::Vector2f(960, 190)); //텍스트 박스 외각 흰 사각형
    rec3.setPosition(30, 500);
    rec3.setFillColor(sf::Color::White);

    sf::RectangleShape rec4(sf::Vector2f(940, 170)); //텍스트 박스 내각 검정 사각형
    rec4.setPosition(40,510);
    rec4.setFillColor(sf::Color::Black);

    sf::RectangleShape rec5(sf::Vector2f(270, 660)); //상태창 외각 흰 사각형
    rec5.setPosition(980, 30);
    rec5.setFillColor(sf::Color::White);

    sf::RectangleShape rec6(sf::Vector2f(250, 640)); //상태창 내각 검정 사각형
    rec6.setPosition(990, 40);
    rec6.setFillColor(sf::Color::Black);




    sf::Image icon; //아이콘 객체
    if (!icon.loadFromFile("C:\\RPG_libraries\\issac.png"))  // 아이콘 경로 설정
    {  /* 이미지 파일 로드 실패 시 처리*/ }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()); //아이콘 적용

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rec);
        window.draw(rec2);
        window.draw(rec3);
        window.draw(rec4);
        window.draw(rec5);
        window.draw(rec6);
        window.display();
    }

    return 0;
}
