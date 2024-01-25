#include "WareHouse.h"
#include "Action.h"
#include <iostream> 
using namespace std;



WareHouse:: WareHouse(const string &configFilePath):
isOpen(1), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), 
customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    // parse(configFilePath) implement parser 
    start();
}

//Destructor
WareHouse::~WareHouse()
{
    clearData();
}

//Destructor

//clearData

//clearData
void WareHouse:: clearData()
{
     for (const Order* ord : pendingOrders)
         { //Deletes all the pending orders
          delete ord;
         }
        for (const Order* ord : inProcessOrders)
        { //Deletes all the in prccess orders
         delete ord;
        }
        for (const Order* ord : completedOrders)
        { //Deletes all the completed orders
         delete ord;
        }
        for (const Customer* cus : customers)
        { //Deletes all the customers
         delete cus;
        }
        for (const Volunteer* vol : volunteers)
        { //Deletes all the volunteers
         delete vol;
        }
        for (const BaseAction* action :actionsLog)
        { //Deletes all the actions
         delete action;
        }
       // Releases the memory associated with the vectors and remove all elements from it
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();
        volunteers.clear();
        actionsLog.clear();
}

//CopyConstructor

//CopyConstructor

//CopyAssignmentOperator
WareHouse& WareHouse::operator=(const WareHouse& other)
{
    if(this != &other)
    {
        clearData(); // Clears all data on the this object
        //Copying fields
        this->orderCounter = other.orderCounter;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;

        for (const Order* otherOrder : other.pendingOrders)
         { //Perform a deep copy of the pending orders
            Order* newOrder = new Order(*otherOrder);
            pendingOrders.push_back(newOrder);
         }
        for (const Order* otherOrder : other.inProcessOrders)
        { //Perform a deep copy of the in proccess orders
            Order* newOrder = new Order(*otherOrder);
            inProcessOrders.push_back(newOrder);
        }
        for (const Order* otherOrder : other.completedOrders)
        { //Perform a deep copy of the completed orders
            Order* newOrder = new Order(*otherOrder);
            completedOrders.push_back(newOrder);
        }
        for (const Customer* otherCustomer : other.customers)
        { // Use the clone method to copy the customers
            customers.push_back(otherCustomer->clone());
        }
        for (const Volunteer* otherVolunteer : other.volunteers)
        { // Use the clone method to copy the volunteers
            volunteers.push_back(otherVolunteer->clone());
        }
        for(const BaseAction* otherAction : other.actionsLog)
        { // Use the clone method of each derived baseAction class to copy the actions
            actionsLog.push_back(otherAction->clone());
        }

    return *this;
}
}
//CopyAssignmentOperator



void WareHouse:: start()
{
cout << "Warehouse is open!" << endl;
open();
}

void WareHouse:: addOrder(Order* order)
{
   orderCounter++;
   pendingOrders.push_back(order);
   
}

 void WareHouse:: addCustomer(Customer* customer)
 {
    customerCounter++;
    customers.push_back(customer);
 }

 void WareHouse:: addAction(BaseAction* action)
 {
    actionsLog.push_back(action);// need to check if we call it on every function since every action consider to be an action.!!!
 }

Customer &WareHouse:: getCustomer(int customerId) const
{
    // need to ask whether we can get a non exitsing customer
    for(Customer* cus: customers)
    {
        if(cus->getId() == customerId)
        {
          return *cus;  
        }
    }
}

Volunteer &WareHouse:: getVolunteer(int volunteerId) const
{
     // need to ask whether we can get a non exitsing vol
    for(Volunteer* vol : volunteers) 
    {
        if(vol->getId() == volunteerId)
        {
            return *vol;
        }
    }

}

Order &WareHouse:: getOrder (int orderId) const
{
     // need to ask whether we can get a non exitsing order
    for(Order* ord : pendingOrders)
    {
        if(ord->getId() == orderId)
        {
            return *ord;
        }
    }
    for(Order* ord : inProcessOrders)
    {
        if(ord->getId() == orderId)
        {
            return *ord;
        }
    }
    for(Order* ord : completedOrders)
    {
        if(ord->getId() == orderId)
        {
            return *ord;
        }
    } 
    
}

const vector<BaseAction*> &WareHouse:: getActions() const
{
    return actionsLog;
}

void WareHouse:: close()
{
    for(Order* ord : pendingOrders)
    {
        cout << "OrderID: " << ord->getId() << ", CustomerID: " << ord->getCustomerId() << ", Status: " << 
        ord->statusToString(ord->getStatus()) << endl;
    }
    for(Order* ord : inProcessOrders)
    {
        cout << "OrderID: " << ord->getId() << ", CustomerID: " << ord->getCustomerId() << ", Status: " << 
        ord->statusToString(ord->getStatus()) << endl;
    }
    for(Order* ord : completedOrders)
    {
        cout << "OrderID: " << ord->getId() << ", CustomerID: " << ord->getCustomerId() << ", Status: " << 
        ord->statusToString(ord->getStatus()) << endl;
    } 

    isOpen = false;
    
    // this action prints all orders with their status
    // changes its isOpen status, exits the loop and finishes the program
    // Make sure you free all memory before finishing the program, so you won’t have memory leaks

}

void WareHouse:: open()
{
    isOpen = true;
    // check what else it does and why is it diffrent than start()
}

int WareHouse:: getCustomerCounter()
{
    return customerCounter;
}

int WareHouse:: getOrderCounter()
{
    return orderCounter;
}

int WareHouse:: getVolunteerCounter()
{
    return volunteerCounter;
}



// vector<Order*> WareHouse:: getPendingOrders()
// {
//     return pendingOrders;
// }

// vector<Order*> WareHouse:: getInProcessOrders()
// {
//     return inProcessOrders;
// }

// vector<Order*> WareHouse:: getCompletedOrders()
// {
//     return completedOrders;
// }
//  vector<Volunteer*> WareHouse:: getVolunteers()
//  {
//     return volunteers;
//  }