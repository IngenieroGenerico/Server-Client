#pragma once
///******************************************************/
///*			C++ Standar Library & STL.				*/
///******************************************************/
#include <string>
#include <random>

// using namespace of std to avoid to write it.
using namespace std;
/**
 * @brief Bassic class to store user data.
 */
class CUser
{
	///**************************************************************************/
	///*						  Member Variables.								*/
	///**************************************************************************/
	
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
	
	///************************************************************************/
	///*                            Constructor & Destructor.                 */
	///************************************************************************/
	
public:

	/**
	 * @brief Constructor.
	 */
	CUser();
	/**
	 * @brief Destructor.
	 */
	~CUser();

	///************************************************************************/
	///*						   Class Functions.							  */
	///************************************************************************/
	
public:

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

private:

	/**
	 * @brief Generate interger randomly with Mersenne Twister.
	 * @return An interger generated randombly
	 */
	int GenerateId();

};

