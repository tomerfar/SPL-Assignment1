#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer); // Added myself
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
        int getCustomerCounter();
        int getOrderCounter();
        int getVolunteerCounter();


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

        vector<CollectorVolunteer*> availableCollectors;
        vector<DriverVolunteer*> availableDrivers;

};