#include <thread>
#include "Chat.h"

int main(int argc, char **argv)
{
    ChatServer es(argv[1], argv[2]);


    std::thread ([&es](){
        es.do_messages(); 
    }).detach();

    es.game_loop();

    return 0;
}
