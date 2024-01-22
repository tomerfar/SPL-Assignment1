// Parser.cpp

#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool Parser::parseConfigFile(const std::string& filePath, WareHouse& wareHouse) {
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) 
    {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line)) 
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "customer") {
            std::string name, customerType;
            int distance, maxOrders;
            iss >> name >> customerType >> distance >> maxOrders;
            Customer* customer = (customerType == "soldier") ? new SoldierCustomer(wareHouse.getCustomerCounter(), name, distance, maxOrders)
                                                              : new CivilianCustomer(wareHouse.getCustomerCounter(), name, distance, maxOrders);
            wareHouse.addCustomer(customer);
        } else if (type == "volunteer") {
            std::string name, role;
            int arg1, arg2, arg3;
            iss >> name >> role;

            if (role == "collector" || role == "limited_collector") {
                iss >> arg1;
                Volunteer* volunteer = (role == "collector") ? new CollectorVolunteer(wareHouse.getVolunteerCounter(), name, arg1)
                                                              : new LimitedCollectorVolunteer(wareHouse.getVolunteerCounter(), name, arg1);
                wareHouse.addVolunteer(volunteer);
            } else if (role == "driver" || role == "limited_driver") {
                iss >> arg1 >> arg2;
                Volunteer* volunteer = (role == "driver") ? new DriverVolunteer(wareHouse.getVolunteerCounter(), name, arg1, arg2)
                                                          : new LimitedDriverVolunteer(wareHouse.getVolunteerCounter(), name, arg1, arg2, arg3);
                wareHouse.addVolunteer(volunteer);
            } else {
                std::cerr << "Error: Unknown volunteer role - " << role << std::endl;
                return false;
            }
        } else if (type == "backup") {
            BackupWareHouse* backupAction = new BackupWareHouse();
            wareHouse.addAction(backupAction);
        } else if (type == "restore") {
            RestoreWareHouse* restoreAction = new RestoreWareHouse();
            wareHouse.addAction(restoreAction);
        } else {
            std::cerr << "Error: Unknown entry in config file - " << type << std::endl;
            return false;
        }
    }

    inputFile.close();
    return true;
}
