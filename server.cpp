#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include "client_message.h"

using namespace std;
struct client_message cm;

class version
{
public:
        // You don't have to use uint8_t, it's the same as "unsigned char"
        uint8_t type = 14;
        uint8_t major;
        uint8_t minor;
        uint16_t extension_length = 0;
        version(uint8_t p_major, uint8_t p_minor) : major(p_major), minor(p_minor) {}
        bool send(int skt)
        {
                return 5 == write(skt, this, 5);
        }
} __attribute__((packed)); // we'll have to make sure this is packed

class game
{
public:
        uint8_t type = 11;
        uint16_t initial_points;
        uint16_t stat_limit;
        uint16_t dlenription_length;
        string dlenription;
        game(uint16_t ip, uint16_t sl, string d) : initial_points(ip), stat_limit(sl), dlenription(d) {}
        bool send(int skt)
        {
                dlenription_length = dlenription.length();
                if (7 != write(skt, this, 7))
                        return false;
                if (dlenription_length != write(skt, dlenription.c_str(), dlenription_length))
                        return false;
                return true;
        }
} __attribute__((packed));

struct character
{
        uint8_t type = 10;
        char name[32];
        uint8_t flags;
        uint16_t attack;
        uint16_t defense;
        uint16_t regen;
        int16_t health;
        uint16_t power;
        uint16_t room_num;
        uint16_t dlen_length;
        char *dlen;
} __attribute__((packed));
// MONSTER 1
struct monster
{
        uint8_t type = 10;
        char name[32] = "Flying Dutchman";
        uint8_t flag = 0xff;
        uint16_t attack = 11;
        uint16_t defense = 15;
        uint16_t regen = 30;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 2;
        uint16_t dlen = 100;
        string description = "A ghost and he is flying on a ship";
} __attribute__((packed));
// MONSTER 2
struct monster2
{
        uint8_t type = 10;
        char name[32] = "Thundrok";
        uint8_t flag = 0xff;
        uint16_t attack = 17;
        uint16_t defense = 25;
        uint16_t regen = 30;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 3;
        uint16_t dlen = 100;
        string description = " a massive, four-armed creature covered in sparking electricity. Its eyes glow with an otherworldly energy, and it wields a pair of jagged, crackling swords.";
} __attribute__((packed));
// Monster 3
struct monster3
{
        uint8_t type = 10;
        char name[32] = "Machi #3";
        uint8_t flag = 0xff;
        uint16_t attack = 30;
        uint16_t defense = 15;
        uint16_t regen = 30;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 4;
        uint16_t dlen = 100;
        string description = "Physically the strongest of the female members. Her Nen specializes in using strings, which can be used to capture and manipulate enemies and take hostages.";
} __attribute__((packed));
// MONSTER 4
struct monster4
{
        uint8_t type = 10;
        char name[32] = "Nobunaga #1";
        uint8_t flag = 0xff;
        uint16_t attack = 30;
        uint16_t defense = 15;
        uint16_t regen = 20;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 5;
        uint16_t dlen = 100;
        string description = "Hand-to-hand combat expertise. Enhanced physical attributes Preternatural perception Swordsmanship ";
} __attribute__((packed));
// MONSTER 5
struct monster5
{
        uint8_t type = 10;
        char name[32] = "Phinks #5";
        uint8_t flag = 0xff;
        uint16_t attack = 82;
        uint16_t defense = 15;
        uint16_t regen = 37;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 6;
        uint16_t dlen = 100;
        string description = "Phinks is an Enhancer-type. While winding his arm in a clockwise direction, his physical power is getting stronger with every rotation.";
} __attribute__((packed));
// MONSTER 6
struct monster6
{
        uint8_t type = 10;
        char name[32] = "Illumi #10";
        uint8_t flag = 0xff;
        uint16_t attack = 72;
        uint16_t defense = 45;
        uint16_t regen = 10;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 7;
        uint16_t dlen = 100;
        string description = "Thanks to his training, he possesses rare abilities such as strong resistance to poison, high tolerance to electricity, great agility and physical strength, and advanced armed and unarmed combat techniques.";
} __attribute__((packed));
// MONSTER 7
struct monster7
{
        uint8_t type = 10;
        char name[32] = "Feitan #2";
        uint8_t flag = 0xff;
        uint16_t attack = 54;
        uint16_t defense = 55;
        uint16_t regen = 58;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 8;
        uint16_t dlen = 100;
        string description = "Feitan is the Phantom Troupe's member #2 and he is a interrogator, being very proficient in the art of torture.";
} __attribute__((packed));
// MONSTER 8
struct monster8
{
        uint8_t type = 10;
        char name[32] = "Uvogin #11";
        uint8_t flag = 0xff;
        uint16_t attack = 100;
        uint16_t defense = 28;
        uint16_t regen = 33;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 9;
        uint16_t dlen = 100;
        string description = "Phantom Troupe #11 and he is barbaric and treacherous in manner, viewing even pain merely as a nuisance. Like all members of the Phantom Troupe, Uvogin never hesitates to kill";
} __attribute__((packed));
// MONSTER 9
struct monster9
{
        uint8_t type = 10;
        char name[32] = "Hisoka #3";
        uint8_t flag = 0xff;
        uint16_t attack = 82;
        uint16_t defense = 55;
        uint16_t regen = 43;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 10;
        uint16_t dlen = 100;
        string description = " It is said that he named both of his techniques after a candy brand and a chewing gum brand he liked when he was a kid, implying that they may have been named 'Texture Surprise' and 'Bungee Gum' respectively";
} __attribute__((packed));
// MONSTER 10
struct monster10
{
        uint8_t type = 10;
        char name[32] = "Chrollo #0";
        uint8_t flag = 0xff;
        uint16_t attack = 100;
        uint16_t defense = 50;
        uint16_t regen = 10;
        int16_t health = 100;
        uint16_t gold = 0;
        uint16_t room_num = 10;
        uint16_t dlen = 93;
        string description = "founder, leader, and member #0 of the Phantom Troupe. His ability is to steal his past oponents techniques";
} __attribute__((packed));
struct room
{
        uint8_t type = 9;
        uint16_t num;
        char name[32];
        uint16_t dlen_length;
        string dlen;
        // room(uint16_t number, string d) : num(number), dlen(d) {}

} __attribute__((packed));
struct connection
{
        uint8_t type = 13;
        uint16_t room_num;
        char room_name[32];
        uint16_t desciption_length;
        string room_desciption;
} __attribute__((packed));

