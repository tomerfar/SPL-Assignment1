#include "Volunteer.h"


//Constructor
Volunteer::Volunteer(int id, const string &name) : id(id), name(name), activeOrderId(NO_ORDER), completedOrderId(NO_ORDER) {}

 
//Volunteer Methods
int Volunteer:: getId() const{
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

bool Volunteer:: isBusy() const{
    if(activeOrderId != NO_ORDER )
    {
        return true;
    }
    return false;
}






// #Collector Volunteer#

CollectorVolunteer:: CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0){}

// CollectorVolunteer *CollectorVolunteer::clone() const 
// {
//     return new CollectorVolunteer(*this);
// }

 // void step() override 
 // {
 //     if(decreaseCoolDown())
 //     {
 //         
 //     }
 // 
 
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
    if (timeLeft != 0) 
    {
        this->timeLeft = timeLeft - 1;
        return false;      
    }
    return true;
}

// bool hasOrdersLeft() const override;
//
// bool canTakeOrder(const Order &order) const override
// {
//      
// }
//      
// void acceptOrder(const Order &order) override
// {
//      this->activeOrderId = order.orderId;
//      
//
//
// string toString() const override;


// #Limited Collector Volunteer#
//Constructors
 LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders)
 : Volunteer(id, name, coolDown, maxOrders){}
 

 LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone()const{
    return new LimitedCollectorVolunteer(*this);
 }

 //Methods
 bool LimitedCollectorVolunteer:: hasOrdersLeft() const{
   if(ordersLeft > 0){
    return true;
   }
   return false;
 }

 bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const
 {
    if(order.getStatus() == OrderStatus::PENDING && ordersLeft > 0 && !isBusy)
    {
        return true;
    }
    return false;
 }

 void LimitedCollectorVolunteer:: acceptOrder(const Order &order)
 {

 }

  int LimitedCollectorVolunteer:: getMaxOrders() const
  {
    return maxOrders;
  }

  int LimitedCollectorVolunteer:: getNumOrdersLeft() const
  {
    return ordersLeft;
  }

  

 