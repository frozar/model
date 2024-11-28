/* OpenSSL vulnerability tester for CAN-2002-0656 (master key overflow)
   Copyright (C) 2002 RUS-CERT, University of Stuttgart.
   Written by Florian Weimer <Weimer@CERT.Uni-Stuttgart.DE>
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   In addition, as a special exception, the author gives permission to
   link this source code with the OpenSSL project's "OpenSSL" library
   and distribute the resulting binaries.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Id: openssl-sslv2-master.c,v 1.17 2002/09/18 17:03:33 rusfw Exp $ */

/*

How does it work?
-----------------

It is difficult to tell OpenSSL 0.9.6e from vulnerable versions
because the OpenSSL developers chose to kill the process if a buffer
overflow attempt is detected.  Over the network, a crash due to a
buffer overflow and an abrupt, but deliberate process termination look
the same: in both cases, the TCP connection breaks down.  At first
glance, it appears that we are out of luck and cannot detect vulnerable
versions.

However, if we overrun the buffer by only a few bytes, the vulnerable
version (without check) does NOT crash.  This way, we can tell 0.9.6e
from previous, vulnerable versions:

               large overflow  small overflow
    pre-0.9.6e      crash         no crash
        0.9.6e      crash           crash
        0.9.6g      error           error

(0.9.6g signals an SSL error over the network, as one would expect.)

Other combinations are possible, of course, and this program tries to
flag them in a reasonable way.  (We consider malformed responses an
indication of lack of care, and a potential security problem.)

This program performs a third connection attempt (actually the first
one), to test compatibility of the the SSL implementations.

How to compile and run it
-------------------------

Compiling this program requires an OpenSSL development environment
(including header files).  You have to link this program with OpenSSL's
"crypto" library (using "-lcrypto", NOT "-lcrypt").  On some systems,
you have to link with "-ldl", too.

After compilation, you can run this program using:

  ./openssl-sslv2-master [-s] [-P] [-I] HOST-IP [PORT]

Then a test is performed against HOST-IP (an IP address in dotted-quad
notation) and the TCP service running on PORT (a decimal number).
PORT can be omitted, then the default port "443" is assumed.

NOTE: You can use this program to test any SSL Version 2.0 server, not
just HTTPS servers. For some services, a "STARTTLS" message is
required to initiate the SSL handshake (e.g. SMTP, POP3, IMAP).  This
message is sent if you supply the "-s" option (for generic/SMTP
STARTTLS support), or "-P" (POP3 STLS support), or "-I" (IMAP STARTTLS
support).

Risks and limitations
---------------------

The server could crash (when the large buffer overflow is attempted),
and fail to restart automatically.  The program detects this, but
obviously cannot undo any damage.

If the server which is being tested does not support SSLv2, it often
reacts in a strange way.  The SSL protocol does not define a clear
rejection message, so no proper diagnosis is return by the tool in
such cases.  To determine the cause of a handshake failure, you should
use a full SSL/TLS implementation, such as "openssl s_client".

Credits
-------

This program was inspired by the source code of the "Slapper" worm.

Thanks to Helmut Springer for testing and spotting a few bugs, and
Michael H. Warfield for helpful information on the STARTTLS protocol.

Updates
-------

Updates of this program will be made available at the following URL:

  <http://CERT.Uni-Stuttgart.DE/advisories/openssl-sslv2-master/>

*/



#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <openssl/md5.h>
#include <openssl/rc4.h>
#include <openssl/x509.h>

#define TIMEOUT 10

typedef struct {
  size_t length;
  unsigned char *data;
} buffer_t;

void *
safe_realloc (void *ptr, size_t size)
{
  void *new = realloc (ptr, size ? size : 1);
  if (!new) {
    abort();
  }
  return new;
}

#define BUFFER_DECLARE(name) { buffer_t name = {0, 0}
#define BUFFER_COMPONENT(name) buffer_t name
#define BUFFER_COMPONENT_INITIALIZE {0, 0}
#define BUFFER_INIT(name) do { (name).data = 0; (name).length = 0; } while (0)
#define BUFFER_FREE(name) free ((name).data); }
#define BUFFER_CLEAR(name) do { free((name).data); (name).length = 0; (name).data = 0; } while (0)
#define BUFFER_RESIZE(name, size) do { (name).data = safe_realloc((name).data, size); (name).length = size; } while (0)
#define BUFFER_LENGTH(name) ((name).length)
#define BUFFER_UNCHECKED(name) ((name).data)

