#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "google-default-arguments"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"


class Color;

// TODO: MISSING COLOR CLASS


/**-----------------------------------------------------------------------------
 * Purpose: Simple recursive data access class
 *			Used in vgui for message parameters and resource files
 *			Destructor deletes all child KeyValues nodes
 *			Data is stored in key (string names) - (string/int/float)value pairs called nodes.
 * -----------------------------------------------------------------------------
*/
class KeyValues {
public:
	/**
	 * By default, the KeyValues class uses a string table for the key names that is
	 *	limited to 4MB. The game will exit in error if this space is exhausted. In
	 *	general this is preferable for game code for performance and memory fragmentation
	 *	reasons.
	 *
	 *	If this is not acceptable, you can use this call to switch to a table that can grow
	 *	arbitrarily. This call must be made before any KeyValues objects are allocated or it
	 *	will result in undefined behavior. If you use the growable string table, you cannot
	 *	share KeyValues pointers directly with any other module. You can serialize them across
	 *	module boundaries. These limitations are acceptable in the Steam backend code
	 *	this option was written for, but may not be in other situations. Make sure to
	 *	understand the implications before using this.
	 */
	virtual static void SetUseGrowableStringTable( bool bUseGrowableTable ) = 0;

	virtual KeyValues( const char *setName ) = 0;

	//
	// AutoDelete class to automatically free the keyvalues.
	// Simply construct it with the keyvalues you allocated and it will free them when falls out of scope.
	// When you decide that keyvalues shouldn't be deleted call Assign(NULL) on it.
	// If you constructed AutoDelete(NULL) you can later assign the keyvalues to be deleted with Assign(pKeyValues).
	// You can also pass temporary KeyValues object as an argument to a function by wrapping it into KeyValues::AutoDelete
	// instance:   call_my_function( KeyValues::AutoDelete( new KeyValues( "test" ) ) )
	//
	class AutoDelete
	{
	public:
		explicit inline AutoDelete( KeyValues *pKeyValues ) : m_pKeyValues( pKeyValues ) {}
		explicit inline AutoDelete( const char *pchKVName ) : m_pKeyValues( new KeyValues( pchKVName ) ) {}
		inline ~AutoDelete( void ) { if( m_pKeyValues ) m_pKeyValues->deleteThis(); }
		inline void Assign( KeyValues *pKeyValues ) { m_pKeyValues = pKeyValues; }
		KeyValues *operator->()	{ return m_pKeyValues; }
		operator KeyValues *()	{ return m_pKeyValues; }
	private:
		AutoDelete( AutoDelete const &x ); // forbid
		AutoDelete & operator= ( AutoDelete const &x ); // forbid
		KeyValues *m_pKeyValues;
	};

	// Quick setup constructors
	virtual KeyValues( const char *setName, const char *firstKey, const char *firstValue ) = 0;
	virtual KeyValues( const char *setName, const char *firstKey, const wchar_t *firstValue ) = 0;
	virtual KeyValues( const char *setName, const char *firstKey, int firstValue ) = 0;
	virtual KeyValues( const char *setName, const char *firstKey, const char *firstValue, const char *secondKey, const char *secondValue ) = 0;
	virtual KeyValues( const char *setName, const char *firstKey, int firstValue, const char *secondKey, int secondValue ) = 0;

	// Section name
	virtual const char *GetName() const = 0;
	virtual void SetName( const char *setName) = 0;

	// gets the name as a unique int
	int GetNameSymbol() const { return m_iKeyName; }

	// File access. Set UsesEscapeSequences true, if resource file/buffer uses Escape Sequences (eg \n, \t)
	virtual void UsesEscapeSequences(bool state) = 0; // default false
	virtual void UsesConditionals(bool state) = 0; // default true
	virtual bool LoadFromFile( IBaseFileSystem *filesystem, const char *resourceName, const char *pathID = NULL, bool refreshCache = false ) = 0;
	virtual bool SaveToFile( IBaseFileSystem *filesystem, const char *resourceName, const char *pathID = NULL, bool sortKeys = false, bool bAllowEmptyString = false, bool bCacheResult = false ) = 0;

	// Read from a buffer...  Note that the buffer must be null terminated
	virtual bool LoadFromBuffer( char const *resourceName, const char *pBuffer, IBaseFileSystem* pFileSystem = NULL, const char *pPathID = NULL ) = 0;

