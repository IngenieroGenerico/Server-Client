#include "CUser.h"

CUser::CUser() 
{
	m_id = GenerateId();
	m_name = "user_" + to_string(m_id);
	m_message = to_string(m_id) + m_name;
}

CUser::~CUser() 
{

}

int CUser::GenerateId()
{
	// Generate random number bettwen 1 and 252 using Mersenne Twister.
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, 252);
	return dis(gen);
}

const int& CUser::GetId() const 
{
	return m_id;
}

const string& CUser::GetName() const 
{
	return m_name;
}

const string& CUser::Message() const 
{
	return m_message;
}
