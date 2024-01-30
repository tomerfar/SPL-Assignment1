#include "../include/Order.h"
#include <iostream> //Check if necessary in this .cpp file
using namespace std;

// Constructor
Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance),
status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

// Methods
int Order::getId() const
{
  return id;
}

int Order::getCustomerId() const
{
  return customerId;
}

int Order::getDistance() const
{
  return distance;
}

void Order::setStatus(OrderStatus status)
{
  this->status = status;
}

void Order::setCollectorId(int collectorId)
{
  this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
  this->driverId = driverId;
}

int Order::getCollectorId() const
{
  return collectorId;
}

int Order::getDriverId() const
{
  return driverId;
}
OrderStatus Order::getStatus() const
{
  return status;
}

const string Order::statusToString(OrderStatus status) const
{
  if (status == OrderStatus::PENDING)
  {
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
const string Order::collectorToString() const // Convert collector id to string
{
  string str_collector = to_string(collectorId);
  if (collectorId == NO_VOLUNTEER)
  {
    str_collector = "None";
  }
  return str_collector;
}

const string Order::driverToString() const // Convert driver id to string
{
  string str_driver = to_string(driverId);
  if (driverId == NO_VOLUNTEER)
  {
    str_driver = "None";
  }
  return str_driver;
}

const string Order::toString() const
{

  return "OrderId: " + to_string(id) +
         "\n" + "OrderStatus: " + this->statusToString(status) +
         "\n" + "CustomerId: " + to_string(customerId) +
         "\n" + "Collector: " + this->collectorToString() +
         "\n" + "Driver: " + this->driverToString();
}

Order *Order::clone() const
{
  return new Order(*this);
}
