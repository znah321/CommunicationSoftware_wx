using System;
using System.Collections.Generic;
using System.Text;

namespace NetDocGeneric
{
    public class SampleClass
    {
        public K GenMethod<K>(K arg) 
        { 
            // set type of arg to K
            return (arg); 
        }        
        public K GenMethodWithMixedArgs<K>(K arg1, K arg2, bool tf)
        { 
            // if true, return arg1
            // if false, return arg2
            return (tf ? arg1 : arg2); 
        }
        public static K GenStaticMethod<K>(K arg) 
        { 
            return (arg); 
        }
        public static K GenStaticMethodWithMixedArgs<K>
            (K arg1, K arg2, bool tf) 
        { 
            return (tf ? arg1 : arg2); 
        }
    }

    public class SampleGenericClass<T>
    {
        public string ParameterizedGenMethod<K>(T clsParam, K arg)
        { 
            return (clsParam.GetType().Name + ", " + 
                arg.GetType().Name); 
        }
        public T GenMethod<T>(T arg) 
        { 
            return (arg); 
        }
        public static K GenStaticMethod<K>(K arg) 
        { 
            return (arg); 
        }
        public static K GenStaticMethodWithMixedArgs<K>
            (K arg1, K arg2, bool tf) 
        { 
            return (tf ? arg1 : arg2); 
        }
        public static string ParameterizedStaticGenMethod<K>
            (T clsParam, K arg) 
        { 
            return (clsParam.GetType().Name + ", " + 
                arg.GetType().Name); 
        }
    }
}
