#include "Action.h"
#include <iostream> 
using namespace std; //Solve the problem of the cout. Or we need to write the before our cout std::



//---BaseAction---------------------------------------------------------------------------------------
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
    this-> errorMsg = errorMsg;
    // update error message and print on screen: “Error: <error_msg>”
}

string BaseAction:: getErrorMsg() const
{
    printErrorMsg(); // Call the function that will pring ther error msg to the screen.
    return errorMsg;
}
void BaseAction:: printErrorMsg() const
 {
    cout << "Error:" << errorMsg << endl;
 }

string BaseAction:: status_to_str() const // added by yuval
{
    switch (getStatus())
    {
        case ActionStatus:: COMPLETED:   return "COMPLETED";
        case ActionStatus:: ERROR:   return "ERROR";
        default: return "UNKNOWN";
    }
}

//---BaseAction------------------------------------------------------------------------------------------

//---Simulate Step---------------------------------------------------------------------------------------
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

 
//---SimulateStep---------------------------------------------------------------------------------------

 
//---AddOrder-------------------------------------------------------------------------------------------
//Constructor
AddOrder:: AddOrder(int id) : BaseAction(), customerId(id), orderId(0) {};

//Methods
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
        orderId = wareHouse.getOrderCounter();
        Order *ord = new Order(orderId, customerId, cus.getCustomerDistance()); 
        wareHouse.addOrder(ord);
        cus.addOrder(ord->getId());
        complete();
    } 
    else {
        error("Cannot place this order. Customer has reached its order limit");
        cout << getErrorMsg() << endl;
    }
}

string AddOrder:: toString() const 
{
    return "order " + to_string(orderId) + " " + status_to_str(); 
}

AddOrder *AddOrder:: clone() const
{
    return new AddOrder(*this);
}

//---AddOrder-------------------------------------------------------------------------------------------


//---AddCustomer----------------------------------------------------------------------------------------
//Constructor
AddCustomer:: AddCustomer(string customerName, string customerType, int distance, int maxOrders): BaseAction(),
  customerName(customerName), distance(distance), maxOrders(maxOrders), customerType((customerType == "Soldier") ? CustomerType::Soldier : CustomerType::Civilian)
   {};
   // the ? operator checks to see if the customer type is Soldier or Civilian and gives the right type according to the string.

//Methods
void AddCustomer::act(WareHouse &wareHouse)
{
    int customerId = wareHouse.getCustomerCounter();
    if(this->customerType == CustomerType::Soldier)
    {
        SoldierCustomer newCustomer = SoldierCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(&newCustomer);
    }
    else
    {
        CivilianCustomer newCustomer = CivilianCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(&newCustomer);
    }
    
    complete();
    
}

string AddCustomer:: customerTypeToString(CustomerType type) const //Convert the enum type to a string. check if neccesary
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
    return "customer " + customerName + " " + customerTypeToString(customerType) + " "
     + to_string(distance) + " " + to_string(maxOrders);
}

AddCustomer *AddCustomer:: clone() const
{
    return new AddCustomer(*this);
}

//---AddCustomer-------------------------------------------------------------------------------------------


//---PrintStatusOrder--------------------------------------------------------------------------------------
 //Constructor
 PrintOrderStatus::PrintOrderStatus(int id):
 BaseAction(), orderId(id){};

 //Methods
 void PrintOrderStatus:: act(WareHouse& wareHouse)
 {
    Order &ord = wareHouse.getOrder(this->orderId); // getOrder returns a refernce to the order, which is why we create a refernce argument
    cout << ord.toString() << endl; // check if correct
 }

 PrintOrderStatus *PrintOrderStatus:: clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus:: toString() const
{
    return "orderStatus " + to_string(orderId) + " " + status_to_str();
}


//---PrintStatusOrder--------------------------------------------------------------------------------------

//---PrintCustomerStatus-----------------------------------------------------------------------------------
//Constructors
PrintCustomerStatus::PrintCustomerStatus(int CustomerId):
BaseAction(), customerId(customerId){};

