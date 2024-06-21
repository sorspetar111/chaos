
namespace WinFormsChaosApp
{
    public partial class Form1 : Form
    {
        private int _containerLength;
        private string _name, _type, _klass, _containerBuffer;
        private uint _pluginId;
        public Form1()
        {
            InitializeComponent();
        }

        public bool IsValid
        {
            get
            {
                return this.VR != null;
            }
        }

        public MediatorVRMatAPI VR { get; set; }
        public string VRMathFile { get; set; }
        private void ContainerClick(object sender, EventArgs e)
        {
            if (this.IsValid)
                this.VR.Container(_containerBuffer, _containerLength);
        }

        private void GetMetaClick(object sender, EventArgs e)
        {
            if (!this.IsValid)
                return;

            var m = new VRMatAPI.Meta();
            var newMeta = this.VR.GetMeta(m);
        }

        private void OpenClick(object sender, EventArgs e)
        {
            OpenFile();
            this.VR = new MediatorVRMatAPI(VRMathFile);
        }

        

        private void PluginClick(object sender, EventArgs e)
        {
            if (!this.IsValid)
                return;

            this._pluginId = this.VR.AddPlugin(_name, _type, _klass);
        }

        private void SaveClick(object sender, EventArgs e)
        {
            if (!this.IsValid)
                return;


            SaveFile();
            this.VR.Save(this.VRMathFile);
        }

     

        private void SetMetaClick(object sender, EventArgs e)
        {
            if (!this.IsValid)
                return;

            var m = new VRMatAPI.Meta();
            var newMeta = this.VR.SetMeta(m);
        }

        private void SaveFile()
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "V-Ray Files (*.vrmat)|*.vrmat|All Files (*.*)|*.*";

            DialogResult result = saveFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                VRMathFile = saveFileDialog.FileName;
            }
        }

        private void OpenFile()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "V-Ray Files (*.vrmat)|*.vrmat|All Files (*.*)|*.*";

            DialogResult result = openFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                VRMathFile = openFileDialog.FileName;
            }
        }

    }


}
