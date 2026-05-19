#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>

#define NETLINK_USER 31

#define MAX_PAYLOAD 1014                    /* maximum payload size*/
struct sockaddr_nl src_addr, dest_addr;     // addreses of source and destanation
struct nlmsghdr *nlh = NULL;                //netlink header
struct iovec iov;                           //input-output vector that holds in it netlink data
int sock_fd;                                // socket desc
struct msghdr msg;                          //msg to send and get reply from Kernel

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        if(strlen(argv[1])>MAX_PAYLOAD)
        {
            printf("Erro: Message is to big. Limit is 1024\n");
            printf("Closing...\n");
            return EXIT_FAILURE;
        }
        sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
        if (sock_fd < 0)
            return EXIT_FAILURE;

        memset(&src_addr, 0, sizeof(src_addr));
        src_addr.nl_family = AF_NETLINK;
        src_addr.nl_pid = getpid(); /* self pid */

        bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

        memset(&dest_addr, 0, sizeof(dest_addr));
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.nl_family = AF_NETLINK;
        dest_addr.nl_pid = 0; /* For Linux Kernel */
        dest_addr.nl_groups = 0; /* unicast */

        nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
        memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
        nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
        nlh->nlmsg_pid = getpid();
        nlh->nlmsg_flags = 0;

        strcpy(NLMSG_DATA(nlh), argv[1]);

        iov.iov_base = (void *)nlh;
        iov.iov_len = nlh->nlmsg_len;
        msg.msg_name = (void *)&dest_addr;
        msg.msg_namelen = sizeof(dest_addr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        printf("Sending message to kernel\n");
        sendmsg(sock_fd, &msg, 0);
        printf("Waiting for message from kernel\n");

        /* Read message from kernel */
        recvmsg(sock_fd, &msg, 0);
        printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
        close(sock_fd);

        return 0;
    }
    return EXIT_FAILURE;
}
