#include "CUser.h"

//Constructor
CUser::CUser() 
{
	m_id = GenerateId();
	m_name = "user_" + to_string(m_id);
	m_message = to_string(m_id) + m_name;
}

//Destructor
CUser::~CUser() 
{
}

// Generate random number bettwen 1 and 252 using Mersenne Twister.
int CUser::GenerateId()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, 252);
	return dis(gen);
}

// Get a constant reference of the id
const int& CUser::GetId() const 
{
	return m_id;
}

// Get a constant reference of the name
const string& CUser::GetName() const 
{
	return m_name;
}

// Get a constant reference of the message
const string& CUser::Message() const 
{
	return m_message;
}
