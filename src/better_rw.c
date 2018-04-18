#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "stack_queue.h"
#include "better_rw.h"
#define INPUT_SIZE 1024
/*int readline(int des,char * dest,int max_size){
    static struct Queue * buff = NULL;
    if(buff == NULL){
        buff = NewQueue(BUFF_SIZE);
    }
    char temp[INPUT_SIZE];
    int pos = 0;
    char is_newline = 0;
    while(!is_newline){
        while(lenQ(buff)){
            dest[pos++] = frontQ(buff);
            popQ(buff);
            if(dest[pos-1] == '\n'){
                return pos;
            }
        }
        read(des,temp,INPUT_SIZE);



}*/

int readline(int ds,char * dest,int max_size){
    int pos = 0;
    while(1){
        if(pos == max_size){
            //printf("Im here!\n");
            char drop;
            while(1){
                int stt = read(ds,&drop,1);
                //printf("Dropped %c\n",drop);
                if(stt == 0 || drop == '\n')
                    return -2;
            }
        }
        int stt = read(ds,dest+pos,1); // Vim testcase...
        if(stt == -1) return -1;
        if(stt == 0 ){
            dest[pos] = '\0';
            return pos;
        }
        if(dest[pos] == '\n'){
		    dest[pos+1] = '\0';
            return pos+1;
        }
        pos++;
    }
}

int writeline(int descriptor,const char * src){
    return write(descriptor,src,strlen(src));
}
