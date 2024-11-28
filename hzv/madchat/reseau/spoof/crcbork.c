/*
 crcbork.c

 A little tool to generate any number of packets from a specified source
 to a specified host.  The packets generated all have invalid checksum
 off by -1.

 You must have libnet installed to compile.  ALl switches are optional
 and defaults will be put in place if left out.
 
 compilation: cc crcbork.c -o crcbork -lnet
 usage: crcbork -s x.x.x.x -d y.y.y.y -c count -b type -z aa:bb:cc:dd:ee:ff
        -x aa:bb:cc:dd:ee:ff
 
 -s: source IP
 -d: destination IP
 -c: byte count to send
 -b: what to break
 {
     type 1					:  Break IP CRC
     type 2					:  Break TCP CRC
     type 3 (actually any number != 1,2)	:  Break BOTH CRC
 }
 -z: source MAC
 -x: destination MAC
 
 Copyright (c) 2002 TCroc <tcroc@cow.pasture.com>

 
 Version V1.0 (Released) - If you find this usefull email
 the author (tcroc@cow.pasture.com) and let him know.  It will
 help to pump his ego a little bit more than it already is.

*/

#define LIBNET_LIL_ENDIAN 1
#include <stdio.h>
#include <err.h>
#include <libnet.h>

int parsemac(u_char *ptr, int flag);

int	verbose, debug;
int	network, packet_size, c;
u_char	enet_src[6] = {0x0d, 0x0e, 0x0a, 0x0d, 0x06, 0x66};
u_char	enet_dst[6] = {0x0d, 0x0e, 0x0a, 0x0d, 0x06, 0x66};

