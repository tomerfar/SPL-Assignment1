#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream> 
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void parse(const std::string& configFilePath); // parser
        void parseLine(const std::string& line); // parser helper function 
        ~WareHouse(); //Destructor
        WareHouse(const WareHouse& other); // Copy Constructor
        WareHouse& operator=(const WareHouse& other); //Copy Assignment Operator
        WareHouse(WareHouse&& other) noexcept; //Move Constructor
        WareHouse& operator=(WareHouse&& other) noexcept; // Move Assignment Operator

        void clearData(); //clears the data of an object
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer); 
        void addVolunteer(Volunteer* Volunteer);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        vector<Order*> getPendingOrders();
        vector<Order*> getInProcessOrders();
        vector<Order*> getCompletedOrders();
        vector<Volunteer*> getVolunteers();
        vector<Customer*> getCustomers();
        int getCustomerCounter();
        int getOrderCounter();
        int getVolunteerCounter();

        void transferToInProcess(Order* ord);
        void transferToPending(Order* ord);
        void transferToCompleted(Order* ord);
        void removeVolunteer(Volunteer* vol);
        


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
        CollectorVolunteer *defaultVol;   
        Order *defaultOrd;
        CivilianCustomer *defaultCus;

};