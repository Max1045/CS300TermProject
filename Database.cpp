//
// Created by Cody Potter on 10/24/2019.
//
#include "Database.h"

//////////////////// Constructor ////////////////////
Database::Database(char* rootPath) {
    path = rootPath;
    reload();
}

//////////////////// File i/o Operations ////////////////////
/**
 * Writes the JSON value currently stored in the rootRef to the JSON data file
 */
void Database::write() {
    std::ofstream ofs(path);
    Json::StyledStreamWriter writer;
    writer.write(ofs, rootRef);
}

/**
 * Loads the data from the JSON data file into memory, build the database maps.
 */
void Database::reload() {
    // just for safety, clear the maps first
    members.clear();
    providers.clear();
    services.clear();

    // open an input stream and read it into the database.rootRef
    std::ifstream ifs(path);
    Json::Reader reader;
    reader.parse(ifs, rootRef);

    // parse the JSON data from the rootRef and build the maps
    Json::Value membersRef = rootRef["members"];
    Json::Value providersRef = rootRef["providers"];
    Json::Value servicesRef = rootRef["services"];

    for( Json::Value::iterator memberIterator = membersRef.begin() ; memberIterator != membersRef.end() ; memberIterator++ ) {
        Member newMember(membersRef[memberIterator.key().asString()], memberIterator.key().asString());
        members.insert(std::pair<std::string, Member>(memberIterator.key().asString(), newMember));
    }

    for( Json::Value::iterator providerIterator = providersRef.begin() ; providerIterator != providersRef.end() ; providerIterator++ ) {
        Provider newProvider(providersRef[providerIterator.key().asString()], providerIterator.key().asString());
        providers.insert(std::pair<std::string, Provider>(providerIterator.key().asString(), newProvider));
    }

    for( Json::Value::iterator serviceIterator = servicesRef.begin() ; serviceIterator != servicesRef.end() ; serviceIterator++ ) {
        Service newService(servicesRef[serviceIterator.key().asString()], serviceIterator.key().asString());
        services.insert(std::pair<std::string, Service>(serviceIterator.key().asString(), newService));
    }
}

/**
 * Updates the database Json Value and then writes it to the file
 */
void Database::update() {

    std::map<std::string, Member>::iterator memberIterator;
    std::map<std::string, Provider>::iterator providerIterator;
    std::map<std::string, Service>::iterator serviceIterator;

    rootRef["members"].clear();
    rootRef["providers"].clear();
    rootRef["services"].clear();

    for ( memberIterator = members.begin(); memberIterator != members.end(); memberIterator++ ) {
        rootRef["members"][memberIterator->first] = memberIterator->second.getJsonValue();
    }

    for ( providerIterator = providers.begin(); providerIterator != providers.end(); providerIterator++ ) {
        rootRef["providers"][providerIterator->first] = providerIterator->second.getJsonValue();
    }

    for ( serviceIterator = services.begin(); serviceIterator != services.end(); serviceIterator++ ) {
        rootRef["services"][serviceIterator->first] = serviceIterator->second.getJsonValue();
    }

    // Writes the data at the database.rootRef to the file
    write();
}

//////////////////// Utilities ////////////////////
/**
 * Generates a random and unique identifier of given length
 * @param length - the required length
 * @return the generated identifier
 */
std::string Database::generateNewID(int length) {
    std::string newID = "";
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        newID += std::to_string((int) rand() % 10);
    }

    // Check for collisions
    std::map<std::string, Member>::iterator memberIterator;
    std::map<std::string, Provider>::iterator providerIterator;
    std::map<std::string, Service>::iterator serviceIterator;

    for ( memberIterator = members.begin(); memberIterator != members.end(); memberIterator++ ) {
        if (memberIterator->first == newID) {
            return generateNewID(9);
        }
    }

    for ( providerIterator = providers.begin(); providerIterator != providers.end(); providerIterator++ ) {
        if (providerIterator->first == newID) {
            return generateNewID(9);
        }
    }

    for ( serviceIterator = services.begin(); serviceIterator != services.end(); serviceIterator++ ) {
        if (serviceIterator->first == newID) {
            return generateNewID(5);
        }
    }

    return newID;
}

