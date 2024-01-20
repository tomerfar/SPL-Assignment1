#include "Order.h"
#include <iostream> //Check if necessary in this .cpp file


//Constructor
Order::Order(int id, int customerId, int distance):
 id(id), customerId(customerId), distance(distance),
 status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

 //Methods
 int Order:: getId() const 
 {
    return id;
 }
 
 int Order:: getCustomerId() const
 {
    return customerId;
 }

 int Order:: getDistance() const
 {
  return distance;
 }

 void Order:: setStatus(OrderStatus status)
 {
    this->status = status;
 }

 void Order:: setCollectorId(int collectorId)
 {
    this->collectorId = collectorId;
 }

  void Order:: setDriverId(int driverId)
  {
    this->driverId = driverId;
  }

  int Order:: getCollectorId() const{
    return collectorId;
  }
  
  int Order:: getDriverId() const{
    return driverId;
  }
  OrderStatus Order:: getStatus() const{
    return status;
  }

   const std::string statusOrder(OrderStatus status){
    if (status == OrderStatus::PENDING){
        return "Pending";
    }
    else if (status == OrderStatus::COLLECTING)
    {
        return "Collecting";
    }
    else if (status == OrderStatus::DELIVERING)
    {
        return "Delivering";
    }
    else if (status == OrderStatus::COMPLETED)
    {
        return "Completed";
    }
    return "None";
  }

  const std::string Order::toString() const{
    string str_collector = std::to_string(driverId);
    if(collectorId == NO_VOLUNTEER)
    {
        str_collector = "None";
    }
    string str_driver = std::to_string(collectorId);
    if(driverId == NO_VOLUNTEER){
        str_driver = "None";
    }
    
     return "OrderId: " + std::to_string(id) +
      "\n" 
     + "OrderStatus: " + std::to_string(static_cast<int>(status)) + // need to check this line.
      "\n" 
     + "CustomerId: " + std::to_string(customerId) +
      "\n" 
     + "Collector: " + str_collector + 
     "\n" 
     + "Driver: " + str_driver;
  }




