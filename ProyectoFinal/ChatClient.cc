#include <thread>
#include "Chat.h"

int main(int argc, char **argv)
{
    // Si se ejecuta el ChatClient sin los argumentos necesarios salta un pequeño menú en la terminal
    // La IP que hemos puesto para testear es 172.0.0.1
    // Si se ejecuta con los argumentos necesarios se inicia como hacíamos hasta ahora
    if(argv[1]==nullptr || argv[2]==nullptr || argv[3]==nullptr){
        printf("Introduce la IP a la que te quieres conectar: ");
        std::string ip;
        std::cin >> ip;
        char* ipC = std::strcpy(new char[ip.length()+1], ip.c_str());
        printf("Introduce el puerto al que te quieres conectar: ");
        std::string p;
        std::cin >> p;
        char* pC = std::strcpy(new char[p.length()+1], p.c_str());
        printf("¿Cual es tu nombre?: ");
        std::string n;
        std::cin >> n;
        char* nC = std::strcpy(new char[n.length()+1], n.c_str());

        ChatClient ec(ipC, pC, nC);
        std::thread ([&ec](){
            ec.net_thread(); 
        }).detach();

        ec.login();
    
        ec.game_thread();
    }
    else{
        ChatClient ec(argv[1], argv[2], argv[3]);
        std::thread ([&ec](){
            ec.net_thread(); 
        }).detach();

        ec.login();
    
        ec.game_thread();
    }
}