unsigned char *
buffer_at(const buffer_t buffer, size_t position)
{
  if (position >= buffer.length) {
    abort();
  }
  if (buffer.data == 0) {
    abort ();
  }

  return buffer.data + position;
}

#define BUFFER_AT(buf, pos) (*(buffer_at((buf), (pos))))

unsigned short
buffer_at_16 (const buffer_t buffer, size_t pos)
{
  return BUFFER_AT (buffer, pos) * 256 + BUFFER_AT (buffer, pos + 1);
}

void
buffer_set_16 (const buffer_t *buffer, size_t pos, unsigned value)
{
  BUFFER_AT (*buffer, pos + 0) = value / 256;
  BUFFER_AT (*buffer, pos + 1) = value % 256;
}

void
buffer_assign (buffer_t *buffer, const unsigned char* data, size_t length)
{
  BUFFER_RESIZE (*buffer, length);
  memcpy (buffer->data, data, length);
}

void
buffer_copy (buffer_t *buffer, const buffer_t new)
{
  BUFFER_RESIZE (*buffer, new.length);
  memcpy (buffer->data, new.data, new.length);
}

void
buffer_repeat (buffer_t *buffer, int character, size_t length)
{
  BUFFER_RESIZE (*buffer, length);
  memset (buffer->data, character, length);
}

void
buffer_slice (buffer_t *result, const buffer_t source, size_t start, size_t length)
{
  if (start + length > source.length) {
    abort ();
  }
  
  BUFFER_RESIZE (*result, length);
  memcpy (result->data, source.data + start, length);
}

void
buffer_assign_string (buffer_t *buffer, const char *string)
{
  buffer_assign (buffer, (unsigned char *)string, strlen(string));
}

void
buffer_append (buffer_t *buffer, const buffer_t new)
{
  size_t old_length = buffer->length;
  BUFFER_RESIZE (*buffer,  old_length + new.length);
  memcpy (buffer->data + old_length, new.data, new.length);
}

void
buffer_append_string (buffer_t *buffer, const char *new)
{
  size_t old_length = buffer->length;
  size_t new_length = strlen(new);
  BUFFER_RESIZE (*buffer,  old_length + new_length);
  memcpy (buffer->data + old_length, new, new_length);
}

int
buffer_starts_with_string (const buffer_t buffer, const char *string)
{
  size_t string_length = strlen(string);
  if (BUFFER_LENGTH (buffer) < string_length) {
    return 0;
  } else {
    return memcmp (buffer.data, string, string_length) == 0;
  }
}

int 
buffer_read(int fd, buffer_t *buffer, size_t size) 
{
  BUFFER_RESIZE(*buffer, size);
  buffer->length = 0;

  while (size > 0) {
    int chunk_length;
    fd_set rset;
    struct timeval timeout;

    FD_ZERO (&rset);
    FD_SET (fd, &rset);
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    if (select (fd + 1, &rset, NULL, NULL, &timeout) == 1) {
      chunk_length = read(fd, buffer->data + buffer->length, size);
    } else {
      chunk_length = -1;
    }

    if (chunk_length < 0) {
      return -1;
    }
    if (chunk_length == 0) {
      return 0;
    }
    
    buffer->length += chunk_length;
    size -= chunk_length;
  }

  return 0;
}
    
int 
buffer_read_blocking(int fd, buffer_t *buffer, size_t size) 
{
  BUFFER_RESIZE(*buffer, size);
  buffer->length = 0;

  while (size > 0) {
    int chunk_length = read(fd, buffer->data + buffer->length, size);

    if (chunk_length < 0) {
      return -1;
    }
    if (chunk_length == 0) {
      return 0;
    }
    
    buffer->length += chunk_length;
    size -= chunk_length;
  }

  return 0;
}

