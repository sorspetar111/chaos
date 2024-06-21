using System.Xml.Linq;
using static WinFormsChaosApp.VRMatAPI;

namespace WinFormsChaosApp
{
    public class MediatorVRMatAPI : IDisposable
    {
        private readonly string _file;
        public MediatorVRMatAPI(string file)
        {
            _file = file;
            Open();
        }

        ~MediatorVRMatAPI()
        {
            Dispose(false);
        }

        public nint Dysplay { get; set; }
        public uint AddPlugin(string name, string type, string klass)
        {
            uint id = VRMatAPI.DataContainer_AddPlugin(Dysplay, name, type, klass);

            if (id < 1)
                throw new Exception("VR Exception! The plugin was not added correctly.");

            return id;
        }

        public void Container(string buffer, int length)
        {
            bool isOK = VRMatAPI.DataContainer_Load(Dysplay, buffer, length) > 0;
            if (!isOK)
                throw new Exception("VR Exception! The container was not load correctly.");

        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }


        public Meta GetMeta(Meta meta, uint element = 1)
        {
            bool isOK = VRMatAPI.DataContainer_GetMeta(Dysplay, element, ref meta) >= 1;

            if (!isOK)
                throw new Exception("VR Exception! The get meta was not load correctly.");

            return meta;
        }

        public void Save(string file, int version = 1)
        {
            bool isSaved = VRMatAPI.DataContainer_Save(Dysplay, file, version) >= 1;
            if (!isSaved)
                throw new Exception("VR Exception! The file was not saved correctly.");
        }

        public Meta SetMeta(Meta meta, uint element = 1)
        {
            bool isOK = VRMatAPI.DataContainer_SetMeta(Dysplay, element, ref meta) >= 1;

            if (!isOK)
                throw new Exception("VR Exception! The get meta was not load correctly.");

            return meta;
        }
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                //   managed resources

            }

            //   unmanaged resources
            if (Dysplay != IntPtr.Zero)
            {
                VRMatAPI.DataContainer_Destroy(Dysplay);
                Dysplay = IntPtr.Zero;
            }
        }

        protected virtual bool Open()
        {
            Dysplay = VRMatAPI.DataContainer_Create();
            bool isOpen = VRMatAPI.DataContainer_Open(Dysplay, _file) >= 1;

            if (!isOpen)
                throw new Exception("VR Exception! The file was not load correctly.");

            return isOpen;
        }
    }



}
