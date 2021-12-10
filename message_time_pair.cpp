#include "message_time_pair.hpp"

using namespace std;

message_time_pair::message_time_pair(string& message, unsigned short time) {
    this->message = message;
    this->time = time;
}