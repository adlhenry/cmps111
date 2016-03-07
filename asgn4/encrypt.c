/*
 * This code encrypts input data using the Rijndael (AES) cipher.  The
 * key length is hard-coded to 128 key bits; this number may be changed
 * by redefining a constant near the start of the file.
 *
 * This program uses CTR mode encryption.
 *
 * Usage: protectfile <key> <filename>
 *
 * Author: Ethan L. Miller (elm@cs.ucsc.edu)
 * Based on code from Philip J. Erdelsky (pje@acm.org)
 * Extended by: Adam Henry (adlhenry@ucsc.edu)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "rijndael.h"

#define KEYBITS 128

// Program name
char *execname;

// Encrypt and decrypt global flags
int eflag, dflag;

// Print a program usage message to stderr
void usage ()
{
	fprintf(stderr,
	"Usage: %s [-e(--encrypt) | -d(--decrypt)] <key> <file>\n", execname);
	exit(EXIT_FAILURE);
}

// Parse program options
void parse_options (int argc, char **argv)
{
	int opt;
	static struct option longopts[] = {
	{"encrypt",	no_argument,	NULL,	'e'},
	{"decrypt",	no_argument,	NULL,	'd'},
	{NULL,				0,		NULL,	0}
	};

	if (argc != 4) {
		usage();
	}
	eflag = dflag = 0;
	while ((opt	= getopt_long(argc, argv, "ed", longopts, NULL)) != -1) {
		switch (opt) {
			case 'e':
				eflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			default:
				usage();
		}
	}
}

/***********************************************************************
 *
 * hexvalue
 *
 * This routine takes a single character as input, and returns the
 * hexadecimal equivalent.  If the character passed isn't a hex value,
 * the program exits.
 *
 ***********************************************************************
 */
int hexvalue (char c)
{
	if (c >= '0' && c <= '9') {
		return (c - '0');
	} else if (c >= 'a' && c <= 'f') {
		return (10 + c - 'a');
	} else if (c >= 'A' && c <= 'F') {
		return (10 + c - 'A');
	} else {
		fprintf(stderr, "ERROR: key digit %c isn't a hex digit!\n", c);
		exit(-1);
	}
}

/***********************************************************************
 *
 * getpassword
 *
 * Get the key from the password. The key is specified as a string of
 * hex digits, two per key byte. password points at the character
 * currently being added to the key.
 *
 ***********************************************************************
 */
void
getpassword (const char *password, unsigned char *key, int keylen)
{
	int i;
	int passlen = strlen(password);
	if (passlen != 16) {
		fprintf(stderr, "ERROR: key %s is not 16 digits\n", password);
		exit(EXIT_FAILURE);
	}
	for (i = keylen - 1; i >= passlen/2; i--) {
		key[i] = 0;
	}
	for (; i >= 0; i--) {
		key[i] = hexvalue(*(password++)) << 4;
		key[i] |= hexvalue(*(password++));
	}
}

// Encrypt or decrypt a file using 128-bit AES encryption
void aes_encrypt (unsigned char *key, char *filename)
{
	unsigned long rk[RKLENGTH(KEYBITS)];	/* round key */
	struct stat sb;
	int i, nbytes, nwritten, ctr;
	int totalbytes;
	int file_id;
	int nrounds;				/* # of Rijndael rounds */
	int fd;
	unsigned char filedata[16];
	unsigned char ciphertext[16];
	unsigned char ctrvalue[16];

	/*
	* Initialize the Rijndael algorithm.  The round key is initialized by this
	* call from the values passed in key and KEYBITS.
	*/
	nrounds = rijndaelSetupEncrypt(rk, key, KEYBITS);
	
	fd = open(filename, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Error opening file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	
	if (stat(filename, &sb) == -1) {
		fprintf(stderr, "Error with file stat %s\n", filename);
		exit(EXIT_FAILURE);
	}
	file_id = sb.st_ino;

	/* fileID goes into bytes 8-11 of the ctrvalue */
	bcopy(&file_id, &(ctrvalue[8]), sizeof(file_id));

	/* This loop reads 16 bytes from the file, XORs it with the encrypted
	 CTR value, and then writes it back to the file at the same position.
	 Note that CTR encryption is nice because the same algorithm does
	 encryption and decryption.  In other words, if you run this program
	 twice, it will first encrypt and then decrypt the file.
	*/
	for (ctr = 0, totalbytes = 0; ; ctr++) {
		/* Read 16 bytes (128 bits, the blocksize) from the file */
		nbytes = read(fd, filedata, sizeof(filedata));
		if (nbytes <= 0) {
			break;
		}
		if (lseek(fd, totalbytes, SEEK_SET) < 0) {
			perror("Unable to seek back over buffer");
			exit(-1);
		}

		/* Set up the CTR value to be encrypted */
		bcopy(&ctr, &(ctrvalue[0]), sizeof(ctr));

		/* Call the encryption routine to encrypt the CTR value */
		rijndaelEncrypt(rk, nrounds, ctrvalue, ciphertext);

		/* XOR the result into the file data */
		for (i = 0; i < nbytes; i++) {
			filedata[i] ^= ciphertext[i];
		}

		/* Write the result back to the file */
		nwritten = write(fd, filedata, nbytes);
		if (nwritten != nbytes) {
			fprintf(stderr,
				"%s: error writing the file (expected %d, got %d at ctr %d\n)",
				execname, nbytes, nwritten, ctr);
			break;
		}

		/* Increment the total bytes written */
		totalbytes += nbytes;
	}
	close(fd);
}

int main (int argc, char **argv)
{
	unsigned char key[KEYLENGTH(KEYBITS)];	/* cipher key */
	char *filename;
	
	execname = basename(argv[0]);
	parse_options(argc, argv);
	getpassword(argv[2], key, sizeof(key));
	filename = argv[3];
	
	char buf[100];
	/* Print the key, just in case */
	for (int i = 0; i < sizeof(key); i++) {
		sprintf(buf+2*i, "%02x", key[sizeof(key)-i-1]);
	}
	fprintf(stderr, "KEY: %s\n", buf);
	
	aes_encrypt(key, filename);
}