int
buffer_read_crlf(int fd, buffer_t *buffer, size_t maximum_size)
{
  int cr_seen = 0;
  int ret = 0;

  BUFFER_DECLARE (temp);
  BUFFER_RESIZE (*buffer, 0);

  while (BUFFER_LENGTH (*buffer) < maximum_size) {
    ret = buffer_read (fd, &temp, 1);
    if (BUFFER_LENGTH (temp) != 1) {
      ret = -1;
    }
    if (ret == -1) {
      break;
    }
    
    buffer_append (buffer, temp);

    if (cr_seen && (BUFFER_AT (temp, 0) == '\n')) {
      break;
    }
    if (BUFFER_AT (temp, 0) == '\r') {
      cr_seen = 1;
    } else {
      cr_seen = 0;
    }
  }

  BUFFER_FREE (temp);

  if (ret) {
    return ret;
  }

  if (BUFFER_LENGTH (*buffer) == maximum_size) {
    return -1;
  }

  return 0;
}

int 
buffer_write (int fd, const buffer_t buffer) 
{
  size_t to_go = buffer.length;

  while (to_go > 0) {
    int chunk_length;
    fd_set wset;
    struct timeval timeout;

    FD_ZERO (&wset);
    FD_SET (fd, &wset);
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    if (select (fd + 1, NULL, &wset, NULL, &timeout) == 1) {
      chunk_length = write(fd, buffer.data + (buffer.length - to_go), to_go);
    } else {
      chunk_length = -1;
    }
    
    if (chunk_length < 0) {
      return -1;
    }
    to_go -= chunk_length;
  }
  return 0;
}

int
buffer_random (buffer_t *buffer, size_t size)
{
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd >= 0) {
    int ret = buffer_read_blocking (fd, buffer, size);
    close (fd);
    return ret;
  } else { 
    BUFFER_RESIZE (*buffer, 0);
    return -1;
  }
}

void
buffer_md5 (buffer_t *result, buffer_t source)
{
  BUFFER_RESIZE (*result, MD5_DIGEST_LENGTH);
  MD5 (source.data, source.length, result->data);
}

int
buffer_encrypt_RSA (buffer_t *encrypted, const buffer_t plaintext, EVP_PKEY *key)
{
  int ret;

  BUFFER_RESIZE (*encrypted, RSA_size(key->pkey.rsa));
  ret = RSA_public_encrypt(plaintext.length, plaintext.data, encrypted->data, key->pkey.rsa, RSA_PKCS1_PADDING);
  if (ret > 0) {
    return 0;
  } else {
    return -1;
  }
}

void
buffer_derive_RC4_key (RC4_KEY *key, const buffer_t material)
{
  RC4_set_key (key, material.length, material.data);
}

void
buffer_crypt_RC4 (buffer_t *result, const buffer_t source, RC4_KEY *key)
{
  BUFFER_RESIZE (*result, source.length);
  RC4 (key, source.length, source.data, result->data);
  
}

/* SSL pseudo-implementation */

#define SSL_MT_ERROR 0
#define SSL_MT_CLIENT_HELLO 1
#define SSL_MT_CLIENT_MASTER_KEY 2
#define SSL_MT_CLIENT_FINISHED 3
#define SSL_MT_SERVER_HELLO 4
#define SSL_MT_SERVER_VERIFY 5
#define SSL_MT_SERVER_FINISHED 6
#define SSL_MT_REQUEST_CERTIFICATE 7
#define SSL_MT_CLIENT_CERTIFICATE 8

#define SSL_CT_X509_CERTIFICATE 0x01

#define SSL_CK_RC4_128_WITH_MD5 0x01,0x00,0x80

typedef struct {
  int fd;
  buffer_t challenge;
  buffer_t certificate;
  buffer_t cipher_spec;
  buffer_t connection_id;
  buffer_t master_key;
  RC4_KEY read_key;
  RC4_KEY write_key;
  int encrypted;
  X509 *x509_context;
  EVP_PKEY *server_public_key;
} ssl_t;

int
ssl_write_record (ssl_t *ssl, const buffer_t buffer)
{
  unsigned char length[2];
  int ret;

  /* check maximum representable length */
  if (buffer.length >= (1 << 15)) {
    abort();
  }
  length[0] = (buffer.length / 256) | 0x80;
  length[1] = buffer.length % 256;

  BUFFER_DECLARE (packet);
  buffer_assign (&packet, length, sizeof (length));
  buffer_append (&packet, buffer);
  ret = buffer_write (ssl->fd, packet);
  BUFFER_FREE (packet);

  return ret;
}

