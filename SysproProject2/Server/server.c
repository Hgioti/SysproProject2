#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */		
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "myLib.h"

#define perror2(s, e) fprintf(stderr, "%s: %s\n", s, strerror(e))
static int flag = 0;
static int sock;
static int threads;
static int blocks;

struct Queue* queue;

void sig_handler(int signum){

            int err;

            printf("\nTerminating Program\n");
            printf("Freeing memory\n");


            /* Destroy mutexes */
            if (err = pthread_mutex_destroy(&mtx)) { /* Destroy mutex */
                    perror2("pthread_mutex_destroy", err); 
                    exit(1); 
            }
            

            /* Destroy termination variables */
            if (err = pthread_cond_destroy(&cvar)) {
                perror2("pthread_cond_destroy", err); 
                exit(1); 
            }
            if (err = pthread_cond_destroy(&cvar2)) {
                perror2("pthread_cond_destroy", err); 
                exit(1); 
            }

            close(sock);
            exit(1);
}



int main(int argc, char *argv[]) {

    int      port, newsock,i,err,queue_size;
    char *blocks_string=(char *)malloc(6);

    struct sockaddr_in server, client;
    socklen_t clientlen;

    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    pthread_cond_init(&cvar, NULL);
    pthread_cond_init(&cvar2, NULL);
    
    

        if (argc != 9) {
            printf("Please give correct number of arguements\n");
            exit(1);
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
        else if(strcmp(argv[7],"-p")==0){
            port=atoi(argv[8]);
        }
        else{
            printf("Wrong Input\n");
            exit(0);
        }

        if(strcmp(argv[1],"-s")==0){
            threads=atoi(argv[2]);
        }
        else if(strcmp(argv[3],"-s")==0){
            threads=atoi(argv[4]);
        }
        else if(strcmp(argv[5],"-s")==0){
            threads=atoi(argv[6]);
        }
        else if(strcmp(argv[7],"-s")==0){
            threads=atoi(argv[8]);
        }
        else{
            printf("Wrong Input\n");
            exit(0);
        }

        if(strcmp(argv[1],"-q")==0){
            queue_size=atoi(argv[2]);
        }
        else if(strcmp(argv[3],"-q")==0){
            queue_size=atoi(argv[4]);
        }
        else if(strcmp(argv[5],"-q")==0){
            queue_size=atoi(argv[6]);
        }
        else if(strcmp(argv[7],"-q")==0){
            queue_size=atoi(argv[8]);
        }
        else{
            printf("Wrong Input\n");
            exit(0);
        }

        if(strcmp(argv[1],"-b")==0){
            strcpy(blocks_string,argv[2]);
            blocks=atoi(argv[2]);
        }
        else if(strcmp(argv[3],"-b")==0){
            strcpy(blocks_string,argv[4]);
            blocks=atoi(argv[4]);
        }
        else if(strcmp(argv[5],"-b")==0){
            strcpy(blocks_string,argv[6]);
            blocks=atoi(argv[6]);
        }
        else if(strcmp(argv[7],"-b")==0){
            strcpy(blocks_string,argv[8]);
            blocks=atoi(argv[8]);
        }
        else{
            printf("Wrong Input\n");
            exit(0);
        }

        queue = createQueue(queue_size);

        printf("Server's parameters are:\n");
        printf("port %d\n", port);
        printf("thread_pool_size %d\n", threads);
        printf("queue_size %d\n", queue_size);
        printf("Block_size %d\n", blocks);
        printf("Server was successfully initialized...\n");

         signal(SIGINT,sig_handler);
    

    /* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */

    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0)
        perror_exit("bind");

    /* Listen for connections */
    if (listen(sock, 5) < 0) 
        perror_exit("listen");

   // printf("Server's parameters are:")
    printf("Listening for connections to port %d\n", port);
    pthread_t comm; 
    pthread_t *tids;
    

    if ((tids = malloc(threads * sizeof(pthread_t))) == NULL) {
         perror("malloc");  exit(1); 
     }




            /* Xrhsimopoiw mia atermonh loopa me signal handler wste na eleu8erwnw th mnhmh sto ^C  */   
            while (1) {

                    
                    clientlen = sizeof(client);

                    /* accept connection */
                	if ((newsock = accept(sock, clientptr, &clientlen)) < 0) 
                        perror_exit("accept");

                	/* Find client's address */
                	if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
                  	    herror("gethostbyaddr"); 
                        exit(1);
                    }
                	printf("Accepted connection from %s\n", rem->h_name);

                    
                            if (write(newsock, blocks_string, 6) < 0)
                                            perror_exit("write"); 
                    

                 
                            /* Communication thread */
                            if (err = pthread_create(&comm, NULL, communication_thread,(void *) newsock)) {    
                                perror2("pthread_create", err); 
                                exit(1); 
                            } 

                            
                            /* Worker thread */
                              for (i=0 ; i<threads ; i++){
                                    if (err = pthread_create(tids+i, NULL, worker_thread,(void *) newsock)) {    
                                        perror2("pthread_create", err); 
                                        exit(1); 
                                    } 
                                }

                  
                        /* Wait for thread termination */                        
                        for (i=0 ; i<threads ; i++){
                                    if (err = pthread_join(*(tids+i), NULL)) {
                                      perror2("pthread_join", err); 
                                      exit(1); 
                                    }
                        }    

                             if (err = pthread_join(comm, NULL)) {                            
                                  perror2("pthread_join", err); 
                                  exit(1); 
                            }

                            printf("Closing connection\n\n");
                            flag=0;
                            close(newsock);
            } 
}