struct accept
{
        uint8_t type = 8;
        uint8_t type_accepted;
} __attribute__((packed));
struct reject
{
        uint8_t type = 7;
        uint8_t error_code;
        uint16_t err_msg_length;
        string err_msg;
} __attribute__((packed));
// struct connected {
//         uint8_t type = 10;

// }__attribute__((packed));
// server recieve type 6 then it starts
int skt;
// MULTIPLE CLIENT START
void shut_down_server(int signal)
{
        printf("\nDoing everything we need to do to shut down the server\n");
        close(skt);
        exit(0);
}
class client
{
public:
        int fd;
        string address;
        mutex write_mutex;
        struct character player;
        thread t;
        // struct player p;
        void run();
        /* This seems like a bit of a hack, but it solves a weird little problem.
         * We can't call run without an instance of a client.  When we're in the
         * constructor's initialization list, we don't have an instance of the class,
         * but we also want to initialize t and start it running.  So, we have to give
         * t a function that actually exists, and run doesn't, because there's not yet
         * a client for it to be associated with.  A static method is just a member
         * of class client, and doesn't require any particular instance of class client.
         * So t is happy to accept it as a function it can run.  But we still need a
         * reference to the class we're making.  "this" is a self-reference, a reference
         * to the specific instance of the client class we're creating.  So if we pass
         * it to start_own_thread, we can call the "run" method of the newly-created
         * class.  This happens AFTER the initialization list is executed, so the class
         * does actually exist when us->run() is called.
         */
        static void start_own_thread(client *us)
        {
                us->run();
        }
        client(int our_fd, string a) : fd(our_fd), t(start_own_thread, this), address(a) {}
} __attribute__((packed));

vector<client *> clients;

/* This will be used when we can't go changing around the list of clients */
mutex clients_mutex;

// clean clients at end
void cleanup_clients();

// struct room rooms;
// struct room rooms2;
// struct character player;
// struct connection room_connection;
// struct accept acpt;
// struct reject reject_CODE;
// struct sockaddr_in client_address;
// struct client_message cm;
// int client_fd;

