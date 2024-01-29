#include "WareHouse.h"
#include "Action.h"

void WareHouse::parse(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);

    if (!configFile.is_open()) {
        std::cerr << "Error: Could not open the config file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "customer") {
            // Parse customer information
            std::string name, type;
            int distance, maxOrders;
            iss >> name >> type >> distance >> maxOrders;

            if (type == "soldier") {
                addCustomer(new SoldierCustomer(getCustomerCounter(), name, distance, maxOrders));
            } else if (type == "civilian") {
                addCustomer(new CivilianCustomer(getCustomerCounter(), name, distance, maxOrders));
            } else {
                std::cerr << "Error: Unknown customer type - " << type << std::endl;
            }
        } else if (command == "volunteer") {
            // Parse volunteer information
            std::string name, role;
            int cooldown, maxDistance, distancePerStep, maxOrders;
            iss >> name >> role;
            
            if (role == "collector") {
                iss >> cooldown;
                addVolunteer(new CollectorVolunteer(getVolunteerCounter(), name, cooldown));
            } else if (role == "limited_collector") {
                iss >> cooldown >> maxOrders;
                addVolunteer(new LimitedCollectorVolunteer(getVolunteerCounter(), name, cooldown, maxOrders));
            } else if (role == "driver") {
                iss >> maxDistance >> distancePerStep;
                addVolunteer(new DriverVolunteer(getVolunteerCounter(), name, maxDistance, distancePerStep));
            } else if (role == "limited_driver") {
                iss >> maxDistance >> distancePerStep >> maxOrders;
                addVolunteer(new LimitedDriverVolunteer(getVolunteerCounter(), name, maxDistance, distancePerStep, maxOrders));
            } else {
                std::cerr << "Error: Unknown volunteer role - " << role << std::endl;
            }
        } 
    }
    configFile.close();
}


void WareHouse::parseLine(const std::string& line) {

    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "customer") { 
        std::string name, type;
        int distance, maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        AddCustomer* addCustomer = new AddCustomer(name, type, distance, maxOrders);
        addCustomer->act(*this);

        
    } else if (command == "step") {
        int numOfSteps;
        iss >> numOfSteps;
        SimulateStep* simulateStep = new SimulateStep(numOfSteps);
        simulateStep->act(*this);


    } else if (command == "order") {
        int customerId;
        iss >> customerId;
        AddOrder* addOrder = new AddOrder(customerId);
        addOrder->act(*this);

    } else if (command == "orderStatus") {
        int orderId;
        iss >> orderId;
        PrintOrderStatus* printOrderStatus = new PrintOrderStatus(orderId);
        printOrderStatus->act(*this);

    } else if (command == "customerStatus") {
        int customerId;
        iss >> customerId;
        PrintCustomerStatus* printCustomerStatus = new PrintCustomerStatus(customerId);
        printCustomerStatus->act(*this);

    } else if (command == "volunteerStatus") {
        int volunteerId;
        iss >> volunteerId;
        PrintVolunteerStatus* printVolunteerStatus = new PrintVolunteerStatus(volunteerId);
        printVolunteerStatus->act(*this);

    } else if (command == "log") {
        PrintActionsLog* printActionsLog = new PrintActionsLog();
        printActionsLog->act(*this);

    } else if (command == "close") { 
        Close* close = new Close();
        close->act(*this);
    

    } else if (command == "backup") {  
        BackupWareHouse* backupWareHouse = new BackupWareHouse();
        backupWareHouse->act(*this);

    } else if (command == "restore") {
        RestoreWareHouse* restoreWareHouse = new RestoreWareHouse();
        restoreWareHouse->act(*this);

    } else {
        std::cerr << "Error: Unknown command - " << command << std::endl;
    }
}