int main(int argc, char **argv)
{

	u_long			src_ip, dst_ip;
	u_char			*packet, *macptr;
	int			eatit, count, retval, btype;
	struct sockaddr_in	sa;
	struct libnet_link_int	*linkfptr;
	char			*dev, **device;
	char			err_buf[LIBNET_ERRBUF_SIZE];

	src_ip = 0; /* DEFAULT SRC IP 000.000.000.000 */
	dst_ip = 0; /* DEFAULT DST IP 000.000.000.000 */
	count = 69;  /* DEFAULT PACKET COUNT TO SEND OUT */
	btype = 3;   /* DEFAULT BREAK ALL CRC */
	dev = NULL;  /* NO DEFAULT DEVICE */
	device = &dev; /* FSCKED UP WAY LIBNET DOES LL DEV SELECTION */

	while ((c = getopt(argc, argv, "d:s:c:b:z:x:")) != EOF)
	{
       		switch (c) {
		case 'd':
			if(!(dst_ip = libnet_name_resolve(optarg, LIBNET_RESOLVE)))
			{
				libnet_error(LIBNET_ERR_FATAL, "Bad destination IP address: %s\n", optarg);
			}
			break;
        	case 's':
			if (!(src_ip = libnet_name_resolve(optarg, LIBNET_RESOLVE)))
			{
				libnet_error(LIBNET_ERR_FATAL, "Bad source IP address: %s\n", optarg);
			}
               		break;
		case 'c':
			count = atoi(optarg);
			break;
		case 'b':
			btype = atoi(optarg);
			break;
		case 'z':
			parsemac(optarg, 1);
			break;
		case 'x':
			parsemac(optarg, 2);
			break;
        	default:
			eatit = usage(argv[0]);
			exit(EXIT_FAILURE);
		}
        }
	
	if (count == 0)
	{
		eatit = usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (dev == NULL)
	{
		if (libnet_select_device(&sa, device, err_buf) == -1)
		{
			libnet_error(LIBNET_ERR_FATAL, "libnet_select_device failed: %s\n", err_buf);
		}
	}

	if ((linkfptr = libnet_open_link_interface(dev, err_buf)) == NULL)
	{
		libnet_error(LIBNET_ERR_FATAL, "libnet_open_link_interface: %s\n", err_buf);
	}
	
	packet_size = LIBNET_ETH_H + LIBNET_IP_H + LIBNET_TCP_H;

	libnet_init_packet(packet_size, &packet);	

	if (packet == NULL)
	{
		libnet_error(LIBNET_ERR_FATAL, "libnet_init_packet failed\n");
	}
	
	libnet_build_ethernet(enet_dst, enet_src, ETHERTYPE_IP, NULL, 0, packet);

	libnet_build_ip(LIBNET_TCP_H, IPTOS_LOWDELAY, 420, 0, 48, IPPROTO_TCP, src_ip, dst_ip, NULL, 0, packet + LIBNET_ETH_H ); 

	libnet_build_tcp(420, 420, 0xa1d95, 0x53, TH_SYN, 1024, 0, NULL, 0, packet + LIBNET_IP_H + LIBNET_ETH_H );
	
	/* This next section is where we break the checksum.  Note that
	I told the checksum generator that the length of the packet was
	one byte shorter than what it realy is.  This causes the checksum
	to come back incorrect.  I tested this on my internet network with
	a sniffer and sure enough its off by exactly 1!! */

	if (btype == 2)  /* KEEP IT REAL Y0 */
	{
		if (libnet_do_checksum(packet + ETH_H, IPPROTO_IP, LIBNET_IP_H) == -1)
		{
			libnet_error(LIBNET_ERR_FATAL, "libnet_do_checksum IP failed\n");
		}
	} else  /* BREAK IT */
	{
		if (libnet_do_checksum(packet + ETH_H, IPPROTO_IP, LIBNET_IP_H +1) == -1)
		{
			libnet_error(LIBNET_ERR_FATAL, "libnet_do_checksum IP failed\n");
		}
	}


	if (btype == 1) /* KEEP IT REAL YO */
	{
		if (libnet_do_checksum(packet + ETH_H , IPPROTO_TCP, LIBNET_TCP_H) == -1)
		{
			libnet_error(LIBNET_ERR_FATAL, "libnet_do_checksum TCP failed\n");
		}
	} else  /* BREAK IT */
	{
		if (libnet_do_checksum(packet + ETH_H, IPPROTO_TCP, LIBNET_TCP_H+1) == -1)
		{
			libnet_error(LIBNET_ERR_FATAL, "libnet_do_checksum TCP failed\n");
		}
	}

	
	while (count > 0)
	{ 
		c = libnet_write_link_layer(linkfptr, dev, packet, packet_size);
	
		if (c < packet_size)
		{
			libnet_error(LN_ERR_WARNING, "libnet_write_ip only wrote %d bytes\n", c);
		}
	--count;
	}
        
        printf("Sent %i packets.\n", count);

	retval = libnet_close_link_interface(linkfptr);
	if (retval == -1)
	{
		libnet_error(LIBNET_ERR_FATAL, "Error in close link interface.\n");
	}

	libnet_destroy_packet(&packet);
	
	return (c == -1 ? EXIT_FAILURE : EXIT_SUCCESS);


} /* END MAIN */

int
usage(char *name)
{
	fprintf(stderr, "usage: %s -s x.x.x.x -d y.y.y.y -c count -b type\n", name);
	return(1);
}

int
parsemac(u_char *arg, int flag)
{
	u_char	        *p;
	u_char	        tmp[5];
	int	        i;
       
	p = arg;
	i = 0;
        
        if (strlen(arg) > 17)
        {
            printf("MAC Address to large ERROR %s\n", arg);
            i = usage(arg);
            exit(EXIT_FAILURE);
        }

	while(i  < 6)
	{
                tmp[0] = '\0';
        
		strcat(tmp, "0x");	

		strncat(tmp, p, (sizeof(u_char)*2));
                
                tmp[5] = '\0';

/* There is such a better way to do this using strtol but I'm to lazy to
fix it.  So :P~~~~~ */

		if (flag == 1)
			enet_src[i] = (u_char) strtol(tmp, NULL, 16);
		else
			enet_dst[i] = (u_char) strtol(tmp, NULL, 16);

		p = p + (sizeof(char)*3);

		++i;
	}
}
/* EOF */