/**
 * Neatly prints a table of the member ids and their associated names (useful for debugging)
 */
void Database::printMembers() {
    std::cout << "----------------------------------" << std::endl;
    std::cout << "|" << std::setw(20) << "Members" << std::setw(13) << "|" << std::endl;
    std::cout << "----------------------------------" << std::endl;

    std::map<std::string, Member>::iterator memberIterator;
    for ( memberIterator = members.begin(); memberIterator != members.end(); memberIterator++ ) {
        std::cout << "| " << std::setw(9) << memberIterator->first << " | " << std::setw(18) << memberIterator->second.name << " |" << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
    std::cout << std::endl;
}

/**
 *  Neatly prints a table of the provider ids and their associated names (useful for debugging)
 */
void Database::printProviders() {
    std::cout << "------------------------------------" << std::endl;
    std::cout << "|" << std::setw(20) << "Providers" << std::setw(15) << "|" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    std::map<std::string, Provider>::iterator providerIterator;
    for ( providerIterator = providers.begin(); providerIterator != providers.end(); providerIterator++ ) {
        std::cout << "| " << std::setw(9) << providerIterator->first << " | " << std::setw(20) << providerIterator->second.name << " |" << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;
    std::cout << std::endl;
}

/**
 * Neatly prints a table of the service ids and their associated fees (useful for debugging)
 */
void Database::printServices() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|" << std::setw(20) << "Services" << std::setw(14) << "|" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::map<std::string, Service>::iterator serviceIterator;
    for ( serviceIterator = services.begin(); serviceIterator != services.end(); serviceIterator++ ) {
        std::cout << "| " << std::setw(5) << serviceIterator->first << " | " << std::setw(23) << serviceIterator->second.fee << " |" << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
    std::cout << std::endl;
}

//////////////////// CRUD Operations for Members ////////////////////
/**
 * Adds a member to the members map
 * @param newName
 * @param newStreet
 * @param newCity
 * @param newState
 * @param newZip
 */
void Database::addMember(std::string newName, std::string newStreet, std::string newCity, std::string newState, std::string newZip) {
    std::string newID = generateNewID(9);
    Member newMember(newID, newName, newStreet, newCity, newState, newZip);

    members.insert(std::pair<std::string, Member>((std::string) newID, newMember));
}

/**
 * Removes a member from the members map
 * @param memberID - id of the member to remove
 * @return true if successful
 */
bool Database::removeMember(std::string memberID) {
    members.erase(memberID);
    return true;
}

//////////////////// CRUD Operations for Providers ////////////////////
/**
 * Add a provider to the providers map
 * @param newName
 * @param newStreet
 * @param newCity
 * @param newState
 * @param newZip
 */
void Database::addProvider(std::string newName, std::string newStreet, std::string newCity, std::string newState, std::string newZip) {
    std::string newID = generateNewID(9);
    Provider newProvider(newID, newName, newStreet, newCity, newState, newZip);

    providers.insert(std::pair<std::string, Provider>(newID, newProvider));
}

/**
 * Removes a provider from providers map
 * @param providerID - id of the provider to erase
 * @return true if successful
 */
bool Database::removeProvider(std::string providerID) {
    providers.erase(providerID);
    return true;
}

//////////////////// CRUD Operations for Services ////////////////////
/**
 * Add service to the services map
 * @param serviceDate
 * @param submissionDate
 * @param memberID
 * @param providerID
 * @param serviceCode
 * @param fee
 */
void Database::addService(std::string serviceDate, std::string submissionDate, std::string memberID, std::string providerID, std::string serviceCode, int fee) {
    std::string newID = generateNewID(5);
    Service newService(newID, serviceDate, submissionDate, memberID, providerID, serviceCode, fee);

    services.insert(std::pair<std::string, Service>(newID, newService));
}

/**
 * remove service from the services map
 * @param serviceID
 * @return
 */
bool Database::removeService(std::string serviceID) {
    services.erase(serviceID);
    return true;
}
