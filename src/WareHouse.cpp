#include "WareHouse.h"
#include <iostream> 
using namespace std;



WareHouse:: WareHouse(const string &configFilePath):
isOpen(1), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), 
customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    // parse(configFilePath) implement parser 
    start();
}

void WareHouse:: start()
{
cout << "Warehouse is open!" << endl;

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
    actionsLog.push_back(action);
 }

Customer& WareHouse:: getCustomer(int customerId) const
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