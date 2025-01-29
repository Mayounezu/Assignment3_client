#pragma once

#include <string>
#include <map>

class Subscriptions {
private:
    std::map<std::string, std::string> activeSubscriptions; // {channel -> subscription_id}

public:
    Subscriptions();

    bool addSubscription(const std::string& channel);
    bool removeSubscription(const std::string& channel);
    bool isSubscribed(const std::string& channel) const;
    std::string getSubscriptionId(const std::string& channel) const;
};
