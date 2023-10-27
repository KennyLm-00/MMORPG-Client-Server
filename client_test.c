#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <gtk/gtk.h>
#define BUFFER_SIZE 1024
int sockfd;

/*
GTK add idle 
GTK chat client clas examples under s22 examples
for when I not pressing buttons
multi threaded so 
*/
// Declare builder as a global variable
GtkBuilder *builder;
struct version {
	uint8_t mtype;
	uint8_t major, minor;
	uint16_t elen;
} __attribute__((packed));

struct game_header {
	uint8_t mtype;
	uint16_t initial_points, stat_limit, description;
} __attribute__((packed));

struct character
{
        uint8_t type;
        char name[32];
        uint8_t flags;
        uint16_t attack;
        uint16_t defense;
        uint16_t regen;
        int16_t health;
        uint16_t power;
        uint16_t room_num;
        uint16_t description_length;
        char *description;
} __attribute__((packed));
struct monster
{
        uint8_t type;
        char name[32];
        uint8_t flag;
        uint16_t attack;
        uint16_t defense;
        uint16_t regen;
        int16_t health;
        uint16_t gold;
        uint16_t room_num;
        uint16_t dlen;
        char *description;
} __attribute__((packed));
struct connection
{
        uint8_t type;
        uint16_t room_num;
        char room_name[32];
        uint16_t desciption_length;
        char *room_desciption;
} __attribute__((packed));
struct room
{
        uint8_t type;
        uint16_t num;
        char name[32];
        uint16_t description_length;
        char* description;
        // room(uint16_t number, string d) : num(number), description(d) {}

} __attribute__((packed));
struct change_room
{
	uint8_t type;
	uint16_t room_num;

} __attribute__((packed));
struct accept
{
        uint8_t type;
        uint8_t type_accepted;
} __attribute__((packed));
struct reject
{
        uint8_t type;
        uint8_t error_code;
        uint16_t err_msg_length;
        char* err_msg;
} __attribute__((packed));
struct client_message {
    uint8_t type;
    uint16_t discript_length;
    char receiver[32];
    char sender[32];
    // uint16_t narration;
    char* discription;
} __attribute__((packed));
/*
sending messages to one another
fighting monsters sending a 3 then server does all the work. Then server sending back update and recv 
*/
// Define the thread function
struct client_message cm;
struct reject reject_CODE;
struct connection room_connect;
void* listen_for_updates(void* data) {
     GtkTextView* text_view = GTK_TEXT_VIEW(data);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    
   uint8_t type;
    struct character new_character;
    struct room rooms;
    while (1) {
        // 
        recv(sockfd, &type, 1, MSG_PEEK);
        
        if(type == 1){
            recv(sockfd, &cm.type, 1, MSG_WAITALL);
            recv(sockfd, &cm.discript_length, 66, MSG_WAITALL);
            cm.discription = (char *)malloc(cm.discript_length + 1);
            recv(sockfd, cm.discription, cm.discript_length, MSG_WAITALL);
            char stats[1024];
            snprintf(stats, sizeof(stats), "Recipient: %s, Sender: %s, Message: %s\n",cm.receiver, cm.sender, cm.discription);
            gtk_text_buffer_insert(buffer, &end, stats, -1);
        }
        else if(type == 7){
            //error
                recv(sockfd, &reject_CODE, 5, MSG_WAITALL);
                reject_CODE.err_msg = (char *)malloc(reject_CODE.err_msg_length + 1);
                recv(sockfd, reject_CODE.err_msg, reject_CODE.err_msg_length, MSG_WAITALL);

            printf("type denied: %d\n\n", type);
        }
        else if(type == 8){
            //accept I will be recieving 
            recv(sockfd, &type, 1, MSG_WAITALL);
            recv(sockfd, &type, 1, MSG_WAITALL);

            printf("type accepted: %d\n\n\n", type);

        }
         else if(type == 9){
                recv(sockfd, &rooms, 37, MSG_WAITALL);
                rooms.description = (char *)malloc(rooms.description_length + 1);
                recv(sockfd, rooms.description, rooms.description_length, MSG_WAITALL);
                rooms.description[rooms.description_length] = '\0';
            gtk_text_buffer_get_end_iter(buffer, &end);
            char room_descript[1024];
            snprintf(room_descript, sizeof(room_descript), "Current Room:  %d, -- Room Name: %s, -- Description:  %s\n",rooms.num, rooms.name, rooms.description);
            gtk_text_buffer_insert(buffer, &end, room_descript, -1);
             gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);

            // Check if the length of the description is greater than the available space on the current line
            int available_space = gtk_widget_get_allocated_width(GTK_WIDGET(text_view)) - gtk_text_view_get_pixels_above_lines(GTK_TEXT_VIEW(text_view)) - gtk_text_view_get_pixels_below_lines(GTK_TEXT_VIEW(text_view)) - gtk_text_view_get_left_margin(GTK_TEXT_VIEW(text_view)) - gtk_text_view_get_right_margin(GTK_TEXT_VIEW(text_view));
            int desc_length = strlen(room_descript);
            if (desc_length > available_space) {
                // Print the description on a new line
                gtk_text_buffer_insert(buffer, &end, "\n", -1);
            }
            
            gtk_text_buffer_insert(buffer, &end, room_descript, -1);
            

        }
         else if(type == 10){
                recv(sockfd, &new_character, 48, MSG_WAITALL);
                new_character.description = (char *)malloc(new_character.description_length + 1);
                recv(sockfd, new_character.description, new_character.description_length, MSG_WAITALL);
                // printf("\nName:  %s\n", new_character.name);
                // printf("Attack:  %d\n", new_character.attack);
                // printf("defense:  %d\n", new_character.defense);
                // printf("Regen:  %d\n", new_character.regen);
                // printf("Health:  %d\n", new_character.health);
                // printf("Power Up:  %d\n", new_character.power);
                // printf("Current Room:  %d\n", new_character.room_num);
                // printf("Description Length:  %d\n", new_character.description_length);
                // printf("Description:  %s\n", new_character.description);
            char player[1024];
            snprintf(player, sizeof(player), "Name: %s\t |Stats: Attack:  %d| Defense:  %d| Health: %d| Regen %d|\n",
            new_character.name, new_character.attack,new_character.defense, new_character.health, new_character.regen);
            int add_text(){
                if(new_character.health < 0){
                    gtk_text_buffer_insert(buffer, &end, "This Player has Died. Rip!\n", -1);
                }
                gtk_text_buffer_get_end_iter(buffer, &end);
                gtk_text_buffer_insert(buffer, &end, player, -1); 
            }
            gdk_threads_add_idle(add_text,0);
            usleep(10000);
          
        }

         else if(type == 13){
         //ROOM CONNECTIONS
                recv(sockfd, &room_connect, 37, MSG_WAITALL);
                room_connect.room_desciption = (char *)malloc(room_connect.desciption_length + 1);
                recv(sockfd, room_connect.room_desciption, room_connect.desciption_length, MSG_WAITALL);
            char roomz[1024];
            gtk_text_buffer_get_end_iter(buffer, &end);
            snprintf(roomz, sizeof(roomz), "Current Connection:  %d, --Room Name: %s, -- Description:  %s\n",room_connect.room_num, room_connect.room_name, room_connect.room_desciption);
            gtk_text_buffer_insert(buffer, &end, roomz, -1);
         }
            
    }
}
static void on_network_connect_clicked(GtkWidget *widget, gpointer data)
{
    GtkTextView* text_view = GTK_TEXT_VIEW(data);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    // struct character new_character;
    const gchar *hostname = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "hostname_entry")));
    const gchar *port_str = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "port_entry")));
    int port = atoi(port_str);
    
    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        gtk_text_buffer_insert(buffer, &end, "Error: could not create socket\n", -1);
        return;
    }
    
    // Resolve the hostname to an IP address
    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) {
        gtk_text_buffer_insert(buffer, &end, "Error: could not resolve hostname\n", -1);
        close(sockfd);
        return;
    }
    
    // Fill in the server address structure
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    
    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        gtk_text_buffer_insert(buffer, &end, "Error: could not connect to server\n", -1);
        close(sockfd);
        return;
    }
    
    // Connection successful
    gtk_text_buffer_insert(buffer, &end, "Connected to server!\n", -1);
    printf("sockfd: %d\n", sockfd);
    struct version	v;
    struct game_header gh;
    uint8_t type;
    if(1 != recv(sockfd, &type, 1, MSG_PEEK)) {
        gtk_text_buffer_insert(buffer, &end, "Error: server didn't follow protocol\n", -1);
        close(sockfd);
        return;
    }
    if(type == 14){
        if(sizeof(v) != recv(sockfd, &v, sizeof(v), MSG_WAITALL)) {
            gtk_text_buffer_insert(buffer, &end, "Error: server didn't follow protocol\n", -1);
            close(sockfd);
            return;
        }
        if(v.elen) {
            char ver[128];
            if(recv(sockfd, ver, sizeof(ver), MSG_WAITALL) != sizeof(ver)) {
                gtk_text_buffer_insert(buffer, &end, "Error: server didn't follow protocol\n", -1);
                close(sockfd);
                return;
            }
            snprintf(ver, sizeof(ver), "Server version is %d.%d\n", v.major, v.minor);

            // print the server version in the text buffer
            GtkTextIter end;
            gtk_text_buffer_get_end_iter(buffer, &end);
            gtk_text_buffer_insert(buffer, &end, ver, -1);
        }

        if(1 != recv(sockfd, &type, 1, MSG_PEEK)) {
            gtk_text_buffer_insert(buffer, &end, "Error: server didn't follow protocol\n", -1);
            close(sockfd);
            return;
        }
    
        else if(type == 11) {
            if(sizeof(gh) != recv(sockfd, &gh, sizeof(gh), MSG_WAITALL)){
            gtk_text_buffer_insert(buffer, &end, "Error: server didn't follow protocol\n", -1);
            close(sockfd);
            return;
            }
    char stats[128];
    snprintf(stats, sizeof(stats), "IP:  %d, Stat Limit:  %d, Description Length:  %d\n", gh.initial_points, gh.stat_limit, gh.description);
    gtk_text_buffer_insert(buffer, &end, stats, -1);

    char desc[gh.description + 1];
    if(recv(sockfd, desc, gh.description, MSG_WAITALL) != gh.description){
        gtk_text_buffer_insert(buffer, &end, "Error: server didn't follow protocol\n", -1);
          close(sockfd);
        return;
    }
    desc[gh.description] = '\0';

    // Set the wrap mode to word/character boundaries
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);

    // Check if the length of the description is greater than the available space on the current line
    int available_space = gtk_widget_get_allocated_width(GTK_WIDGET(text_view)) - gtk_text_view_get_pixels_above_lines(GTK_TEXT_VIEW(text_view)) - gtk_text_view_get_pixels_below_lines(GTK_TEXT_VIEW(text_view)) - gtk_text_view_get_left_margin(GTK_TEXT_VIEW(text_view)) - gtk_text_view_get_right_margin(GTK_TEXT_VIEW(text_view));
    int desc_length = strlen(desc);
    if (desc_length > available_space) {
        // Print the description on a new line
        gtk_text_buffer_insert(buffer, &end, "\n", -1);
    }
    
    gtk_text_buffer_insert(buffer, &end, desc, -1);
    
    }
    else {
        gtk_text_buffer_insert(buffer, &end, "Error: unknown message type\n", -1);
    }


    }   
}

