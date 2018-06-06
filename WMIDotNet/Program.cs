using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using System.Threading;
using System.Diagnostics;
namespace WMIDotNet
{
    class Program
    {
        static Stopwatch _watch = new Stopwatch();
        static void Main(string[] args)
        {
            //ManagementClass mCls = new ManagementClass("Win32_Processor");
            //while (true)
            //{

            //    var instance = mCls.GetInstances();
            //    //Console.WriteLine("获取托管对象耗时：{0}ms", _watch.ElapsedMilliseconds);
            //    //_watch.Start();
            //    _watch.Restart();
            //    foreach (ManagementObject obj in instance)
            //    {
            //        var vaule = obj.Properties["CurrentClockSpeed"].Value;
            //        Console.WriteLine("{0}", vaule);
            //    }
            //    _watch.Stop();
            //    Console.WriteLine("查询托管对象属性耗时：{0}ms", _watch.ElapsedMilliseconds);
            //    //Thread.Sleep(500);
            //}
            //Example();
            //while (true)
            //{
            //    ManagementClass perfOSDataClass = new ManagementClass("Win32_PerfRawData_PerfOS_Memory");
            //    long lAvailableBytes = Convert.ToInt64(GetManagementObjectPropertyValue(perfOSDataClass, "AvailableBytes"));
            //    Console.WriteLine("AvailableBytes:{0:F2}G", (double)lAvailableBytes / 1024 / 1024 / 1024);
            //    ManagementClass oreratingSysClass = new ManagementClass("Win32_OperatingSystem");
            //    long lFreePhysicalMemory = Convert.ToInt64(GetManagementObjectPropertyValue(oreratingSysClass, "FreePhysicalMemory"));
            //    Console.WriteLine("FreePhysicalMemory:{0:F2}G", (double)lFreePhysicalMemory / 1024 / 1024 );
            //    Thread.Sleep(500);
            //}
            //ShowManagementClassProperites("Win32_Processor");

            WMIMemoryInfo memoInfo = new WMIMemoryInfo();
            WMICPUInfo cpuInfo = new WMICPUInfo();
            while (true)
            {
                //Console.WriteLine("占用率：{0}", memoInfo.UsedSizeRate);
                //Console.WriteLine(WMIBase.GetManagementObjectPropertyValue("Win32_PerfFormattedData_PerfProc_Process", "PercentProcessorTime"));
                Console.WriteLine("{0}\t", cpuInfo.UsageRate);
                Thread.Sleep(500);
            }

            Console.ReadKey();
        }

        static void Example()
        {
            // Get the WMI class
            ManagementClass processClass =
                new ManagementClass();
            processClass.Path = new
                ManagementPath("Win32_Process");

            // Get the methods in the class
            MethodDataCollection methods =
                processClass.Methods;

            // display the methods
            Console.WriteLine("Method Names: ");
            foreach (MethodData method in methods)
            {
                Console.WriteLine(method.Name);
            }
            Console.WriteLine();

            // Get the properties in the class
            PropertyDataCollection properties =
                processClass.Properties;

            // display the properties
            Console.WriteLine("Property Names: ");
            foreach (PropertyData property in properties)
            {
                Console.Write("{0} = {1}", property.Name);
            }
            Console.WriteLine();

            // Get the Qualifiers in the class
            QualifierDataCollection qualifiers =
                processClass.Qualifiers;

            // display the qualifiers
            Console.WriteLine("Qualifier Names: ");
            foreach (QualifierData qualifier in qualifiers)
            {
                Console.WriteLine(qualifier.Name);
            }
        }

        static void GetMemory()
        {
            ManagementClass memoryClass = new ManagementClass();
            memoryClass.Path = new ManagementPath("Win32_PhysicalMemory");
            //获取实例
            /*var instances = memoryClass.GetInstances();
            foreach (var p in memoryClass.Properties)
            {
                Console.Write("{0} = ", p.Name);
                foreach (ManagementObject obj in instances)
                {
                    Console.WriteLine("{0}", obj.Properties[p.Name].Value);
                }
            }*/
            long memoryCapacity = 0;
            foreach (ManagementObject obj in memoryClass.GetInstances())
            {
                memoryCapacity = Convert.ToInt64(obj.Properties["Capacity"].Value);
            }
            
            ManagementClass perfOSDataClass = new ManagementClass("Win32_PerfRawData_PerfOS_Memory");
            /*foreach(var p in perfOSDataClass.Properties)
            {
                Console.Write("{0} = ", p.Name);
                foreach (ManagementObject obj in perfOSDataClass.GetInstances())
                {
                    Console.WriteLine("{0}", obj.Properties[p.Name].Value);
                }
            }*/
            long memoryAvailableBytes = 0;
            foreach (ManagementObject obj in perfOSDataClass.GetInstances())
            {
                memoryAvailableBytes = Convert.ToInt64(obj.Properties["AvailableBytes"].Value);
            }
            //Console.WriteLine("内存大小：{0}byte", memoryCapacity);
            //Console.WriteLine("可用大小: {0}byte", memoryAvailableBytes);
            //Console.WriteLine("空闲率: {0:F0}%", (double)memoryAvailableBytes/(double)memoryCapacity*100);
            long memoryUsed = memoryCapacity - memoryAvailableBytes;
            Console.WriteLine("占用率: {0:F2}%", (double)memoryUsed / (double)memoryCapacity * 100);
        }
        

        static void ShowManagementClassProperites(string className)
        {
            ManagementClass wmiClass = new ManagementClass(className);
            foreach (var p in wmiClass.Properties)
            {
                Console.Write("{0} = ", p.Name);
                foreach (ManagementObject obj in wmiClass.GetInstances())
                {
                    Console.WriteLine("{0}", obj.Properties[p.Name].Value);
                }
            }
        }

        static object GetManagementObjectPropertyValue(ManagementClass wmiClass, string propertyName)
        {
            object ret = null;
            foreach (ManagementObject obj in wmiClass.GetInstances())
            {
                ret = obj.Properties[propertyName].Value;
            }
            return ret;
        }
    }
}
