#include "weakPointer.hpp"
#include "sharedPointer.hpp"

#include <iostream>
#include <string_view>

class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }

	void saySomething(std::string_view sw) {
		std::cout << "\tYour remark: " << sw << '\n';
	}
};

void printSharedPtrData(const sharedPointer<Resource>& sp, std::string_view sw)
{
	std::cout << "\t" << sw << ": ";
	std::cout << "use_count = " << sp.use_count()
			  << ", weak_count = " << sp.weak_count() << '\n';
}

void printWeakPtrData(const weakPointer<Resource>& sp, std::string_view sw)
{
	std::cout << "\t" << sw << ": ";
	std::cout << "expired: " << std::boolalpha << sp.expired()
			  << ", weak_count = " << sp.weak_count() << '\n';
}

//using shptr = sharedPointer<Resource>;
//using wptr = weakPointer<Resource>;

int main()
{
	sharedPointer<Resource> shp{new Resource()};
	shp->saySomething("checkng operator->");

	sharedPointer<Resource> shp2{shp};
	Resource* res = shp2.get();
	res->saySomething("checkng result of get()");
	printSharedPtrData(shp2, "shared pointer 2");

	sharedPointer<Resource> shp3{};
	printSharedPtrData(shp3, "shared pointer 3");
	shp3 = shp2;
	(*shp3).saySomething("checking operator*");

	//The following generate exception to check dereferencing a null sharedPointer case.
	//sharedPointer<Resource> shp4;
	//(*shp4).saySomething("checking operator*");

	weakPointer<Resource> wp1{};
	weakPointer<Resource> wp2(wp1);
	printWeakPtrData(wp2, "weak pointer 2");

	weakPointer<Resource> wp3(shp3);
	printWeakPtrData(wp3, "weak pointer 3");

	weakPointer<Resource> wp4(wp3);
	printWeakPtrData(wp4, "weak pointer 4");

	sharedPointer<Resource> shp4 = wp4.lock();
	printSharedPtrData(shp4, "shared pointer 4");

	return 0;
}

