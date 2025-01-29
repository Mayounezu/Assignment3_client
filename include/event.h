#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>

class Event
{
private:
    std::string channel_name;
    std::string city;
    std::string name;
    int date_time;
    std::string description;
    std::map<std::string, std::string> general_information;
    std::string eventOwnerUser;

public:
    // Constructors
    Event(std::string channel_name, std::string city, std::string name, int date_time, std::string description, std::map<std::string, std::string> general_information);
    Event(const std::string & frame_body);

    // Destructor
    virtual ~Event();

    // Setters
    void setEventOwnerUser(std::string setEventOwnerUser);

    // Getters
    const std::string &getEventOwnerUser() const;
    const std::string &get_channel_name() const;
    const std::string &get_city() const;
    const std::string &get_description() const;
    const std::string &get_name() const;
    int get_date_time() const;
    const std::map<std::string, std::string> &get_general_information() const;

    std::string toString() const;
};

// Structure to hold event data
struct names_and_events {
    std::string channel_name;
    std::vector<Event> events;
};

// Function to parse events from a JSON file
names_and_events parseEventsFile(std::string json_path);

// Utility function to split strings by a delimiter
void split_str(const std::string &str, char delimiter, std::vector<std::string> &result);
