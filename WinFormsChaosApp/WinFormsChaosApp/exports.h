#pragma once

#ifdef VRMAT_EXPORTS
#define VRMAT_API __declspec(dllexport)
#else
#define VRMAT_API __declspec(dllimport)
#endif

/* ==========================================================================
 * A Data container is a simple structure to hold the content of a vrmat XML file into the memory
 * It basically represents a flat list of plugins, each of which contains a number key-value pairs called parameters.
 * Both plugins and parameters does not come in any specific order due to the underlying storage mechanism. Enumerating elements will most probably differ from the order in the XML file
 * In the API both plugins and parameters are uniquely identified by their id
 * In the XML the identifiers are the plugins and parameters names - thus for practical reasons those shall be unique 
 * A plugin can refer to another plugin, building a logical graph by having a parameter that has a string value(s) with the other plugin's name
 * The API supports addition, enumeration and removal of both plugins and parameters, setting and getting parameter data, as well as setting and getting document/plugin/prameter meta
 * The meta is mostly important for parsing back the XML.
 * the parameter values can be 0, 1 or more integers, floats, fixed-size array of floats and strings. Generic list types are not supported - e.g. all list entries must be of the same type
 * The colors and vectors are represented by an array of 3 floats - r,g,b or x,y,z
 * the acolor is an array of 4 floats r,g,b,a
 * the 3x3 matrix is an array of 9 floats - 3 vectors for each column
 * the 3x4 transform is an array of 12 floats - 4 vectors or rather 1 matrix and 1 additional vector
 * all strings are null-terminated C-strings
 * integers and booleans are interchangiable, as well as colors and vectors. Only the meta type can determine the logical type
 * There are many attributes existing in the XML files, which are eiter used with specific context, not covered in this API, or not used at all
 * Some of the attributes are duplicated for historic reasons, and others split certain information into two values. The API is providing an abstraction for all of that
   ==========================================================================*/

namespace VRMat
{
	/// <summary>
	/// Raw Data Type
	/// The fundamental data type the container can store
	/// </summary>
	enum class Rdt : unsigned int
	{
		None    = 0,
		Float   = 1 << 0,
		Int     = 1 << 1,
		String  = 1 << 2,
	};

	/// <summary>
	/// Structured data type
	/// The structuring options for the fundamental data types
	/// Floats can be array-ed, ints and strings can only be single
	/// </summary>
	enum class Stdt : unsigned int
	{
		None    = 0,
		Single  = 1 << 3,
		Array3  = 1 << 4,
		Array4  = 1 << 5,
		Array9  = 1 << 6,
		Array12 = 1 << 7
	};

	/// <summary>
	/// Sequential data type
	/// Specifies if a structured data type is a list of structures or a single structure
	/// </summary>
	enum class Sqdt : unsigned int
	{
		None    = 0,
		Single  = 1 << 8,
		List    = 1 << 9
	};

	/// <summary>
	/// Holds possible values for the Meta::mask field
	/// </summary>
	enum class MetaCategories
	{
		None           = 0,

		RootVersion    = 1 << 0,
		RootCategory   = 1 << 1,
		RootPreview    = 1 << 2,
		RootTag        = 1 << 3,
		RootFileName   = 1 << 4,

		PluginName     = 1 << 5,
		PluginType     = 1 << 6,
		PluginClass    = 1 << 7,
		PluginVersion  = 1 << 8,

		ParamName     = 1 << 9,
		ParamType     = 1 << 10,
		ParamCustom   = 1 << 11,
		ParamFilePath = 1 << 12,

		Root          = RootVersion | RootCategory | RootPreview | RootTag | RootFileName,
		Plugin        = PluginName | PluginType | PluginClass | PluginVersion,
		Param         = ParamName | ParamType | ParamCustom | ParamFilePath,
		All           = Root | Plugin | Param
	};

#pragma pack(push, 1)

	/// <summary>
	/// The combined meta data for all possible attributes to all possible types
	/// </summary>
	struct Meta
	{
		/// <summary>
		/// specifies which attributes shall be considered for getting or setting
		/// </summary>
		unsigned int mask;

