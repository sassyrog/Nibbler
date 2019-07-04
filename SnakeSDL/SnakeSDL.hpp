/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SnakeSDL.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Roger Ndaba <rogerndaba@gmil.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 13:21:34 by Roger Ndaba       #+#    #+#             */
/*   Updated: 2019/07/04 17:20:19 by Roger Ndaba      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SNAKESDL_HPP
#define SNAKESDL_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <NibblerException.hpp>
#include <Snake.hpp>

class SnakeSDL : public Snake {
   private:
    std::vector<TVertex> *_body;
    std::vector<TVertex> *_obstacles;
    SDL_Window *_display;
    SDL_Renderer *_renderer;
    // SDL_Event _event;
    Uint32 _start;
    Uint32 _now;
    TVertex _food;
    TVertex _bonus;
    SnakeT SNAKE;
    int WINW;
    int WINH;

    bool *_key;
    int _prevKey;
    bool _doExit;
    float _speed;
    int _score;
    int _trackFood;
    bool _valBonus;

   public:
    class SnakeSDLException : public NibblerException {
       private:
        std::string _exc;

       public:
        SnakeSDLException(std::string);
        SnakeSDLException(SnakeSDLException const &);
        SnakeSDLException &operator=(SnakeSDLException const &);
        virtual const char *what() const throw();
    };

    SnakeSDL(const int, const int);
    SnakeSDL(SnakeSDL const &);
    SnakeSDL &operator=(SnakeSDL const &);
    ~SnakeSDL();

    SnakeT getSnake() const;
    void updateSnake(SnakeT);

    void init();
    void initObstacles();
    bool checkFood();
    void randFood();
    bool moveHead(int);
    bool checkCollusion(TVertex &);
};

extern "C" {
SnakeSDL *createSnake(int, int);
void deleteSnake(SnakeSDL *);
}

#endif  //SNAKESDL_HPP