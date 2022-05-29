#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* temporary = _data;

    memcpy(temporary, &type, sizeof(uint8_t));
    temporary += sizeof(uint8_t);

    memcpy(temporary, nick.c_str(), 8 * sizeof(char));
    temporary += 8 * sizeof(char);

    memcpy(temporary, message.c_str(), 80 * sizeof(char));
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char* temporary = _data;

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

        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        ChatMessage msg;
        Socket* sock = new Socket(socket);

        socket.recv(msg,sock);

        if(msg.type == ChatMessage::LOGIN){
            std::cout << "Conectado:" << msg.nick << "\n";
            clients.push_back(std::move(std::make_unique<Socket>(*sock)));
        }
        else if(msg.type == ChatMessage::LOGOUT){
            auto it = clients.begin();

            while(it!=clients.end() && !(**it==*sock)) ++it;

            if(it==clients.end()){
                printf("Cliente no encontrado\n");
            }
            else{
                std::cout << "Desconectado:" << msg.nick << "\n";
                clients.erase(it);
                Socket* del = (*it).release();
                delete del;
            }
        }
        else if(ChatMessage::MESSAGE){
            for(int i = 0; i < clients.size(); ++i){
                if(!(*clients[i]==*sock)) {
                    socket.send(msg,*clients[i]);
                }
            }
        }
        

        
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);


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

    // creates a renderer to render our images
    rend = SDL_CreateRenderer(win, -1, render_flags);


    SDL_Surface* surface;
    surface = IMG_Load("Assets/pato.png");
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

    SDL_Surface* pastoSurf;
    pastoSurf = IMG_Load("Assets/pasto.png");
    pastoTex = SDL_CreateTextureFromSurface(rend, pastoSurf);

    // clears main-memory
    SDL_FreeSurface(pastoSurf);
    

    SDL_QueryTexture(pastoTex, NULL, NULL, &pastoDest.w, &pastoDest.h);
    pastoDest.x = (1000 - pastoDest.w) / 2;
    pastoDest.y = ((1000 - pastoDest.h) / 2) + 50;

}

void ChatClient::logout()
{
    // Completar
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {   

        std::cout << "Homaero vas a ir a tomar cerveza" << "\n";
        SDL_SetRenderDrawColor( rend, 0, 170, 255, 255 );
        SDL_RenderCopy(rend, pastoTex, NULL, &pastoDest);

        //SDL_RenderCopy(rend, tex, NULL, &dest);

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);

        std::string msg = "";

        // if(msg=="q"){
        //     logout();
        // }
        // else{
        //     ChatMessage msgC(nick,msg);
        //     msgC.type = ChatMessage::MESSAGE;
        //     socket.send(msgC, socket);
        // }
    }
}

void ChatClient::net_thread()
{
    ChatMessage msg;
    Socket* sock = new Socket(socket);
    while(true)
    {
        
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        socket.recv(msg, sock);

        std::cout << msg.nick << ": " << msg.message << "\n";
    }
}

