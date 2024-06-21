using System.Runtime.InteropServices;

namespace WinFormsChaosApp
{

   
    public static class VRMatAPI
    {
        const string ConstDll = "vrmat.dll";


        public delegate void UniqueNameCallback(string uniqueName, IntPtr userData);
        
        public delegate void ValueCallback(IntPtr value, IntPtr userData);

        [Flags]
        public enum Rdt : uint
        {
            None  ,
            Float ,
            Int ,
            String
        }

        public enum Sqdt : uint
        {
            None = 0,
            Single = 1 << 8,
            List = 1 << 9
        }

        public enum Stdt : uint
        {
            None = 0,
            Single = 1 << 3,
            Array3 = 1 << 4,
            Array4 = 1 << 5,
            Array9 = 1 << 6,
            Array12 = 1 << 7
        }

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr DataContainer_Create();

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DataContainer_Destroy(IntPtr dc);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_GetMeta(IntPtr dc, uint element, ref Meta meta);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DataContainer_GetUniqueName(IntPtr dc, string pluginName, UniqueNameCallback resultHandler, IntPtr userData);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_Load(IntPtr dc, string buffer, int length);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_Open(IntPtr dc, string fileName);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_Save(IntPtr dc, string fileName, int version);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_SetMeta(IntPtr dc, uint element, ref Meta meta);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DataContainer_AddPlugin(IntPtr dc, string name, string type, string klass);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DataContainer_AddParameter(IntPtr dc, uint pluginId, string name, string type, int custom);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DataContainer_GetElementId(IntPtr dc, uint pluginId, string name);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_RemoveElement(IntPtr dc, uint elementId);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_SetValue(IntPtr dc, uint paramId, IntPtr data, uint type, int listLength);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_GetValue(IntPtr dc, uint paramId, ValueCallback valueCallback, IntPtr userData);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr DataContainer_Iterator_Create(IntPtr dc, uint pluginId, IntPtr pluginIterator, ValueCallback callback, IntPtr userData);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DataContainer_Iterator_Destroy(IntPtr it);

        [DllImport(ConstDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DataContainer_Iterator_Increment(IntPtr it, ValueCallback callback, IntPtr userData);




        // Define struct for Meta
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct Meta
        {
            public uint mask;
            public int version;
            [MarshalAs(UnmanagedType.LPStr)]
            public string category;
            [MarshalAs(UnmanagedType.LPStr)]
            public string preview;
            [MarshalAs(UnmanagedType.LPStr)]
            public string tag;
            [MarshalAs(UnmanagedType.LPStr)]
            public string fileName;
            [MarshalAs(UnmanagedType.LPStr)]
            public string pluginName;
            [MarshalAs(UnmanagedType.LPStr)]
            public string pluginType;
            [MarshalAs(UnmanagedType.LPStr)]
            public string pluginClass;
            public int pluginVersion;
            [MarshalAs(UnmanagedType.LPStr)]
            public string paramName;
            [MarshalAs(UnmanagedType.LPStr)]
            public string paramType;
            public int paramCustom;
            public int paramFilePath;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct ValueData
        {
            public uint pluginId;
            public uint paramId;
            public uint listCount;
            public uint listIndex;
            public uint isList;
            public uint components;
            public Rdt rdt;
            public Stdt stdt;
            public Sqdt sqdt;
            public IntPtr data;
        }
    }
}

