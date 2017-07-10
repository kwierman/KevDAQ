#include "KevDAQ.hh"


namespace kevdaq{

    KevDAQClass::KevDAQClass(int const& i): fMyInt(i)
    {

    }

    KevDAQClass::~KevDAQClass()
    {

    }
    void KevDAQClass::setInt(int const& i)
    {
      fMyInt = i;
    }

    int KevDAQClass::getInt()
    {
      return this->fMyInt;
    }



}

/************************************************************
BOOST Python Part of this file
************************************************************/

#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE(libkevdaq)
{
    class_< ::kevdaq::KevDAQClass >("KevDAQClass",
        "This is KevDAQClass's DOCstring",
        init<int>(args("i"), "__init__ docstring")
        )
    .def(init<int>())
    .def("getInt", &kevdaq::KevDAQClass::getInt);
}


/*
using namespace boost::python;

class_<Foo,bases<Bar,Baz> >("Foo",
          "This is Foo's docstring."
          "It describes our Foo extension class",

          init<int,char const*>(args("x","y"), "__init__ docstring")
          )
   .def(init<double>())
   .def("get_name", &Foo::get_name, return_internal_reference<>())
   .def("set_name", &Foo::set_name)
   .def_readwrite("value", &Foo::value);
*/