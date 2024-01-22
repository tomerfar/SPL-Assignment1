#include "Action.h"
#include <iostream> 
using namespace std; //Solve the problem of the cout. Or we need to write the before our cout std::



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
        if(order->getStatus() == OrderStatus::PENDING)
        {
            for(Volunteer* vol : wareHouse.getVolunteers())
            {
                if(vol->canTakeOrder(*order))
                {
                    vol->acceptOrder(*order);
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
        //Add status errors
    }

    Customer &cus = wareHouse.getCustomer(customerId);

    if(cus.canMakeOrder())
    {
        Order *ord = new Order(wareHouse.getOrderCounter(), customerId, cus.getCustomerDistance()); 
        wareHouse.addOrder(ord);
        cus.addOrder(ord->getId());
        complete();
        // add status complete
    } 
    else {
        error("Cannot place this order. Customer has reached its order limit");
        cout << getErrorMsg() << endl;
        // add status error
    }
}



//---AddCustomer---------------------------------------------------------------------------------------
AddCustomer:: AddCustomer(string customerName, string customerType, int distance, int maxOrders): BaseAction(),
  customerName(customerName), distance(distance), maxOrders(maxOrders){
    if (customerType == "Soldier")
    {
        this->customerType = CustomerType::Soldier;
    }
    else
    {
        this->customerType = CustomerType::Civilian;
    }
}

//Methods
void AddCustomer::act(WareHouse &wareHouse)
{
    int customerId = wareHouse.getCustomerCounter();
    if(this->customerType == CustomerType::Soldier)
    {
        Customer newCustomer = SoldierCustomer(customerId, customerName, distance, maxOrders);
    }
    else
    {
        Customer newCustomer = CivilianCustomer(customerId, customerName, distance, maxOrders);
    }
    wareHouse.addCustomer(newCustomer);
    complete();
    
}

string AddCustomer:: customerTypeToString(CustomerType type) //Convert the enum type to a string. check if neccesary
{
    switch (type) 
    {
    case CustomerType::Soldier:
        return "Soldier";
    case CustomerType::Civilian:
        return "Civilian";
    }
}

string AddCustomer:: toString() const
{
    cout << "customer " << this->customerName << "" + this->customerType << "" << this->distance << "" << this->maxOrders << "" << endl;
    // std_type = (*this).customerTypeToString(customerType);
    // return "Customer name: " + customerName + 
    // "\n"
    // + "Customer type: " + std_type + 
    // "\n"
    // + "distance " + to_string(distance) + 
    // "\n"
    // + "maxOrders " + to_string(maxOrders); 
    // ALL OF THIS NEEDS TO GO ON PRINT CUSTOMER STATUS!!!!
}

AddCustomer *AddCustomer:: clone() const
{
    return new AddCustomer(*this);
}


//---PrintOrder--------------------------------------------------------------------------------------
 //Constructor
 PrintOrderStatus::PrintOrderStatus(int id):
 BaseAction(), id(id){};

 //Methods
 void PrintOrder:: act(WareHouse& wareHouse)
 {
    Order *ord = warehouse.getOrder(id);
    cout << ord->toString() << endl;
 }
 string PrintOrder:: toString() const
 {
    cout << "orderStatus " <<  this->id << this->getstatus() << endl;
 }


 
 









