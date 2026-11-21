#include <iostream>
#include <unistd.h> 
#include <cstdio>
#include <algorithm>
#include <sys/wait.h>

int main() {

    int pipes[2][2];
    pid_t children[2];

    for(int i =0 ; i < 2 ; i++){
        if(pipe(pipes[i]) == -1){
            std::perror("pipe");
            return -1;
        }
    }

    for(int i =0 ; i < 2 ; i++){
        pid_t pid = fork();

        if(pid == -1){
            std::perror("fork");
            return 1;
        }

        if(pid ==0){

            for(int j = 0; j<2; j++){
                close(pipes[j][0]);

                if(j != i ){
                    close(pipes[j][1]);
                }
            }

            sleep(i+1);
            
            const char message[] = "Hello parent!";
            
            write(pipes[i][1] , message , sizeof(message)-1 );


            close(pipes[i][1]);
            return 0;
        }
        children[i] = pid;
    }

    close(pipes[0][1]);
    close(pipes[1][1]);

    fd_set master;
    fd_set readSet;

    FD_ZERO(&master);
    FD_ZERO(&readSet);

    FD_SET(pipes[0][0] , &master);
    FD_SET(pipes[1][0] , &master);

    int maxfd = std::max(
        pipes[0][0],
        pipes[1][0]
    );

    char buffer[100]{};

    int active = 2;
    while (active > 0){

        readSet = master;
        ssize_t n ;
        
        int ready = select(maxfd+1 , &readSet , nullptr ,nullptr ,nullptr);

        if(ready == -1){
            if(errno == EINTR){
                continue;
            }
            std::perror("select");
            break;
        }
        
        for(int i=0;i < 2; i++){
            if(FD_ISSET(pipes[i][0] , &readSet)){
                n = read(pipes[i][0] ,buffer , sizeof(buffer)-1 );

                 if(n > 0){
                    buffer[n] = '\0';

                    std::cout << buffer << '\n'; 
                }else if(n == 0){
                    std::cout << "EOF" << '\n';

                    FD_CLR(pipes[i][0] , &master);

                    close(pipes[i][0]);
                  
                    active--;
                }else if(n == -1){
                    std::perror("read");
                    
                    close(pipes[i][0]);
                 
                }  
            }
        }

       

       
    }
        
    

    for (int i = 0; i < 2; i++) {
         waitpid(children[i], nullptr, 0);
    }

    return 0;
    


}