static void make_character_clicked(GtkWidget *widget, gpointer data)
{
    GtkTextView* player_view = GTK_TEXT_VIEW(data);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(player_view);
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);

    
    struct character new_character;
    struct accept acpt;
    struct reject reject_CODE;
    uint8_t type;
    //  recv(sockfd, &type, 1, MSG_WAITALL);
    // recv(sockfd, &new_character, sizeof(new_character), MSG_WAITALL);
    new_character.type = 10;
    strncpy(new_character.name, "", sizeof(new_character.name));
    // new_character.flags = 0;
    // new_character.attack = 0;
    // new_character.defense = 0;
    new_character.regen = 0;
    new_character.health = 100;
    new_character.power = 0;
    new_character.room_num = 1;

    // Retrieve the character fields from the entry widgets
    // GtkWidget *player_name = GTK_WIDGET(gtk_builder_get_object(builder, "player_name"));
    const char *name = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "player_name")));
    strncpy(new_character.name, name, sizeof(new_character.name) - 1);

    // GtkWidget *player_flags = GTK_WIDGET(gtk_builder_get_object(builder, "player_flags"));
    const char *flags_text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "player_flags")));
    new_character.flags = atoi(flags_text);

    // GtkWidget *player_attack = GTK_WIDGET(gtk_builder_get_object(builder, "player_attack"));
    const char *attack_text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "player_attack")));    
    new_character.attack = atoi(attack_text);

    // GtkWidget *player_defense = GTK_WIDGET(gtk_builder_get_object(builder, "player_defense"));
    const char *defense_text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "player_defense")));    
    new_character.defense = atoi(defense_text);

    // GtkWidget *player_regen = GTK_WIDGET(gtk_builder_get_object(builder, "player_regen"));
    const char *regen_text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "player_regen")));    
    new_character.regen = atoi(regen_text);

    const char *description = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "player_description")));    
    new_character.description = description;
    new_character.description_length = strlen(new_character.description);


    printf("Character you have entered\n");    
    printf("|name:%s\n|flags:%s\n|attack:%s\n|defense:%s\n|regen:%s\n", name, flags_text, attack_text, defense_text, regen_text);
    printf("sockfd: %d\n", sockfd);
     // Connect to server
    // int sockfd = connect_button();
    if (sockfd < 0) {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "Error: could not connect to server\n", -1);
        return;
    }

    // Send the new_character structure to the server
    if (send(sockfd, &new_character, 48, 0) < 0) {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "Error: could not send character data to server\n", -1);
        return;
    }
    if(send(sockfd, new_character.description, new_character.description_length, 0) < 0){

       gtk_text_buffer_insert(buffer, &iter, "no descirption \n", -1);
    }else {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "----Character Made!!--\n", -1);
        gtk_text_buffer_get_end_iter(buffer, &iter);
        char character_str[256];
        snprintf(character_str, sizeof(character_str), "Name: %s\n--Attack: %d\n--Defense: %d\n--Regen: %d\n--Health: %d\n--Power: %d\n--Room Num: %d\n--Description: %s\n",
        new_character.name, new_character.attack, new_character.defense, new_character.regen, new_character.health, new_character.power, new_character.room_num, new_character.description);
        gtk_text_buffer_insert(buffer, &iter, character_str, -1);
    }

    //accept can send accept pr errpr if stats are to high
       uint16_t stats = new_character.attack + new_character.defense + new_character.regen;
    recv(sockfd, &type, 1, MSG_WAITALL);
        if (type == 8)
        {
          recv(sockfd, &type, 1, MSG_WAITALL);
            acpt.type_accepted = 10;
        printf("Character was accpeted!\n");
        }
        else
        {
          recv(sockfd, &type, 1, MSG_WAITALL);
            printf("Character bad\n");
            recv(sockfd, &reject_CODE.err_msg_length, 2, MSG_WAITALL);
                reject_CODE.err_msg = "\nCharacter is OVER 9000!\n";
                // reject_CODE.err_msg_length = reject_CODE.err_msg.length();    
        }
        pthread_t T;
	    pthread_create(&T, 0, listen_for_updates, data);

}
static void start_game_clicked(GtkWidget *widget, gpointer data){

    GtkTextView* text_view = GTK_TEXT_VIEW(data);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);
    struct room r;
    

    uint8_t six = 6;
    if (sockfd < 0) {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "Error: could not START GAME\n", -1);
        return;
    }

    // Send the new_character structure to the server
    if (send(sockfd, &six, 1, 0) < 0) {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "Error: could not START GAME\n", -1);
        return;
    }
    else {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "----Game Started!--\n", -1);
        gtk_text_buffer_get_end_iter(buffer, &iter);
    //         printf("Room number: %d\n", r.num);
    // printf("Room name: %s\n", r.name);
    // printf("Room description: %s\n", r.description);

    }

}
static void room_change_clicked(GtkWidget *widget, gpointer data){

    GtkTextView* room_view = GTK_TEXT_VIEW(data);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(room_view), FALSE);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(room_view);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    
    struct character new_character;
    struct room rooms;
    struct change_room change;
    // printf("Room number: %d\n", r.num);
    // printf("Room name: %s\n", r.name);
    // printf("Room description: %s\n", r.description);
    const char *room_change = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "room_number")));    
    rooms.num = atoi(room_change);
    // change.room_num = atoi(room_change);
    uint8_t two = 2;
    if (sockfd < 0) {
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "Error: could not  Room Change\n", -1);
        return;
    }

    // Send the  structure to the server
    if (send(sockfd, &two, 1, 0) < 0) {
        printf("am I in this not room\n\n\n\n\n");
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "Error: could not Room Change\n", -1);
        return;
    }
    else {
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "----Room Change Occured--\n", -1);
        gtk_text_buffer_get_end_iter(buffer, &end);
        char room_descript[1024];
        snprintf(room_descript, sizeof(room_descript), "Current Room:  %d\n",rooms.num);
        gtk_text_buffer_get_end_iter(buffer, &end);
        send(sockfd, &rooms.num, 2, 0);
        gtk_text_buffer_insert(buffer, &end, room_descript, -1);

    }

}
static void fight_button_clicked(GtkWidget *widget, gpointer data){

    GtkTextView* fight_view = GTK_TEXT_VIEW(data);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(fight_view), FALSE);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(fight_view);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    
 
    struct character new_character;
    uint8_t two = 3;
    if (sockfd < 0) {
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "Error: could not  intiate fight\n", -1);
        return;
    }

    // Send the  structure to the server
    if (send(sockfd, &two, 1, 0) < 0) {
        printf("am I in this not room\n\n\n\n\n");
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "Error: could not intiate fight\n", -1);
        return;
    }
    else {
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "---- Fight Occured!!--\n", -1);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
    
    // uint8_t type = 10;
    // if (send(sockfd, &type, 1, 0) < 0) {
    //     gtk_text_buffer_get_end_iter(buffer, &end);
    //     gtk_text_buffer_insert(buffer, &end, "Error: could not send type 10\n", -1);
    //     return;
    // }

    // // Receive the updated character information from the server
    // if (recv(sockfd, &new_character, sizeof(new_character), 0) < 0) {
    //     gtk_text_buffer_get_end_iter(buffer, &end);
    //     gtk_text_buffer_insert(buffer, &end, "Error: could not receive character information\n", -1);
    //     return;
    // }

    // Display updates in the GtkTextView
    // gtk_text_buffer_get_end_iter(buffer, &end);
    // gtk_text_buffer_insert(buffer, &end, "Fight updates:\n", -1);
    // char character_str[256];
    // snprintf(character_str, sizeof(character_str), "Name: %s\n--Attack: %d\n--Defense: %d\n--Regen: %d\n--Health: %d\n--Power: %d\n--Room Num: %d\n--Description: %s\n",
    // new_character.name, new_character.attack, new_character.defense, new_character.regen, new_character.health, new_character.power, new_character.room_num, new_character.description);
    // gtk_text_buffer_insert(buffer, &end, character_str, -1);

}

