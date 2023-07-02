#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */
#include <sys/stat.h>
#include "clientLib.h"

void perror_exit(char *message);

int main(int argc, char *argv[]) {

    int             port, sock, i;
  
    char *buf=(char *)malloc(256);
    char *IP=(char *)malloc(256);
    char *bl=(char *)malloc(6);
    buf[0]='\0';

    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;


if (argc != 7) {
        printf("Please give correct number of arguements\n");
        exit(1);
    }


    if(strcmp(argv[1],"-i")==0){
        strcpy(IP,(argv[2]));
    }
    else if(strcmp(argv[3],"-i")==0){
        strcpy(IP,(argv[4]));
    }
    else if(strcmp(argv[5],"-i")==0){
        strcpy(IP,(argv[6]));
    }
    else{
        printf("Wrong Input\n");
        exit(0);
    }

    if(strcmp(argv[1],"-p")==0){
        port=atoi(argv[2]);
    }
    else if(strcmp(argv[3],"-p")==0){
        port=atoi(argv[4]);
    }
    else if(strcmp(argv[5],"-p")==0){
        port=atoi(argv[6]);
    }
    else{
        printf("Wrong Input\n");
        exit(0);
    }

    if(strcmp(argv[1],"-d")==0){
        strcpy(buf,(argv[2]));
    }
    else if(strcmp(argv[3],"-d")==0){
        strcpy(buf,(argv[4]));
    }
    else if(strcmp(argv[5],"-d")==0){
        strcpy(buf,(argv[6]));
    }
    else{
        printf("Wrong Input\n");
        exit(0);
    }

	/* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    	perror_exit("socket");
	/* Find server address */
    if ((rem = gethostbyname(IP)) == NULL) {	
	   herror("gethostbyname"); exit(1);
    }

    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);         /* Server port */


    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
	   perror_exit("connect");
    printf("Connecting to %s port %d\n", IP, port);


    if (read(sock, bl, 6) < 0)
                perror_exit("read");

    int blocks=atoi(bl);

        /*  Stelnw to directory ston server     */
        i=0;
        while(1) { 
        
            if (write(sock, buf + i, 1) < 0)
               perror_exit("write");

            if(buf[i]=='\0')
                break;

            i++;
        }

        
        getFiles();
        printf("serverIP: %s\n",IP);
        printf("port: %d\n",port);
        printf("directory: %s\n",buf);
        
        free(buf);


        int flag1=0,j=0;
        char str[9];

        char *filenam=(char *)malloc(100);
        char *folder=(char *)malloc(200);

        char *content=(char *)malloc(blocks);
        
             

        FILE *fp;
        struct stat newsb;
        struct stat st = {0};
        
        filenam[0]='\0';
        folder[0]='\0';
        content[0]='\0';

    
        while(1){

                   if(flag1!=2){ 

                            if (read(sock, str, 8) < 0)
                                    perror_exit("read");
                    }

                    str[8]='\0';

                    if(strcmp(str,"end")==0)
                        break;




                    /*  flag1==0, pairnw to filename, dhmiourgw antisoixous fakelous, diagrafw to arxeio an uparxei kai telos allazw se flag=1  */
                    /*  flag1==1, metaferw ta metadata kai allazw se flag=2 */
                    /*  flag1==2, metaferw ta periexomena tou arxeiou kai allazw se flag=0  */
                    if(     (flag1==0)             ){


                            /*  Termination Condition   */
                            if(strcmp(str,"#")!=0){

                                        strcat(filenam,str);
                            }
                            else{

                                        flag1=1;                                                               
                                        printf("\n%s\n",filenam);
                                        
                                        for(i=0;filenam[i]!='\0';i++){                                              
                                                
                                                if(filenam[i] == '/'){
                                                        
                                                        /*  Dhmiourgw fakelous  */
                                                        if (stat(folder, &st) == -1) {
                                                                mkdir(folder, 0777);
                                                        }                   
                                                }

                                               strncat(folder,&filenam[i],1); 
                                        }
                                        folder[0]='\0';

                                        /*  Diagrafei to arxeio an uparxei  */
                                        if(stat(filenam,&st)==0)                           
                                            remove(filenam);

                                        
                                        fp = fopen (filenam, "w+");

                                        if (stat(filenam, &newsb) == -1) {
                                                perror("stat");
                                                exit(EXIT_FAILURE);
                                        }

                                                         
                                        filenam[0]='\0';

                            }
                    }                         
                    else if(    (flag1==1)          ){

                        /*  Termination Condition   */
                            if(strcmp(str,"#")!=0){


                                        //printf("%s\n             ",str);

                                        if(j==0){
                                            newsb.st_ino=atoi(str);
                                            printf("I-node number:            %ld\n", (long) newsb.st_ino);
                                        }
                                        else if(j==1){
                                            newsb.st_mode=atoi(str); 
                                            printf("Mode:                     %lo (octal)\n",(unsigned long) newsb.st_mode);                                                 
                                        }
                                        else if(j==2){
                                            newsb.st_nlink=atoi(str);
                                        }
                                        else if(j==3){
                                            newsb.st_uid=atoi(str);
                                        }
                                        else if(j==4){
                                            newsb.st_gid=atoi(str);
                                        }
                                        else if(j==5){
                                            newsb.st_blksize=atoi(str);
                                               printf("Preferred I/O block size: %ld bytes\n",(long) newsb.st_blksize);
                                        }
                                        else if(j==6){
                                            newsb.st_size=atoi(str);
                                            printf("File size:                %lld bytes\n",(long long) newsb.st_size);
                                        }
                                        else if(j==7){
                                            newsb.st_blocks=atoi(str);
                                            printf("Blocks allocated:         %lld\n",(long long) newsb.st_blocks);
                                        }

                                        j++;
                            }
                            else{
                                        
                                        j=0;
                                        flag1=2;
                            }



                    }       
                    else if(    (flag1 == 2)          ){ 



                                /*  Termination Condition   */
                                while(1){

                                        if (read(sock, content, blocks) < 0)
                                                perror_exit("read");


                                        if(strcmp(content,"$")==0)
                                                break;

                                        fprintf(fp, "%s",content);
                                }
                                

                                        
                                        filenam[0]='\0';
                                        flag1=0;
                                
                    }


           
    }
    fclose(fp);
    free(filenam);
    free(folder);
    free(IP);
    free(content);

    /* Close socket and exit */
    close(sock);                 
    
    
}			     





void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
