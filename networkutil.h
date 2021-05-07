#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H
#include "utils.h"
#include "QtNetwork"
#include "QString"
#include "iostream"
#include "QVector2D"
class NetworkUtil
{
public:
    NetworkUtil(QHostAddress this_ip, QHostAddress other_ip):this_ip(this_ip), sender(new QUdpSocket()),other_ip(other_ip), receiver(new QUdpSocket()){
        receiver->bind(this_ip, this_port);
    }
    void sendCommand(QVector2D* command, bool recover){
        if (!recover){
            std::string res = std::to_string(int(command->x())) + " " + std::to_string(int(command->y()));
            std::cout << "send: " << res << std::endl;
            QByteArray data = QByteArray(res.c_str());
            this->sender->writeDatagram(data.data(), data.size(), other_ip, other_port);
        } else{
            this->sender->writeDatagram(QByteArray("#").data(), QByteArray("#").size(), other_ip, other_port);
        }
    }
    bool receiveMessage(QVector2D& delta, bool& recover){
        QByteArray data;
        if (this->receiver->hasPendingDatagrams()){
            data.resize(this->receiver->pendingDatagramSize());
            this->receiver->readDatagram(data.data(), data.size());
            std::string message = data.data();
            if(message.at(0) != '#'){
                int mid = message.find(" ", 0);
                int x = std::stoi(message.substr(0,mid));
                int y = std::stoi(message.substr(mid+1));
                delta *= QVector2D(0,0);
                delta += QVector2D(x,y);
                recover = false;
            }
            else{
                recover = true;
            }
            return true;
        }
        return false;
    }
private:
    QHostAddress this_ip;
    QUdpSocket* sender;
    QHostAddress other_ip;
    QUdpSocket* receiver;
};

#endif // NETWORKUTIL_H
