using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MyApp
{
    
    class Program
    {
        [DllImport("JSILibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void InterpretText(string variables, string values, string functions, string textToInterpret, StringBuilder result, int resLength);

        [DllImport("JSILibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void InterpretTextEx(string[] variables, string[] values, int nbVar, string[] functions, int nbFunc, string textToInterpret, StringBuilder result, int resLength);

        static void Main(string[] args)
        {

            TestInterpretText();
            TestInterpretTextEx();

            Console.ReadLine();
        }        

        static void TestInterpretText()
        {
            string variables = "nom;age";
            string values = "\"France - Irelande\";18";
            string funcs = "function Extract(nom, start, len) { return nom.substring(start, start + len); } function Square(n) { return n*n; }";
            string textToInterpret = "Square(age)";
            var result = new StringBuilder(1000);
            InterpretText(variables, values, funcs, textToInterpret, result, result.Capacity);

            Console.WriteLine("Age = " + result);
        }

        static void TestInterpretTextEx()
        {
            string[] variables = { "nom", "age" };
            string[] values = { "\"France - Irelande\"", "18" };
            string[] funcs = { "function Extract(nom, start, len) { return nom.substring(start, start + len); }", "function Square(n) { return n*n; }" };
            string textToInterpret = "Extract(nom, 0, 6)";
            var result = new StringBuilder(1000);
            InterpretTextEx(variables, values, variables.Length, funcs, funcs.Length, textToInterpret, result, result.Capacity);

            Console.WriteLine("Nom = " + result);
        }
    }
}
