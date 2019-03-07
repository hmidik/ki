#include "sensor/wireless.h"
#include "sensor/ki_store.h"
#include "sensor/door.h"
#include "common/device.h"
#include "common/data.h"
#include <string.h>

/**
 * This function is polled by the main loop and should handle any packets coming
 * in over the 868 MHz communication channel.
 */
void handle_communication(void)
{
  uint8_t wi_data_buff[WIRELESS_PAYLOAD_LENGTH];
  wireless_message_t wireless_msg;
  ki_store_result_t rc;

  if(wireless_dequeue_incoming(wi_data_buff))
  {
    memcpy(&wireless_msg, wi_data_buff, WIRELESS_PAYLOAD_LENGTH);

    if((wireless_msg.ctl_cmd != PING) && (wireless_msg.ctl_cmd != RESTART))
    {
      memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
      memcpy(wi_data_buff, "ACK", sizeof("ACK"));
      wireless_enqueue_outgoing(wi_data_buff);
    }

    switch (wireless_msg.ctl_cmd)
    {
      case PING:
        // Send an alive signal!
        memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
        memcpy(wi_data_buff, "PONG", sizeof("PONG"));
        wireless_enqueue_outgoing(wi_data_buff);
        break;

      case RESTART:
        // Restart sensor!
        reset_device();
        break;

      case ADDKITOKEN:
        rc = ki_store_add(wireless_msg.token);
        switch (rc)
        {
          case KI_STORE_SUCCESS:
            memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
            memcpy(wi_data_buff, "KI_STORE_SUCCESS", sizeof("KI_STORE_SUCCESS"));
            wireless_enqueue_outgoing(wi_data_buff);
            break;

          case KI_STORE_ERROR_FULL:
            memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
            memcpy(wi_data_buff, "KI_STORE_ERROR_FULL", sizeof("KI_STORE_ERROR_FULL"));
            wireless_enqueue_outgoing(wi_data_buff);
            break;

          case KI_STORE_ERROR_UNKNOWN:
            memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
            memcpy(wi_data_buff, "KI_STORE_ERROR_UNKNOWN", sizeof("KI_STORE_ERROR_UNKNOWN"));
            wireless_enqueue_outgoing(wi_data_buff);
            break;
        }
        break;

      case RMKITOKEN:
        rc = ki_store_remove(wireless_msg.token);
        switch (rc)
        {
          case KI_STORE_SUCCESS:
            memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
            memcpy(wi_data_buff, "KI_STORE_SUCCESS", sizeof("KI_STORE_SUCCESS"));
            wireless_enqueue_outgoing(wi_data_buff);
            break;

          case KI_STORE_ERROR_FULL:
            memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
            memcpy(wi_data_buff, "KI_STORE_ERROR_FULL", sizeof("KI_STORE_ERROR_FULL"));
            wireless_enqueue_outgoing(wi_data_buff);
            break;

          case KI_STORE_ERROR_UNKNOWN:
            memset(wi_data_buff, 0, WIRELESS_PAYLOAD_LENGTH);
            memcpy(wi_data_buff, "KI_STORE_ERROR_UNKNOWN", sizeof("KI_STORE_ERROR_UNKNOWN"));
            wireless_enqueue_outgoing(wi_data_buff);
            break;
        }
        break;

      case OPENDOOR:
        door_trigger();
        break;

      default:
        break;
    }
  }
}
