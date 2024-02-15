#include "Operator.hpp"
#include "User.hpp"

Operator::Operator(std::string & nickname, std::string & username, std::string & password) : User(nickname, username, password)
{

}

Operator::~Operator()
{

}