#include "TestClass.h"

#include <swilib.h>

TestClass::TestClass(const std::string &str) {
	m_str = str;
}

void TestClass::showMessage() {
	ShowMSG(0, (int) m_str.c_str());
}

TestClass::~TestClass() {
	ShowMSG(0, (int) "Destructor callled?");
}
