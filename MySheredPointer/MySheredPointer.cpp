#include <iostream>
#include <string>

class Date;

class SharedPointerCounter
{
public:
	explicit SharedPointerCounter() : m_counter(0) 
	{
		std::cout << "\nSharedPointerCounter constructor" << std::endl;
	}
	~SharedPointerCounter() 
	{
		std::cout << "\nSharedPointerCounter destructor" << std::endl;
	}

	SharedPointerCounter(const SharedPointerCounter&) = delete;

	SharedPointerCounter& operator=(const SharedPointerCounter&) = delete;

	void operator++() 
	{
		++m_counter;
	}

	void operator--()
	{
		--m_counter;
	}

	unsigned int get() const 
	{
		return m_counter;
	}

private:
	unsigned int m_counter = 0;
};


template<class T>
class MySheredPointer
{
public:
	explicit MySheredPointer(T* ip_t) : mp_managedObject(ip_t)
	{
		std::cout << "MySheredPointer constructor ";
		mp_counter = new SharedPointerCounter();
		++(*mp_counter);
		std::cout << "SharedPointerCounter created. Counter is " << mp_counter->get() << ". " << std::endl;
	}
	~MySheredPointer()
	{
		--(*mp_counter);
		std::cout << "MySheredPointer destructor. Counter is " << mp_counter->get() << ". ";
		if (mp_counter->get() == 0)
		{
			delete mp_managedObject;
			delete mp_counter;
			mp_managedObject = nullptr;
			mp_counter = nullptr;
			std::cout << "SharedPointerCounter and maneged obgect were deleted in reset";
		}
		std::cout << std::endl;
	}

	MySheredPointer(const MySheredPointer<T>& i_objToMove) noexcept
	{
		mp_managedObject = i_objToMove.mp_managedObject;
		mp_counter = i_objToMove.mp_counter;
		++(*mp_counter);
		std::cout << "MySheredPointer copy constructor. Counter is " << mp_counter->get() << ". " << std::endl;;
	};

	MySheredPointer(const MySheredPointer<T>&& i_objToMove) noexcept
	{
		mp_managedObject = i_objToMove.mp_managedObject;
		mp_counter = i_objToMove.mp_counter;
		++(*mp_counter);
		std::cout << "MySheredPointer move constructor. Counter is " << mp_counter->get() << ". " << std::endl;;
	};

	long use_count() const
	{
		return mp_counter->get();
	}

	void reset(T* newObject)
	{
		--(*mp_counter);
		if (mp_counter->get() == 0)
		{
			delete mp_managedObject;
			delete mp_counter;
			mp_managedObject = nullptr;
			mp_counter = nullptr;
			std::cout << "Old SharedPointerCounter and maneged obgect were deleted in reset";
		}

		mp_managedObject = newObject;
		mp_counter = new SharedPointerCounter();
		++(*mp_counter);
		std::cout << "New SharedPointerCounter created in reset. Counter is " << mp_counter->get() << ". " << std::endl;
	}

	T& operator*() const
	{
		return *mp_managedObject;
	}

	T* operator->() const
	{
		return mp_managedObject;
	}

	explicit operator bool() const
	{
		return mp_managedObject;
	}

private:
	T* mp_managedObject = nullptr;
	SharedPointerCounter* mp_counter = nullptr;
};

class Date
{
public:
	Date() 
	{
		m_date = "08.07.2021";
		m_objectCounter++;
		m_objectNumber = m_objectCounter;
	}
	~Date() 
	{
	}

	void getData()
	{
		std::cout << "Current date " << m_objectNumber << " : " << m_date << std::endl;
	}
private:
	static  int m_objectCounter;
	int m_objectNumber = 0;
	std::string m_date;
};
int Date::m_objectCounter = 0;



int main()
{
	MySheredPointer<Date> up_date(new Date);
	up_date->getData();
	MySheredPointer<Date> up_date2(std::move(up_date));
	up_date2->getData();
	MySheredPointer<Date> up_date3(std::move(up_date));
	up_date3->getData();
	up_date3.reset(new Date);
	up_date3->getData();
}
