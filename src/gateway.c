#include "gateway/modem.h"
#include "gateway/wireless.h"
#include "common/device.h"
#include "common/data.h"
#include <string.h>

/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the modem or 868 MHz communication channel.
 */
void handle_communication(void)
{
	uint8_t const *modem_data_buff;
	size_t modem_data_length = 0;
	wireless_message_t wireless_msg;
	modem_message_t modem_msg;

	// Packet coming in over the modem
	if(modem_dequeue_incoming( &modem_data_buff, &modem_data_length))
	{
		memcpy(&modem_msg, modem_data_buff, modem_data_length);

		// Check if the command ment to be executed by the gateway?
		if(modem_msg.flags & GATEWAY_COMMAND_FLAG)
		{
			switch (modem_msg.ctl_cmd)
			{
				case PING:
					// Send an alive signal!
          memset(&modem_data_buff, 0, MODEM_MAX_PAYLOAD_LENGTH);
					memcpy(&modem_data_buff, "PONG", sizeof("PONG"));
					modem_enqueue_outgoing(modem_data_buff, sizeof(modem_data_buff));
					break;

				case RESTART:
				  // Restart gateway!
					reset_device();
					break;

				default:
					break;
      }
		}
		else if(modem_msg.flags & SENSOR_COMMAND_FLAG)
		{
			device_id_t to_sensor_id;
			memcpy(to_sensor_id.bytes, modem_msg.sensor_id, sizeof(modem_msg.sensor_id));
			wireless_msg.ctl_cmd = modem_msg.ctl_cmd;
			wireless_msg.flags = modem_msg.flags;
			wireless_msg.wi_data_length = sizeof(modem_msg.token);
			memcpy(wireless_msg.token, modem_msg.token, sizeof(modem_msg.token));

      // Send to sensor
			wireless_enqueue_outgoing(to_sensor_id, wireless_msg.wi_msg);
		}
	}

  device_id_t from_sensor_id;
  uint8_t wireless_data_buff[WIRELESS_PAYLOAD_LENGTH];
  modem_message_t to_modem_msg;
  // Packet coming in over the wireless interface
  if(wireless_dequeue_incoming(&from_sensor_id, wireless_data_buff))
  {
    memcpy(to_modem_msg.sensor_id, from_sensor_id.bytes, sizeof(from_sensor_id.bytes));
    memcpy(to_modem_msg.modem_data, wireless_data_buff, sizeof(wireless_data_buff));
    memset(&modem_data_buff, 0, MODEM_MAX_PAYLOAD_LENGTH);
    memcpy(&modem_data_buff, to_modem_msg.modem_msg, sizeof(to_modem_msg.modem_msg));
    modem_enqueue_outgoing(modem_data_buff, sizeof(modem_data_buff));
  }
}