static void send_message_clicked(GtkWidget *widget, gpointer data) {
    GtkTextView *text_view = GTK_TEXT_VIEW(data);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);

    // Retrieve the recipient and message fields from the entry widgets
    const char *sender_name = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "sender_name")));
    const char *recipient_name = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "message_name")));
    const char *message = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "sendit")));

    // Set the recipient and sender fields in the message packet
    strcpy(cm.receiver, recipient_name);
    strcpy(cm.sender, sender_name);  // replace "Your Name" with the correct sender name
    strcpy(cm.discription, message);  // replace "Your Name" with the correct sender name
    printf("Sender: %s, Recipient: %s, Message: %s\n", cm.sender, cm.receiver, cm.discription);
    // Send the message packet to the server
    if (send(sockfd, &cm, 67, 0) < 0) {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "Error: could not send message to server\n", -1);
    } if(send(sockfd, message, cm.discript_length, 0) < 0){
        
    }
        else {
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, "Message sent successfully\n", -1);

        // Output the message to the chat window
        char stats[256];
        snprintf(stats, sizeof(stats), "Recipient: %s, Sender: %s, Message: %s\n", cm.receiver, cm.sender, cm.discription);
        gtk_text_buffer_insert(buffer, &iter, stats, -1);
    }
}



int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Load the GUI from the XML file
    builder = gtk_builder_new_from_file("example2.xml");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *connect_button = GTK_WIDGET(gtk_builder_get_object(builder, "connect_button"));
    GtkWidget *makecharacter = GTK_WIDGET(gtk_builder_get_object(builder, "make_character"));
    GtkWidget *startgame = GTK_WIDGET(gtk_builder_get_object(builder, "start_game"));
    GtkWidget *send_message = GTK_WIDGET(gtk_builder_get_object(builder, "send_message"));
    GtkWidget *changeroom = GTK_WIDGET(gtk_builder_get_object(builder, "change_room"));
    GtkWidget *text_view = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));
    GtkWidget *message_view = GTK_WIDGET(gtk_builder_get_object(builder, "m_recv"));
    GtkWidget *player_view = GTK_WIDGET(gtk_builder_get_object(builder, "playerview"));
    GtkWidget *room_view = GTK_WIDGET(gtk_builder_get_object(builder, "roomview"));
    GtkWidget *fight = GTK_WIDGET(gtk_builder_get_object(builder, "fight_button"));
    GtkWidget *fight_view = GTK_WIDGET(gtk_builder_get_object(builder, "fightview"));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(player_view), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(room_view), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(fight_view), FALSE);