/* Thread function */
void *communication_thread(void *i){   

    
            int err;
            int newsock=(int) i;
            char buf[1];
          
            char *str=(char *)malloc(100);
            str[0]='\0';

            if (err = pthread_mutex_lock(&mtx)) {                      
                perror2("pthread_mutex_lock", err); 
                exit(1); 
            }


            /*  Diabazw to path input apo ton client     */
            while(read(newsock, buf, 1) > 0) 
            {             
                        strncat(str, buf, 1);

                        if(buf[0]=='\0')
                            break;           
            }
            
            printf("About to scan directory %s\n",str);   
            ReadFiles(str);


                     
           
            /*      End stamement : Empty the Queue     */
            while(!Empty(queue)){
             
                        pthread_cond_broadcast(&cvar2);
                        pthread_cond_wait(&cvar, &mtx);                 
            }

            /*  Allazw to flag gia na termatisoun oi workers  */ 
            flag=1;
 
            /*  H communication thread perimenei na teleiwsoun oloi oi workers kanontas signal broadcast kai wait     */
            /*  Ka8e fora pou teleiwnei enas worker anebazei to flag kata 1     */
            while(flag!=(threads+1)){

                        pthread_cond_broadcast(&cvar2);
                        pthread_cond_wait(&cvar, &mtx);  
            }


            
            

            /*  Send Termination String     */
            if (write(newsock, "end", 8) < 0)
                    perror_exit("write");

            free(str);
            if (err = pthread_mutex_unlock(&mtx)) 
            {                  
                perror2("pthread_mutex_unlock", err); 
                exit(1);         
            }


    


    printf("CThread %d exits\n",pthread_self());
    pthread_exit(NULL);
}





