#ifndef IPS_H
#define IPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define IP_RADIUS 500

typedef struct 
{
    uint8_t octets[4];
} ipv4_address;

typedef struct 
{
    ipv4_address gateway;
    ipv4_address mask;
    uint32_t gateway_network;
    uint32_t mask_value;

} network_config;

//Функции парсингов и преобразований
int parser_ipv4(const char *str, ipv4_address *ip);

uint32_t ip_to_uint32(const ipv4_address *ip);

ipv4_address uint32_to_ip(uint32_t ip_int);

//Получение адреса сети
uint32_t get_network_address(uint32_t ip, uint32_t mask);

//Получение ip
uint32_t generate_ip(uint32_t base_ip);

//Проверка на принадлежность адреса к сети
int is_same_subnet(const network_config *config, uint32_t dest_ip);

//Инициализация сети
int initialize_network(network_config *config, const char *gateway_str, const char *mask_str);

void net_tester(network_config *net,const char *adress, const char *mask, int number);

#endif 