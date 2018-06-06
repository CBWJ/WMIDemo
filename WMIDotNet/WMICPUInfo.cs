using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace WMIDotNet
{
    public class WMICPUInfo
    {
        public int UsageRate
        {
            get
            {
                return Convert.ToInt32(WMIBase.GetManagementObjectPropertyValue("Win32_Processor", "LoadPercentage"));
            }
        }

        public float GetCpuUsage()
        {
            var cpuCounter = new PerformanceCounter
            {
                CategoryName = "Processor",
                CounterName = "% Processor Time",
                InstanceName = "_Total"
            };
            return cpuCounter.NextValue();
        }
    }
}