		/*root
		* attributes for the entire vrmat file as a whole
		*/

		/// <summary>
		/// Specifies the version of the .vrmat file in terms of XML layout
		/// 0 - The usual format we all use
		/// 1 - The new fancy and more compact format, without any historic legacy, which nobody uses
		/// </summary>
		int version;

		/// <summary>
		/// For version 1, specifies an additional category attribute
		/// </summary>
		const char* category;

		/// <summary>
		/// Holds a base64 encoded string that represents the preview image if any.
		/// </summary>
		const char* preview;

		/// <summary>
		/// Specifies what is the root tag of the XML document. 
		/// Typically <vrmat> or <vropt>.
		/// The Legacy <vismat> and <visopt> are also supported
		/// </summary>
		const char* tag;

		/// <summary>
		/// If the data container was loaded from a file on the disk, this attribute holds
		/// the full path to the file
		/// </summary>
		const char* fileName;

		/*plugin
		* attributes for each <Asset> and <vrayplugin> in the XML document
		* for historic reasons there are two nested tags here
		*/

		/// <summary>
		/// The plugin name or the URL - <Asset url="...">
		/// This is mandatory and should (must) be unique
		/// </summary>
		const char* pluginName;

		/// <summary>
		/// The plugin type - <vrayplugin name="...">
		/// </summary>
		const char* pluginType;

		/// <summary>
		/// The plugin class or category - both <Asset type="..."> and <vrayplugin type="...">
		/// </summary>
		const char* pluginClass;

		/// <summary>
		/// Plugin version. If not specified the serializer will write out the latest version at build time.
		/// This is used only for migration
		/// </summary>
		int pluginVersion;

		/*param
		* attributes for each <parameter> of each <vrayplugin>
		*/

		/// <summary>
		/// The parameter name - both <parameter label="..."> and <parameter name="...">
		/// </summary>
		const char* paramName;

		/// <summary>
		/// The parameter type. <parameter type="..."> and <parameter listype="..."> 
		/// This describes what is the data in the <value> tag. 
		/// Possible values are "integer", "bool", "float", "string", "color", "acolor", "vector", "matrix", "transform", "plugin"
		/// Types like "color texture", "float texture", etc are legacy and are only used to parse old vrmat files
		/// The serializer will take care of putting the right strings in "type" and "listType" attributes
		/// </summary>
		const char* paramType;

		/// <summary>
		/// Specifies whether the parameter is an existing V-Ray plugin parameter or some custom user data - <parameter isUserData="...">
		/// </summary>
		int paramCustom;

		/// <summary>
		/// Specifies if the parameter is supposed to be a file path value or not - <parametr handler="...">
		/// This obviously requires the data type to be a string. This is mostly used for the UI
		/// </summary>
		int paramFilePath;
	};

	/// <summary>
	/// A proxy helper structure to exchange parameter values in or out of the container
	/// When getting string lists, this structure describes a single list item
	/// For any other types the structure describes the entire data, whether it is an array of primiteves, structures or a single value
	/// </summary>
	struct ValueData
	{
		/// <summary>
		/// The internal plugin ID of the parameter the value is obtained for
		/// </summary>
		unsigned pluginId = 0;

		/// <summary>
		/// The internal ID of the parameter the value is obtained for
		/// </summary>
		unsigned paramId = 0;

		/// <summary>
		/// Specifies how many items the value contains, if the data type is a list
		/// </summary>
		unsigned listCount = 0;

		/// <summary>
		/// Specifies the current index of the value entry, if the data type is a list
		/// Typically this is used for string lists, but any data type can be iterated though
		/// </summary>
		unsigned listIndex = 0;

		/// <summary>
		/// Speifies wether the value type is a list of primitives, strucutres or values
		/// This is a shorthand for sqdt == Sqdt::List
		/// </summary>
		unsigned isList = 0;

		/// <summary>
		/// Specifies the number of components of the strucutred data type
		/// for stdt == Array3 => 3
		/// for stdt == Array4 => 4
		/// etc.
		/// This is valud only if rdt == Rdt.Float. All other RDTs have one component only
		/// </summary>
		unsigned components = 0;

