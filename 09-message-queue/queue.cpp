#include <mqueue.h>
#include <fcntl.h>
#include <cstdio>
#include <string>
#include <sys/stat.h>

int main(){


    struct mq_attr attr{};
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 128;


    mqd_t qd = mq_open(
        "/lab2_chat",
        O_CREAT | O_RDWR,
        0600,
        &attr
    );

    if (qd == -1){
        std::perror("mq_open");
        return 1;
    }

    std::string message = "Hello Message Queue";

    if(mq_send(qd , message.data() , message.size() , 0)){
        std::perror("mq_send");
        mq_close(qd);
        return 1;
    }
    
    mq_close(qd);

    return 0;

}