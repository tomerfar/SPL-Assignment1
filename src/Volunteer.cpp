#include "Volunteer.h"
using namespace std;

//Constructor
Volunteer::Volunteer(int id, const string &name) : id(id), name(name), activeOrderId(NO_ORDER), completedOrderId(NO_ORDER) {}

 
//Volunteer Methods
int Volunteer:: getId() const
{
    return id;
}

const std::string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}

bool Volunteer:: isBusy() const
{
    return activeOrderId != NO_ORDER;
}

// #Collector Volunteer#

CollectorVolunteer:: CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0){}

CollectorVolunteer *CollectorVolunteer::clone() const 
{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer:: step() 
{
    bool isFinish = decreaseCoolDown(); //returns true only if time left == 0
    if(isFinish)
    {
      completedOrderId = activeOrderId; //volunteer finished with the order
      activeOrderId = NO_ORDER; // change isBusy status to false, volunteer will be able to accept more orders now
    }
}
 
 
int CollectorVolunteer:: getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer:: getTimeLeft() const
{
    return timeLeft;
}

bool CollectorVolunteer:: decreaseCoolDown() //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
{
    if (timeLeft > 0) 
    {
        timeLeft = timeLeft - 1;      
    }
    return timeLeft == 0;
}

bool CollectorVolunteer:: hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer:: canTakeOrder(const Order &order) const 
{
     return !isBusy() && order.getStatus() == OrderStatus::PENDING;
}
     
// Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)

void CollectorVolunteer:: acceptOrder(const Order &order)  
{
    activeOrderId = order.getId(); // here it also changes the status of isBusy to true
    timeLeft = coolDown;

}     

string CollectorVolunteer:: toString() const
{
    string str_orderID = to_string(getActiveOrderId());
    if(getActiveOrderId() == NO_ORDER)
    {
        str_orderID = "None";
    }

    return "VolunteerID: " + to_string(getId()) + 
    "\n"
    + "isBusy: " + to_string(isBusy()) + 
    "\n"
    + "OrderID: " + str_orderID + 
    "\n"
    + "timeLeft: " + to_string(getTimeLeft()) + 
    "\n"
    + "ordersLeft: No Limit";
}


// #Limited Collector Volunteer#
//Constructors
LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders)
: CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders) , ordersLeft(maxOrders){}


LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone()const
{
    return new LimitedCollectorVolunteer(*this);
}

//Methods
bool LimitedCollectorVolunteer:: hasOrdersLeft() const
{
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const
{
    return CollectorVolunteer::canTakeOrder(order) && ordersLeft > 0;
}

void LimitedCollectorVolunteer:: acceptOrder(const Order &order)
{
    
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;

}

int LimitedCollectorVolunteer:: getMaxOrders() const
{
    return maxOrders;
}

int LimitedCollectorVolunteer:: getNumOrdersLeft() const
{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const
{
    string str_orderID = std::to_string(getActiveOrderId());
    if(getActiveOrderId() == NO_ORDER)
    {
        str_orderID = "None";
    }

    return "VolunteerID: " + to_string(getId()) + 
    "\n"
    + "isBusy: " + to_string(isBusy()) + 
    "\n"
    + "OrderID: " + str_orderID + 
    "\n"
    + "timeLeft: " + to_string(getTimeLeft()) + 
    "\n"
    + "ordersLeft: " + to_string(getNumOrdersLeft());
}
  

  

// #Driver Volunteer#

//Constructor
DriverVolunteer:: DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
: Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

//Methods
DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}


int DriverVolunteer:: getDistanceLeft() const
{
    return distanceLeft;
}


int DriverVolunteer:: getMaxDistance() const
{
    return maxDistance;
}


int DriverVolunteer:: getDistancePerStep() const
{
    return distancePerStep;
}


bool DriverVolunteer:: decreaseDistanceLeft() //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
{
    distanceLeft = distanceLeft - distancePerStep;
    return distanceLeft <= 0;
}


bool DriverVolunteer:: hasOrdersLeft() const
{
    return true;
}


// Signal if the volunteer is not busy and the order is within the maxDistance
// check if we need to keep order.getstatus

bool DriverVolunteer:: canTakeOrder(const Order &order) const
{
    return !isBusy() && order.getStatus() == OrderStatus::COLLECTING && order.getDistance() <= maxDistance;
}


void DriverVolunteer:: acceptOrder(const Order &order)
{
    
    activeOrderId = order.getId();
     distanceLeft = order.getDistance();
}


void  DriverVolunteer:: step() // Decrease distanceLeft by distancePerStep
{
    bool isFinished = decreaseDistanceLeft();
    if(isFinished)
    {
        completedOrderId = activeOrderId; //volunteer finished with the order
        activeOrderId = NO_ORDER; // change isBusy status back to false, will be free to accept more orders now
        
    }
}


string DriverVolunteer:: toString() const 
{
    string str_orderID = std::to_string(getActiveOrderId());
    if(getActiveOrderId() == NO_ORDER)
    {
        str_orderID = "None";
    }

    return "VolunteerID: " + to_string(getId()) + 
    "\n"
    + "isBusy: " + to_string(isBusy()) + 
    "\n"
    + "OrderID: " + str_orderID + 
    "\n"
    + "distanceLeft: " + to_string(getDistanceLeft()) + 
    "\n"
    + "ordersLeft: No Limit";
}




//# Limited Driver Volunteer
//Constructors
 LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders)
 : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){}

//Methods
 LimitedDriverVolunteer *LimitedDriverVolunteer::clone()const
 {
    return new LimitedDriverVolunteer(*this);
 }


int LimitedDriverVolunteer:: getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer:: getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer:: hasOrdersLeft() const{
   return ordersLeft > 0;
 }

bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const
 {
   return DriverVolunteer::canTakeOrder(order) && ordersLeft > 0;
 }

 void LimitedDriverVolunteer:: acceptOrder(const Order &order)
 {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;  
 }
 
 string LimitedDriverVolunteer::toString() const
 {
     string str_orderID = to_string(getActiveOrderId());
    if(getActiveOrderId() == NO_ORDER)
    {
        str_orderID = "None";
    }
    return "VolunteerID: " + to_string(getId()) + 
    "\n"
    + "isBusy: " + to_string(isBusy()) + 
    "\n"
    + "OrderID: " + str_orderID + 
    "\n"
    + "distanceLeft: " + to_string(getDistanceLeft()) + 
    "\n"
    + "ordersLeft: " + to_string(getNumOrdersLeft());
 }







