/*
 *
 * Copyright (C) 1998-2019 Maruthi Seshidhar Inukonda - All Rights Reserved.
 * maruthi.inukonda@gmail.com
 *
 * This file is released under the LGPL v2.
 *
 * This file may be redistributed under the terms of the Lesser GNU Public
 * License.
 *
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>

#include <myshm.h> // Header file common between kernel and userspace code.
#include <libmyshm.hpp> // Library header file (only user-space).

using namespace std;

void print_help(const char *, int);

typedef enum operation { OP_NONE, OP_CREATE, OP_DELETE, OP_SYSREAD, OP_SYSWRITE, OP_MAPREAD, OP_MAPWRITE } operation_t;
#define OP_MAX 10
operation_t op[OP_MAX];
int op_cnt = 0;

char *lock_file = NULL;

char *shm_file = NULL;
int shm_fd = -1;
char *shm_addr = NULL;
int shm_oflag = 0;

char *msg = NULL;
int msg_len = 0;
int msg_buf_len = 0;

int main(int argc, char *argv[])
{
	int c;
	bool create_flag = false, delete_flag = false, read_flag = false, write_flag = false;
	int ret = 0;

	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"help", 1, 0, 'h'},
			{"lock", 1, 0, 'l'},
			{"message", 1, 0, 'm'},
			{"operation", 1, 0, 'o'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv,"hl:m:o:" , long_options, &option_index);
		if (c == -1)        //end of the command, getopt semantics
			break;

		switch (c) {
			case 'h':
				print_help(argv[0], -1);
				break;  

			case 'l':
				if (!optarg) {
					print_help(argv[0], -1);
				}
				if (lock_file) {
					cerr << "Duplicate lock file options\n";
					print_help(argv[0], -1);
				}

				lock_file = optarg;
				break;

			case 'm':
				if (!optarg) {
					print_help(argv[0], -1);
				}
				if (msg) {
					cerr << "Duplicate message options\n";
					print_help(argv[0], -1);
				}

				msg = optarg;
				msg_len = strlen(optarg);
				break;

			case 'o':
				if (!optarg) {
					print_help(argv[0], -1);
				}
				if (op_cnt == OP_MAX) {
					cerr << "Too many operations\n";
					print_help(argv[0], -1);
				}

				if (strcmp("create", optarg) == 0) {
					op[op_cnt] = OP_CREATE;
					op_cnt++;
					create_flag = true;
				} else if (strcmp("delete", optarg) == 0) {
					op[op_cnt] = OP_DELETE;
					op_cnt++;
					delete_flag = true;
				} else if (strcmp("sysread", optarg) == 0) {
					op[op_cnt] = OP_SYSREAD;
					op_cnt++;
					read_flag = true;
				} else if (strcmp("syswrite", optarg) == 0) {
					op[op_cnt] = OP_SYSWRITE;
					op_cnt++;
					write_flag = true;
				} else if (strcmp("mapread", optarg) == 0) {
					op[op_cnt] = OP_MAPREAD;
					op_cnt++;
					read_flag = true;
				} else if (strcmp("mapwrite", optarg) == 0) {
					op[op_cnt] = OP_MAPWRITE;
					op_cnt++;
					write_flag = true;
				} else {
					cerr << "Invalid operation: " << optarg << "\n";
					print_help(argv[0], -1);
				}
				break;

			case ':':
				cerr <<  "Argument missing for option " << long_options[option_index].name << "\n";
				print_help(argv[0], -1);
				break;

			case '?':
				cerr << "Error, No such option\n";
				print_help(argv[0], -1);
				break;

			default:
				cerr <<  "?? getopt returned character code " << c << "\n";
		}
	}

	if (optind > argc - 1) {
		cerr << "Need shm argument\n";
		print_help(argv[0], -1);
	} else if (optind < argc - 1) {
		cerr <<  "Too many arguments\n";
		print_help(argv[0], -1);
	}
	shm_file = argv[optind];

	const int buf_len = MYSHM_LEN+1;
	char cmpbuf[buf_len];

	// --message is must along with read/write
	if (read_flag || write_flag) {
		if (msg_len == 0 || msg == NULL) {
			cerr << "Need message argument along with read/write\n";
			print_help(argv[0], -1);
		}
		off_t off = 0;
		size_t len;

		memset(cmpbuf, 0, buf_len);
		// Fill the given msg into a compare buffer.
		do {
			len = msg_len < MYSHM_LEN - off ? msg_len : MYSHM_LEN - off;
			if (len <= 0) {
				break;
			}
			//cout << "len: " << len << " off:" << off << " \n";
			memcpy(cmpbuf+off, msg, len);
			off += len;
		} while (true);
	}

	// Figure out appropriate oflag from --op
	if (create_flag) {
		shm_oflag = O_CREAT;
	} else if (write_flag) {
		shm_oflag = O_RDWR;
	} else if (read_flag) {
		shm_oflag = O_RDONLY;
	} else { // just read or no operation
		shm_oflag = O_RDONLY;
	}

	shm_fd = myshm_open(shm_file, shm_oflag, 0700);
	if (shm_fd < 0) {
		perror("myshm_open failed");
		exit(-2);
	}
#if 0
	//if (truncate(shm_file, MYSHM_LEN) < 0) {
	//if (ftruncate(shm_fd, MYSHM_LEN) < 0) {
		perror("truncate failed");
		exit(-2);
	}
#endif
	char buf[buf_len];

	for (int i=0; i < op_cnt; i++) {
		memset(buf, 0, buf_len);
		switch (op[i]) {
			case OP_SYSREAD: {
				off_t off = 0;
				size_t len;
				ssize_t nb;
				// seek to offset 0 (previous write/read may have moved it off)
				if (lseek(shm_fd, off, SEEK_SET) < 0) {
					perror("lseek failed");
					exit(3);
				}
				// Read the data from shm into buf.
				// Doing I/O in msg_len to ensure off is adjusted by driver correctly.
				do {
					len = msg_len < MYSHM_LEN - off ? msg_len : MYSHM_LEN - off;
					if (len <= 0) {
						break;
					}
					nb = read(shm_fd, buf+off, len);
					if (nb < 0) {
						perror("read failed");
						exit(3);
					}
					//cout << "read " << nb << " bytes at off " << off << " \n";
					off += nb;
				} while (nb != 0);

				// Compare the data read from shm with cmpbuf
				for (off = 0; off < MYSHM_LEN; off++) {
					if (*(cmpbuf+off) != *(buf+off)) {
						cerr << "mismatch of data. off=" << off << "\n";
						cerr << "cmpbuf: " << *(cmpbuf+off) << "\n";
						cerr << "buf   : " << *(buf+off) << "\n";
						ret = -3;
						break;
					}
				}
#if 0
				if ((r = memcmp(buf, cmpbuf, MYSHM_LEN)) != 0) {
					cerr << "mismatch of data. ret=" << r << "\n";
					cerr << "mismatch of data\n";
					cerr << "len(cmpbuf): " << strlen(cmpbuf) << "\n";
					cerr << "len(buf)   : " << strlen(buf) << "\n";
					cerr << "cmpbuf: " << cmpbuf << "\n";
					cerr << "buf   : " << buf << "\n";
					ret = -3;
				}
#endif
				break;
			}

			case OP_SYSWRITE: {
				off_t off = 0;
				size_t len;
				ssize_t nb;
				// seek to offset 0 (previous write/read may have moved it off)
				if (lseek(shm_fd, off, SEEK_SET) < 0) {
					perror("lseek failed");
					exit(3);
				}
				// Write the message to shm from msg.
				// Doing I/O in msg_len to ensure off is adjusted by driver correctly.
				do {
					len = msg_len < MYSHM_LEN - off ? msg_len : MYSHM_LEN - off;
					if (len <= 0) {
						break;
					}
					nb = write(shm_fd, msg, len);
					if (nb < 0) {
						perror("write failed");
						exit(3);
					}
					//cout << "wrote " << nb << " bytes at off " << off << " \n";
					off += nb;
				} while (nb != 0);

				break;
			}

			case OP_MAPREAD: {
				off_t off = 0;
				size_t len;
				// memory map the shm's kernel buffer into user-space segment.
				shm_addr = (char*) mmap(NULL, MYSHM_LEN, PROT_READ, MAP_SHARED, shm_fd, off);
				if (shm_addr == MAP_FAILED) {
					perror("mmap read failed");
					exit(3);
				}
				// Read the data from shm into buf.
				// Doing I/O in msg_len to ensure off is not adjusted by driver in mmap'd read.
				do {
					len = msg_len < MYSHM_LEN - off ? msg_len : MYSHM_LEN - off;
					if (len <= 0) {
						break;
					}
					memcpy(buf + off, shm_addr + off, len);
					off += len;
				} while (true);

				// Compare the data read from shm with cmpbuf
				for (off = 0; off < MYSHM_LEN; off++) {
					if (*(cmpbuf+off) != *(buf+off)) {
						cerr << "mismatch of data. off=" << off << "\n";
						cerr << "cmpbuf: " << *(cmpbuf+off) << "\n";
						cerr << "buf   : " << *(buf+off) << "\n";
						ret = -3;
						break;
					}
				}
#if 0
				if ((r = memcmp(buf, cmpbuf, MYSHM_LEN)) != 0) {
					cerr << "mismatch of data. ret=" << r << "\n";
					cerr << "mismatch of data\n";
					cerr << "len(cmpbuf): " << strlen(cmpbuf) << "\n";
					cerr << "len(buf)   : " << strlen(buf) << "\n";
					cerr << "cmpbuf: " << cmpbuf << "\n";
					cerr << "buf   : " << buf << "\n";
					ret = -3;
				}
#endif
				// unmap the shm's kernel buffer.
				munmap(shm_addr, MYSHM_LEN);
				//cout << "buf: " << buf << "\n";
				break;
			}

			case OP_MAPWRITE: {
				off_t off = 0;
				size_t len;
				// memory map the shm's kernel buffer into user-space segment.
				shm_addr = (char*) mmap(NULL, MYSHM_LEN, PROT_WRITE|PROT_READ, MAP_SHARED, shm_fd, off);
				if (shm_addr == MAP_FAILED) {
					perror("mmap write failed");
					exit(3);
				}
				// Write the message to shm from msg.
				// Doing I/O in msg_len to ensure off is not adjusted by driver in mmap'd write.
				do {
					len = msg_len < MYSHM_LEN - off ? msg_len : MYSHM_LEN - off;
					if (len <= 0) {
						break;
					}
					memcpy(shm_addr + off, msg, len);
					off += len;
				} while (true);
				// unmap the shm's kernel buffer.
				munmap(shm_addr, MYSHM_LEN);
				break;
			}
		} // end switch
	} // end for(ops)
	
	close(shm_fd);

	if (delete_flag) {
		myshm_unlink(shm_file);
	}

	return ret;
}

void print_help(const char *name, int exit_value)
{
	cerr << "Usage: " << name << " [options] <shmname>\n";
	cerr << "Options:\n";
	cerr << "--operation <optype> : Where optype can be create, sysread, syswrite, mapread, mapwrite, delete\n";
	cerr << "--message <message>  : Message to be written/read-compare to/from the shm\n";
	cerr << "--help               : Show this help\n";
	cerr << "--lock <lockfile>    : Use this lock for reader write synchronization. Default /tmp/myrdwr_lock\n";

	exit(exit_value);
}

