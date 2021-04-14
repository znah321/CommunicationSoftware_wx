/*
 * cppUseCases.hpp
 *
 * Copyright 2019 The MathWorks, Inc.
 */
 
#ifndef cppUseCases_hpp
#define cppUseCases_hpp
#include <string>

 // classes
 // enums
 // functions
 // typedefs
 // templates
 // member functions
 // constructors
 // data members
 // functions with pointers, reference for non fundamental types
 // functions with pointers, reference, boxed Arr for fundamental types
 // functions with const char *, char *, std::string
 // explicit instantiation
 // implicit instantiation
 // functions with std::size_t, std::uint8_t inputs
 // typedefs to value types and non-value types


namespace ns {

    class EmptyClass {};
    enum MyEnum1 { en1 = 1, en2, en4 = 4 };

    class MyClass1 {
    public:
        int fundamentalProp;
        const int constProp = 5;
        bool boolProp;
        std::string strProp;
        EmptyClass nonFundamentalProp;
        MyEnum1 enumProp;

        MyClass1() {}
        MyClass1(const MyClass1& myc1) {
            this->fundamentalProp = myc1.fundamentalProp;
            this->boolProp = myc1.boolProp;
            this->strProp = myc1.strProp;
            this->nonFundamentalProp = myc1.nonFundamentalProp;
            this->enumProp = myc1.enumProp;
        }

        void setFundamentalProp(const int& fundProp);
    };

    class MyClass2 {
    public:
        double a;
        short b;
        long c;
    };
}

ns::MyEnum1 getMyEnum1Copy(ns::MyEnum1 en);

ns::MyClass1 getMyClass1Copy(ns::MyClass1 myc1);

// shape
double addClassByPtr(const ns::MyClass2* myc2);

double addClassByRef(const ns::MyClass2& myc2);

// direction

// inout
void updateClassByPtr(ns::MyClass2* myc2, double a, short b, long c);

void updateClassByRef(ns::MyClass2& myc2, double a, short b, long c);

// in
void readClassByPtr(ns::MyClass2* myc2);

void readClassByRef(ns::MyClass2& myc2);

// out
void fillClassByPtr(ns::MyClass2* myc2, double a, short b, long c);

void fillClassByRef(ns::MyClass2& myc2, double a, short b, long c);

// cases for shapes
void readScalarPtr(const int * in); // len = 1

void readScalarPtr(const ns::MyClass2 * in); // len = 1

void readMatrix1DPtr(const int * mat, size_t m);

void readMatrix1DPtrFixedSize(const int * mat); // fixed size = 5

void readMatrix2DPtr(const int * mat, size_t m, size_t n);

void readMatrix2DPtrFixedSize(const int * mat); // fixed size = 5

void readMatrix3DPtr(const int * mat, size_t m, size_t n, size_t p);

void readMatrix1DFixedArr(const int mat[5]);

void readMatrix2DFixedArr(const int mat[5][5]);

void readMatrix3DFixedArr(const int mat[5][5][5]);

void readMatrix1DArr(const int mat[], size_t len);

int * getRandomValues(size_t len);

int * getRandomValuesFixedSize(); // fixed size = 5

// cases for direction

// inout
void updateMatrix1DPtrByX(int * mat, size_t len, int x);

void updateMatrix1DArrByX(int mat[], size_t len, int x);

void swap(int & a, int & b);

// in
int addValuesByPtr(int * mat, size_t len);

int addValuesByArr(int mat[], size_t len);

void readString(std::string & str);

// out
void fillRandomValuesToPtr(int * mat, size_t len);

void fillRandomValuesToArr(int mat[], size_t len);

void fillString(std::string & str);

// cases for type and shape

const char * getStringCopy(const char * str);

void readCharArray(const char * chArray, size_t len);

void readInt8Array(const char * int8Array, size_t len);

const char * getRandomCharScalar();

const char * getRandomInt8Scalar();

void updateCharArray(char * chArray, size_t len);

// template prototype
template <typename T>
class templClass {};

// explicit instantiation
template class templClass<int>;

// implicit instantiation
void useTemplClass(templClass<double> in) {}

// template with non-type parameter
template <int size>
class my_int_array {
public:
    int arr[size];
};

// explicit instantiation
template class my_int_array<4>;
void useTemplClassNonType(my_int_array<4> intArr4);

void useMixedTypes(std::size_t, std::int16_t, std::uint64_t);

// typedefs
typedef int16_t intData;
typedef intData * intDataPtr;

using clsData = ns::MyClass1;
using clsDataRef = clsData &;

void useTypedef(intData);
void useTypedefPtr(intDataPtr);
void useTypedefToClass(clsData);
void useTypedefRef(clsDataRef);
void useTypedefConstRef(const clsData &);

#endif

