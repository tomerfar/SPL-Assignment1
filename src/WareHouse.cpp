#include "../include/WareHouse.h"
#include "../include/Action.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// Constructor
WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(),
customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    parse(configFilePath);
    cout << "Warehouse is open!" << endl;
    start();
}

// Destructor

WareHouse::~WareHouse()
{
    clearData();
}

// clearData 
void WareHouse::clearData()
{ // erases pointed to memory in the heap
    for (const Order *ord : pendingOrders)
    { 
        delete ord;
    }
    for (const Order *ord : inProcessOrders)
    { 
        delete ord;
    }
    for (const Order *ord : completedOrders)
    { 
        delete ord;
    }
    for (const Customer *cus : customers)
    { 
        delete cus;
    }
    for (const Volunteer *vol : volunteers)
    { 
        delete vol;
    }
    for (const BaseAction *action : actionsLog)
    { 
        delete action;
    }
    // erases all elements in the vectors (pointers to the resources)
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    customers.clear();
    volunteers.clear();
    actionsLog.clear();
}


// CopyConstructor
WareHouse::WareHouse(const WareHouse &other) :
  isOpen(other.isOpen),actionsLog(), volunteers(), pendingOrders(),
  inProcessOrders(), completedOrders(), customers(),
  customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
  orderCounter(other.orderCounter)
{ // Perform a deep copy of resources: orders, customers, volunteers, and actions
    for (const Order *otherOrder : other.pendingOrders)
    {
        pendingOrders.push_back(otherOrder->clone());
    }
    for (const Order *otherOrder : other.inProcessOrders)
    {
        inProcessOrders.push_back(otherOrder->clone());
    }
    for (const Order *otherOrder : other.completedOrders)
    {
        completedOrders.push_back(otherOrder->clone());
    }
    for (const Customer *otherCustomer : other.customers)
    {
        customers.push_back(otherCustomer->clone());
    }
    for (const Volunteer *otherVolunteer : other.volunteers)
    {
        volunteers.push_back(otherVolunteer->clone());
    }
    for (const BaseAction *otherAction : other.actionsLog)
    {
        actionsLog.push_back(otherAction->clone());
    }
}

// CopyAssignmentOperator
WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {
        clearData(); 
        // Copying fields
        this->orderCounter = other.orderCounter;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;

        // Perform a deep copy of orders, customers, volunteers, and actions
        for (const Order *otherOrder : other.pendingOrders)
        {
            pendingOrders.push_back(otherOrder->clone());
        }
        for (const Order *otherOrder : other.inProcessOrders)
        {
            inProcessOrders.push_back(otherOrder->clone());
        }
        for (const Order *otherOrder : other.completedOrders)
        {
            completedOrders.push_back(otherOrder->clone());
        }
        for (const Customer *otherCustomer : other.customers)
        {
            customers.push_back(otherCustomer->clone());
        }
        for (const Volunteer *otherVolunteer : other.volunteers)
        {
            volunteers.push_back(otherVolunteer->clone());
        }
        for (const BaseAction *otherAction : other.actionsLog)
        {
            actionsLog.push_back(otherAction->clone());
        }
    }
    return *this;
}

// Move Constructor - reallocate to null so fields don't get deleted once leaving the scope
WareHouse::WareHouse(WareHouse &&other) noexcept : isOpen(other.isOpen), actionsLog(move(other.actionsLog)), 
volunteers(move(other.volunteers)), pendingOrders(move(other.pendingOrders)), inProcessOrders(move(other.inProcessOrders)),
completedOrders(move(other.completedOrders)), customers(move(other.customers)), customerCounter(other.customerCounter), 
volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter) {};
                                                

