using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WMIDotNet
{
    public class WMIMemoryInfo
    {
        public long PhysicalMemory
        {
            get
            {
                return Convert.ToInt64(WMIBase.GetManagementObjectPropertyValue("Win32_PhysicalMemory", "Capacity"));
            }
        }
        public long FreePhysicalMemory
        {
            get
            {
                return Convert.ToInt64(WMIBase.GetManagementObjectPropertyValue("Win32_PerfRawData_PerfOS_Memory", "AvailableBytes"));
            }
        }

        public long UsedPhysicalMemory
        {
            get
            {
                return PhysicalMemory - FreePhysicalMemory;
            }
        }

        public int UsedSizeRate
        {
            get
            {
                return Convert.ToInt32((double)UsedPhysicalMemory / (double)PhysicalMemory * 100);
            }
        }
    }
}
