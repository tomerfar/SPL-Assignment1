#include "WareHouse.h"



WareHouse:: WareHouse(const string &configFilePath) 
{

}


void WareHouse:: start()
{
isOpen = true;
//need to check if we print here warehouse is open or do other things.

}

void WareHouse:: addOrder(Order* order)
{
   orderCounter++;
   pendingOrders.push_back(order);
   
}

 void WareHouse:: addAction(BaseAction* action)
 {
    
 }


// CivilianCustomer dne(-1, "Does Not Exist", -1, -1); // default customer if id doesn't exist



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
            return *ord
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

void WareHouse:: close()
{
    isOpen = false;
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