int
ssl_read_record (ssl_t *ssl, buffer_t *record)
{
  size_t length = 0;		/* suppress warning */
  size_t padding = 0;
  int ret;

  BUFFER_DECLARE (length_buffer);
  ret = buffer_read (ssl->fd, &length_buffer, 2);
  if (BUFFER_LENGTH(length_buffer) == 2) {
    if (BUFFER_AT(length_buffer, 0) & 0x80) {
      length = (BUFFER_AT(length_buffer, 0) & 0x7f) * 256 + BUFFER_AT(length_buffer, 1);
    } else {
      length = (BUFFER_AT(length_buffer, 0) & 0x3f) * 256 + BUFFER_AT(length_buffer, 1);
      ret = buffer_read (ssl->fd, &length_buffer, 1);
      if (BUFFER_LENGTH (length_buffer) == 1) {
	padding = BUFFER_AT(length_buffer, 0);
      } else {
	ret = -1;
      }
    }
  } else {
    ret = -1;
  }
  BUFFER_FREE (length_buffer);

  if (ret == -1) {
    BUFFER_RESIZE (*record, 0);
    return -1;
  }
  
  ret = buffer_read(ssl->fd, record, length);
  if (ret < 0) {
    return ret;
  }

  if ((BUFFER_LENGTH (*record) == 3) && (BUFFER_AT (*record, 0) == SSL_MT_ERROR)) {
    return 0;
  }

  if (ssl->encrypted) {
    if (MD5_DIGEST_LENGTH + padding >= BUFFER_LENGTH (*record)) {
      return -1;
    }
    
    BUFFER_DECLARE (temp);
    buffer_crypt_RC4 (&temp, *record, &ssl->read_key);
    buffer_slice (record, temp, MD5_DIGEST_LENGTH, BUFFER_LENGTH (*record) - MD5_DIGEST_LENGTH - padding);
    BUFFER_FREE (temp);
  }

  return 0;
}

int
ssl_send_client_hello (ssl_t *ssl)
{
  static const unsigned char client_hello[] =
    {SSL_MT_CLIENT_HELLO,
     0x00, 0x02,		/* client version 2 */
     0x00, 0x03,		/* 3 bytes of cipher spec follow */
     0x00, 0x00,		/* no session ID follows */
     0x00, 0x10,		/* 16 bytes of challenge follow */
     SSL_CK_RC4_128_WITH_MD5
    };
  int ret;
    
  BUFFER_DECLARE (hello);
  buffer_assign (&hello, client_hello, sizeof (client_hello));
  
  /* Add challenge. */
  buffer_random (&ssl->challenge, 0x10);
  buffer_append (&hello, ssl->challenge);

  ret = ssl_write_record (ssl, hello);
  BUFFER_FREE (hello);

  return ret;
}

#define ERR_SERVER_HELLO_LENGTH 1
#define ERR_SERVER_HELLO_MISMATCH 2
#define ERR_SERVER_HELLO_HIT 3
#define ERR_SERVER_NOT_HELLO 4
#define ERR_SERVER_HELLO_READ 5
#define ERR_SERVER_HELLO_NO_CIPHER 6
#define ERR_SERVER_HELLO_X509 7
#define ERR_SERVER_HELLO_ERROR 8

