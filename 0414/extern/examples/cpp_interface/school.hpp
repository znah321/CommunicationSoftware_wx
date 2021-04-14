/*
 * school.hpp
 *
 * Copyright 2019 The MathWorks, Inc.
 */

#ifndef school_hpp
#define school_hpp
#include <string>
#include <iostream>

class Person
{
    std::string name;
    size_t age;

public:
    Person() {}
    Person(const std::string& name, size_t age) : name(name), age(age) {}

    void setName(const std::string& name)
    {
        this->name = name;
    }

    void setAge(size_t age)
    {
        this->age = age;
    }

    std::string getName()
    {
        return this->name;
    }

    size_t getAge()
    {
        return this->age;
    }
};

class Teacher : public Person
{
public:
    Teacher() {}
    Teacher(const std::string& name, size_t age) : Person(name, age) {}
    std::string getName()
    {
        std::string name = Person::getName();
        std::cout << "Teacher name: " << name << std::endl;
        return name;
    }
};

class Student : public Person
{
public:
    Student() {}
    Student(const std::string& name, size_t age) : Person(name, age) {}
    std::string getName()
    {
        std::string name = Person::getName();
        std::cout << "Student name: " << name << std::endl;
        return name;
    }
};

std::string getName(Person * p)
{
    return p->getName();
}


#endif