		/// <summary>
		/// Specifies the raw data type for the value
		/// </summary>
		Rdt rdt = Rdt::None;

		/// <summary>
		/// Specifies the structured data type for the value
		/// </summary>
		Stdt stdt = Stdt::None;
		
		/// <summary>
		/// Specifies the sequential data type for the value
		/// </summary>
		Sqdt sqdt = Sqdt::None;

		/// <summary>
		/// The raw data pointer. 
		/// Don't mutate this data, as it may be shared among several parameters
		/// </summary>
		const void* data = nullptr;
	};

#pragma pack(pop)

	/// <summary>
	/// A callback to be called when the user requests an uniqe name.
	/// The container allocates the memory necessary for the string and passes that to the user.
	/// Once the call returns the memory cleaned and is no longer available
	/// </summary>
	/// <param name="uniqueName">The unique name that the container has constructed. Must not be freed or changed</param>
	/// <param name="userData">A generic pointer that the user supplied to the original call</param>
	using UniqueNameCallback = void(*)(const char* uniqueName, void* userData);

	/// <summary>
	/// A callback to be called when the user gets a parameter value, or iterates over a set of parameters
	/// The container allocates the memory necessary for the data structure that is passed to the user
	/// Once the call returns the memory cleaned and is no longer available
	/// </summary>
	/// <param name="value">A pointer to the ValueData strucutre, containing details about the parameter value</param>
	/// <param name="userData">A generic pointer that the user supplied to the original call</param>
	using ValueCallback = void(*)(const ValueData* value, void* userData);
}

