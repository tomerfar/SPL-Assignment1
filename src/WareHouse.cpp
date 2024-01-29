#include "../include/WareHouse.h"
#include "../include/Action.h"

#include <fstream>
#include <sstream>
#include <iostream> 
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
 actionsLog(other.actionsLog), volunteers(other.volunteers),
 pendingOrders(other.pendingOrders), inProcessOrders(other.inProcessOrders),
 completedOrders(other.completedOrders), customers(other.customers),
 customerCounter(other.customerCounter),
 volunteerCounter(other.volunteerCounter),
 orderCounter(other.orderCounter)
 {
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
 }
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
    }
    return *this;
}
//CopyAssignmentOperator

//Move Constructor
WareHouse::WareHouse(WareHouse&& other) noexcept:
isOpen(move(other.isOpen)), actionsLog(move(other.actionsLog)), volunteers(move(other.volunteers)),
 pendingOrders(move(other.pendingOrders)), inProcessOrders(move(other.inProcessOrders)),
 completedOrders(move(other.completedOrders)), customers(move(other.customers)),
 customerCounter(other.customerCounter),
 volunteerCounter(other.volunteerCounter),
 orderCounter(other.orderCounter)
{
    //check what to do with the counter fields, they arent pointers so we cant set them do be nullptr
   other.pendingOrders.clear();
   other.inProcessOrders.clear();
   other.completedOrders.clear();
   other.customers.clear();
   other.volunteers.clear();
   other.actionsLog.clear();
};
//Move Constructor

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
    volunteerCounter = other.volunteerCounter;
    orderCounter = other.orderCounter;

  }
  return *this;
}
//Move Assignment Operator

void WareHouse:: start()
{
    open();
    cout << "Warehouse is open!" << endl;
    string input;
    while (isOpen) {
        if (!input.empty()) {
            cout << "Enter command: ";
            getline(cin, input);
            std::istringstream iss(input);
            std::string command;
            iss >> command;
            parseLine(command);
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

void WareHouse:: transferToInProcess(Order* ord) // go over function, understand pointers 
{
    inProcessOrders.push_back(ord);
    for (auto it = getPendingOrders().begin(); it != getPendingOrders().end(); ++it) //Finds the order we want to delete and erase it
    {
        if(*it == ord)
        {
            getPendingOrders().erase(it);
            break;
        }
    }
}

void WareHouse:: transferToPending(Order* ord)
{
    pendingOrders.push_back(ord);
    for (auto it = getInProcessOrders().begin(); it != getInProcessOrders().end(); ++it) //Finds the order we want to delete and erase it
    {
        if(*it == ord)
        {
            getInProcessOrders().erase(it);
            break;
        }
    }
}

void WareHouse:: transferToCompleted(Order* ord)
{
    completedOrders.push_back(ord);
    for (auto it = getCompletedOrders().begin(); it != getInProcessOrders().end(); ++it) //Finds the order we want to delete and erase it
    {
        if(*it == ord)
        {
            getInProcessOrders().erase(it);
            break;
        }
    }
}

void WareHouse:: removeVolunteer(Volunteer* vol)
{
    for (auto it = volunteers.begin(); it != volunteers.end(); ++it) {
        if (*it == vol) {
            volunteers.erase(it);
            break;
        }
    }
}