// Move Assignment Operator
WareHouse &WareHouse::operator=(WareHouse &&other) noexcept
{
    if (this != &other)
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

void WareHouse::start()
{
    open();
    string input;
    while (isOpen)
    {    
        cout << "Enter command: ";
        getline(cin, input);
        parseLine(input);
    }
}

//Set of default Objects for Orders, Customers, Volunteers
CollectorVolunteer *defaultVol = new CollectorVolunteer(-1, "default", -1);
Order *defaultOrd = new Order(-1, -1, -1);
CivilianCustomer *defaultCus = new CivilianCustomer(-1, "default", -1, -1);

void WareHouse::addOrder(Order *order)
{
    orderCounter++;
    pendingOrders.push_back(order);
}

void WareHouse::addCustomer(Customer *customer)
{
    customerCounter++;
    customers.push_back(customer);
}

void WareHouse::addVolunteer(Volunteer *volunteer)
{
    volunteerCounter++;
    volunteers.push_back(volunteer);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action); 
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *cus : customers)
    {
        if (cus->getId() == customerId)
        {
            return *cus;
        }
    }
    return *defaultCus;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{

    for (Volunteer *vol : volunteers)
    {
        if (vol->getId() == volunteerId)
        {
            return *vol;
        }
    }
    return *defaultVol; // returned when volunteer is deleted
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *ord : pendingOrders)
    {
        if (ord->getId() == orderId)
        {
            return *ord;
        }
    }
    for (Order *ord : inProcessOrders)
    {
        if (ord->getId() == orderId)
        {
            return *ord;
        }
    }
    for (Order *ord : completedOrders)
    {
        if (ord->getId() == orderId)
        {
            return *ord;
        }
    }
    return *defaultOrd;
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::close()
{
    for (Order *ord : pendingOrders)
    {
        cout << "OrderID: " << ord->getId() << ", CustomerID: " << ord->getCustomerId() << ", Status: " << ord->statusToString(ord->getStatus()) << endl;
    }
    for (Order *ord : inProcessOrders)
    {
        cout << "OrderID: " << ord->getId() << ", CustomerID: " << ord->getCustomerId() << ", Status: " << ord->statusToString(ord->getStatus()) << endl;
    }
    for (Order *ord : completedOrders)
    {
        cout << "OrderID: " << ord->getId() << ", CustomerID: " << ord->getCustomerId() << ", Status: " << ord->statusToString(ord->getStatus()) << endl;
    }

    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

int WareHouse::getCustomerCounter()
{
    return customerCounter;
}

int WareHouse::getOrderCounter()
{
    return orderCounter;
}

int WareHouse::getVolunteerCounter()
{
    return volunteerCounter;
}

vector<Order *> WareHouse::getPendingOrders()
{
    return pendingOrders;
}

vector<Order *> WareHouse::getInProcessOrders()
{
    return inProcessOrders;
}

vector<Order *> WareHouse::getCompletedOrders()
{
    return completedOrders;
}
vector<Volunteer *> WareHouse::getVolunteers()
{
    return volunteers;
}

vector<Customer *> WareHouse::getCustomers()
{
    return customers;
}

void WareHouse::transferToInProcess(Order *ord) //move order from pending to inProcess
{
    inProcessOrders.push_back(ord);

    auto it = std::find(pendingOrders.begin(), pendingOrders.end(), ord);
    if (it != pendingOrders.end())
    {
        pendingOrders.erase(it);
    }
}

void WareHouse::transferToPending(Order *ord) //move order from inProcess back to Pending
{
    pendingOrders.push_back(ord);

    auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), ord);
    if (it != inProcessOrders.end())
    {
        inProcessOrders.erase(it);
    }
}

void WareHouse::transferToCompleted(Order *ord) //move order from inProcess to Completed
{
    completedOrders.push_back(ord);

    auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), ord);
    if (it != inProcessOrders.end())
    {
        inProcessOrders.erase(it);
    }
}

void WareHouse::removeVolunteer(Volunteer *vol) // delete volunteer when reaches his orders limit
{
    auto it = std::find(volunteers.begin(), volunteers.end(), vol);
    if (it != volunteers.end())
    {
        volunteers.erase(it);
    }
}
