#pragma once
#include <map>
#include <string>
class Subscriptions{
     map<std::string, int> subscriptionMap;
     protected:
     static int id;
     static Subscriptions instance;

private
     //implementing the singleton pattern
     Subscriptions(){
          id = 0;
          subscriptionMap = new map<std::string, int>();
     }

     Subscriptions(const Subscriptions&) = delete;
     Subscriptions& operator=(const Subscriptions&) = delete;
     static Subscriptions* instance;
public:

     static Subscriptions* getInstance(){
       if(instance == nullptr){
         instance = new Subscriptions();
       }
         return *instance;
     }

     void subscribe(std::string channelName);
     void unsubscribe(std::string channelName);
     int getSubscriptionId(std::string channelName);
     void clearSubscriptions();
     ~Subscriptions();

}
