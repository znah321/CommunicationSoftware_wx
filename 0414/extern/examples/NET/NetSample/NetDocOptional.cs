using System;
using System.Text;

namespace NetDocOptional
{
    public class MyClass
    {
        public string Greeting(
            int x,
            string str1 = "hello",
            string str2 = "world")
        {
            return str1 + " " + str2;
        }
    }
}
