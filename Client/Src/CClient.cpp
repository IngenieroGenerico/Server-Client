#include "CClient.h"

CClient::CClient()
{
}

CClient::~CClient()
{
}

void CClient::Start()
{
    try {
        //Change text color from console to print it with the specific color.
        SetConsoleColor(WHITE);

        //Try to initializate context and create a connection to the specific PORT.
        asio::io_context context;
        tcp::socket socket(context);
        tcp::resolver resolver(context);
        asio::connect(socket, resolver.resolve("localhost", "8080"));

        // Logic to send Request to Server, every second.
        while (m_ids.size() <= MAX_IDS) {
            
            //Create new user.
            CUser new_user;
            //Save new Id into the Id vector.
            StoreId(new_user.GetId());


            //Initializa reques to Server, First save time when send the request.
            auto start = chrono::high_resolution_clock::now(); //Start time.
            //Write process...store the message into the buffer and send it through the socket.
            asio::write(socket, asio::buffer(new_user.Message() + "\n"));

            // Read response from server
            char response[1024];
            size_t length = socket.read_some(asio::buffer(response));
            string received(response, length);

            //Once response is recieved from the server, store actual time into a variable.
            auto end = chrono::high_resolution_clock::now(); // End time.
            //Get response time substracting the start time minus the end time.
            chrono::duration<double> response_time = end - start;


            cout << "Request: Insert/User " << " id = " + new_user.GetId() << ", name = " + new_user.GetName() << endl;
            SetConsoleColor(YELLOW);
            if (received == "DENIED id already exist") {

                SetConsoleColor(RED);
            }
            cout << "Response: " << received << ", time = " << response_time.count() << " seconds" << endl << endl;

            //Wait 1 second before send next request.
            this_thread::sleep_for(chrono::seconds(1));
        }

        //Once we reach this part of code it means all identifiers are used so print it into the console.
        cout << "OUt of identifiers" << endl;
        //Close socket to avoid execeptions or errors.
        socket.close();
    }
    catch (exception& e) {
        //Catch Exception if exist and print it to the console.
        cerr << "Client Exception: " << e.what() << endl;
    }
}


void CClient::StoreId(int id)
{
    //Check if size is not 0 to avoid index out range when find for an element.
    if (m_ids.size() != 0) 
    {
        //Create an itt that points to a element if exist, if not then itt point to the end of the vector.
        auto itt = find(m_ids.begin(), m_ids.end(), id);
        if (itt == m_ids.end()) 
        {
            //add id to the vector.
            m_ids.push_back(id);
        }
    }
    else 
    {
        //add id to the vector.
        m_ids.push_back(id);
    }
}