int
ssl_receive_server_hello (ssl_t *ssl)
{
  int ret;
  size_t certificate_length;
  size_t cipher_length;
  size_t connection_length;
  
  BUFFER_DECLARE (hello);
  ret = ssl_read_record (ssl, &hello);
  if (ret == 0) {
    if (BUFFER_LENGTH (hello) < 12) {
      if ((BUFFER_LENGTH (hello) == 3) && (BUFFER_AT (hello, 0) == SSL_MT_ERROR)) {
	if (buffer_at_16 (hello, 1) == 1) {
	  ret = ERR_SERVER_HELLO_NO_CIPHER;
	} else {
	  ret = ERR_SERVER_HELLO_ERROR;
	}
      } else {
	ret = ERR_SERVER_HELLO_LENGTH;
      }
    } else {
      if (BUFFER_AT (hello, 0) != SSL_MT_SERVER_HELLO) {
	ret = ERR_SERVER_NOT_HELLO;
      }
      if (BUFFER_AT (hello, 1) != 0) {
	ret = ERR_SERVER_HELLO_HIT;
      }
      if (BUFFER_AT (hello, 2) != SSL_CT_X509_CERTIFICATE) {
	ret = ERR_SERVER_HELLO_MISMATCH;
      }
      if (buffer_at_16 (hello, 3) != 2) {
	ret = ERR_SERVER_HELLO_MISMATCH;
      }
    }
  } else {
    ret = ERR_SERVER_HELLO_READ;
  }
  if (ret == 0) {
    certificate_length = buffer_at_16 (hello, 5);
    cipher_length = buffer_at_16 (hello, 7);
    connection_length = buffer_at_16 (hello, 9);
    buffer_slice (&ssl->certificate, hello, 11, certificate_length);
    buffer_slice (&ssl->cipher_spec, hello, 11 + certificate_length, cipher_length);
    buffer_slice (&ssl->connection_id, hello, 11 + certificate_length + cipher_length, connection_length);
  }
  BUFFER_FREE (hello);

  return ret;
}

int
ssl_check_server_hello (ssl_t *ssl) 
{
  unsigned j;
  unsigned char *p;

  p = BUFFER_UNCHECKED(ssl->certificate);
  ssl->x509_context = d2i_X509(NULL, &p, BUFFER_LENGTH(ssl->certificate));
  if (ssl->x509_context == 0) {
    return ERR_SERVER_HELLO_X509;
  }

  ssl->server_public_key = X509_get_pubkey(ssl->x509_context);
  if (ssl->server_public_key == 0) {
    return ERR_SERVER_HELLO_X509;
  }

  if (ssl->server_public_key->type != EVP_PKEY_RSA) {
    return ERR_SERVER_HELLO_X509;
  }

  j = 0;
  while (j + 2 < BUFFER_LENGTH(ssl->cipher_spec)) {
    if ((BUFFER_AT(ssl->cipher_spec, j + 0) == 0x01)
	&& (BUFFER_AT(ssl->cipher_spec, j + 1) == 0x00)
	&& (BUFFER_AT(ssl->cipher_spec, j + 2) == 0x80)) {
      return 0;
    }
    j += 3;
  }
  return ERR_SERVER_HELLO_NO_CIPHER;
}

int
ssl_send_client_master_key (ssl_t *ssl, size_t overflow_count)
{
  size_t additional_length = 0 + overflow_count;
  const unsigned char data[] =
    {SSL_MT_CLIENT_MASTER_KEY,
     SSL_CK_RC4_128_WITH_MD5,
     0x00, 0x00,		/* at 4: zero bytes of clear key material */
     0x00, 0x00,		/* at 6: length of encrypted key material */
     0x00, 0x00,		/* at 8: length of additional key material*/
    };
  int ret;
  
  buffer_random (&ssl->master_key, 16);

  /* derive key material */

  BUFFER_DECLARE (temp1);
  BUFFER_DECLARE (temp2);
  
  buffer_copy (&temp1, ssl->master_key);
  buffer_append_string (&temp1, "0");
  buffer_append (&temp1, ssl->challenge);
  buffer_append (&temp1, ssl->connection_id);
  buffer_md5 (&temp2, temp1);
  buffer_slice (&temp1, temp2, 0, 16);
  buffer_derive_RC4_key (&ssl->read_key, temp1);
  
  buffer_copy (&temp1, ssl->master_key);
  buffer_append_string (&temp1, "1");
  buffer_append (&temp1, ssl->challenge);
  buffer_append (&temp1, ssl->connection_id);
  buffer_md5 (&temp2, temp1);
  buffer_slice (&temp1, temp2, 0, 16);
  buffer_derive_RC4_key (&ssl->write_key, temp1);

  BUFFER_FREE(temp2);
  BUFFER_FREE(temp1);
  

  BUFFER_DECLARE (record);
  buffer_assign (&record, data, sizeof(data));

  BUFFER_DECLARE (encrypted);
  buffer_encrypt_RSA (&encrypted, ssl->master_key, ssl->server_public_key);
  buffer_set_16 (&record, 6, BUFFER_LENGTH (encrypted));
  buffer_append (&record, encrypted);
  BUFFER_FREE (encrypted);

  BUFFER_DECLARE (additional);
  buffer_repeat (&additional, 'A', additional_length);
  buffer_set_16 (&record, 8, additional_length);
  buffer_append(&record, additional);
  BUFFER_FREE (additional);
  
  ret = ssl_write_record (ssl, record);
  BUFFER_FREE (record);

  if (ret == 0) {
    ssl->encrypted = 1;
  }

  return ret;
}

