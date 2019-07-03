/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SnakeSFML.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Roger Ndaba <rogerndaba@gmil.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 18:16:26 by Roger Ndaba       #+#    #+#             */
/*   Updated: 2019/07/03 15:54:31 by Roger Ndaba      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SnakeSFML.hpp"

SnakeSFML::SnakeSFML(int w, int h)
    : _start(0), WINW(w), WINH(h), _prevKey(3), _doExit(false), _speed(10), _score(0) {
    TVertex tv;

    tv.x1 = (WINW / 2);
    tv.y1 = (WINH / 2);
    tv.x2 = tv.x1 + 15;
    tv.y2 = tv.y1 + 15;
    std::vector<TVertex>* tvt = new std::vector<TVertex>;
    tvt->push_back(tv);
    this->_key = new bool[4];
    std::fill(_key, _key + 4, false);
    _key[3] = true;
    this->_body = tvt;
    this->_obstacles = new std::vector<TVertex>;
    this->initObstacles();
    this->randFood();
    this->init();
}

SnakeSFML::SnakeSFML(SnakeT Snake) {
    WINW = Snake.WINW;
    WINH = Snake.WINH;
    _body = Snake.body;
    _obstacles = Snake.obstacles;
    _food = Snake.food;
    _key = Snake.key;
    _prevKey = Snake.prevKey;
    _doExit = Snake.doExit;
    _speed = Snake.speed;
    _score = Snake.score;
}

SnakeSFML::SnakeSFMLException::SnakeSFMLException(std::string exc) {
    this->_exc = "\033[31m" + exc + "\033[0m";
}

SnakeSFML::SnakeSFMLException::SnakeSFMLException(SnakeSFML::SnakeSFMLException const& copy) {
    *this = copy;
}

const char* SnakeSFML::SnakeSFMLException::what() const throw() {
    return _exc.c_str();
}

SnakeSFML::SnakeSFMLException& SnakeSFML::SnakeSFMLException::operator=(SnakeSFML::SnakeSFMLException const& rhs) {
    if (this != &rhs) {
    }
    return *this;
}

SnakeSFML::~SnakeSFML() {
    _display.close();
}

SnakeSFML::SnakeSFML(SnakeSFML const& copy) {
    *this = copy;
};

SnakeSFML& SnakeSFML::operator=(SnakeSFML const& rhs) {
    if (this != &rhs) {
    }
    return *this;
}

void SnakeSFML::init() {
    _display.create(sf::VideoMode(WINW, WINH + 60), "Snake SFML");
    sf::Text text;

    sf::Font font;
    if (!font.loadFromFile("fonts/big_noodle_titling.ttf")) {
        throw SnakeSFMLException("No font");
    }

    int prevEvent = 0;
    while (!_doExit) {
        sf::Event ev;
        _display.pollEvent(ev);

        _now = _clock.getElapsedTime().asMilliseconds();
        if (_now >= _start + (1000 * (1.0 / _speed))) {
            _display.clear(sf::Color::Black);

            if (checkFood()) {
                randFood();
                // _speed += 0.5;
                _score += _speed;
                // al_set_timer_speed(_timer, 1.0 / _speed);
            }

            if (_key[KEY_UP]) {
                _doExit = moveHead(KEY_UP);
            } else if (_key[KEY_DOWN]) {
                _doExit = moveHead(KEY_DOWN);
            } else if (_key[KEY_LEFT]) {
                _doExit = moveHead(KEY_LEFT);
            } else if (_key[KEY_RIGHT]) {
                _doExit = moveHead(KEY_RIGHT);
            } else {
                _doExit = moveHead(-1);
            }
            sf::RectangleShape r;
            r.setSize(sf::Vector2f(WINW, WINH + 60));
            r.setPosition(0, 0);
            r.setFillColor(sf::Color(245, 255, 153));
            _display.draw(r);
            r.setSize(sf::Vector2f(WINW - 4, WINH - 2));
            r.setPosition(2, 60);
            r.setFillColor(sf::Color(0, 0, 0));
            _display.draw(r);

            text.setFillColor(sf::Color(0, 0, 0));
            text.setFont(font);
            std::stringstream ss;
            ss << _score;
            std::string s = "Score : " + ss.str();
            text.setString(s.c_str());
            text.setPosition(20, 35);
            text.setCharacterSize(18);
            _display.draw(text);
            text.setString("Snake SFML");
            text.setPosition((WINW / 2) - 30, 5);
            text.setCharacterSize(24);
            _display.draw(text);

            r.setSize(sf::Vector2f(14, 14));

            for (std::vector<TVertex>::iterator it = _obstacles->begin(); it != _obstacles->end(); ++it) {
                r.setPosition(it->x1, it->y1);
                r.setFillColor(sf::Color(245, 255, 153));
                _display.draw(r);
            }
            for (std::vector<TVertex>::iterator it = _body->begin(); it != _body->end(); ++it) {
                r.setPosition(it->x1, it->y1);
                if (it == _body->begin())
                    r.setFillColor(sf::Color::Red);
                else
                    r.setFillColor(sf::Color::Yellow);
                _display.draw(r);
            }
            r.setPosition(_food.x1, _food.y1);
            r.setFillColor(sf::Color::White);
            _display.draw(r);
            _display.display();
            _start = _now;
            ev.type = sf::Event::LostFocus;
            prevEvent = 0;
        }
        if (ev.type == sf::Event::KeyPressed && prevEvent == 0) {
            int tmp;
            for (int i = 0; i < 4; i++) {
                if (_key[i])
                    tmp = i;
            }
            switch (ev.key.code) {
                case sf::Keyboard::Up: {
                    if (tmp != KEY_DOWN && tmp != KEY_UP) {
                        _prevKey = tmp;
                        std::fill(_key, _key + 4, false);
                        _key[KEY_UP] = true;
                    }
                } break;
                case sf::Keyboard::Down: {
                    if (tmp != KEY_UP && tmp != KEY_DOWN) {
                        _prevKey = tmp;
                        std::fill(_key, _key + 4, false);
                        _key[KEY_DOWN] = true;
                    }
                } break;
                case sf::Keyboard::Left: {
                    if (tmp != KEY_RIGHT && tmp != KEY_LEFT) {
                        _prevKey = tmp;
                        std::fill(_key, _key + 4, false);
                        _key[KEY_LEFT] = true;
                    }
                } break;
                case sf::Keyboard::Right: {
                    if (tmp != KEY_LEFT && tmp != KEY_RIGHT) {
                        _prevKey = tmp;
                        std::fill(_key, _key + 4, false);
                        _key[KEY_RIGHT] = true;
                    }
                } break;
                case sf::Keyboard::Escape:
                    _doExit = true;
                    break;
                default:
                    break;
            }
            prevEvent = 1;
        } else if (ev.type == sf::Event::Closed)
            _doExit = true;
    }
}

bool SnakeSFML::checkFood() {
    TVertex tmp = (*_body)[0];
    if (tmp.y1 == _food.y1 &&
        tmp.x1 == _food.x1 &&
        tmp.x2 == _food.x2 &&
        tmp.y2 == _food.y2) {
        _body->push_back(tmp);
        return true;
    }
    return false;
}

bool SnakeSFML::moveHead(int key) {
    TVertex head = *_body->begin();
    TVertex tail = head;
    switch (key) {
        case 0: {
            _body->pop_back();
            tail.y1 = head.y1 - 15;
            tail.y2 = head.y2 - 15;
            _body->insert(_body->begin(), tail);
        } break;
        case 1: {
            _body->pop_back();
            tail.y1 = head.y1 + 15;
            tail.y2 = head.y2 + 15;
            _body->insert(_body->begin(), tail);
        } break;
        case 2: {
            _body->pop_back();
            tail.x1 = head.x1 - 15;
            tail.x2 = head.x2 - 15;
            _body->insert(_body->begin(), tail);
        } break;
        case 3: {
            _body->pop_back();
            tail.x1 = head.x1 + 15;
            tail.x2 = head.x2 + 15;
            _body->insert(_body->begin(), tail);
        } break;
        default: {
            _body->pop_back();
            tail.x1 = head.x1 + 15;
            tail.x2 = head.x2 + 15;
            _body->insert(_body->begin(), tail);
        } break;
    }
    return (checkCollusion(tail)) ? true : false;
}
bool SnakeSFML::checkCollusion(TVertex& tv) {
    std::vector<TVertex>::iterator it;
    for (it = _body->begin() + 1; it != _body->end(); ++it) {
        if (tv.x1 == it->x1 && tv.x2 == it->x2 && tv.y1 == it->y1 && tv.y2 == it->y2) {
            return true;
        }
    }
    for (it = _obstacles->begin(); it != _obstacles->end(); ++it) {
        if (tv.x1 == it->x1 && tv.x2 == it->x2 && tv.y1 == it->y1 && tv.y2 == it->y2) {
            return true;
        }
    }
    if (tv.x1 < 0 || tv.x2 > WINW || tv.y1 < 60 || tv.y2 > WINH + 60)
        return true;
    return false;
}

void SnakeSFML::initObstacles() {
    TVertex tv;
    TVertex tv2;
    for (int i = 0; i < 10; i++) {
        int rx = 2 + (std::rand() % ((WINW / 15) - 2));
        int ry = 2 + (std::rand() % ((WINH / 15) - 2));

        tv.x1 = rx * 15, tv.y1 = (ry * 15) + 60, tv.x2 = tv.x1 + 15, tv.y2 = tv.y1 + 15;

        tv2.x1 = tv.x1 - 15, tv2.y1 = tv.y1, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);
        tv2.x1 = tv.x1 - 15, tv2.y1 = tv.y1 - 15, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);
        tv2.x1 = tv.x1 - 15, tv2.y1 = tv.y1 + 15, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);

        tv2.x1 = tv.x2, tv2.y1 = tv.y1, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);
        tv2.x1 = tv.x2, tv2.y1 = tv.y1 - 15, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);
        tv2.x1 = tv.x2, tv2.y1 = tv.y1 + 15, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);

        tv2.x1 = tv.x1, tv2.y1 = tv.y1 - 15, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);
        tv2.x1 = tv.x1, tv2.y1 = tv.y2, tv2.x2 = tv2.x1 + 15, tv2.y2 = tv2.y1 + 15;
        _obstacles->push_back(tv2);
        _obstacles->push_back(tv);
    }
}

void SnakeSFML::randFood() {
    int tmpx = WINW / 15;
    int tmpy = WINH / 15;

    int ranx = 1 + (std::rand() % (tmpx - 1)) - 1;
    int rany = 1 + (std::rand() % (tmpy - 1)) - 1;
    _food.x1 = ranx * 15;
    _food.x2 = _food.x1 + 15;
    _food.y1 = (rany * 15) + 60;
    _food.y2 = _food.y1 + 15;
    std::vector<TVertex>::iterator it;
    for (it = _obstacles->begin(); it != _obstacles->end(); ++it) {
        if (_food.x1 == it->x1 && _food.x2 == it->x2 && _food.y1 == it->y1 && _food.y2 == it->y2) {
            SnakeSFML::randFood();
            std::cout << "yip" << std::endl;
        }
    }
}

SnakeSFML* createSnake(int w, int h) {
    return new SnakeSFML(w, h);
}

void deleteSnake(SnakeSFML* sa) {
    delete sa;
}