//A way you can make character have its own little box is copying stats into two boxes so two make button signals
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(connect_button, "clicked", G_CALLBACK(on_network_connect_clicked), text_view);
    g_signal_connect(makecharacter, "clicked", G_CALLBACK(make_character_clicked), player_view);
    g_signal_connect(startgame, "clicked", G_CALLBACK(start_game_clicked), text_view);
    g_signal_connect(send_message, "clicked", G_CALLBACK(send_message_clicked), message_view);
    g_signal_connect(changeroom, "clicked", G_CALLBACK(room_change_clicked), room_view);
    g_signal_connect(fight, "clicked", G_CALLBACK(fight_button_clicked), fight_view);

GtkStyleContext *window_context = gtk_widget_get_style_context(window);
gtk_style_context_add_class(window_context, "background");
const char *window_css = "window { background: linear-gradient(to bottom, #3E6184, #396490); }";
GtkCssProvider *window_provider = gtk_css_provider_new();
gtk_css_provider_load_from_data(window_provider, window_css, -1, NULL);
gtk_style_context_add_provider(window_context, GTK_STYLE_PROVIDER(window_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

//button styling


GtkStyleContext *button_context = gtk_widget_get_style_context(connect_button);
gtk_style_context_add_class(button_context, "my-button-style");

const char *button_css = ".my-button-style { background: white; color: #177ccf; border: 3px solid lightblue;  font-weight: bold;} \
                          .my-button-style:hover { background: lightblue; color: white;}";
GtkCssProvider *button_provider = gtk_css_provider_new();
gtk_css_provider_load_from_data(button_provider, button_css, -1, NULL);
gtk_style_context_add_provider(button_context, GTK_STYLE_PROVIDER(button_provider),
                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
GtkStyleContext *button_context5 = gtk_widget_get_style_context(fight);
gtk_style_context_add_class(button_context5, "my-fight-style");

const char *button_css5 = ".my-fight-style { background: white; color: #177ccf; border: 1px solid lightblue;  font-weight: bold;} \
                          .my-fight-style:hover {  background: #177ccf; color: white; padding:0.8rem; transition:ease 0.5s;}";
GtkCssProvider *button_provider5 = gtk_css_provider_new();
gtk_css_provider_load_from_data(button_provider5, button_css5, -1, NULL);
gtk_style_context_add_provider(button_context5, GTK_STYLE_PROVIDER(button_provider5),
                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
//BUTTON 2
GtkStyleContext *button_context2 = gtk_widget_get_style_context(makecharacter);
gtk_style_context_add_class(button_context2, "my-button-style");

const char *button_css2 = ".my-button-style { background: white; color: #177ccf; border: 2px solid lightblue; } \
                          .my-button-style:hover { background: lightblue; color: white;}";
GtkCssProvider *button_provider2 = gtk_css_provider_new();
gtk_css_provider_load_from_data(button_provider2, button_css2, -1, NULL);
gtk_style_context_add_provider(button_context2, GTK_STYLE_PROVIDER(button_provider2),
                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
//Button 3
GtkStyleContext *button_context3 = gtk_widget_get_style_context(startgame);
gtk_style_context_add_class(button_context3, "my-button-style");

const char *button_css3 = ".my-button-style { background: linear-gradient(to bottom, #f3f6f6, #2a8eb9); font-weight: bold;color:white;} \
                          .my-button-style:hover { background: lightblue; color: white;}";
GtkCssProvider *button_provider3 = gtk_css_provider_new();
gtk_css_provider_load_from_data(button_provider3, button_css3, -1, NULL);
gtk_style_context_add_provider(button_context3, GTK_STYLE_PROVIDER(button_provider3),
                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

GtkWidget *textview_frame = gtk_builder_get_object(builder, "textview_frame");
GtkStyleContext *frame_context = gtk_widget_get_style_context(textview_frame);
gtk_style_context_add_class(frame_context, "my-frame-label");
const char *frame_css = ".my-frame-label { color: lightblue; font-weight:bold; font-size: 14px;}";
GtkCssProvider *frame_provider = gtk_css_provider_new();
gtk_css_provider_load_from_data(frame_provider, frame_css, -1, NULL);
gtk_style_context_add_provider(frame_context, GTK_STYLE_PROVIDER(frame_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

GtkWidget *playerview_frame = gtk_builder_get_object(builder, "playerview_frame");
GtkStyleContext *player_context = gtk_widget_get_style_context(playerview_frame);
gtk_style_context_add_class(player_context, "my-frame-label");
const char *player_css = ".my-frame-label { color: lightblue; font-weight:bold; font-size: 14px; border-bottom:1px lightgray solid;}";
GtkCssProvider *player_provider = gtk_css_provider_new();
gtk_css_provider_load_from_data(player_provider, player_css, -1, NULL);
gtk_style_context_add_provider(player_context, GTK_STYLE_PROVIDER(player_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

// GtkWidget *fightview2 = gtk_builder_get_object(builder, "fightview");
// GtkStyleContext *player_context7 = gtk_widget_get_style_context(fightview2);
// gtk_style_context_add_class(player_context7, "my-frame-label");
// const char *player_css7 = ".my-frame-label {   background: transparent;}";
// GtkCssProvider *player_provider7 = gtk_css_provider_new();
// gtk_css_provider_load_from_data(player_provider7, player_css7, -1, NULL);
// gtk_style_context_add_provider(player_context7, GTK_STYLE_PROVIDER(player_provider7),
//         GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


GtkWidget *roomview_frame = gtk_builder_get_object(builder, "roomview_frame");
GtkStyleContext *player_context2 = gtk_widget_get_style_context(roomview_frame);
gtk_style_context_add_class(player_context2, "my-frame-label");
const char *player_css2 = ".my-frame-label { color: white; font-weight:bold; }";
GtkCssProvider *player_provider2 = gtk_css_provider_new();
gtk_css_provider_load_from_data(player_provider2, player_css2, -1, NULL);
gtk_style_context_add_provider(player_context2, GTK_STYLE_PROVIDER(player_provider2),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

GtkWidget *roomview_frame3 = gtk_builder_get_object(builder, "monsterview_frame");
GtkStyleContext *room_context = gtk_widget_get_style_context(roomview_frame3);
gtk_style_context_add_class(room_context, "my-frame-label");
const char *player_css3 = ".my-frame-label { color: white; font-weight:bold; }";
GtkCssProvider *player_provider3 = gtk_css_provider_new();
gtk_css_provider_load_from_data(player_provider3, player_css3, -1, NULL);
gtk_style_context_add_provider(room_context, GTK_STYLE_PROVIDER(player_provider3),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


GtkWidget *label = gtk_builder_get_object(builder, "creation_label");
GtkStyleContext *context = gtk_widget_get_style_context(label);
gtk_style_context_add_class(context, "my-label-class");

const char *css = ".my-label-class { color: white; font-weight: 900; font-size:30px; font-family: Gill Sans Extrabold, sans-serif;border: 5px white dotted;}";
GtkCssProvider *provider = gtk_css_provider_new();
gtk_css_provider_load_from_data(provider, css, -1, NULL);
gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider),
                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
   
    gtk_widget_show(window);
    gtk_main();

    return 0;
}