void client::run()
{
        // struct character player;
        struct accept acpt;
        struct reject reject_CODE;
        struct sockaddr_in client_address;
        char buffer[1024];
        ssize_t len;
        bool reject = true;
        ////////////////////////////////
        // MONSTERS ///////////////////////
        //////////////////////////////////
        struct monster monster1;
        struct monster2 monster2;
        struct monster3 monster3;
        struct monster4 monster4;
        struct monster5 monster5;
        struct monster6 monster6;
        struct monster7 monster7;
        struct monster8 monster8;
        struct monster9 monster9;
        struct monster10 monster10;
        ////////////////////////////////
        // FLOORS ///////////////////////
        ////////////////////////////////
        struct room rooms;
        struct room rooms2;
        struct room rooms3;
        struct room rooms4;
        struct room rooms5;
        struct room rooms6;
        struct room rooms7;
        struct room rooms8;
        struct room rooms9;
        struct room rooms10;
        // Floor 1
        rooms.num = 1;
        strncpy(rooms.name, "Floor 1 ", 32);
        rooms.dlen = "Welcome to the starting room";
        rooms.dlen_length = rooms.dlen.length() + 1;
        // Floor 2
        rooms2.num = 2;
        strncpy(rooms2.name, "Floor 2 ", 32);
        rooms2.dlen = "second room";
        rooms2.dlen_length = rooms2.dlen.length() + 1;
        // Floor 3
        rooms3.num = 3;
        strncpy(rooms3.name, "Floor 3 ", 32);
        rooms3.dlen = "third room";
        rooms3.dlen_length = rooms3.dlen.length() + 1;
        // Floor 4
        rooms4.num = 4;
        strncpy(rooms4.name, "Floor 4 ", 32);
        rooms4.dlen = "4th room";
        rooms4.dlen_length = rooms4.dlen.length() + 1;
        // Floor 5
        rooms5.num = 5;
        strncpy(rooms5.name, "Floor 5 ", 32);
        rooms5.dlen = "5th room";
        rooms5.dlen_length = rooms5.dlen.length() + 1;
        // Floor 6
        rooms6.num = 6;
        strncpy(rooms6.name, "Floor 6 ", 32);
        rooms6.dlen = "6th room";
        rooms6.dlen_length = rooms6.dlen.length() + 1;
        // Floor 7
        rooms7.num = 7;
        strncpy(rooms7.name, "Floor 7 ", 32);
        rooms7.dlen = "7th room";
        rooms7.dlen_length = rooms7.dlen.length() + 1;
        // Floor 8
        rooms8.num = 8;
        strncpy(rooms8.name, "Floor 8 ", 32);
        rooms8.dlen = "8th room";
        rooms8.dlen_length = rooms8.dlen.length() + 1;
        // Floor 9
        rooms9.num = 9;
        strncpy(rooms9.name, "Floor 9 ", 32);
        rooms9.dlen = "9th room";
        rooms9.dlen_length = rooms9.dlen.length() + 1;
        // Floor 10
        rooms10.num = 10;
        strncpy(rooms10.name, "Floor 10 ", 32);
        rooms10.dlen = "10th room";
        rooms10.dlen_length = rooms10.dlen.length() + 1;
        // FLOORS END
        ////////////////////////////////
        // CONNECTION ////////////////
        //////////////////////////////////
        struct connection room_connection;
        struct connection room_connection2;
        struct connection room_connection3;
        struct connection room_connection4;
        struct connection room_connection5;
        struct connection room_connection6;
        struct connection room_connection7;
        struct connection room_connection8;
        struct connection room_connection9;
        struct connection room_connection10;
        // Connection 10
        room_connection10.room_num = 10;
        strncpy(room_connection10.room_name, "Connections: Room10", 32);
        room_connection10.room_desciption = "A portal? Should we see where it leads?";
        room_connection10.desciption_length = room_connection10.room_desciption.length() + 1;
        // Connection 9
        room_connection9.room_num = 9;
        strncpy(room_connection9.room_name, "Connections: Room9", 32);
        room_connection9.room_desciption = "Room 9, there is a monster in here unfortunately";
        room_connection9.desciption_length = room_connection9.room_desciption.length() + 1;
        // Connection 8
        room_connection8.room_num = 8;
        strncpy(room_connection8.room_name, "Connections: Room8", 32);
        room_connection8.room_desciption = "Room 8, a room with a chest, what awaits inside?";
        room_connection8.desciption_length = room_connection8.room_desciption.length() + 1;
        // Connection 7
        room_connection7.room_num = 7;
        strncpy(room_connection7.room_name, "Connections: Room7", 32);
        room_connection7.room_desciption = "Room 7 brings you underground, is that a good thing?";
        room_connection7.desciption_length = room_connection7.room_desciption.length() + 1;
        // Connection 6
        room_connection6.room_num = 6;
        strncpy(room_connection6.room_name, "Connections: Room6", 32);
        room_connection6.room_desciption = "Room 6 is lost in the sauce";
        room_connection6.desciption_length = room_connection6.room_desciption.length() + 1;
        // Connection 5
        room_connection5.room_num = 5;
        strncpy(room_connection5.room_name, "Connections: Room5", 32);
        room_connection5.room_desciption = "Room 5 is a break room with a bouncy castle, health boost";
        room_connection5.desciption_length = room_connection5.room_desciption.length() + 1;
        // Connection 4
        room_connection4.room_num = 4;
        strncpy(room_connection4.room_name, "Connections: Room4", 32);
        room_connection4.room_desciption = "WARNING: ROOM IS DARK, GET THERMAL GOGGLES IN ROOM 10";
        room_connection4.desciption_length = room_connection4.room_desciption.length() + 1;
        // Connection 3
        room_connection3.room_num = 3;
        strncpy(room_connection3.room_name, "Connections: Room3", 32);
        room_connection3.room_desciption = "A room where u swim";
        room_connection3.desciption_length = room_connection3.room_desciption.length() + 1;
        // Connection 2
        room_connection.room_num = 2;
        strncpy(room_connection.room_name, "Connections: Room2", 32);
        room_connection.room_desciption = "A room where u fly";
        room_connection.desciption_length = room_connection.room_desciption.length() + 1;
        // Connection 1
        room_connection2.room_num = 1;
        strncpy(room_connection2.room_name, "Connections: Room1 ", 32);
        room_connection2.room_desciption = "living room with black walls";
        room_connection2.desciption_length = room_connection2.room_desciption.length() + 1;
        // CONNECTION END
        // MESSAGE
        // strncpy(cm.receiver, "Message client Recieve", 32);
        // strncpy(cm.sender, "Message client Send", 30);
        // END MESSAGE
        for (;;)
        {
                // bool reject = true;
                struct server_message sm;

                uint8_t type;
                // MSG PEEK peaks at the byte but doesnt removve it from type
                recv(fd, &type, 1, MSG_PEEK);

                if (type == 0)
                {
                }
                else if (type == 1)
                {

                        recv(fd, &cm.type, 1, MSG_WAITALL);
                        recv(fd, &cm.discript_length, 66, MSG_WAITALL);
                        // recv(fd, cm.receiver, 32, MSG_WAITALL);
                        // recv(fd, cm.sender, 30, MSG_WAITALL);
                        // recv(fd, &cm.narration, 2, MSG_WAITALL);

                        // //narraction marker true or not
                        // bool is_narration = (cm.narration == 1);

                        cm.discription = (char *)malloc(cm.discript_length + 1);
                        recv(fd, cm.discription, cm.discript_length, MSG_WAITALL);

                        // Add a loop here:
                        for (int i = 0; i < clients.size(); i++)
                        {
                                if (strcmp(clients[i]->player.name, cm.receiver) == 0) // Check the client the loop is on
                                {
                                        clients_mutex.lock(); // Lock the mutex for the client we're sending the message to, not our own
                                        printf("Message was sent by: %s|\t And Recipient was: %s\n", cm.sender, cm.receiver);
                                        write(clients[i]->fd, &cm.type, 1); // Use the file descriptor for the client we're sending the message to, not our own
                                        write(clients[i]->fd, &cm.discript_length, 2);
                                        write(clients[i]->fd, cm.receiver, 32);
                                        write(clients[i]->fd, cm.sender, 32);
                                        write(clients[i]->fd, cm.discription, cm.discript_length);
                                        clients_mutex.unlock();
                                }
                        }
                        // End of the new loop

                        printf("Type: %u\n", cm.type);
                        printf("Recipient: %s\n", cm.receiver);
                        printf("Sender: %s\n", cm.sender);
                        // printf("Narration: %u\n", cm.narration);
                        printf("Message: %s\n", cm.discription);
                }
                else if (type == 2)
                {
                        int old_room = player.room_num;
                        recv(fd, &type, 1, MSG_WAITALL);
                        recv(fd, &player.room_num, 2, MSG_WAITALL);
                        int current_room = player.room_num;
                        printf("Player switches to: %d\t Player Name: %s\t CURRENT room: %d\n", player.room_num, player.name, current_room);
                        printf("===================================YOUR CURRENT STATS==================================================================================================================\n");
                        printf("\n");
                        printf("Health: %d\t Attack: %d\t Regen: %d\t Defense: %d\t Gold: %d\n", player.health, player.attack, player.regen, player.defense, player.power);
                        printf("\n");
                        printf("=======================================================================================================================================================================\n");

                        if (current_room == 1)
                        {
                                write_mutex.lock();
                                acpt.type_accepted = 2;
                                // player.room_num = 1;
                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms, 37);
                                write(fd, rooms.dlen.c_str(), rooms.dlen_length);
                                write(fd, &room_connection, 37);
                                write(fd, room_connection.room_desciption.c_str(), room_connection.desciption_length);

                                write_mutex.unlock();
                        }
                        else if (current_room == 2)
                        {
                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();
                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms2, 37);
                                write(fd, rooms2.dlen.c_str(), rooms2.dlen_length);
                                // monster 1
                                write(fd, &monster1, 48);
                                write(fd, monster1.description.c_str(), monster1.dlen);

                                // connects to 1
                                write(fd, &room_connection2, 37);
                                write(fd, room_connection2.room_desciption.c_str(), room_connection2.desciption_length);
                                // connects to 3
                                write(fd, &room_connection3, 37);
                                write(fd, room_connection3.room_desciption.c_str(), room_connection3.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 3)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms3, 37);
                                write(fd, rooms3.dlen.c_str(), rooms3.dlen_length);
                                // monster 2
                                write(fd, &monster2, 48);
                                write(fd, monster2.description.c_str(), monster2.dlen);

                                // connects to 2
                                write(fd, &room_connection, 37);
                                write(fd, room_connection.room_desciption.c_str(), room_connection.desciption_length);
                                // connects to 4
                                write(fd, &room_connection4, 37);
                                write(fd, room_connection4.room_desciption.c_str(), room_connection4.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 4)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms4, 37);
                                write(fd, rooms4.dlen.c_str(), rooms4.dlen_length);
                                // monster 3
                                write(fd, &monster3, 48);
                                write(fd, monster3.description.c_str(), monster3.dlen);
                                // connected to room 5
                                write(fd, &room_connection5, 37);
                                write(fd, room_connection5.room_desciption.c_str(), room_connection5.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 5)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms5, 37);
                                write(fd, rooms5.dlen.c_str(), rooms5.dlen_length);
                                // monster 4
                                write(fd, &monster4, 48);
                                write(fd, monster4.description.c_str(), monster4.dlen);
                                // connected to room 6
                                write(fd, &room_connection6, 37);
                                write(fd, room_connection6.room_desciption.c_str(), room_connection6.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 6)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms6, 37);
                                write(fd, rooms6.dlen.c_str(), rooms6.dlen_length);
                                // monster 5
                                write(fd, &monster5, 48);
                                write(fd, monster5.description.c_str(), monster5.dlen);

                                // connected to room 7
                                write(fd, &room_connection7, 37);
                                write(fd, room_connection7.room_desciption.c_str(), room_connection7.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 7)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms7, 37);
                                write(fd, rooms7.dlen.c_str(), rooms7.dlen_length);
                                // monster 8
                                write(fd, &monster6, 48);
                                write(fd, monster6.description.c_str(), monster6.dlen);
                                // connected to room 8
                                write(fd, &room_connection8, 37);
                                write(fd, room_connection8.room_desciption.c_str(), room_connection8.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 8)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms8, 37);
                                write(fd, rooms8.dlen.c_str(), rooms8.dlen_length);
                                // monster 9
                                write(fd, &monster7, 48);
                                write(fd, monster7.description.c_str(), monster7.dlen);
                                // connected to room 9
                                write(fd, &room_connection9, 37);
                                write(fd, room_connection9.room_desciption.c_str(), room_connection9.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 9)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms9, 37);
                                write(fd, rooms9.dlen.c_str(), rooms9.dlen_length);
                                // monster 10
                                write(fd, &monster10, 48);
                                write(fd, monster10.description.c_str(), monster10.dlen);
                                // connected to room 10
                                write(fd, &room_connection10, 37);
                                write(fd, room_connection10.room_desciption.c_str(), room_connection10.desciption_length);
                                write_mutex.unlock();
                        }
                        else if (current_room == 10)
                        {

                                acpt.type_accepted = 2;
                                // player.room_num = 2;
                                write_mutex.lock();

                                write(fd, &acpt, 2);
                                write(fd, &player, 48);
                                write(fd, player.dlen, player.dlen_length);
                                write(fd, &rooms10, 37);
                                write(fd, rooms10.dlen.c_str(), rooms10.dlen_length);
                                // connects to 1
                                write(fd, &room_connection2, 37);
                                write(fd, room_connection2.room_desciption.c_str(), room_connection2.desciption_length);
                                // connects to 9
                                write(fd, &room_connection9, 37);
                                write(fd, room_connection9.room_desciption.c_str(), room_connection9.desciption_length);
                                write_mutex.unlock();
                        }
                        else
                        {
                                reject_CODE.error_code = 1;
                                reject_CODE.err_msg = "\nCANNOT CHANGE ROOM\n";
                                reject_CODE.err_msg_length = reject_CODE.err_msg.length();
                                write_mutex.lock();
                                write(fd, &reject_CODE, 1);
                                write(fd, reject_CODE.err_msg.c_str(), reject_CODE.err_msg_length);
                                write_mutex.unlock();
                        }
                        clients_mutex.lock();

                        for (int i = 0; i < clients.size(); i++)
                        {
                                if (clients[i]->player.room_num == current_room)
                                {
                                        write_mutex.lock();
                                        write(clients[i]->fd, &player, 48);
                                        write(clients[i]->fd, player.dlen, player.dlen_length);

                                        write(fd, &clients[i]->player.type, 1);
                                        write(fd, &clients[i]->player.name, 47);
                                        write(fd, &clients[i]->player.dlen, clients[i]->player.dlen_length);
                                        write_mutex.unlock();
                                }
                                if (clients[i]->player.room_num == old_room)
                                {
                                        write_mutex.lock();
                                        write(clients[i]->fd, &player, 48);
                                        write(clients[i]->fd, player.dlen, player.dlen_length);

                                        write(fd, &clients[i]->player.type, 1);
                                        write(fd, &clients[i]->player.name, 47);
                                        write(fd, &clients[i]->player.dlen, clients[i]->player.dlen_length);
                                        write_mutex.unlock();
                                }
                        }
                        clients_mutex.unlock();
                }

                else if (type == 3)
                {
                        recv(fd, &type, 1, MSG_WAITALL);

                        if (player.room_num == 2)
                        {
                                if (monster1.flag & 0x80)
                                {
                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster1.defense;
                                        int enemy_damage = monster1.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster1.health -= player_damage;
                                                write_mutex.lock();
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                write_mutex.unlock();
                                                // write(fd, &monster1, 48);
                                                // write(fd, monster1.description.c_str(), monster1.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster1.name);
                                        }
                                        else
                                        {
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster1.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster1.health < 0)
                                        {
                                                player.health = player.health + player.regen;

                                                monster1.flag &= (~0x80);
                                                write_mutex.lock();
                                                write(fd, &monster1, 48);
                                                write(fd, monster1.description.c_str(), monster1.dlen);
                                                write_mutex.unlock();

                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster1.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster1, 48);
                                                                write(clients[i]->fd, monster1.description.c_str(), monster1.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage > 0)
                                        {
                                                player.health -= enemy_damage;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write_mutex.lock();

                                                write(fd, &monster1, 48);
                                                write(fd, monster1.description.c_str(), monster1.dlen);
                                                write_mutex.unlock();

                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster1.name, enemy_damage, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write_mutex.lock();

                                                write(fd, &monster1, 48);
                                                write(fd, monster1.description.c_str(), monster1.dlen);
                                                write_mutex.unlock();

                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster1.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write_mutex.lock();

                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                write_mutex.unlock();

                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster1.name, player.name);
                                        }
                                }
                        }
                        // IF ROOM 3
                        if (player.room_num == 3)
                        {
                                if (monster2.flag & 0x80)
                                {

                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster2.defense;
                                        int enemy_damage2 = monster2.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster2.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster2, 48);
                                                // write(fd, monster2.description.c_str(), monster2.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster2.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster2, 48);
                                                // write(fd, monster2.description.c_str(), monster2.dlen);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster2.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster2.health <= 0)
                                        {
                                                player.health = player.health + player.regen;
                                                monster2.flag &= (~0x80);
                                                write(fd, &monster2, 48);
                                                write(fd, monster2.description.c_str(), monster2.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster2.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster2, 48);
                                                                write(clients[i]->fd, monster2.description.c_str(), monster2.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage2 > 0)
                                        {
                                                player.health -= enemy_damage2;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster2, 48);
                                                write(fd, monster2.description.c_str(), monster2.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster2.name, enemy_damage2, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster2, 48);
                                                write(fd, monster2.description.c_str(), monster2.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster2.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster2.name, player.name);
                                        }
                                }
                        }
                        // IF ROOM 4
                        if (player.room_num == 4)
                        {
                                if (monster3.flag & 0x80)
                                {
                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster3.defense;
                                        int enemy_damage3 = monster3.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster3.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster3, 48);
                                                // write(fd, monster3.description.c_str(), monster3.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster3.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster3.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster3.health < 0)
                                        {
                                                player.health = player.health + player.regen;
                                                monster3.flag &= (~0x80);
                                                write(fd, &monster3, 48);
                                                write(fd, monster3.description.c_str(), monster3.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster3.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster3, 48);
                                                                write(clients[i]->fd, monster3.description.c_str(), monster3.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage3 > 0)
                                        {
                                                player.health -= enemy_damage3;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster3, 48);
                                                write(fd, monster3.description.c_str(), monster3.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster3.name, enemy_damage3, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster3, 48);
                                                write(fd, monster3.description.c_str(), monster3.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster3.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster3.name, player.name);
                                        }
                                }
                        }
                        // PLAYER ROOM 5
                        if (player.room_num == 5)
                        {
                                if (monster4.flag & 0x80)
                                {

                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster4.defense;
                                        int enemy_damage4 = monster4.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster4.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster4, 48);
                                                // write(fd, monster4.description.c_str(), monster4.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster4.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster4.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster4.health <= 0)
                                        {
                                                player.health = player.health + player.regen;
                                                monster4.flag &= (~0x80);
                                                write(fd, &monster4, 48);
                                                write(fd, monster4.description.c_str(), monster4.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster4.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster4, 48);
                                                                write(clients[i]->fd, monster4.description.c_str(), monster4.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage4 > 0)
                                        {
                                                player.health -= enemy_damage4;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster4, 48);
                                                write(fd, monster4.description.c_str(), monster4.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster4.name, enemy_damage4, player.name);
                                        }
                                        else
                                        {
                                                write(fd, &monster4, 48);
                                                write(fd, monster4.description.c_str(), monster4.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster4.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster4.name, player.name);
                                        }
                                }
                        }
                        // PLAYER ROOM 5
                        if (player.room_num == 6)
                        {
                                if (monster5.flag & 0x80)
                                {

                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster5.defense;
                                        int enemy_damage5 = monster5.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster5.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster5, 48);
                                                // write(fd, monster5.description.c_str(), monster5.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster5.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster5.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster5.health <= 0)
                                        {
                                                player.health = player.health + player.regen;

                                                monster5.flag &= (~0x80);
                                                write(fd, &monster5, 48);
                                                write(fd, monster5.description.c_str(), monster5.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster5.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster5, 48);
                                                                write(clients[i]->fd, monster5.description.c_str(), monster5.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage5 > 0)
                                        {
                                                player.health -= enemy_damage5;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster5, 48);
                                                write(fd, monster5.description.c_str(), monster5.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster5.name, enemy_damage5, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster5, 48);
                                                write(fd, monster5.description.c_str(), monster5.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster5.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster5.name, player.name);
                                        }
                                }
                        }
                        if (player.room_num == 7)
                        {
                                if (monster6.flag & 0x80)
                                {

                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster6.defense;
                                        int enemy_damage6 = monster6.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster6.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster6, 48);
                                                // write(fd, monster6.description.c_str(), monster6.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster6.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster6.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster6.health <= 0)
                                        {
                                                player.health = player.health + player.regen;

                                                monster6.flag &= (~0x80);
                                                write(fd, &monster6, 48);
                                                write(fd, monster6.description.c_str(), monster6.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster6.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster6, 48);
                                                                write(clients[i]->fd, monster6.description.c_str(), monster6.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage6 > 0)
                                        {
                                                player.health -= enemy_damage6;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster6, 48);
                                                write(fd, monster6.description.c_str(), monster6.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster6.name, enemy_damage6, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster6, 48);
                                                write(fd, monster6.description.c_str(), monster6.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster6.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster6.name, player.name);
                                        }
                                }
                        }
                        if (player.room_num == 8)
                        {
                                if (monster7.flag & 0x80)
                                {

                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster7.defense;
                                        int enemy_damage7 = monster7.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster7.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster7, 48);
                                                // write(fd, monster7.description.c_str(), monster7.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster7.name);
                                        }
                                        else
                                        {
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster7.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster7.health <= 0)
                                        {
                                                player.health = player.health + player.regen;

                                                monster7.flag &= (~0x80);
                                                write(fd, &monster7, 48);
                                                write(fd, monster7.description.c_str(), monster7.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster7.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster7, 48);
                                                                write(clients[i]->fd, monster7.description.c_str(), monster7.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage7 > 0)
                                        {
                                                player.health -= enemy_damage7;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster7, 48);
                                                write(fd, monster7.description.c_str(), monster7.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster7.name, enemy_damage7, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster7, 48);
                                                write(fd, monster7.description.c_str(), monster7.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster7.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster7.name, player.name);
                                        }
                                }
                        }
                        if (player.room_num == 9)
                        {
                                if (monster8.flag & 0x80)
                                {

                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster8.defense;
                                        int enemy_damage8 = monster8.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster8.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster8, 48);
                                                // write(fd, monster8.description.c_str(), monster8.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster8.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster8.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster8.health <= 0)
                                        {
                                                player.health = player.health + player.regen;

                                                monster8.flag &= (~0x80);
                                                write(fd, &monster8, 48);
                                                write(fd, monster8.description.c_str(), monster8.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster8.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster8, 48);
                                                                write(clients[i]->fd, monster8.description.c_str(), monster8.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage8 > 0)
                                        {
                                                player.health -= enemy_damage8;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster8, 48);
                                                write(fd, monster8.description.c_str(), monster8.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster8.name, enemy_damage8, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster8, 48);
                                                write(fd, monster8.description.c_str(), monster8.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster8.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster8.name, player.name);
                                        }
                                }
                        }
                        if (player.room_num == 10)
                        {
                                if (monster9.flag & 0x80)
                                {
                                        // damage the player and enemy inflict on each other
                                        int player_damage = player.attack - monster9.defense;
                                        int enemy_damage9 = monster9.attack - player.defense;

                                        // If the player's damage is greater than 0, subtract it from the enemy's health
                                        if (player_damage > 0)
                                        {
                                                monster9.health -= player_damage;
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                // write(fd, &monster9, 48);
                                                // write(fd, monster9.description.c_str(), monster9.dlen);
                                                printf("Player: %s\t Does damage: %d\t, ENEMY: %s\n", player.name, player_damage, monster9.name);
                                        }
                                        else
                                        {
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("Player: %s\t Does NO damage to ENEMY: %s\n", player.name, monster9.name);
                                        }

                                        // If the enemy's health reaches 0, the player wins
                                        if (monster9.health <= 0)
                                        {
                                                player.health = player.health + player.regen;

                                                monster9.flag &= (~0x80);
                                                monster10.flag &= (~0x80);
                                                write(fd, &monster9, 48);
                                                write(fd, monster9.description.c_str(), monster9.dlen);
                                                printf("Player: %s\t HAS DEFEATED: %s\n", player.name, monster9.name);
                                                for (int i = 0; i < clients.size(); i++)
                                                {
                                                        if (clients[i]->player.room_num == player.room_num)
                                                        {
                                                                write_mutex.lock();
                                                                write(clients[i]->fd, &monster9, 48);
                                                                write(clients[i]->fd, monster9.description.c_str(), monster9.dlen);
                                                                write_mutex.unlock();
                                                        }
                                                }
                                        }

                                        // If the enemy's damage is greater than 0, subtract it from the player's health
                                        if (enemy_damage9 > 0)
                                        {
                                                player.health -= enemy_damage9;
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster9, 48);
                                                write(fd, monster9.description.c_str(), monster9.dlen);
                                                printf("Enemy: %s\t Does damage: %d\t, PLayer: %s\n", monster9.name, enemy_damage9, player.name);
                                        }
                                        else
                                        {
                                                // write(fd, &player, 48);
                                                // write(fd, player.dlen, player.dlen_length);
                                                write(fd, &monster9, 48);
                                                write(fd, monster9.description.c_str(), monster9.dlen);
                                                printf("Enemy: %s\t Does NO damage to Player: %s\n", monster9.name, player.name);
                                        }

                                        // If the player's health reaches 0, the enemy wins
                                        if (player.health <= 0)
                                        {
                                                player.flags &= (~0x80);
                                                write(fd, &player, 48);
                                                write(fd, player.dlen, player.dlen_length);
                                                printf("ENEMY: %s\t HAS DEFEATED PLAYER: %s\n", monster9.name, player.name);
                                        }
                                }
                        }
                }
                else if (type == 6)
                {
                        recv(fd, &type, 1, MSG_WAITALL);

                        write_mutex.lock();

                        write(fd, &player, 48);
                        write(fd, player.dlen, player.dlen_length);
                        write(fd, &rooms, 37);
                        write(fd, rooms.dlen.c_str(), rooms.dlen_length);
                        write(fd, &room_connection, 37);
                        write(fd, room_connection.room_desciption.c_str(), room_connection.desciption_length);

                        write_mutex.unlock();
                }
                else if (type == 10)
                {
                        while (reject)
                        {

                                recv(fd, &player, 48, MSG_WAITALL);
                                player.dlen = (char *)malloc(player.dlen_length + 1);
                                recv(fd, player.dlen, player.dlen_length, MSG_WAITALL);

                                player.health = 100;
                                player.power = 50;
                                player.room_num = 1;
                                player.flags &= (~0x20);

                                uint16_t stats = player.attack + player.defense + player.regen;

                                if (stats <= 400)
                                {
                                        acpt.type_accepted = 10;

                                        write_mutex.lock();
                                        // reject == false causes seg fault
                                        write(fd, &acpt, 2);
                                        write(fd, &player, 48);
                                        write(fd, player.dlen, player.dlen_length);
                                        write(fd, &rooms, 37);
                                        write(fd, rooms.dlen.c_str(), rooms.dlen_length);
                                        write(fd, &room_connection, 37);
                                        write(fd, room_connection.room_desciption.c_str(), room_connection.desciption_length);
                                        write_mutex.unlock();
                                        reject = false;
                                }
                                else
                                {
                                        reject_CODE.error_code = 4;
                                        reject_CODE.err_msg = "\nCharacter is OVER 9000!\n";
                                        reject_CODE.err_msg_length = reject_CODE.err_msg.length();
                                        write_mutex.lock();
                                        write(fd, &reject_CODE, 4);
                                        write(fd, reject_CODE.err_msg.c_str(), reject_CODE.err_msg_length);
                                        write_mutex.unlock();
                                }
                                // reject = false;

                                // Might as well print the name, since we have it.  It'll be 1 byte in, right after the type
                                printf("Name:  %s\n", player.name);
                                // The next line is just so we don't have to keep typing character_beginning[33]
                                uint8_t flags = player.flags;
                                printf("Flags:  %x\n", flags);
                                /* This method, with the "if", works fine, but takes four lines */
                                if (flags & 0x80)
                                        printf("Alive:  True\n");
                                else
                                        printf("Alive:  False\n");
                                printf("Join Battle:  %s\n", (flags & 0x40) ? "True" : "False");
                                printf("Monster:  %s\n", (flags & 0x20) ? "True" : "False");
                                printf("Started:  %s\n", (flags & 0x10) ? "True" : "False");
                                printf("Ready:  %s\n", (flags & 0x08) ? "True" : "False");
                                /* The bit sizes of these fields must total 8, same as our uint8_t */
                                struct flag_byte
                                {
                                        uint8_t reserved : 3;
                                        uint8_t ready : 1;
                                        uint8_t started : 1;
                                        uint8_t monster : 1;
                                        uint8_t join_battle : 1;
                                        uint8_t alive : 1;
                                };                                                                    // This works fine without being packed, but it wouldn't be a bad idea
                                printf("sizeof(struct flag_byte) = %lu\n", sizeof(struct flag_byte)); // size is 1 byte
                                struct flag_byte fb;
                                memcpy(&fb, &flags, 1); // Because C++ didn't like fb = flags;
                                if (fb.alive)           // Same as above, we just use fb.alive instead of flags & 0x80
                                        printf("Alive:  True\n");
                                else
                                        printf("Alive:  False\n");
                                printf("\nJoin Battle:  %s\n", (fb.join_battle) ? "True" : "False");
                                printf("Monster:  %s\n", (fb.monster) ? "True" : "False");
                                printf("Started:  %s\n", (fb.started) ? "True" : "False");
                                printf("Ready:  %s\n", (fb.ready) ? "True" : "False");

                                printf("\nName:  %s\n", player.name);
                                printf("Attack:  %d\n", player.attack);
                                printf("defense:  %d\n", player.defense);
                                printf("Regen:  %d\n", player.regen);
                                printf("Health:  %d\n", player.health);
                                printf("Power Up:  %d\n", player.power);
                                printf("Current Room:  %d\n", player.room_num);
                                printf("Description Length:  %d\n", player.dlen_length);
                                printf("Description:  %s\n", player.dlen);
                        }
                        reject = true;
                }
        }
        // reject = true;
        // c->write_mutex.unlock();
        printf("Done with client %d(from address %s)\n", fd, address.c_str());
        // it'd be cool if we actually freed up this place in client_fds
        close(fd);
        fd = -1;
        /* This is actually kind of a problem! */
        // cleanup_clients(); // would join us from inside ourselves
}
void cleanup_clients()
{
        clients_mutex.lock();
        for (int i = 0; i < clients.size(); i++)
        {
                if (clients[i]->fd == -1)
                {
                        // delete that client
                        clients[i]->t.join();
                        delete clients[i];
                        clients[i] = clients[clients.size() - 1];
                        clients.pop_back();
                }
        }
        clients_mutex.unlock();
}