// all available exports are C exports
extern "C"
{
	/// <summary>
	/// Creates a new data container. Once the user is done with it, DataContainer_Destroy() must be called
	/// </summary>
	/// <returns>A pointer to the allocated data container</returns>
	VRMAT_API void* DataContainer_Create();

	/// <summary>
	/// Destroys an existing data container, created with DataContainer_Create().
	/// Once it is destroyed the pointer is not pointing to a heap memory and shall not be used
	/// </summary>
	/// <param name="dc">The data container to be destroyed</param>
	VRMAT_API void DataContainer_Destroy(const void* dc);

	/// <summary>
	/// Opens an existing .vrmat, .vropt, etc.. file and stores it to an existing data container, created with DataContainer_Create()
	/// The contents of the container is destroyed before loading the new file
	/// </summary>
	/// <param name="dc">A data container to be filled in with the parsed data</param>
	/// <param name="fileName">The file name to read</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_Open(void* dc, const char* fileName);

	/// <summary>
	/// Loads a string of XML into an existing data container, created with DataContainer_Create()
	/// The contents of the container is destroyed before loading the buffer
	/// </summary>
	/// <param name="dc">A data container to be filled in with the parsed data</param>
	/// <param name="buffer">A null-terminated string containing the data to load</param>
	/// <param name="length">The lenght of the string if it is known. Pass 0 to provoke strlen(buffer)</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_Load(void* dc, const char* buffer, int length);

	/// <summary>
	/// Saves the contenst of an existing data container to an XML file.
	/// </summary>
	/// <param name="dc">A data container to be filled in with the parsed data</param>
	/// <param name="fileName">The file write out</param>
	/// <param name="version">The version of the XML layout. Typically 0 to support legacy implementations</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_Save(const void* dc, const char* fileName, int version);

	/// <summary>
	/// Gets the meta for a given element
	/// </summary>
	/// <param name="dc">The data container containing the element to get the meta for</param>
	/// <param name="element">The element id to get the meta for - 0 for the container itself, a plugin id or a parameter id</param>
	/// <param name="meta">A pointer to a Meta structure to mutate</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_GetMeta(const void* dc, unsigned int element, VRMat::Meta* meta);

	/// <summary>
	/// Sets meta for a given element
	/// </summary>
	/// <param name="dc">The data container containing the element to set the meta for</param>
	/// <param name="element">The element id to get the meta for - 0 for the container itself, a plugin id or a parameter id</param>
	/// <param name="meta">A pointer to a Meta structure to get the data from</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_SetMeta(void* dc, unsigned int element, const VRMat::Meta* meta);

	/// <summary>
	/// Acquired an uniqe name for a plugin, taking into account the existing plugins in the container
	/// </summary>
	/// <param name="dc">The data container containing the plugins to account for</param>
	/// <param name="pluginName">A suggested plugin name, used as a template for the unique name. The function will add some suffices to that name if necessary</param>
	/// <param name="resultHandler">A callback to be called passing the generated (or the original) name</param>
	/// <param name="userData">An user supplied value to be passed with the callback</param>
	VRMAT_API void DataContainer_GetUniqueName(const void* dc, const char* pluginName, VRMat::UniqueNameCallback resultHandler, void* userData);

	/// <summary>
	/// Adds a new plugin to the container
	/// </summary>
	/// <param name="dc">The data container to add the plugin to</param>
	/// <param name="name">The name of the plugin. It is user responsibility to assure name uniqueness if required</param>
	/// <param name="type">The type of the plugin - Typically MtlSingleBRDF, VRayMtlBRDF, TexBitmap, etc. but the function will accept any non-null name</param>
	/// <param name="klass">The clas or category of the plugin - Typically material, bitmap, BRDF, light, etc. but the function will accept any non-null type</param>
	/// <returns>The internal id of the newly craeted plugin if successfull or 0 otherwise</returns>
	VRMAT_API unsigned int DataContainer_AddPlugin(void* dc, const char* name, const char* type, const char* klass);

	/// <summary>
	/// Adds a new paramter to a given plugin
	/// </summary>
	/// <param name="dc">The data container containing the plugin to add the parameter to</param>
	/// <param name="pluginId">The internal id of the plugin to add the parameter to</param>
	/// <param name="name">The name of the parameter. Typically an unique name</param>
	/// <param name="type">The type of the parameter. This type shall exactly describe the data the parameter holds</param>
	/// <param name="custom">Specifies whether the parameter is custom or an existing V-Ray plugin parameter</param>
	/// <returns>The internal id of the newly craeted parameter if successfull or 0 otherwise</returns>
	VRMAT_API unsigned int DataContainer_AddParameter(void* dc, unsigned int pluginId, const char* name, const char* type, int custom);

	/// <summary>
	/// Converts a plugin or parameter name into an internal id
	/// </summary>
	/// <param name="dc">The data container containing the plugin or the parameter</param>
	/// <param name="pluginId">If a parameter id is acquired contains the internal plugin id that owns the parameter, otherwise 0</param>
	/// <param name="name">The name of the plugin or parameter to acquire the id of</param>
	/// <returns>The internal id of the plugin or the parameter if successfull or 0 otherwise</returns>
	VRMAT_API unsigned int DataContainer_GetElementId(const void* dc, unsigned int pluginId, const char* name);

	/// <summary>
	/// Deletes an existing plugin or a parameter from the container
	/// </summary>
	/// <param name="dc">The data container containing to delete the element from</param>
	/// <param name="elementId">The internal plugin or parameter id to delete. Shall not be 0</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_RemoveElement(void* dc, unsigned int elementId);

	/// <summary>
	/// Sets a new value to a given parameter
	/// </summary>
	/// <param name="dc">The data container containing the parameter ot set the value of</param>
	/// <param name="paramId">The internal parameter id to set the value of</param>
	/// <param name="data">The actual data to set. The data will be copied and stored in the container.
	/// The data shall be a pointer to a flat array (for structured types) of integers, floats or null-terminated character pointers</param>
	/// <param name="type">A bitwise-OR of the Rdt, Stdt and Sqdt enumerations describing the nature of the data being set</param>
	/// <param name="listLength">The number of (structured or not) list entries in the data, or 0 if the value is a single primitive or structure</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	/// <example>
	/// Setting 2 vectors of 3 componetns shall look like so:
	/// float vec[6] = { x1, y1, z1, x2, y2, z2 };
	/// DataContainer_SetValue(dc, pid, vec, Rdt::Float | Stdt::Array3 | Sqdt::List, _countof(vec)/3);
	/// VRMat::Meta meta;
	/// meta.mask = MetaCategories::ParamType;
	/// meta.paramType = "vector";
	/// DataContainer_SetMeta(dc, pid, &meta);
	/// 
	/// Setting a simple integer would look like:
	/// int i = 128;
	/// DataContainer_SetValue(dc, pid, &i, Rdt::Int | Stdt::Single | Sqdt::Single, 0);
	/// VRMat::Meta meta;
	/// meta.mask = MetaCategories::ParamType;
	/// meta.paramType = "integer";
	/// DataContainer_SetMeta(dc, pid, &meta);
	/// 
	/// Setting a list of strings would look like:
	/// const char lst[]=
	/// {
	///		"item1",
	///		"item2"
	/// };
	/// DataContainer_SetValue(dc, pid, lst, Rdt::String | Stdt::Single | Sqdt::List, _countof(lst));
	/// VRMat::Meta meta;
	/// meta.mask = MetaCategories::ParamType;
	/// meta.paramType = "string";
	/// DataContainer_SetMeta(dc, pid, &meta);
	/// </example>
	VRMAT_API int DataContainer_SetValue(void* dc, unsigned int paramId, const void* data, unsigned int type, int listLength);

	/// <summary>
	/// Gets the value of a given parameter
	/// </summary>
	/// <param name="dc">The data container containing the parameter to get the value of</param>
	/// <param name="paramId">The internal parameter id to get the value of</param>
	/// <param name="valueCallback">A callback to be called passing the allocated ValueData structure</param>
	/// <param name="userData">An user supplied value to be passed with the callback</param>
	/// <returns>Nonzero if successfull, zero otherwise</returns>
	VRMAT_API int DataContainer_GetValue(const void* dc, unsigned int paramId, VRMat::ValueCallback valueCallback, void* userData);

	/// <summary>
	/// Returns a pointer to a new iterator to iterate the container's plugins or a plugin's parameters.
	/// Once the user is done with it, DataContainer_Iterator_Destroy() must be called
	/// </summary>
	/// <param name="dc">The data conatiner that owns the plugins or parameters ot be enumerated</param>
	/// <param name="pluginId">The internal plugin id of the plugin to iterate the parameters of, or 0 if the container's plugins are to be enumerated</param>
	/// <param name="plugin_iterator">An existing plugin iterator to iterate the parameters for, or nullptr if the container's plugins are to be enumerated</param>
	/// <param name="callback">A callback to be called passing the allocated ValueData structure. For plugin iterators the strucutre will have only the pluginId field valid
	/// For parameter iterators all fields are valid</param>
	/// <param name="userData">An user supplied value to be passed with the callback</param>
	/// <returns>A pointer to the newly created iterator, or nullptr if failed</returns>
	VRMAT_API void* DataContainer_Iterator_Create(const void* dc, unsigned int pluginId, const void* plugin_iterator, VRMat::ValueCallback callback, void* userData);

	/// <summary>
	/// Destroys an existing data container iterator, created with DataContainer_Iterator_Create().
	/// Once it is destroyed the pointer is not pointing to a heap memory and shall not be used
	/// </summary>
	/// <param name="it">A pointer to the itertor to be destroyed</param>
	VRMAT_API void DataContainer_Iterator_Destroy(const void* it);

	/// <summary>
	/// Increments an existing plugin or parameter iterator
	/// </summary>
	/// <param name="it">A pointer to the iterator to be incremented</param>
	/// <param name="callback">A callback to be called passing the allocated ValueData structure. For plugin iterators the strucutre will have only the pluginId field valid
	/// For parameter iterators all fields are valid</param>
	/// <param name="userData">An user supplied value to be passed with the callback</param>
	/// <returns>Non-zero if the increment was successfull, 0 if the iterator has reached the iterable's end</returns>
	VRMAT_API int DataContainer_Iterator_Increment(void* it, VRMat::ValueCallback callback, void* userData);
}
