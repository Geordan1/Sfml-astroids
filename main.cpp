// to com: g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o prog -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>
#include <string>
//includes
class Player {
    public:
        int x;
        int y;
        int x_speed;
        int y_speed;
        void init(int x_,int y_,int x_s,int y_s) {
            x = x_;
            y = y_;
            x_speed = x_s;
            y_speed = y_s;
        }
        sf::Vector2f update() {
            x += x_speed;
            y += y_speed;
            return sf::Vector2f(x, y);
        }
        void move_to(int x_, int y_) {
            int vex = x+25-x_;
            int vey = y+25-y_;
            x_speed -= vex/100;
            y_speed -= vey/100;
        }
        void wrap() {
            if (x < 0) {
                x = 950;
            }
            if (x > 950) {
                x = 0;
            }
            if (y < 0) {
                y = 550;
            }
            if (y > 550) {
                y = 0;
            }
        }
};
class Bullet {
    public:
        int x;
        int y;
        int x_speed;
        int y_speed;
        void init(int x_,int y_,int x_s,int y_s) {
            x = x_;
            y = y_;
            x_speed = x_s;
            y_speed = y_s;
        }
        void wrap() {
            if (x < 0) {
                x = 980;
            }
            if (x > 980) {
                x = 0;
            }
            if (y < 0) {
                y = 580;
            }
            if (y > 580) {
                y = 0;
            }
        }
        sf::Vector2f update() {
            x += x_speed;
            y += y_speed;
            return sf::Vector2f(x, y);
        }
};
class astroid {
    public:
        int x;
        int y;
        int x_speed;
        int y_speed;
        bool real;
        void init(int x_,int y_,int x_s,int y_s) {
            x = x_;
            y = y_;
            x_speed = x_s;
            y_speed = y_s;
            real = true;
        }
        void wrap() {
            if (x < 0) {
                x = 925;
            }
            if (x > 925) {
                x = 0;
            }
            if (y < 0) {
                y = 525;
            }
            if (y > 525) {
                y = 0;
            }
        }
        sf::Vector2f update() {
            x += x_speed;
            y += y_speed;
            return sf::Vector2f(x, y);
        }
        void del() {
            real = false;
        }
        bool go() {
            return real;
        }
};
class Button {
    public:
        int x;
        int y;
        int width;
        int height;
        void init(int x_, int y_, int width_, int height_) {
            x = x_;
            y = y_;
            width = width_;
            height = height_;
        }
        bool detect(int x_, int y_) {
            if (x+width >= x_ && x <= x_ && y+height >= y_ && y <= y_) {
                return true;
            } else {
                return false;
            }
        }
};
//classes
bool detect(int x, int y, int width, int height, int x_, int y_, int width_, int height_) {
    if (x+width >= x_ && x <= x_+width_ && y+height >= y_ && y <= y_+height_) {
        return true;
    } else {
        return false;
    }
}
//functions
int main() {
    sf::RenderWindow window(sf::VideoMode(1000,600), "Astroids", sf::Style::Close | sf::Style::None);
    window.setFramerateLimit(30);
    Player player;
    player.init(400, 400, 0, 0);
    int frame_counter = 0;
    std::vector<Bullet*> bullets;
    bullets.reserve(10);
    std::vector<astroid*> astroids;
    astroids.reserve(10);
    int num;
    int more;
    more = 0;
    int score = 0;
    std::cin >> num;
    bool ongame = false;
    Button homeButton;
    homeButton.init(450, 400, 100, 50);
    for (int i = 0; i < num; i++) {
        astroids.push_back(new astroid);
        astroids[astroids.size()-1] -> init(i*200, 200, -4 + (std::rand() % 8),  -4 + (std::rand() % 8));
    }
    while (window.isOpen()) {
        if (ongame) {
            sf::Event evnt;
            while (window.pollEvent(evnt)) {
                if (evnt.type == sf::Event::Closed) {
                    window.close();
                }
                if (evnt.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                    bullets.push_back(new Bullet);
                    bullets[bullets.size()-1] -> init(player.x+15, player.y+15, 0-(player.x+25-(int)mousepos.x)/10, 0-(player.y+25-((int)mousepos.y))/10);
                }
            }
            if (frame_counter == 5) {
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                player.move_to((int)mousepos.x, (int)mousepos.y);
                frame_counter = 0;
            } else {
                frame_counter += 1;
            }
            player.wrap();
            window.clear(sf::Color(0,0,0));
            for (int i = 0; i < bullets.size(); i++) {
                for (int b = 0; b < astroids.size(); b++) {
                    if (astroids[b] -> go()) {
                        if (detect(astroids[b] -> x,astroids[b] -> y,75,75,bullets[i] -> x,bullets[i] -> y,20,20)) {
                            astroids[b] -> del();
                            more += 1;
                            score += 1;
                        }
                    }
                }
                sf::CircleShape bul(10);
                bul.setFillColor(sf::Color(255,255,255));
                bul.setPosition(bullets[i] -> update());
                window.draw(bul);
            }
            for (int i = 0; i < astroids.size(); i++) {
                if (astroids[i] -> go()) {
                    if (detect(astroids[i] -> x,astroids[i] -> y,75,75,player.x,player.y,50,50)) {
                        ongame = false;
                    }
                    astroids[i] -> wrap();
                    sf::RectangleShape bul(sf::Vector2f(75,75));
                    bul.setFillColor(sf::Color(100,100,100));
                    bul.setPosition(astroids[i] -> update());
                    window.draw(bul);
                }
            }
            sf::CircleShape pshape(25);
            pshape.setFillColor(sf::Color(255,255,255));
            pshape.setPosition(player.update());
            window.draw(pshape);
            sf::Font font;
            if (!font.loadFromFile("/Users/geordansfetsos/Desktop/ALL/code/astroids/font.ttf")) {
                std::cout << "l";
            }
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(28);
            text.setPosition(sf::Vector2f(400,20));
            text.setString("Astroids destrod: " + std::to_string(score));
            text.setFillColor(sf::Color(255,255,255));
            window.draw(text);
            window.display();
            for (int i = 0; i < more; i++) {
                astroids.push_back(new astroid);
                astroids[astroids.size()-1] -> init(player.x+500, player.y+500, -4 + (std::rand() % 8),  -4 + (std::rand() % 8));   
            }
            more = 0;
        } else {
            sf::Event evnt;
            player.x = 400;
            player.y = 400;
            player.x_speed = 0;
            player.y_speed = 0;
            int frame_counter = 0;
            bullets.clear();
            astroids.clear();
            //std::vector<Bullet*> bullets;
            //bullets.reserve(10);
            //std::vector<astroid*> astroids;
            //astroids.reserve(10);
            more = 0;
            int score = 0;
            for (int i = 0; i < num; i++) {
                astroids.push_back(new astroid);
                astroids[astroids.size()-1] -> init(i*200, 200, -4 + (std::rand() % 8),  -4 + (std::rand() % 8));
            }
            while (window.pollEvent(evnt)) {
                if (evnt.type == sf::Event::Closed) {
                    window.close();
                }
                if (evnt.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                    if (homeButton.detect((int)mousepos.x, (int)mousepos.y)) {
                        ongame = true;
                    }
                }
            }
            window.clear(sf::Color(0,0,0));
            sf::RectangleShape shape(sf::Vector2f(homeButton.width, homeButton.height));
            shape.setPosition(sf::Vector2f(homeButton.x, homeButton.y));
            shape.setFillColor(sf::Color(255,255,255));
            window.draw(shape);
            sf::Font font;
            if (!font.loadFromFile("/Users/geordansfetsos/Desktop/ALL/code/astroids/font.ttf")) {
                std::cout << "l";
            }
            sf::Text text;
            text.setFont(font);
            text.setString("PLAY");
            text.setFillColor(sf::Color(0,0,0));
            text.setCharacterSize(25);
            text.setPosition(sf::Vector2f(homeButton.x+5, homeButton.y+10));
            window.draw(text);
            text.setFont(font);
            text.setString("Astroids");
            text.setFillColor(sf::Color(255,255,255));
            text.setCharacterSize(100);
            text.setPosition(sf::Vector2f(200, 50));
            window.draw(text);
            window.display();
        }
    }
    
    return 0;
}
//main