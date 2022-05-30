#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    // Serializar los campos type, nick y message en el buffer _data
    char *temporary = _data;

    memcpy(temporary, &type, sizeof(uint8_t));
    temporary += sizeof(uint8_t);

    memcpy(temporary, nick.c_str(), 8 * sizeof(char));
    temporary += 8 * sizeof(char);

    memcpy(temporary, message.c_str(), 80 * sizeof(char));
}

int ChatMessage::from_bin(char *bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    // Reconstruir la clase usando el buffer _data
    char *temporary = _data;

    memcpy(&type, temporary, sizeof(uint8_t));
    temporary += sizeof(uint8_t);

    nick = temporary;
    temporary += 8 * sizeof(char);

    message = temporary;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{

    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        // Recibir Mensajes en y en función del tipo de mensaje
        //  - LOGIN: Añadir al vector clients
        //  - LOGOUT: Eliminar del vector clients
        //  - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        ChatMessage msg;
        Socket *sock = new Socket(socket);

        socket.recv(msg, sock);

        if (msg.type == ChatMessage::LOGIN)
        {
            std::cout << "Conectado:" << msg.nick << "\n";
            clients.push_back(std::move(std::make_unique<Socket>(*sock)));
            if (clients.size() > 1)
            {
                initClient();
            }
        }
        else if (msg.type == ChatMessage::LOGOUT)
        {
            auto it = clients.begin();

            while (it != clients.end() && !(**it == *sock))
                ++it;

            if (it == clients.end())
            {
                printf("Cliente no encontrado\n");
            }
            else
            {
                std::cout << "Desconectado:" << msg.nick << "\n";
                clients.erase(it);
                Socket *del = (*it).release();
                delete del;
            }
        }
        else if (msg.type == ChatMessage::INFO)
        {
            socket.send(msg, *clients.back());
        }
        else if (msg.type == ChatMessage::MESSAGE || msg.type == ChatMessage::DELETE)
        {
            for (int i = 0; i < clients.size(); ++i)
            {
                if (!(*clients[i] == *sock))
                {
                    socket.send(msg, *clients[i]);
                }
            }
        }
    }
}

/**
 * Game Loop que lleva el Server, desde el que se crean los patos
 *
 */
void ChatServer::game_loop()
{
    while (true)
    {
        if (!clients.empty())
        {
            if (timeSinceLastSpawn >= duckSpawningTime)
            {
                ChatMessage msg;
                msg.type = ChatMessage::NEWPATO;
                msg.nick = "Server";

                int side = rand() % 2;
                int x, y;
                if (side == 0)
                {
                    x = 0;
                }
                else
                    x = winW;
                y = rand() % (int)(winH / 2);

                int gold = rand() % 10;
                msg.message = std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(gold);

                for (int i = 0; i < clients.size(); ++i)
                {
                    socket.send(msg, *clients[i]);
                }

                timeSinceLastSpawn = 0;
            }
            else
                ++timeSinceLastSpawn;
        }
    }
}

/**
 * Funcion que manda mensaje al primer cliente para actualizar la información a los nuevos que se unan
 *
 */

