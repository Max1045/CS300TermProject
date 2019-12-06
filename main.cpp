/**
 * Created by Bela Kurzenhauser, Cody Potter, Corbin Stark, Dan Scott, Forest Pearson and Max VanRaden.
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include "Database.h"


//Helper function prototypes, implementation found in Helpers.cpp
void getMember(Database & database);
void getProvider(Database & database);
void deleteMember(Database & database);
void deleteProvider(Database & database);
void changeMember(Database & database);
void changeProvider(Database & database);
void printHeader(char target);
string getCurrentDate();
void generateMemberReports(Database db);
void generateProviderReports(Database db);
void printDirectory(Database db);
string validateMember(Database db);
string verifyService(Database db);

using namespace std;



int main(int argc, char** argv) {
	printHeader('c');
	Database database((char*) "data/sample-db.json");

	cout << "Welcome to the ChocAn Simulator." << endl;
	cout << "Would you like to simulate the (m)anager terminal or the (p)rovider terminal?" << endl;
	char res = 0;
	while(res != 'm' && res != 'p') {
		//cin.ignore(256, '\n');
		//cin.clear();
		cin >> res;
		if(res != 'm' && res != 'p')
			cout << "Invalid input. Type 'm' for the manager terminal or 'p' for the provider terminal." << endl;
	}

	char choice = 0;
	if(res == 'm') {
		printHeader(res);
		string inputName;
		string inputStreet;
		string inputCity;
		string inputState;
		string inputZip;
		string inputID;
		do {
			cout << "Welcome to the manager terminal. Please select an option." << endl;
			cout << "(a) Add a new provider" << endl;
			cout << "(b) Add a new member" << endl;
			cout << "(c) Edit a provider" << endl;
			cout << "(d) Edit a member" << endl;
			cout << "(e) Delete a provider" << endl;
			cout << "(f) Delete a member" << endl;
			cout << "(g) Generate member reports" << endl;
			cout << "(x) Exit" << endl;
			cin.ignore(256, '\n');
			cin >> choice;
		}
		while(choice != 'a' && choice != 'b' && choice != 'c' && choice != 'd' && choice != 'e' && choice != 'f' && choice != 'g' && choice !='x');

		switch(choice) {
			case 'a':     
				getProvider(database);
				break;
			case 'b':
				getMember(database);
				break;
			case 'c':
				changeProvider(database);
				break;
			case 'd':	
				changeMember(database);
				break;
			case 'e':
				deleteProvider(database);
				break;
			case 'f':
				deleteMember(database);
				break;
			case 'g':
				database.printMembers();
				generateMemberReports(database);
				break;
			case 'x':
				break;
		}
	}

	if(res == 'p') {
		string provID = "";
		cout << "Please enter provider number: (9 digits)" << endl;
		cin.ignore(256, '\n');
		cin >> provID;
		if(database.providers.find(provID) == database.providers.end())
		{
			cout << "Invalid provider ID" << endl;
			return 0;
		}
		printHeader(res);
		do {
			cout << "Welcome to the provider terminal" << endl;
			cout << "(a) View the provider directory" << endl;
			cout << "(b) Provide service" << endl;
			cout << "(c) Generate provider reports" << endl;
			cout << "(x) Exit" << endl;
			cin.ignore(256, '\n');
			cin >> choice;
		}
		while(choice != 'a' && choice != 'b' && choice != 'c' && choice != 'x');

		switch(choice) {
			case 'a':
				printDirectory(database);
				break;
			case 'b':
				{
					string memID = validateMember(database);
					if(memID == "null")
						break;
					string servCode = verifyService(database);
					if(servCode == "null")
						break;
					else
					{
						time_t now = time(0);
						tm* localTime = localtime(&now);
						//enter relevant dates
						string currDate = "";
						string servDate = "";
						string year = to_string(localTime->tm_year - 100);
						string month = to_string(localTime->tm_mon + 1);
						string day = to_string(localTime->tm_mday);
						string hour = to_string(localTime->tm_hour);
						string minute = to_string(localTime->tm_min);
						string second = to_string(localTime->tm_sec);
						currDate = month + "/" + day + "/" + year + " " + hour + ":" + minute + ":" + second;
						cout << "Enter the expected service date: (mm/dd/yyyy)" << endl;
						cin.ignore(256, '\n');
						cin >> servDate;

						//print summary
						cout << "Service Summary:" << endl;
						cout << "Date of Service: " << servDate << endl;
						cout << "Date of Submission: " << currDate << endl;
						cout << "Member ID: " << memID << endl;
						cout << "Provider ID: " << provID << endl;
						cout << "Service ID: " << servCode << endl;
						cout << "Fee: " << database.directory.at(servCode).fee << endl;
            
						string servID = database.addService(servDate, currDate, memID, provID, servCode, database.directory.at(servCode).fee);
						database.members.at(memID).services.push_back(servID);
						database.providers.at(memID).services.push_back(servID);
						database.update();
					}
				}
				break;
			case 'c':
				generateProviderReports(database);
				break;
			case 'x':
				break;
		}
	}

	return 0;
}
