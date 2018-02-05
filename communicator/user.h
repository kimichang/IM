#ifndef USER_H
#define USER_H
#include <string>

enum Status{
  online,busying,leaveforawhile,offline
};


class User {
private:
    long userid;
    string ip;
    string port;
    Status status;
    string email;
    string serverip;
public:
    long getuserid(){ return this->userid;}
    string getip() {return this->ip;}
    string getport() {return this->port;}
    Status getstatus(){return this->status;}
    string getemail() {return this->email;}
    string getserverip() {return this->serverip;}
    User(long userid,string ip,string port,Status status,string email,string serverip){
        this->userid = userid;
        this->ip = ip;
        this->port = port;
        this->status = status;
        this->email = email;
        this->serverip = serverip;
    }



};


#endif // USER_H
