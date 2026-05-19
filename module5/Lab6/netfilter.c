#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/netfilter_ipv4.h>
#include<linux/skbuff.h>
#include<linux/ip.h>
#include<linux/inet.h>
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
 
static struct kobject *nfkobject;                   //net filter k_object
static __be32 ip_address;                           //ip address to use with sysfs
static const char sysfs_name[] = "my_firewall";     // name of dir in /sys/kernel/
static struct nf_hook_ops nfin;                     
 
//Function that called when want so see what inside kobj
static ssize_t firewall_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%pI4\n", &ip_address);
}
//Function that called when want so put inside kobj
static ssize_t firewall_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{  
        // using in4_pton to parsing arg of echo into kernel IP address
        if(in4_pton(buf, count, (__u8 *)&ip_address, '\n', NULL)) 
        {
            printk(KERN_INFO "IP address set to %pI4\n", &ip_address);
            return count;
        }

         return -EINVAL;
}

//creating kobj
static struct kobj_attribute fw_attribute_1 =__ATTR(my_firewall_1, 0660, firewall_show,
                                                   firewall_store);

//netfilter hooker
static unsigned int hook_func_in(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
 
{
    struct ethhdr *eth;
    struct iphdr *ip_header;
 
    eth = (struct ethhdr*)skb_mac_header(skb);
    ip_header = (struct iphdr *)skb_network_header(skb);
    printk(KERN_INFO "src mac %pM, dst mac %pM\n", eth->h_source, eth->h_dest);
    printk(KERN_INFO "src IP addr: %pI4\n", &ip_header->saddr);
    printk(KERN_INFO "stored IP addr: %pI4\n", &ip_address);
    //comparing addresses
    //drop if equal
    if(ip_header->saddr == ip_address)
    {
        printk(KERN_INFO "dropping packet\n");
        return NF_DROP;
    }
    printk(KERN_INFO "accepting packet\n");
    return NF_ACCEPT;
}
static int __init init_main(void)
{
    //cread kobject and file in it
    int error = 0;
    pr_debug("Module initialized successfully \n");
    nfkobject = kobject_create_and_add(sysfs_name,
                                                kernel_kobj);
    if(!nfkobject)
            return -ENOMEM;

    error = sysfs_create_file(nfkobject, &fw_attribute_1.attr);
    if (error) {
            pr_debug("failed to create the firewall_1 file in /sys/kernel/%s \n", sysfs_name);
    }
    //register hooker
    nfin.hook     = hook_func_in;
    nfin.hooknum  = NF_INET_PRE_ROUTING;
    nfin.pf       = PF_INET;
    nfin.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net, &nfin); 
    
    return error;
}
 
 
 
static void __exit cleanup_main(void)
{
    //unreg hooker
    nf_unregister_net_hook(&init_net, &nfin); 
    pr_debug ("Module un initialized successfully \n");
    //del kobject
    kobject_put(nfkobject);
  
}

MODULE_AUTHOR("Stasyan");
MODULE_DESCRIPTION("Netfilter module utilising sysfs");
MODULE_LICENSE("GPL");

module_init(init_main);
module_exit(cleanup_main);
