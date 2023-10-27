using namespace std;
struct client_message {
    uint8_t type;
    uint16_t discript_length;
    char receiver[32];
    char sender[32];
    // uint16_t narration;
    char* discription;
} __attribute__((packed));

struct change_room
{
	uint8_t type = 2;
	uint16_t room_num;

} __attribute__((packed));
struct server_message
{
	uint8_t type;

} __attribute__((packed));
