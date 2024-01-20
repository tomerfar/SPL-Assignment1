#include "Action.h"
#include <iostream> 




ActionStatus BaseAction:: getStatus() const
{
    return status;
}

void BaseAction:: complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction:: error(string errorMsg)
{
    status = ActionStatus:: ERROR;
    // update error message and print on screen: “Error: <error_msg>”
    // add later
}

string BaseAction:: getErrorMsg() const
{
    // add later
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

 void SimulateStep:: act(WareHouse &wareHouse)
 {
    for(Order* order : wareHouse.getPendingOrders())
    {
        if(*order->getStatus() == OrderStatus::PENDING)
        {
            for(Volunteer* vol : wareHouse.getVolunteers())
            {
                if(vol->canTakeOrder(order))
                {
                    vol->acceptOrder(order);
                }
            }
        }
         
        
    }
    
 }

 

AddOrder:: AddOrder(int id) : BaseAction(), customerId(id) {};

void AddOrder:: act(WareHouse &wareHouse)
{
    if (customerId > wareHouse.getCustomerCounter()) 
    {
        error("Cannot place this order. Customer does not exist.");  
        cout << getErrorMsg() << endl;
    }

    Customer &cus = wareHouse.getCustomer(customerId);

    if(cus.canMakeOrder())
    {
        Order *ord = new Order(wareHouse.getOrderCounter(), customerId, cus.getCustomerDistance()); 
        wareHouse.addOrder(ord);
        cus.addOrder(ord->getId());
        complete();
    } 
    else {
        error("Cannot place this order. Customer has reached its order limit");
        cout << getErrorMsg() << endl;
    }
}