int main(int argc, char **argv)
{
        uint16_t listen_port = 5025; // 5025 is the default
        if (argc > 1)
        {
                listen_port = atoi(argv[1]);
                if (listen_port < 1)
                {
                        printf("Invalid port:  %s\n", argv[1]);
                        exit(1);
                }
                printf("Will listen on port %d\n", listen_port);
        }
        else
        {
                printf("No port specified, defaulting to %d\n", listen_port);
        }
        struct sigaction sa;
        sa.sa_handler = shut_down_server;
        sigaction(SIGINT, &sa, 0);

        struct sockaddr_in sad;
        sad.sin_port = htons(listen_port);
        sad.sin_addr.s_addr = INADDR_ANY;
        sad.sin_family = AF_INET;

        skt = socket(AF_INET, SOCK_STREAM, 0); // Step 1
        if (skt == -1)
        {
                perror("socket");
                return 1;
        }
        if (bind(skt, (struct sockaddr *)(&sad), sizeof(struct sockaddr_in)))
        { // step 2
                perror("bind");
                return 1;
        }
        if (listen(skt, 5))
        { // step 3
                perror("listen");
                return 1;
        }
        int client_fd;
        struct sockaddr_in client_address;
        socklen_t address_size = sizeof(struct sockaddr_in);
        version our_version(6, 17);
        game our_game(400, 10000, "Welcome to Heavens arena, fight to the death until on the top floor<3");

        // bool reject = true;
        // void run();

        while (1)
        {
                client_fd = accept(skt, (struct sockaddr *)(&client_address), &address_size); // step 4
                printf("Connection made from address %s\n", inet_ntoa(client_address.sin_addr));
                our_version.send(client_fd);
                our_game.send(client_fd);

                clients_mutex.lock();
                clients.push_back(new client(client_fd, inet_ntoa(client_address.sin_addr)));
                clients_mutex.unlock();
                printf("Currently we have %lu clients\n", clients.size());
                cleanup_clients();
        }
        return 0;
}
