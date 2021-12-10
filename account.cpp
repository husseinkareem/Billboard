#include "account.hpp"

#include <string>

using namespace std;

Account::Account(string& company_name, int amount, string& message) {
    this->company_name = company_name;
    this->amount = amount;
    this->message = message;
}