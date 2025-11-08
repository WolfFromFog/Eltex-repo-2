#include "ips.h"
#include <stdio.h>


//Прасер строки в 4 октета
int parser_ipv4(const char  *str, ipv4_address *ip)
{
    int octets[4];
    if(sscanf(str, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2] ,&octets[3]) != 4)
    {
        return 0;
    }

    for (int i = 0; i < 4; i++)
    {
        if(octets[i] < 0 || octets[i] > 255) 
        {
            return 0;
        }
        ip->octets[i]=(uint8_t)octets[i];
    }
    return 1;
}

//Перевод ip адреса в одно 32-битное число
uint32_t ip_to_uint32(const ipv4_address* ip)
{
    return  ((uint32_t)ip->octets[0] << 24) |
            ((uint32_t)ip->octets[1] << 16) |
            ((uint32_t)ip->octets[2] << 8)  |
            ((uint32_t)ip->octets[3]);
}

// Преобразование 32-битного числа в IP адрес
ipv4_address uint32_to_ip(uint32_t ip_int) {
    ipv4_address ip;
    ip.octets[0] = (ip_int >> 24) & 255;
    ip.octets[1] = (ip_int >> 16) & 255;
    ip.octets[2] = (ip_int >> 8) & 255;
    ip.octets[3] = ip_int & 255;
    return ip;
}

// Получение числового адреса сети 
uint32_t get_network_address(uint32_t ip, uint32_t mask)
{
    return ip & mask;
}

//Генерирует рандомный ip в радиусе $IP_RADIUS адресов
uint32_t generate_ip(uint32_t base_ip) 
{
     // Генерируем смещение в диапазоне [-IP_RADIUS, +IP_RADIUS]
    int offset = (rand() % ( 2 * IP_RADIUS + 1)) - IP_RADIUS;
    
    // Применяем смещение к базовому IP
    return base_ip + offset;
}

//Проверка, что адрес в нужной сети
int is_same_subnet(const network_config *config, uint32_t dest_ip) {
    uint32_t dest_network = get_network_address(dest_ip, config->mask_value);
    uint32_t gateway_network = get_network_address(config->gateway_network, config->mask_value);
    return dest_network == gateway_network;
}

// Инициализация параметров сети
int initialize_network(network_config *config, const char *gateway_str, const char *mask_str) {
    if (!parser_ipv4(gateway_str, &config->gateway) || 
        !parser_ipv4(mask_str, &config->mask)) {
        return 0;
    }
    
    config->gateway_network = ip_to_uint32(&config->gateway);
    config->mask_value = ip_to_uint32(&config->mask);
    
    return 1;
}

int net_tester(network_config *net, const char* address, const char* mask, int number)
{
    int bool = initialize_network(net, address, mask);

    if(bool)
    {

    int stat_same = 0, stat_other = 0;

    uint32_t gen_ip = 0;
    ipv4_address gen_address;

    for (int i = 0; i <number; i++)
    {
        gen_ip = generate_ip(net->gateway_network);
        gen_address = uint32_to_ip(gen_ip);
        printf("Сгенерирован адрес: %d.%d.%d.%d -- ", gen_address.octets[0],gen_address.octets[1],gen_address.octets[2],gen_address.octets[3]);
        if(is_same_subnet(net, gen_ip))
        {
            printf("Входит в подсеть.\n");
            stat_same++;
        }
        else
        {
            printf("Невходит в подсеть.\n");
            stat_other++;
        }
    }
    printf("=========СТАТИСТИКА=========\n");
    printf("Адресс сети: %s\n",address);
    printf("Масска сети: %s\n",mask);
    printf("Всего запросов:                    %d\n", number);
    printf("Количество запросов в эту сеть:    %d, %.2f\% \n",stat_same, ( (double)stat_same /number * 100) );
    printf("Количество запросов в другую сеть: %d, %.2f\% \n",stat_other,((double)stat_other/number * 100) );
    }
    else
    {
        printf("Ошибка инициализации!!!\n");
    }

   
    

}

