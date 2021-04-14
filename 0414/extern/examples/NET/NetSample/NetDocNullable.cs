using System;

namespace NetDocNullable
{
    public class MyClass
    {
        private Nullable<double> myField = null;

        public Nullable<double> GetField()
        {
            return myField;
        }
        public Nullable<double> SetField(Nullable<double> db)
        {
            myField = db;
            return myField;
        }
    }
}