#define ERR_SSL_ERROR_LENGTH -1
#define ERR_SSL_ERROR_READ -2

int
ssl_fetch_verify_or_error (ssl_t *ssl)
{
  int ret;

  BUFFER_DECLARE (record);
  ret = ssl_read_record (ssl, &record);

  if (ret < 0) {
    ret = ERR_SSL_ERROR_READ;
    goto out;
  }
  

  if (BUFFER_LENGTH (record) < 1) {
    ret = ERR_SSL_ERROR_LENGTH;
    goto out;
  }

  if ((BUFFER_AT (record, 0) == SSL_MT_ERROR) 
      && (BUFFER_LENGTH (record) != 3)) {
    ret = ERR_SSL_ERROR_LENGTH;
    goto out;
  }

  ret = BUFFER_AT (record, 0);

 out:
  BUFFER_FREE (record);
  return ret;
}

ssl_t *
ssl_connect (const char *host, int port, const char *starttls)
{
  struct sockaddr_in sa;

  ssl_t *ssl = malloc(sizeof (ssl_t));
  BUFFER_INIT (ssl->challenge);
  BUFFER_INIT (ssl->certificate);
  BUFFER_INIT (ssl->cipher_spec);
  BUFFER_INIT (ssl->connection_id);
  BUFFER_INIT (ssl->master_key);
  ssl->x509_context = 0;
  ssl->server_public_key = 0;
  ssl->encrypted = 0;

  ssl->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ssl->fd < 0) {
    free (ssl);
    return 0;
  }
  
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr(host);
  sa.sin_port = htons(port);
  if (sa.sin_addr.s_addr == 0) {
    free (ssl);
    return 0;
  }


  if (connect (ssl->fd, (struct sockaddr *) &sa, sizeof(sa)) != 0) {
    free (ssl);
    return 0;
  }

  fcntl (ssl->fd, F_SETFL, O_NONBLOCK);
  
  return ssl;
}

void
ssl_disconnect (ssl_t *ssl)
{
  close (ssl->fd);
  BUFFER_CLEAR (ssl->challenge);
  BUFFER_CLEAR (ssl->certificate);
  BUFFER_CLEAR (ssl->cipher_spec);
  BUFFER_CLEAR (ssl->connection_id);
  BUFFER_CLEAR (ssl->master_key);

  if (ssl->server_public_key) {
    EVP_PKEY_free (ssl->server_public_key);
  }

  if (ssl->x509_context) {
    X509_free (ssl->x509_context);
  }

  free (ssl);
}
  

#define SERVER_VALID_ERROR 0
#define HANDSHAKE_FAILURE 1
#define SERVER_CRASH 2
#define SERVER_MALFORMED 3
#define SERVER_CONNECT_FAILURE 4
#define SERVER_X509 5
#define SERVER_VERIFY 6
#define HELLO_FAILURE 7
#define SERVER_STARTTLS_FAILED 8

