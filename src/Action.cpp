#include "../include/Action.h"
#include <iostream>
using namespace std;

//---BaseAction---------------------------------------------------------------------------------------
// Add Constructor
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
    printErrorMsg(); // Call the function that will print the error msg to the screen.
    return errorMsg;
}

void BaseAction::printErrorMsg() const
{
    std::cout << "Error:" << errorMsg << std::endl;
}

string BaseAction::status_to_str() const 
{
    switch (getStatus())
    {
    case ActionStatus::COMPLETED:
        return "COMPLETED";
    case ActionStatus::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

//---BaseAction------------------------------------------------------------------------------------------

//---Simulate Step---------------------------------------------------------------------------------------
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){};

void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    for (int i = 1; i <= numOfSteps; i++)
    {
        for (Order *order : wareHouse.getPendingOrders()) // find orders to transfer to inProcess
        {
            if (order->getStatus() == OrderStatus::PENDING) // if pending
            {
                for (Volunteer *vol : wareHouse.getVolunteers()) // find volunteer that can take order
                {
                    if (vol->canTakeOrder(*order) && vol->isCollector()) // if found
                    {
                        vol->acceptOrder(*order);
                        order->setCollectorId(vol->getId());
                        order->setStatus(OrderStatus::COLLECTING);
                        wareHouse.transferToInProcess(order);
                    }
                }
            }
            else
            { // if collecting
                for (Volunteer *vol : wareHouse.getVolunteers())
                {
                    if (vol->canTakeOrder(*order) && !(vol->isCollector()))
                    {
                        vol->acceptOrder(*order);
                        order->setDriverId(vol->getId());
                        order->setStatus(OrderStatus::DELIVERING);
                        wareHouse.transferToInProcess(order);
                    }
                }
            }
        } // Step 1 finished
        for (Volunteer *vol : wareHouse.getVolunteers()) // performs a "step" for each volunteer
        {
            vol->step(); // Step 2 finished
        }
        for (Volunteer *vol : wareHouse.getVolunteers()) // check for each vol if they finished their order processing
        {
            if (vol->getCompletedOrderId() != NO_ORDER) // if finished with order, transfer to pending or completed
            {
                Order &completedOrder = wareHouse.getOrder(vol->getCompletedOrderId());
                if (completedOrder.getStatus() == OrderStatus::COLLECTING) // if collected
                {
                    wareHouse.transferToPending(&completedOrder);
                }
                else
                { // if delivered
                    wareHouse.transferToCompleted(&completedOrder);
                    completedOrder.setStatus(OrderStatus::COMPLETED);
                }

                if (!vol->hasOrdersLeft() && vol->getActiveOrderId() == NO_ORDER) // if reached order limit and not actively processing an order, remove vol
                {
                    wareHouse.removeVolunteer(vol);
                }
                // Step 3 + 4 finished
            }
        }
    }
    complete(); // this action never returns an error
}

string SimulateStep::toString() const
{
    return "simulateStep " + to_string(numOfSteps) + " " + status_to_str();
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//---AddOrder-------------------------------------------------------------------------------------------
// Constructor
AddOrder::AddOrder(int id) : BaseAction(), customerId(id), orderId(0){};

// Methods
void AddOrder::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);

    if (customerId > wareHouse.getCustomerCounter())
    {
        error("Cannot place this order. Customer does not exist.");
        getErrorMsg();
    }
    else
    {

        Customer &cus = wareHouse.getCustomer(customerId);

        if (cus.canMakeOrder())
        {
            orderId = wareHouse.getOrderCounter();
            Order *ord = new Order(orderId, customerId, cus.getCustomerDistance());
            wareHouse.addOrder(ord);
            cus.addOrder(ord->getId());
            complete();
        }
        else
        {
            error("Cannot place this order. Customer has reached its order limit");
            getErrorMsg();
        }
    }
}

string AddOrder::toString() const
{
    return "order " + to_string(customerId) + " " + status_to_str();
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

//---AddCustomer----------------------------------------------------------------------------------------
// Constructor
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) : BaseAction(),
customerName(customerName), customerType((customerType == "soldier") ? CustomerType::Soldier : CustomerType::Civilian),
distance(distance), maxOrders(maxOrders){};
// the ? operator checks to see if the customer type is Soldier or Civilian and gives the right type according to the string.

// Methods
void AddCustomer::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    int customerId = wareHouse.getCustomerCounter();
    if (this->customerType == CustomerType::Soldier)
    {
        SoldierCustomer* newCustomer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(newCustomer);
    }
    else
    {
        CivilianCustomer* newCustomer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(newCustomer);
    }
    complete();
}