//Methods
void PrintCustomerStatus:: act(WareHouse &wareHouse)
{
    if (this->customerId > wareHouse.getCustomerCounter())
    {
        error("Customer doesn't exist.");
        cout << getErrorMsg() << endl;  
    }
    else
    {
        Customer &cus = wareHouse.getCustomer(this->customerId);
        cout << "Customer Id: " << to_string(cus.getId()) << endl;
        for(int orderId : cus.getOrdersIds())
        {
            Order &ord = wareHouse.getOrder(orderId);
            cout << "OrderID: " << to_string(orderId) << endl;
            cout << "OrderStatus: " << ord.statusToString(ord.getStatus()) << endl;
        }
    }
}

PrintCustomerStatus *PrintCustomerStatus:: clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus:: toString() const // check if correct
{
    return "customerStatus " + to_string(customerId) + " " + status_to_str();
}

//---PrintCustomerStatus-----------------------------------------------------------------------------------

//---PrintVolunteerStatus----------------------------------------------------------------------------------
//Constructors
 PrintVolunteerStatus::PrintVolunteerStatus(int id):
 BaseAction(), volunteerId(id){};

 //Methods
 void PrintVolunteerStatus:: act(WareHouse &wareHouse)
 {
    if(volunteerId > wareHouse.getVolunteerCounter())
    {
        error("Volunteer doesn't exist.");
        cout << getErrorMsg() << endl; 
    }
    else
    {
        Volunteer &vol = wareHouse.getVolunteer(volunteerId);
        cout << vol.toString() << endl;
    }
 }

PrintVolunteerStatus *PrintVolunteerStatus:: clone() const
  {
    return new PrintVolunteerStatus(*this);
  }

string PrintVolunteerStatus:: toString() const
   {
    return "volunteerStatus " + to_string(this->volunteerId) + " " + status_to_str();
   }
   
//---PrintVolunteerStatus-----------------------------------------------------------------------------------

//---PrintActionsLog----------------------------------------------------------------------------------------

//check that we don't need to implement constructot (default given)

void PrintActionsLog:: act(WareHouse &wareHouse) 
{
    for(BaseAction* action : wareHouse.getActions())
    {
        cout<< this->toString() << endl;
    }
}


PrintActionsLog *PrintActionsLog:: clone() const
{
    return new PrintActionsLog(*this);
}


string PrintActionsLog:: toString() const 
{
    return "printActionsLog "  + status_to_str();
}



//---PrintActionsLog----------------------------------------------------------------------------------------


//---Close--------------------------------------------------------------------------------------------------


//check that we don't need to implement constructot (default given)

    void Close:: act(WareHouse &wareHouse) 
    {
        wareHouse.close();
        // see what else need to do 
    }


    Close *Close:: clone() const
    {
        return new Close(*this);
    }


    string Close:: toString() const 
    {
        return "close "  + status_to_str();
    }

//---Close--------------------------------------------------------------------------------------------------


//---BackupWareHouse----------------------------------------------------------------------------------------
//Constructor
 BackupWareHouse::BackupWareHouse(): BaseAction(){};

 //Methods
 void BackupWareHouse:: act(WareHouse &wareHouse)
 {
    if(backup == nullptr) //check to see if the address is empty, meaning backup isnt pointing to anything yet
    {
      backup = new WareHouse(wareHouse); //Activate the copy constructor
      complete();
    }
    else
    {
     *backup = wareHouse;  // Activate the copy assigment operator
     complete();
    }
   
 }

 BackupWareHouse *BackupWareHouse:: clone() const
 {
    return new BackupWareHouse(*this);
 }

  string BackupWareHouse:: toString() const
  {
    return "backup Completed";
  }
//---BackupWareHouse----------------------------------------------------------------------------------------


//---RestoreWareHouse---------------------------------------------------------------------------------------
//Constructor
RestoreWareHouse:: RestoreWareHouse(): BaseAction(){};

//Methods
 void RestoreWareHouse:: act(WareHouse &wareHouse)
 {
    wareHouse = *backup; // use the assignment operator again, needs to create it
 }

 RestoreWareHouse *RestoreWareHouse:: clone() const
 {
    return new RestoreWareHouse(*this);
 }

string RestoreWareHouse:: toString() const
{

}




//---RestoreWareHouse---------------------------------------------------------------------------------------