#ifndef WEAKPOINTER_HPP
#define WEAKPOINTER_HPP

#include "sharedPointer.hpp"

#include <iostream>

template <typename T>
class weakPointer
{
private:
	T* m_ptr;
	controlBlock* ctrlb;
public:
	weakPointer();
	weakPointer(const weakPointer<T>& wptr);
	weakPointer(const sharedPointer<T>& shptr);
	weakPointer<T>& operator= (const weakPointer<T>& wptr);
	~weakPointer();
	sharedPointer<T> lock();
	bool expired() const;
	int weak_count() const;
};

template <typename T>
weakPointer<T>::weakPointer()
	: m_ptr{nullptr}
	, ctrlb{nullptr}
{
	std::cout << "Created a weak pointer without a link to any resource!\n";
}	

template <typename T>
weakPointer<T>::weakPointer(const weakPointer<T>& wptr)
	: m_ptr{wptr.m_ptr}
	, ctrlb{wptr.ctrlb}
{
	if(ctrlb) {
		++(ctrlb->weak_count);
	}
	std::cout << "Copied a weak pointer! weak_count = "
			  << ((ctrlb) ? std::to_string(ctrlb->weak_count) : "NAN") << '\n';
}	

template <typename T>
weakPointer<T>::weakPointer(const sharedPointer<T>& shptr)
	: m_ptr{shptr.m_ptr}
	, ctrlb{shptr.ctrlb}
{
	if(ctrlb) {
		++(ctrlb->weak_count);
	}
	std::cout << "Created a weak pointer based on shared pointer! weak_count = "
			  << ((ctrlb) ? std::to_string(ctrlb->weak_count) : "NAN") << '\n';
}	

template <typename T>
weakPointer<T>& weakPointer<T>::operator= (const weakPointer<T>& wptr) {
	if(&wptr != this) {
		if(ctrlb) {
			if(ctrlb->weak_count == 1 && ctrlb->ref_count == 0) {
				delete ctrlb;
			} else {
				--(ctrlb->weak_count);
			}
		}
		m_ptr = wptr.m_ptr;
		ctrlb = wptr.ctrlb;
		if(ctrlb) {
			++(ctrlb->weak_count);
		}
		std::cout << "Weak pointer is assigned! weak_count = "
				  << ((ctrlb) ? std::to_string(ctrlb->weak_count) : "NAN") << '\n';
	}
	return *this;
}	

template <typename T>
weakPointer<T>::~weakPointer() {
	if(ctrlb) {
		if(ctrlb->weak_count == 1 && ctrlb->ref_count == 0) {
			delete ctrlb;
			std::cout << "Weak pointer and corresponding control block destroyed!\n";
		} else if (ctrlb->weak_count != 0){
			--(ctrlb->weak_count);
			std::cout << "Weak pointer destroyed! weak_count = " << ctrlb->weak_count << '\n';
		}
	} else {
		std::cout << "Weak pointer without any associated resource is destroyed!\n";
	}
}	

template <typename T>
sharedPointer<T> weakPointer<T>::lock()
{
	sharedPointer<T> shp{};
	if(m_ptr) {
		shp.m_ptr = m_ptr;
		shp.ctrlb = ctrlb;
		++(ctrlb->ref_count);
	}	
	return shp;
}

template <typename T>
bool weakPointer<T>::expired() const
{
	return bool(!m_ptr);	
}

template <typename T>
int weakPointer<T>::weak_count() const
{	
	if(ctrlb) {
		return ctrlb->weak_count;
	}
	return 0;
}

#endif

