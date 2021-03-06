#include <Ice/Ice.h>
#include <Chat.h>

#include "ServerImpl.h"

ServerImpl::ServerImpl(Ice::ObjectAdapterPtr adapter){
    this->adapter = adapter;
}

RoomPrx ServerImpl::CreateRoom(const string& name,
                               const Ice::Current&){
//    cout<<"2"<<endl;
    ServerPrx serverPrx = ServerPrx::uncheckedCast(adapter->addWithUUID(this));
    RoomPtr room = new RoomImpl(name, serverPrx);
    RoomPrx roomPrx = RoomPrx::uncheckedCast(adapter->addWithUUID(room));
    roomList.push_back(roomPrx);
//    cout<<"2.5"<<endl;
    return roomPrx;
}

RoomList ServerImpl::getRooms(const Ice::Current&){
    return roomList;
}
void ServerImpl::addRoom(const RoomPrx& room, const Ice::Current&){
    roomList.push_back(room);
}
RoomPrx ServerImpl::FindRoom(const string& name,
                             const Ice::Current&){
    for(vector<RoomPrx>::iterator it = roomList.begin(); it != roomList.end(); ++it) {
        if((*it)->getName() == name){
            return *it;
        }
    }
    throw NoSuchRoomExists();
}

void ServerImpl::RegisterUser(const UserPrx& userPrx,
                              const Ice::Current&) {
    userList.push_back(userPrx);

    string userName = userPrx->getName();
    cout << userName << " joined" << endl;
}

UserPrx ServerImpl::FindUser(const string& userName, const Ice::Current&){
    
    for(vector<UserPrx>::iterator it = userList.begin(); it != userList.end(); ++it) {
        if( (*it)->getName() == userName ){
            return *it;
        }
    }
    throw NoSuchUserExists();
}

void ServerImpl::removeUser(const UserPrx& user, const Ice::Current&){
    userList.erase(find(userList.begin(), userList.end(),user));
}
