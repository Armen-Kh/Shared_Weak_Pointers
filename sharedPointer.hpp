#ifndef SHAREDPOINTER_HPP
#define SHAREDPOINTER_HPP

#include <stdexcept>
#include <iostream>

template <typename T>
class weakPointer; 

class controlBlock
{
public:
	int ref_count{};
	int weak_count{};
	controlBlock() { std::cout << "Control block constructed!\n"; }
	~controlBlock() { std::cout << "Control block destructed!\n"; }
};

template <typename T>
class sharedPointer
{
private:
	T* m_ptr;
	controlBlock* ctrlb;
public:
	sharedPointer();
	sharedPointer(T* res);
	sharedPointer(const sharedPointer<T>& shptr);
	sharedPointer& operator= (const sharedPointer<T>& shptr);
	~sharedPointer();
	int use_count() const;
	int weak_count() const;
	T* get();
	T& operator*();
	T* operator->();
	
	friend class weakPointer<T>;
};

template <typename T>
sharedPointer<T>::sharedPointer()
	: m_ptr{nullptr}
	, ctrlb{nullptr}
{
	std::cout << "Created a shared pointer without owning any resources!\n";
}	

template <typename T>
sharedPointer<T>::sharedPointer(T* res)
	: m_ptr{res}
	, ctrlb{new controlBlock()}
{
	++(ctrlb->ref_count);
	std::cout << "Created a shared pointer and Resource acquired! ref_count = "
			  << ctrlb->ref_count << '\n';
}	

template <typename T>
sharedPointer<T>::sharedPointer(const sharedPointer<T>& shptr)
	: m_ptr{shptr.m_ptr}
	, ctrlb{shptr.ctrlb}
{
	if(ctrlb) {
		++(ctrlb->ref_count);
	}
	std::cout << "Copied a shared pointer! ref_count = "
			  << ((ctrlb) ? std::to_string(ctrlb->ref_count) : "NAN") << '\n';
}	

template <typename T>
sharedPointer<T>& sharedPointer<T>::operator= (const sharedPointer<T>& shptr) {
	if(&shptr != this) {
		if(m_ptr) {
			if(ctrlb->ref_count == 1) {
				delete m_ptr;
				delete ctrlb;
			} else {
				--(ctrlb->ref_count);
			}
		}
		m_ptr = shptr.m_ptr;
		ctrlb = shptr.ctrlb;
		if(ctrlb) {
			++(ctrlb->ref_count);
		}
		std::cout << "Shared pointer is assigned! ref_count = "
				  << ((ctrlb) ? std::to_string(ctrlb->ref_count) : "NAN") << '\n';
	}
	return *this;
}	

template <typename T>
sharedPointer<T>::~sharedPointer() {
	if(m_ptr) {	
		if(ctrlb->ref_count == 1) {
			delete m_ptr;
			if(ctrlb->weak_count == 0) {
				delete ctrlb;
			}
			std::cout << "Shared pointer and Resource destroyed!\n";
		} else if (ctrlb->ref_count != 0){
			--(ctrlb->ref_count);
			std::cout << "Shared pointer destroyed! ref_count = " << ctrlb->ref_count << '\n';
		}
	} else {
		std::cout << "Shared pointer without any ownership is destroyed!\n";
	}
}	

template <typename T>
int sharedPointer<T>::use_count() const
{	
	if(ctrlb) {
		return ctrlb->ref_count;
	}
	return 0;
}

template <typename T>
int sharedPointer<T>::weak_count() const
{	
	if(ctrlb) {
		return ctrlb->weak_count;
	}
	return 0;
}

template <typename T>
T* sharedPointer<T>::get()
{
	return m_ptr;
}

template <typename T>
T& sharedPointer<T>::operator*() {
	if(m_ptr) {
		return *m_ptr;
	}
	throw std::runtime_error("Dereferencing a null shared pointer! No resource exists.");
}

template <typename T>
T* sharedPointer<T>::operator->()
{
	return m_ptr;
}

#endif