int
ssl_try_run (const char *host, int port, size_t overflow_count, const char *starttls) 
{
  int ret = 0;
  ssl_t *ssl; 
  
  ssl = ssl_connect (host, port, starttls);
  if (!ssl) {
    return SERVER_CONNECT_FAILURE;
  }

  if (starttls) {
    BUFFER_DECLARE (temp);
    
    /* read initial banner, ignore failure. */
    buffer_read_crlf (ssl->fd, &temp, 1024);
    buffer_assign_string (&temp, starttls);
    buffer_append_string (&temp, "\r\n");
    buffer_write (ssl->fd, temp);
    ret = buffer_read_crlf (ssl->fd, &temp, 1024);
    if (!(buffer_starts_with_string (temp, "220")
	  || buffer_starts_with_string (temp, "+OK")
	  || buffer_starts_with_string (temp, "a002 OK"))) {
      ret = -1;
    }
    
  BUFFER_FREE (temp);

    if (ret == -1) {
      ssl_disconnect (ssl);
      return SERVER_STARTTLS_FAILED;
    }
  }
  
  if (ssl_send_client_hello (ssl)) {
    ssl_disconnect (ssl);
    return HELLO_FAILURE;
  }
  
  ret = ssl_receive_server_hello (ssl);
  if (ret) {
    ssl_disconnect (ssl);
    switch (ret) {
    case ERR_SERVER_HELLO_READ:
      return HELLO_FAILURE;

    case ERR_SERVER_HELLO_X509:
    case ERR_SERVER_HELLO_NO_CIPHER:
    case ERR_SERVER_HELLO_ERROR:
	return SERVER_X509;

    default:
      return HANDSHAKE_FAILURE;
    }
  }

  switch (ssl_check_server_hello (ssl)) {
  case 0:
    break;
  case ERR_SERVER_HELLO_X509:
  case ERR_SERVER_HELLO_NO_CIPHER:
  case ERR_SERVER_HELLO_ERROR:
    ssl_disconnect (ssl);
    return SERVER_X509;

  default:
    ssl_disconnect (ssl);
    return SERVER_X509;
  }
    
  if (ssl_send_client_master_key (ssl, overflow_count)) {
    ssl_disconnect (ssl);
    return HANDSHAKE_FAILURE;
  }

  ret = ssl_fetch_verify_or_error (ssl);
  switch (ret) {
  case ERR_SSL_ERROR_READ:
    ret = SERVER_CRASH;
    break;

  case ERR_SSL_ERROR_LENGTH:
    ret = SERVER_MALFORMED;
    break;
    
  case SSL_MT_SERVER_VERIFY:
    ret = SERVER_VERIFY;
    break;
      
  case SSL_MT_ERROR:
    ret = SERVER_VALID_ERROR;
    break;
    
  default:
    ret = SERVER_MALFORMED;
  }

  ssl_disconnect (ssl);
  return ret;
}

void
vulnerable (const char *host, int port, const char* message)
{
  printf("%s %d VULNERABLE: %s\n", host, port, message);
  exit(1);
}

void
patched (const char *host, int port, const char* message)
{
  printf("%s %d PATCHED: %s\n", host, port, message);
  exit(0);
}

void
undecided (const char *host, int port, const char* message)
{
  printf("%s %d UNDECIDED: %s\n", host, port, message);
  exit(2);
}

void
usage (const char* prog)
{
  fprintf(stderr, "openssl-sslv2-master, an OpenSSL vulnerability detector\n");
  fprintf(stderr, "Copyright (C) 2002 RUS-CERT, University of Stuttgart\n\n");
  fprintf(stderr, "usage: %s [-s] [-P] [-I] HOST-IP [PORT]\n\n", prog);
  fprintf(stderr, "    -s   SMTP STARTTLS support\n");
  fprintf(stderr, "    -P   POP3 STARTTLS support\n");
  fprintf(stderr, "    -I   IMAP STARTTLS support\n\n");
  fprintf(stderr, "Send comments to Florian Weimer <Weimer@CERT.Uni-Stuttgart.DE>.\n");
  exit (4);
}

