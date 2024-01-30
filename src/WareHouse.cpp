#include "../include/WareHouse.h"
#include "../include/Action.h"

#include <fstream>
#include <sstream>
#include <iostream> 
#include <algorithm>
using namespace std;


//Constructor
WareHouse:: WareHouse(const string &configFilePath):
isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), 
customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    parse(configFilePath);
    start();
}


// Assume you have already included necessary headers



//Destructor

WareHouse::~WareHouse()
{
    clearData();
}
//Destructor

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
//clearData

//CopyConstructor
WareHouse:: WareHouse(const WareHouse& other): isOpen(other.isOpen),
 actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
 customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
 orderCounter(other.orderCounter)
 { //Perform a deep copy of resources: orders, customers, volunteers, and actions
    for (const Order* otherOrder : other.pendingOrders) 
    { 
        // Order* newOrder = new Order(otherOrder->clone());
        pendingOrders.push_back(otherOrder->clone());
    }
    for (const Order* otherOrder : other.inProcessOrders)
    { 
        //Order* newOrder = new Order(otherOrder->clone());
        inProcessOrders.push_back(otherOrder->clone());
    }
    for (const Order* otherOrder : other.completedOrders)
    { 
        // Order* newOrder = new Order(otherOrder->clone());
        completedOrders.push_back(otherOrder->clone());
    }
    for (const Customer* otherCustomer : other.customers)
    { 
        customers.push_back(otherCustomer->clone());
    }
    for (const Volunteer* otherVolunteer : other.volunteers)
    { 
        volunteers.push_back(otherVolunteer->clone());
    }
    for(const BaseAction* otherAction : other.actionsLog)
    { 
        actionsLog.push_back(otherAction->clone());
    }
 }

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
        //Perform a deep copy of orders, customers, volunteers, and actions

        for (const Order* otherOrder : other.pendingOrders)
        { 
            //Order* newOrder = new Order(otherOrder->clone());
            pendingOrders.push_back(otherOrder->clone());
        }
        for (const Order* otherOrder : other.inProcessOrders)
        { 
            //Order* newOrder = new Order(otherOrder->clone());
            inProcessOrders.push_back(otherOrder->clone());
        }
        for (const Order* otherOrder : other.completedOrders)
        { 
            //Order* newOrder = new Order(otherOrder->clone());
            completedOrders.push_back(otherOrder->clone());
        }
        for (const Customer* otherCustomer : other.customers)
        { 
            customers.push_back(otherCustomer->clone());
        }
        for (const Volunteer* otherVolunteer : other.volunteers)
        { 
            volunteers.push_back(otherVolunteer->clone());
        }
        for(const BaseAction* otherAction : other.actionsLog)
        { 
            actionsLog.push_back(otherAction->clone());
        }
    }
    return *this;
}

//Move Constructor
WareHouse::WareHouse(WareHouse&& other) noexcept:
isOpen(other.isOpen),
 actionsLog(move(other.actionsLog)), volunteers(move(other.volunteers)),
 pendingOrders(move(other.pendingOrders)), inProcessOrders(move(other.inProcessOrders)),
 completedOrders(move(other.completedOrders)), customers(move(other.customers)),
 customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
 orderCounter(other.orderCounter)
{
   // reallocate to null so fields don't get deleted once leaving the scope 
   
};

//Move Assignment Operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept
{
  if(this != &other)
  {
    clearData();
    actionsLog = move(other.actionsLog);
    volunteers = move(other.volunteers);
    pendingOrders = move(other.pendingOrders);
    inProcessOrders = move(other.inProcessOrders);
    completedOrders = move(other.completedOrders);
    customers = move(other.customers);
    customerCounter = other.customerCounter;
    isOpen = other.isOpen;
    volunteerCounter = other.volunteerCounter;
    orderCounter = other.orderCounter;

  }
  return *this;
}

void WareHouse:: start()
{
    open();
    cout << "Warehouse is open!" << endl;
    string input = "s";
    while (isOpen) {
        if (!input.empty()) {
            cout << "Enter command: ";
            getline(cin, input);
            parseLine(input);
        } else {
            cout << "Invalid input" << endl;
        }
    }
}

CollectorVolunteer* defaultVol = new CollectorVolunteer(-1, "default", -1);
Order* defaultOrd = new Order(-1, -1, -1);
CivilianCustomer* defaultCus = new CivilianCustomer(-1, "default", -1, -1);


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

void WareHouse::addVolunteer(Volunteer* volunteer) {
    volunteerCounter++;
    volunteers.push_back(volunteer);
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
    return *defaultCus;
}

Volunteer &WareHouse:: getVolunteer(int volunteerId) const
{

    for(Volunteer* vol : volunteers) 
    {
        if(vol->getId() == volunteerId)
        {
            return *vol;
        }
    }
    return *defaultVol;

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
    return *defaultOrd;
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
//---getters----------------------------------------------------------------------------
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

vector<Order*> WareHouse:: getPendingOrders()
{
     return pendingOrders;
}

vector<Order*> WareHouse:: getInProcessOrders()
{
     return inProcessOrders;
}

vector<Order*> WareHouse:: getCompletedOrders()
{
     return completedOrders;
}
vector<Volunteer*> WareHouse:: getVolunteers()
{
     return volunteers;
}

vector<Customer*> WareHouse:: getCustomers()
{
    return customers;
}
  //---getters----------------------------------------------------------------------------



void WareHouse::transferToInProcess(Order* ord)
{
    inProcessOrders.push_back(ord);

    auto it = std::find(pendingOrders.begin(), pendingOrders.end(), ord);
    if (it != pendingOrders.end()) {
        pendingOrders.erase(it);
    }
}




void WareHouse::transferToPending(Order* ord)
{
    pendingOrders.push_back(ord);

    auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), ord);
    if (it != inProcessOrders.end()) {
        inProcessOrders.erase(it);
    }
}


void WareHouse::transferToCompleted(Order* ord)
{
    completedOrders.push_back(ord);

    auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), ord);
    if (it != inProcessOrders.end()) {
        inProcessOrders.erase(it);
    }
}


void WareHouse::removeVolunteer(Volunteer* vol)
{
    auto it = std::find(volunteers.begin(), volunteers.end(), vol);
    if (it != volunteers.end()) {
        volunteers.erase(it);
    }
}
