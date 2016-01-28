#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

struct message {
    long mtype;
    char mtext[80];
};

//const char * dir = "/tmp/msg.temp";
//const char * g_outpath = "/home/box/message.txt";
const char * g_dir = "/home/skap/README";
const char * g_outpath = "output";


int main(){
	key_t key = ftok(g_dir, 0);
	if (key == -1){
		printf("ftok: %s\n", strerror(errno));
		return 0;
	}
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int msgid = msgget(key, mode | IPC_CREAT);
	if (msgid == -1) {
		printf("msgid: %s\n", strerror(errno));
		return 0;
	}
	struct message msg;
	int got = msgrcv(msgid, &msg, 80, 0, 0);
	if (got == -1){
		printf("msgrcv: %s\n", strerror(errno));
		return 0;
	}
	FILE * f = fopen(g_outpath, "wb");
	fwrite(msg.mtext, sizeof(char), got, f);
	fclose(f);

	return 0;
}