	// Read from a utlbuffer...
	virtual bool LoadFromBuffer( char const *resourceName, CUtlBuffer &buf, IBaseFileSystem* pFileSystem = NULL, const char *pPathID = NULL ) = 0;

	// Find a keyValue, create it if it is not found.
	// Set bCreate to true to create the key if it doesn't already exist (which ensures a valid pointer will be returned)
	virtual KeyValues *FindKey(const char *keyName, bool bCreate = false) = 0;
	virtual KeyValues *FindKey(int keySymbol) const = 0;
	virtual KeyValues *CreateNewKey() = 0;		// creates a new key, with an autogenerated name.  name is guaranteed to be an integer, of value 1 higher than the highest other integer key name
	virtual void AddSubKey( KeyValues *pSubkey ) = 0;	// Adds a subkey. Make sure the subkey isn't a child of some other keyvalues
	virtual void RemoveSubKey(KeyValues *subKey) = 0;	// removes a subkey from the list, DOES NOT DELETE IT

	// Key iteration.
	//
	// NOTE: GetFirstSubKey/GetNextKey will iterate keys AND values. Use the functions
	// below if you want to iterate over just the keys or just the values.
	//
	virtual KeyValues *GetFirstSubKey() { return m_pSub; }	// returns the first subkey in the list
	virtual KeyValues *GetNextKey() { return m_pPeer; }		// returns the next subkey
	virtual const KeyValues *GetNextKey() const { return m_pPeer; }		// returns the next subkey

	virtual void SetNextKey( KeyValues * pDat) = 0;
	virtual KeyValues *FindLastSubKey() = 0;	// returns the LAST subkey in the list.  This requires a linked list iteration to find the key.  Returns NULL if we don't have any children

	//
	// These functions can be used to treat it like a true key/values tree instead of
	// confusing values with keys.
	//
	// So if you wanted to iterate all subkeys, then all values, it would look like this:
	//     for ( KeyValues *pKey = pRoot->GetFirstTrueSubKey(); pKey; pKey = pKey->GetNextTrueSubKey() )
	//     {
	//		   Msg( "Key name: %s\n", pKey->GetName() );
	//     }
	//     for ( KeyValues *pValue = pRoot->GetFirstValue(); pKey; pKey = pKey->GetNextValue() )
	//     {
	//         Msg( "Int value: %d\n", pValue->GetInt() );  // Assuming pValue->GetDataType() == TYPE_INT...
	//     }
	virtual KeyValues* GetFirstTrueSubKey() = 0;
	virtual KeyValues* GetNextTrueSubKey() = 0;

	virtual KeyValues* GetFirstValue() = 0;	// When you get a value back, you can use GetX and pass in NULL to get the value.
	virtual KeyValues* GetNextValue() = 0;


	// Data access
	virtual int   GetInt( const char *keyName = NULL, int defaultValue = 0 ) = 0;
	virtual uint64 GetUint64( const char *keyName = NULL, uint64 defaultValue = 0 ) = 0;
	virtual float GetFloat( const char *keyName = NULL, float defaultValue = 0.0f ) = 0;
	virtual const char *GetString( const char *keyName = NULL, const char *defaultValue = "" ) = 0;
	virtual const wchar_t *GetWString( const char *keyName = NULL, const wchar_t *defaultValue = L"" ) = 0;
	virtual void *GetPtr( const char *keyName = NULL, void *defaultValue = (void*)0 ) = 0;
	virtual bool GetBool( const char *keyName = NULL, bool defaultValue = false, bool* optGotDefault = NULL ) = 0;
	virtual Color GetColor( const char *keyName = NULL /* default value is all black */) = 0;
	virtual bool  IsEmpty(const char *keyName = NULL) = 0;

	// Data access
	virtual int   GetInt( int keySymbol, int defaultValue = 0 ) = 0;
	virtual float GetFloat( int keySymbol, float defaultValue = 0.0f ) = 0;
	virtual const char *GetString( int keySymbol, const char *defaultValue = "" ) = 0;
	virtual const wchar_t *GetWString( int keySymbol, const wchar_t *defaultValue = L"" ) = 0;
	virtual void *GetPtr( int keySymbol, void *defaultValue = (void*)0 ) = 0;
	virtual Color GetColor( int keySymbol /* default value is all black */) = 0;
	virtual bool  IsEmpty( int keySymbol ) = 0;

