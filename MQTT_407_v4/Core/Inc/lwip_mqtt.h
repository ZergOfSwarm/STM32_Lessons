#ifndef LWIP_MQTT_H
#define LWIP_MQTT_H
#include "lwip/apps/mqtt.h"
void example_do_connect(mqtt_client_t *client, const char * topic);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
void example_publish(mqtt_client_t *client, void *arg);
static void mqtt_pub_request_cb(void *arg, err_t result);

#endif LWIP_MQTT_H
