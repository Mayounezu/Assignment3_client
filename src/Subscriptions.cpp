#include "Subscriptions.h"
#include <sstream>

Subscriptions::Subscriptions() {}

// Adds a new subscription, returns true if successful, false if already subscribed
bool Subscriptions::addSubscription(const std::string& channel) {
    if (activeSubscriptions.find(channel) != activeSubscriptions.end()) {
        return false; // Already subscribed
    }
    
    std::ostringstream subscriptionId;
    subscriptionId << "sub-" << channel; // Generate a unique subscription ID
    activeSubscriptions[channel] = subscriptionId.str();
    
    return true;
}

// Removes a subscription, returns true if successful
bool Subscriptions::removeSubscription(const std::string& channel) {
    if (!isSubscribed(channel)) { 
        return false;
    }
    return activeSubscriptions.erase(channel) > 0;
}


// Checks if already subscribed
bool Subscriptions::isSubscribed(const std::string& channel) const {
    return activeSubscriptions.find(channel) != activeSubscriptions.end();
}

// Gets the subscription ID for a given channel
std::string Subscriptions::getSubscriptionId(const std::string& channel) const {
    auto it = activeSubscriptions.find(channel);
    if (it != activeSubscriptions.end()) {
        return it->second;
    }
    return "";
}
