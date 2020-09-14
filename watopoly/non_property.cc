#include "non_property.h"
#include "bank.h"


std::string NonProperty::getFaculty() {
	return "School";
}

std::string NonProperty::printBuilding() {
	return "NonProperty, Name: " + name;
}
NonProperty::NonProperty(std::string name)
	: Building {name, true} {
}


int NonProperty::getPropertyValue() {
	return 0;
}