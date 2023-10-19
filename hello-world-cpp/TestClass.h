#pragma once

#include <string>

class TestClass {
	public:
		std::string m_str;
		
		TestClass(const std::string &str);
		
		~TestClass();
		
		void showMessage();
};