/* Thread function */
void *worker_thread(void *soc){   

    int err;
    int newsock=(int) soc;
    int k,i=0,j=0,c;

    char buf[1];

    char *content=(char *)malloc(blocks);    
    char *str=(char *)malloc(100);
    char *cl_str=(char *)malloc(100);
    char *met=(char *)malloc(30);

    cl_str[0]='\0';
    str[0]='\0';


    DIR *d;
    struct dirent *dir;
    struct stat sb;



    if (err = pthread_mutex_lock(&mtx)) {                       
                perror2("pthread_mutex_lock", err); 
                exit(1); 
    }

    FILE *fp;


    /*  Termination condition,  to metaballei h comm thread otan ftasei sto telos   */
    while(flag==0){


            /*  Wait if queue is empty  */
            if(Empty(queue)){
                //printf("Queue is empty\n");
                pthread_cond_signal(&cvar);
                pthread_cond_wait(&cvar2, &mtx);

            }

            i=0;
            j=0;
           

            

            /*  Empty the queue contents    */
            while(!Empty(queue)){

                            /*  Get the Filename    */
                            strcpy(str,dequeue(queue));
                                       

                            if (stat(str, &sb) == -1) {
                                        perror("stat");
                                        exit(EXIT_FAILURE);
                            }

                                

                            
                            printf("[WThread %d]: Received task <%s>\n",pthread_self(),str);
                            printf("[WThread %d]: About to read file %s\n",pthread_self(),str);


                            cl_str[0]='\0';
                            strcpy(cl_str,str);                 
                            k=0;
                            i=0;

                            /*   Metaferw to path ston client  ana 8 bytes */
                            while(cl_str[i]!='\0'){
                                
                                        if (write(newsock, cl_str+k, 8) < 0)
                                            perror_exit("write"); 

                                        i=k;
                                        k+=8;

                                        while((cl_str[i]!='\0') && (i<k)){
                                            i++;
                                        }                   
                            }

                           
                            /*  Termination char    */
                            if (write(newsock,"#", 8) < 0)
                                        perror_exit("write"); 






                            /*Pairnw kai metaferw ston client ta Metadata   */
                                            met[0]='\0';
                                           
                                            sprintf(met,"%ld",sb.st_ino);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%lo",sb.st_mode);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%ld",sb.st_nlink);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%ld",sb.st_uid);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%ld",sb.st_gid);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%ld",sb.st_blksize);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%lld",sb.st_size);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                            sprintf(met,"%lld",sb.st_blocks);
                                            if (write(newsock,met, 8) < 0)
                                                perror_exit("write");
                                        
                                        /*  Termination char    */
                                        if (write(newsock,"#", 8) < 0)
                                                perror_exit("write");







                                     /*Metaferw kai ta periexomena tou arxeiou  */
                                           
                                            fp = fopen(str,"r");
                                            content[0]='\0';

                                            
                                            /*  Xrhsimopoiw thn fgetc opou mazeuw ta grammata se ena string twn 8 kai to stelnw     */
                                           while(1) {

                                                    c = fgetc(fp);                                  

                                                    if( feof(fp) ) {   

                                                            if (write(newsock, content, blocks) < 0)
                                                                perror_exit("write");

                                                            if (write(newsock, "$", blocks) < 0)
                                                                     perror_exit("write");
                                                             break ;
                                                    }

                                            
                                                    strncat(content,&c,1);
                                                    if(j<(blocks-1)){
                                                                              
                                                        j++;
                                                    }
                                                    else{
                                                        
                                                         if (write(newsock, content, blocks) < 0)
                                                             perror_exit("write");

                                                        content[0]='\0';
                                                        j=0;
                                                    }
                                           }
                                           fclose(fp);
                            

                                           pthread_cond_signal(&cvar);
                                           pthread_cond_wait(&cvar2, &mtx);                   
                    }                   
    }

    free(str);
    free(cl_str);
    free(content);
    free(met);

    flag++;
    pthread_cond_signal(&cvar);


    if (err = pthread_mutex_unlock(&mtx)) {                  
                perror2("pthread_mutex_unlock", err); 
                exit(1); 
    }

    


    printf("WThread %d exits\n",pthread_self());
    pthread_exit(NULL);
}





/*  H sunarthsh auth briskei ta files anadromika me thn dirent  */
void ReadFiles(char *basePath)
{
    
    char *path=(char *)malloc(256);
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    struct stat sd;


    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {

            
                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);

                /*  An full perimenei to comm thread    */
                if(Full(queue)){

                            //printf("Queue is full\n");
                            pthread_cond_broadcast(&cvar2);
                            pthread_cond_wait(&cvar, &mtx);


                }

                if (stat(path, &sd) == -1) {
                                    perror("stat");
                                    exit(EXIT_FAILURE);
                    
                }

                            

                    /*  Tsekarw an einai arxeio kai to bazw sthn oura   */
                        if((sd.st_mode & S_IFMT)==S_IFREG){

                            printf("[CThread %d]: Adding file %s to the queue\n",pthread_self(),path);   
                            enqueue(queue,path);
                        }
                       
                        ReadFiles(path);
                
        }
    }

    closedir(dir);
    free(path);
}


void perror_exit(char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}