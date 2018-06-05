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
            ManagementClass mCls = new ManagementClass("Win32_Processor");
            while (true)
            {
                
                var instance = mCls.GetInstances();
                //Console.WriteLine("获取托管对象耗时：{0}ms", _watch.ElapsedMilliseconds);
                //_watch.Start();
                _watch.Restart();
                foreach (ManagementObject obj in instance)
                {
                    var vaule = obj.Properties["CurrentClockSpeed"].Value;
                    Console.WriteLine("{0}", vaule);
                }
                _watch.Stop();
                Console.WriteLine("查询托管对象属性耗时：{0}ms", _watch.ElapsedMilliseconds);
                //Thread.Sleep(500);
            }
            Console.ReadKey();
        }
    }
}