string AddCustomer::customerTypeToString(CustomerType type) const // Convert the enum type to a string
{
    switch (type)
    {
    case CustomerType::Soldier:
        return "Soldier";
    case CustomerType::Civilian:
        return "Civilian";
    default:
        return "Undefined";
    }
}

string AddCustomer::toString() const
{
    return "customer " + customerName + " " + customerTypeToString(customerType) + " " + to_string(distance) + " " + to_string(maxOrders) + " COMPLETED";
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

//---PrintStatusOrder--------------------------------------------------------------------------------------
// Constructor
PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id){};

// Methods
void PrintOrderStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if (orderId >= wareHouse.getOrderCounter())
    {
        error(" Order doesn't exist.");
        getErrorMsg();
    }
    else
    {
        Order &ord = wareHouse.getOrder(orderId);
        std::cout << ord.toString() << std::endl;
        complete();
    }
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    return "orderStatus " + to_string(orderId) + " " + status_to_str();
}

//---PrintCustomerStatus-----------------------------------------------------------------------------------
// Constructors
PrintCustomerStatus::PrintCustomerStatus(int CustomerId) : BaseAction(), customerId(CustomerId){};

// Methods
void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if (customerId >= wareHouse.getCustomerCounter())
    {
        error(" Customer doesn't exist.");
        getErrorMsg();
    }
    else
    {
        Customer &cus = wareHouse.getCustomer(customerId);
        std::cout << "Customer Id: " << to_string(cus.getId()) << std::endl;
        for (int orderId : cus.getOrdersIds())
        {
            Order &ord = wareHouse.getOrder(orderId);
            std::cout << "OrderID: " << to_string(orderId) << std::endl;
            std::cout << "OrderStatus: " << ord.statusToString(ord.getStatus()) << std::endl;
        }
        std::cout << "numOrdersLeft: " << to_string(cus.getMaxOrders() - cus.getNumOrders()) << std::endl;
        complete();
    }
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + to_string(customerId) + " " + status_to_str();
}

//---PrintVolunteerStatus----------------------------------------------------------------------------------
// Constructors
PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id){};

// Methods
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if (volunteerId >= wareHouse.getVolunteerCounter() || wareHouse.getVolunteer(volunteerId).getId() == -1)
    {
        error(" Volunteer doesn't exist.");
        getErrorMsg();
    }
    else
    {
        Volunteer &vol = wareHouse.getVolunteer(volunteerId);
        std::cout << vol.toString() << std::endl;
        complete();
    }
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + to_string(volunteerId) + " " + status_to_str();
}

//---PrintActionsLog----------------------------------------------------------------------------------------
// Constructor
PrintActionsLog::PrintActionsLog() : BaseAction() {}

// Methods
void PrintActionsLog::act(WareHouse &wareHouse)
{
    for (BaseAction *action : wareHouse.getActions())
    {
        std::cout << action->toString() << std::endl;
    }
    wareHouse.addAction(this);
    complete(); // This action never results in an error.
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log " + status_to_str();
}

//---Close--------------------------------------------------------------------------------------------------

// Constructor
Close::Close() : BaseAction() {}

// Methods
void Close::act(WareHouse &wareHouse) 
{
    wareHouse.addAction(this);
    wareHouse.close();
    complete(); // This action never results in an error.
}

Close *Close::clone() const
{
    return new Close(*this);
}

string Close::toString() const
{
    return "close " + status_to_str();
}

//---Close--------------------------------------------------------------------------------------------------

//---BackupWareHouse----------------------------------------------------------------------------------------
// Constructor
BackupWareHouse::BackupWareHouse() : BaseAction(){};

// Methods
void BackupWareHouse::act(WareHouse &wareHouse)
{

    if (backup == nullptr) // check to see if the address is empty, meaning backup isnt pointing to anything yet
    {
        backup = new WareHouse(wareHouse); // Activate the copy constructor
    }
    else
    {
        *backup = wareHouse; // Activate the copy assigment operator
    }
    wareHouse.addAction(this);
    complete(); // This action never results in an error.
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backup COMPLETED";
    return "backup COMPLETED";
}
//---BackupWareHouse----------------------------------------------------------------------------------------

//---RestoreWareHouse---------------------------------------------------------------------------------------
// Constructor
RestoreWareHouse::RestoreWareHouse() : BaseAction(){};

// Methods
void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
    {
        error(" Backup doesn't exist");
        getErrorMsg();
    }
    else
    {
        wareHouse = *backup;
        complete();
    }
    wareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restore " + status_to_str();
}
//---RestoreWareHouse---------------------------------------------------------------------------------------