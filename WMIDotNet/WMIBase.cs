using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;

namespace WMIDotNet
{
    public class WMIBase
    {
        /// <summary>
        /// 获取托管对象属性
        /// </summary>
        /// <param name="wmiClass"></param>
        /// <param name="propertyName"></param>
        /// <returns></returns>
        public static object GetManagementObjectPropertyValue(ManagementClass wmiClass, string propertyName)
        {
            object ret = null;
            using(wmiClass)
                foreach (ManagementObject obj in wmiClass.GetInstances())
                {
                    if(obj.Properties[propertyName] != null)
                        ret = obj.Properties[propertyName].Value;
                }
            return ret;
        }

        public static object GetManagementObjectPropertyValue(string className, string propertyName)
        {
            ManagementClass wmiClass = new ManagementClass(className);
            return GetManagementObjectPropertyValue(wmiClass, propertyName);
        }
    }
}
