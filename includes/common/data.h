

// Commands
#define PING        0x01
#define RESTART     0x02
#define ADDKITOKEN  0x03
#define RMKITOKEN   0x04
#define OPENDOOR    0x05

#define ERROR       0xDF
#define PONG        0xEF
#define ACK         0xFF

// flags
#define GATEWAY_COMMAND_FLAG          0x01
#define SENSOR_COMMAND_FLAG           0x02


// Wireless data structures
typedef union wireless_message
{
  uint8_t ctl_cmd;
  uint8_t flags;
  uint8_t wi_data_length;
  uint8_t token[16];
  uint8_t wi_data[WIRELESS_PAYLOAD_LENGTH - 19];
  uint8_t wi_msg[WIRELESS_PAYLOAD_LENGTH];
} wireless_message_t;

// Wiered data structures
typedef union modem_message
{
  uint8_t ctl_cmd;
  uint8_t flags;
  uint8_t modem_data_length;
  uint8_t sensor_id[16];
  uint8_t token[16];
  uint8_t modem_data[MODEM_MAX_PAYLOAD_LENGTH - 35];
	uint8_t modem_msg[MODEM_MAX_PAYLOAD_LENGTH];
} modem_message_t;

// Pong massage
modem_message_t pong_msg = {PONG};