void ChatServer::initClient()
{
    ChatMessage m;
    m.type = ChatMessage::NEWCLIENT;
    m.nick = "Server";
    m.message = "New client, send info";
    socket.send(m, *clients[0]);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 * Login del cliente, manda un mensaje de Login al server e inicializa SDL y todo lo necesario para el juego
 *
 */
void ChatClient::login()
{
    // SDL PART

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        printf("error initializing SDL: %s\n", SDL_GetError());

    win = SDL_CreateWindow("GAME", // creates a window
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           winW, winH, 0);

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
    }

    // creates a renderer to render our images
    rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Surface *surface;
    surface = IMG_Load("../Assets/pato.png");
    tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_RenderCopy(rend, tex, NULL, NULL);
    SDL_RenderPresent(rend);

    // clears main-memory
    SDL_FreeSurface(surface);

    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 10;
    dest.h /= 10;
    dest.x = (1000 - dest.w) / 2;
    dest.y = (1000 - dest.h) / 2;

    SDL_Surface *surface2;
    surface2 = IMG_Load("../Assets/patoDorado.png");
    goldTex = SDL_CreateTextureFromSurface(rend, surface2);
    SDL_RenderCopy(rend, goldTex, NULL, NULL);
    SDL_RenderPresent(rend);

    // clears main-memory
    SDL_FreeSurface(surface2);

    SDL_QueryTexture(goldTex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 10;
    dest.h /= 10;
    dest.x = (1000 - dest.w) / 2;
    dest.y = (1000 - dest.h) / 2;

    SDL_Surface *pastoSurf;
    pastoSurf = IMG_Load("../Assets/pasto.png");
    pastoTex = SDL_CreateTextureFromSurface(rend, pastoSurf);

    // clears main-memory
    SDL_FreeSurface(pastoSurf);

    SDL_QueryTexture(pastoTex, NULL, NULL, &pastoDest.w, &pastoDest.h);
    pastoDest.x = (1000 - pastoDest.w) / 2;
    pastoDest.y = ((1000 - pastoDest.h) / 2) + 50;

    int fontSize = 128;
    text_color = {255, 255, 255};
    std::string fontpath = "../Assets/discoduck3dital.ttf";
    font = TTF_OpenFont(fontpath.c_str(), fontSize);
    text_surface = TTF_RenderText_Solid(font, std::to_string(points).c_str(), text_color);
    ftexture = SDL_CreateTextureFromSurface(rend, text_surface);
    t_width = text_surface->w;
    t_height = text_surface->h;
    textDst.x = winW / 2;
    textDst.y = 50;
    textDst.w = t_width;
    textDst.h = t_height;

    // Login message
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

/**
 * Cierra SDL y manda mensaje de Logout al server
 *
 */
void ChatClient::logout()
{

    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close SDL
    SDL_Quit();

    // Completar
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

/**
 * Recibe los mensajes del server y de los otros clientes
 * Además los procesa y hace cambios en su bucle interno
 */
void ChatClient::net_thread()
{
    ChatMessage msg;
    Socket *sock = new Socket(socket);
    while (true)
    {

        // Recibir Mensajes de red
        // Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        socket.recv(msg, sock);

        std::cout << msg.nick << ": " << msg.message << "\n";

        if (msg.type == ChatMessage::NEWPATO)
        {

            std::stringstream ss(msg.message);
            std::string word;
            std::pair<int, int> xy;
            ss >> word;
            xy.first = stoi(word);
            ss >> word;
            xy.second = stoi(word);
            ss >> word;
            if (stoi(word) == 1)
            {
                createPato(xy, true);
            }
            else
                createPato(xy, false);
        }
        else if (msg.type == ChatMessage::DELETE)
        {
            ducks[stoi(msg.message)]->alive = false;
        }
        else if (msg.type == ChatMessage::NEWCLIENT)
        {
            ChatMessage m;
            m.type = ChatMessage::INFO;
            m.nick = nick;
            for (Duck *d : ducks)
            {
                m.message = std::to_string(d->getX()) + " " + std::to_string(d->getY()) + " " + std::to_string(d->vel) + " " + std::to_string(d->gold);
                socket.send(m, socket);
            }
            
        }
        else if (msg.type == ChatMessage::INFO)
        {
            std::stringstream ss(msg.message);
            std::string word;
            std::pair<int, int> xy;
            int vel;
            ss >> word;
            xy.first = stoi(word);
            ss >> word;
            xy.second = stoi(word);
            ss >> word;
            vel = stoi(word);
            ss >> word;
            if (stoi(word) == 1)
            {
                Duck *d = new Duck(rend, goldTex, true);

                d->setPos(xy.first, xy.second);
                d->setSize(dest.w, dest.h);

                d->updateVel(vel);
                    

                ducks.push_back(d);
            }
            else
            {
                Duck *d = new Duck(rend, tex, false);

                d->setPos(xy.first, xy.second);
                d->setSize(dest.w, dest.h);

                d->updateVel(vel);

                ducks.push_back(d);
            }
        }
    }
}

/**
 * Bucle de juego para cada cliente
 *
 */
void ChatClient::game_thread()
{
    while (!close)
    {
        SDL_RenderClear(rend);
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;

            case SDL_MOUSEBUTTONDOWN:

                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    click.x = event.motion.x;
                    click.y = event.motion.y;
                    int i = 0;
                    while (i < ducks.size() && !ducks[i]->checkShot(click))
                    {
                        ++i;
                    }
                    if (i < ducks.size())
                    {
                        if (ducks[i]->gold)
                            points += 500;
                        else
                            points += 100;
                        ChatMessage msg;
                        msg.type = ChatMessage::DELETE;
                        msg.nick = nick;
                        msg.message = std::to_string(i);
                        socket.send(msg, socket);
                    }
                }
                break;
            default:
                break;
            }
        }
        for (int i = 0; i < ducks.size(); i++)
        {
            if (!ducks[i]->alive)
            {
                delete ducks[i];
                ducks.erase(ducks.begin() + i);
            }
        }

        for (auto d : ducks)
        {
            d->update();
            d->render();
        }

        SDL_SetRenderDrawColor(rend, 0, 170, 255, 255);

        SDL_RenderCopy(rend, pastoTex, NULL, &pastoDest);

        // Render de la puntuación
        text_surface = TTF_RenderText_Solid(font, std::to_string(points).c_str(), text_color);
        ftexture = SDL_CreateTextureFromSurface(rend, text_surface);
        t_width = text_surface->w;
        t_height = text_surface->h;
        textDst.x = 20;
        textDst.y = 0;
        textDst.w = t_width;
        textDst.h = t_height;

        SDL_RenderCopy(rend, ftexture, NULL, &textDst);

        // SDL_RenderCopy(rend, tex, NULL, &dest);

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
}

/**
 * Crea un nuevo pato en cada cliente a partir de un mensaje del server
 * El mensaje contiene las coordenadas de aparición del pato, calculado por el server
 * @param xy
 */
void ChatClient::createPato(std::pair<int, int> xy, bool g)
{
    if (g)
    {
        Duck *d = new Duck(rend, goldTex, true);

        d->setPos(xy.first, xy.second);
        d->setSize(dest.w, dest.h);

        if (xy.first == winW)
            d->setVel();

        ducks.push_back(d);
    }
    else
    {
        Duck *d = new Duck(rend, tex, false);

        d->setPos(xy.first, xy.second);
        d->setSize(dest.w, dest.h);

        if (xy.first == winW)
            d->setVel();

        ducks.push_back(d);
    }
}