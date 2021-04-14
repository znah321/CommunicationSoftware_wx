using System;
/*
 * C# Assembly used in MATLAB .NET documentaion.
 * Method getNewData is used to demonstrate 
 * how MATLAB handles a System.Object 
 * that includes another System.Object.
 */
namespace NetDocCell
{
    public class MyGraph
    {
        public Object[] getNewData()
        /*
         * Create a System.Object array to use in MATLAB examples.
         * Returns containerArr System.Object array containing:
         *  fLabel System.String object
         *  plotData System.Object array containing:
         *      xLabel System.String object
         *      doubleArr System.Double array
        */
        {
            String fLabel = "Figure Showing New Graph Data";
            Double[] doubleArr = { 
								18, 32, 3.133, 44, -9.9, -13, 33.03 };
            String xLabel = "X-Axis Label";
            Object[] plotData = { xLabel, doubleArr };
            Object[] containerArr = { fLabel, plotData };
            return containerArr;
        }
    }
}