int
main (int argc, char **argv)
{
  const char *host = 0;
  const char *port_str = 0;
  int port = 0;
  char *starttls = 0;
  int remaining;
  int ret, large, small;
  
  while (1) {
    int c = getopt (argc, argv, "sIP");

    if (c == -1) {
      break;
    }
    
    switch (c) {
    case 's':
      starttls = "STARTTLS";
      break;

    case 'I':
      starttls = "a002 STARTTLS";
      break;

    case 'P':
      starttls = "STLS";
      break;
    }
  }

  remaining = argc - optind;
  if ((remaining == 1) || (remaining == 2)) {
    host = argv[optind];
    port = (remaining == 2) ? atoi(argv[optind + 1]) : 443;
    port_str = (argc == 2) ? argv[optind + 1] : "443";
  } else {
    usage (argv[0]);
  }
  
  ret = ssl_try_run (host, port, 0, starttls);
  switch (ret) {
  case SERVER_VERIFY: 
    break;

  case SERVER_VALID_ERROR:
    undecided (host, port, "first try does not result in VERIFY response");
    break;
    
  case SERVER_CONNECT_FAILURE:
    undecided (host, port, "initial connection failed");
    break;

  case HELLO_FAILURE:
    undecided (host, port, "initial connection unresponsive");
    break;

  case HANDSHAKE_FAILURE:
    undecided (host, port, "initial handshake failed");
    break;
    
  case SERVER_X509:
    undecided (host, port, "first handshake: X.509 mismatch");
    break;
    
  case SERVER_CRASH:
  case SERVER_MALFORMED:
    undecided (host, port, "first handshake failed");
    break;

  case SERVER_STARTTLS_FAILED:
    undecided (host, port, "STARTTLS failed");

  default:
    abort ();
  }

  large = ssl_try_run (host, port, 16000, starttls);
  switch (large) {
  case SERVER_VALID_ERROR:
  case SERVER_MALFORMED:
  case SERVER_CRASH:
  case SERVER_VERIFY:
    break;

  case SERVER_CONNECT_FAILURE:
    undecided (host, port, "second connection failed");
    break;
    
  case HELLO_FAILURE:
    undecided (host, port, "second connection unresponsive");
    break;

  case SERVER_X509:
    undecided (host, port, "second handshake: X.509 mismatch");
    break;
    
  case HANDSHAKE_FAILURE:
    undecided (host, port, "second handshake failed");
    break;

  case SERVER_STARTTLS_FAILED:
    undecided (host, port, "STARTTLS failed");

  default:
    abort ();
  }

  small = ssl_try_run (host, port, 9, starttls);
  switch (small) {
  case SERVER_VALID_ERROR:
  case SERVER_MALFORMED:
  case SERVER_CRASH:
  case SERVER_VERIFY:
    break;

  case SERVER_CONNECT_FAILURE:
    vulnerable (host, port, "third connection failed, DoS vulnerability");
    break;
    
  case HELLO_FAILURE:
    undecided (host, port, "third connection unresponsive");
    break;

  case SERVER_X509:
    undecided (host, port, "third handshake: X.509 mismatch");
    break;
    
  case HANDSHAKE_FAILURE:
    undecided (host, port, "third handshake failed");
    break;

  case SERVER_STARTTLS_FAILED:
    undecided (host, port, "STARTTLS failed");

  default:
    abort ();
  }
  
  switch (large) {
  case SERVER_VERIFY:
    undecided (host, port, "large buffer results in VERIFY response");
    break;

  case SERVER_MALFORMED:
    vulnerable (host, port, "large buffer induces malformed response");
    break;

  case SERVER_CRASH:
    switch (small) {
    case  SERVER_CRASH:
      patched (host, port, "detects small overflow, but crashes (0.9.6e)");
      break;
      
    case  SERVER_VERIFY:
      vulnerable (host, port, "does not detect small overflow");
      break;
      
    case  SERVER_MALFORMED:
      vulnerable (host, port, "small overflow results in malformed response");
      break;

    case  SERVER_VALID_ERROR:
      vulnerable (host, port, "small overflow -> error, large overflow -> crash");
      break;
    }
    break;

  case SERVER_VALID_ERROR:
    switch (small) {
    case  SERVER_VALID_ERROR:
      patched (host, port, "fully patched (0.9.6g)");
      break;

    case  SERVER_CRASH:
      vulnerable (host, port, "odd error, small overflow crashes");
      break;
      
    case  SERVER_MALFORMED:
      vulnerable (host, port, "odd error, malformed response on small overflow");
      break;
      
    case  SERVER_VERIFY:
      vulnerable (host, port, "small overflow results in VERIFY response");
      break;
    }
    break;
  }

  undecided (host, port, "should never happen");

  return 3;			/* never happens */
}