	// Key writing
	virtual void SetWString( const char *keyName, const wchar_t *value ) = 0;
	virtual void SetString( const char *keyName, const char *value ) = 0;
	virtual void SetInt( const char *keyName, int value ) = 0;
	virtual void SetUint64( const char *keyName, uint64 value ) = 0;
	virtual void SetFloat( const char *keyName, float value ) = 0;
	virtual void SetPtr( const char *keyName, void *value ) = 0;
	virtual void SetColor( const char *keyName, Color value) = 0;
	void SetBool( const char *keyName, bool value ) { SetInt( keyName, value ? 1 : 0 ); }

	// Memory allocation (optimized)
	virtual void *operator new( size_t iAllocSize ) = 0;
	virtual void *operator new( size_t iAllocSize, int nBlockUse, const char *pFileName, int nLine ) = 0;
	virtual void operator delete( void *pMem ) = 0;
	virtual void operator delete( void *pMem, int nBlockUse, const char *pFileName, int nLine ) = 0;

	virtual KeyValues& operator=( const KeyValues& src ) = 0;

	// Adds a chain... if we don't find stuff in this keyvalue, we'll look
	// in the one we're chained to.
	virtual void ChainKeyValue( KeyValues* pChain ) = 0;

	virtual void RecursiveSaveToFile( CUtlBuffer& buf, int indentLevel, bool sortKeys = false, bool bAllowEmptyString = false ) = 0;

	virtual bool WriteAsBinary( CUtlBuffer &buffer ) = 0;
	virtual bool ReadAsBinary( CUtlBuffer &buffer, int nStackDepth = 0 ) = 0;

	// Allocate & create a new copy of the keys
	virtual KeyValues *MakeCopy( void ) const = 0;

	// Allocate & create a new copy of the keys, including the next keys. This is useful for top level files
	// that don't use the usual convention of a root key with lots of children (like soundscape files).
	virtual KeyValues *MakeCopy( bool copySiblings ) const = 0;

	// Make a new copy of all subkeys, add them all to the passed-in keyvalues
	virtual void CopySubkeys( KeyValues *pParent ) const = 0;

	// Clear out all subkeys, and the current value
	virtual void Clear( void ) = 0;

	// Data type
	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_NUMTYPES,
	};
	virtual types_t GetDataType(const char *keyName = NULL) = 0;

	// Virtual deletion function - ensures that KeyValues object is deleted from correct heap
	virtual void deleteThis() = 0;

	virtual void SetStringValue( char const *strValue ) = 0;

	// unpack a key values list into a structure
	virtual void UnpackIntoStructure( struct KeyValuesUnpackStructure const *pUnpackTable, void *pDest, size_t DestSizeInBytes ) = 0;

	// Process conditional keys for widescreen support.
	virtual bool ProcessResolutionKeys( const char *pResString ) = 0;

	// Dump keyvalues recursively into a dump context
	virtual bool Dump( class IKeyValuesDumpContext *pDump, int nIndentLevel = 0 ) = 0;

	// Merge in another KeyValues, keeping "our" settings
	virtual void RecursiveMergeKeyValues( KeyValues *baseKV ) = 0;

public:
	// Functions that invoke the default behavior
	virtual static int GetSymbolForStringClassic( const char *name, bool bCreate = true ) = 0;
	virtual static const char *GetStringForSymbolClassic( int symbol ) = 0;

	// Functions that use the growable string table
	virtual static int GetSymbolForStringGrowable( const char *name, bool bCreate = true ) = 0;
	virtual static const char *GetStringForSymbolGrowable( int symbol ) = 0;

	// Functions to get external access to whichever of the above functions we're going to call.
	static int CallGetSymbolForString( const char *name, bool bCreate = true ) { return s_pfGetSymbolForString( name, bCreate ); }
	static const char *CallGetStringForSymbol( int symbol ) { return s_pfGetStringForSymbol( symbol ); }
};


// utility functions

#define FOR_EACH_SUBKEY( kvRoot, kvSubKey ) \
	for ( KeyValues * kvSubKey = kvRoot->GetFirstSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextKey() )

#define FOR_EACH_TRUE_SUBKEY( kvRoot, kvSubKey ) \
	for ( KeyValues * kvSubKey = kvRoot->GetFirstTrueSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextTrueSubKey() )

#define FOR_EACH_VALUE( kvRoot, kvValue ) \
	for ( KeyValues * kvValue = kvRoot->GetFirstValue(); kvValue != NULL; kvValue = kvValue->GetNextValue() )



#pragma clang diagnostic pop