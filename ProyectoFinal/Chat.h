#include <string>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Serializable.h"
#include "SDL2/SDL_ttf.h"
#include "Socket.h"
#include "Duck.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 *  Mensaje del protocolo de la aplicación de Chat
 *
 *  +-------------------+
 *  | Tipo: uint8_t     | 0 (login), 1 (mensaje), 2 (logout)
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */


    
class ChatMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2,
        NEWPATO = 3,
        DELETE  = 4,
        NEWCLIENT = 5,
        INFO = 6,
        UPDATESCORE=7
    };

    ChatMessage(){};

    ChatMessage(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string nick;
    std::string message;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el servidor de chat
 */
class ChatServer
{

    

public:

    ChatServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();

    void game_loop();

    void initClient();

private:

    float winH = 1000;
    float winW = 1000;
    
    int duckSpawningTime = 3;
    std::chrono::time_point<std::chrono::system_clock> timeSinceLastSpawn;

    std::unordered_map<std::string,int> scoreboard;

    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */
    std::vector<std::unique_ptr<Socket>> clients;

    /**
     * Socket del servidor
     */
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el cliente de chat
 */
class ChatClient
{
    float winH = 1000;
    float winW = 1000;
    SDL_Window* win;
    SDL_Renderer* rend;  
    SDL_Texture* ftexture;
    SDL_Texture* tex;
    SDL_Texture* goldTex;
    SDL_Texture* pastoTex;
    SDL_Texture* miraTex;
    SDL_Rect dest;
    SDL_Rect miradest;
    SDL_Rect pastoDest;

    bool close = false;

    std::vector<Duck*> ducks;

    SDL_Point click;
    
    int points = 0;

    int t_width = 0;
    int t_height = 0;
    TTF_Font* font;
    SDL_Surface* text_surface;
    SDL_Rect textDst;
    SDL_Color text_color;

public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    ChatClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();

    void game_thread();

    void createPato(std::pair<int,int> xy, bool g);

private:



    /**
     * Socket para comunicar con el servidor
     */
    Socket socket;

    /**
     * Nick del usuario
     */
    std::string nick;
};

