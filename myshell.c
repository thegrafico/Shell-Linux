#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

extern int errno; /* system error number */

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators

extern char **environ;                         //VARIABLE DE AMBIENTE

void syserrmsg(char *, char *);      // error message printout
void syserr(char* );                // error message printout

/*******************************************************************/

int main (int argc, char ** argv)
{   
    //-------------FORK-------------------
    pid_t pid;  
    pid = getpid();
    //------------------------------------


    //VARIABLES PARA MANEJAR LA ENTRADA DE NUESTRO SHELL
    char linebuf[MAX_BUFFER];                  // line buffer
    char cmndbuf[MAX_BUFFER];                  // command buffer
    char cwdbuf[MAX_BUFFER];
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args

// keep reading input until "quit" command or eof of redirected input 
 
    while (!feof(stdin)) { 

// set up prompt

        if(getcwd(cwdbuf, sizeof(cwdbuf)))   // read current working directory
            printf("[%s] ", cwdbuf);        
        else
            printf("getcwd ERROR");
        fflush(stdout);

// get command line from input
  
        if (fgets(linebuf, MAX_BUFFER, stdin )) { // read a line
        
// tokenize the input into args array 

            arg = args;
            *arg++ = strtok(linebuf,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL 
 
            if (args[0]) {                     // if there's anything there

                cmndbuf[0] = 0;                // set zero-length command string
            
                    // check for internal/external command 
                    //-------------------------------CD COMMAND-----------------------------
                if (!strcmp(args[0],"cd")) {   // "cd" command
                    if (!args[1]) {            // no directory argument
                        if (getcwd(cwdbuf, sizeof(cwdbuf))) //Obtener el directorio donde estoy
                            printf("%s\n", cwdbuf);           //mostrar ese directorio
                        else
                            syserrmsg("retrieving cwd",NULL);       //error
                    } else {
                        if (chdir(args[1])) {  //si cambiamos de directorio
                            syserrmsg("change directory failed",NULL);
                        } else {
                            strcpy(cwdbuf,"PWD=");                      //cambiamos la variable PWD del ENVIRON
                            if ((getcwd(&cwdbuf[4], sizeof(cwdbuf)-4))) {
                                if (putenv(strdup(cwdbuf)))                //ACTUALIZAMOS la variable environ: PWD
                                    syserrmsg("change of PWD environment variable failed",NULL); //error si no se cambia
                            } else {
                                syserrmsg("retrieving cwd",NULL);       //error message
                            }
                        }
                    }
                 //--------------------------------OUTPUT REDIRECTION PARA 2 INPUTS--------------------------     

                }else //command         command                 redirection             name 
                if (args[0] != NULL && args[1] != NULL && !strcmp(args[2],">") && args[3] != NULL ) {  // "clr" command
                     strcat(args[3], ".txt"); //convertir mi argumento en un txt
                     int fil = open(args[3], O_CREAT |  O_APPEND | O_WRONLY); //crear el archivo, append si hay informacion y abrir en Write/Read ONLY
                     int red; //redirection
                     
                     //si ocurre un error
                     if (fil < 0){
                        perror("open");
                        exit(1);
                     }
                     //ENVIAR EL OUTPUT al archivo fil
                     red = dup2(fil, 1);

                    //si ocurre un error
                     if (red < 0){
                        perror("dub2");
                        exit(1);
                     }
                     //los system call
                     system(args[0]);
                     system(args[1]);

                     close(fil); //close write file
                     printf("Hecho, puedes buscar el archivo en la carpeta raiz donde se encuentra este archivo.\n");
                     return 0;
                    /* //-----------------------------------OUTPUT REDIRECTION PARA UN ARGUMENTO---------------------------
                }else //command                  redirection             name 
                if (args[0] != NULL && !strcmp(args[1],">") && args[2] != NULL ) {  // "clr" command
                     strcat(args[2], ".txt"); //convertir mi argumento en un txt
                     int fil = open(args[2], O_CREAT |  O_APPEND | O_WRONLY); //crear el archivo, append si hay informacion y abrir en Write/Read ONLY
                     int red; //redirection
                     
                     //si ocurre un error
                     if (fil < 0){
                        perror("open");
                        exit(1);
                     }
                     //ENVIAR EL OUTPUT al archivo fil
                     red = dup2(fil, 1);

                    //si ocurre un error
                     if (red < 0){
                        perror("dub2");
                        exit(1);
                     }
                     //los system call
                     system(args[0]);
                     close(fil); //close write file
                     printf("Hecho, puedes buscar el archivo en la carpeta raiz donde se encuentra este archivo.\n");
                     return 0;*/
                }
                   //----------------------------CLEAR COMMAN(clr))------------------------
                 else    
                if (!strcmp(args[0],"clr")) {  // "clr" command
                    
                    //get the PID CHILL
                    pid = getpid(); //obtener id del hijo
                    printf("Process ID in child after fork: %d\n", pid);
                    args[0] = "clear"; //enviar parametro al fork

                }//----------------------------------------------ECHO COMMAND---------------------------------
                else    
                if (!strcmp(args[0],"echo") && args[1] != NULL) {  //si hay un primer argumento
                        args[0] = "echo";                //si hay mas de 2 argumentos print el mensaje en el fork
                }
                //----------------------------------------------PAUSE COMMAND---------------------------------
                else    
                if (!strcmp(args[0],"pause")) {  //si el primer parametro es = a "pause"
                   
                        getpass("press enter to continue");
                    } 
                //------------------------------------------DIR COMMAND----------------------------------
                else 
                if (!strcmp(args[0],"dir")) {  // "dir" command
                    args[0] = "ls";
                }
                //--------------------------------------ENVIRON COMMAND----------------------------
                 else
                if (!strcmp(args[0],"environ")) { // "environ" command
                    char ** envstr = environ;
                    while (*envstr) {            // print out environment
                        printf("%s\n",*envstr++);
                    }                            
                }
                //------------------------------HELP COMMAND---------------------------------
                else    
                if (!strcmp(args[0],"help")) {  //si hay un primer argumento
                     const char * fn = "readme.txt"; //file name
                     const static int maxString = 1024; //read buffer size
                        
                     char buf[maxString];
                     FILE * fr = fopen(fn, "r"); //open to read
                     char *rc;
                     while ((rc = fgets(buf, maxString, fr))) //mientras haya algo que leer
                     {
                        printf("%s", buf); //print el mensaje
                     }
                     fclose(fr);
            }
                //---------------------------------QUIT (EXIT) COMMAND----------------------------- 
                else
                if (!strcmp(args[0],"quit")) {   //si el primer parametro es quit
                    return 0;//cierra el programa
                }
                 /*//--------------------------------INPUT REDIRECTION--------------------------     
                 else //    
                if (!strcmp(args[1],"<")) {
                     int maxString = 1024; //read buffer size
                     char buf[maxString];
                     FILE * fr = fopen(args[2], "r+"); //open to read

                     char *rc;
                     while ((rc = fgets(buf, maxString, fr)))
                     {

                        printf("%s\n", buf);
                      system(buf);
                     }
                     fclose(fr);
                     return 0;
                }//*/
                    //----------------------SI NO SE INGRESA NINGUNO DE LOS COMANDOS----------------------------------
                 else {                         // pass command on to OS shell
                    int i = 1;
                    strcpy(cmndbuf, args[0]);
                    while (args[i]) {
                        strcat(cmndbuf, " ");
                        strcat(cmndbuf, args[i++]);
                    }
                }

                //---------------------------FORK, AQUI CORRE LOS COMANDOS QUE PASEMOS------------------------
                switch (fork()) {
                    case -1:
                        syserr("fork"); //print Error Message
                        break;
                    case 0:
                        pid = getpid();        
                        execvp(args[0] , args); //EXECUTE THE COMMAND PASADOS POR PARAMETRO
                        syserr("execvp\n"); // error if return from exec                
                }
                /* continued execution in parent process */
                pid = getpid();
                //printf("Process ID in parent after fork: %d\n", pid);
                 //getchar(); // hang until I type something (alternatively,
                // let parent wait for return of child process.

                //PARA QUE NO ESPERE POR EL CHILL PROCESS
                wait(0);
            }
        }
    }
    return 0; 
}

//MENSAJES DE ERROR
void syserr(char * msg)   // report error code and abort
{
   //fprintf(stderr,"%s: %s", strerror(errno), msg);
   abort();
}

void syserrmsg(char * msg1, char * msg2)
{
    if (msg1)
        fprintf(stderr,"ERROR - %s ", msg1);
    if (msg2)
        fprintf(stderr,"%s ", msg2);
    fprintf(stderr,"; ");
    perror(NULL);
    return;
}
