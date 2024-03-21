#pragma once
#include <string>
#include <random>

using namespace std;
/**
 * @brief Bassic class to store user data.
 */
class CUser
{
private:
	/**
	 * @brief Used to store user identifier.
	 */
	int m_id;
	/**
	 * @brief Used to store user name.
	 */
	string m_name;
	/**
	 * @brief Used to store message that will be send it to server.
	 */
	string m_message;
	/**
	 * @brief Generate interger randomly with Mersenne Twister.
	 * @return An interger generated randombly
	 */
	int GenerateId();

public:
	/**
	 * @brief Constructor.
	 */
	CUser();
	/**
	 * @brief Destructor.
	 */
	~CUser();

	/**
	 * @brief Get user id.
	 * @return constant reference of the user id.
	 */
	const int& GetId()const;
	/**
	 * @brief Get user name.
	 * @return constant reference of user name. 
	 */
	const string& GetName()const;
	/**
	 * @brief Get message that will be send it to the server.
	 * @return constant reference of user message. 
	 */
	const string& Message()const;

};

