#pragma once

#include <inttypes.h>
#include "GalacticUUID.h"
#include "GalacticResult.h"

#ifdef _WIN32
#define GALACTIC_NOVTABLE __declspec(novtable)
#else
#define GALACTIC_NOVTABLE
#endif

namespace Galactic3D
{
	namespace Interfaces
	{
		// The current api version.
		const uint32_t APIMajorVersion = 1;
		const uint32_t APIMinorVersion = 1;

		/* {f854dda8-fd5a-4638-b391-71fd58e58d57} */
		const tUUID ID_ModuleInterface = DefineUUID(0xf854dda8, 0xfd5a, 0x4638, 0xb3, 0x91, 0x71, 0xfd, 0x58, 0xe5, 0x8d, 0x57);

		/* {0fd1b382-aca5-4434-bf10-0bbaa24d1f0f} */
		const tUUID ID_RefCounted = DefineUUID(0x0fd1b382, 0xaca5, 0x4434, 0xbf, 0x10, 0x0b, 0xba, 0xa2, 0x4d, 0x1f, 0x0f);

		/* {8d078537-e58f-4373-8487-5d2d09e67415} */
		const tUUID ID_BaseObject = DefineUUID(0x8d078537, 0xe58f, 0x4373, 0x84, 0x87, 0x5d, 0x2d, 0x09, 0xe6, 0x74, 0x15);

		/* {f3903bdb-425e-4b9d-ac1b-6b10c502bf63} */
		const tUUID ID_Argument = DefineUUID(0xf3903bdb, 0x425e, 0x4b9d, 0xac, 0x1b, 0x6b, 0x10, 0xc5, 0x02, 0xbf, 0x63);

		/* {b82bef65-9b8a-401e-93df-aef05da65e8a} */
		const tUUID ID_Array = DefineUUID(0xb82bef65, 0x9b8a, 0x401e, 0x93, 0xdf, 0xae, 0xf0, 0x5d, 0xa6, 0x5e, 0x8a);

		/* {d2218297-376a-46d5-9384-fc43bf3b6634} */
		const tUUID ID_Dictionary = DefineUUID(0xd2218297, 0x376a, 0x46d5, 0x93, 0x84, 0xfc, 0x43, 0xbf, 0x3b, 0x66, 0x34);

		/* {71b5c863-8985-4a8c-bc68-f5264aee78f9} */
		const tUUID ID_NativeState = DefineUUID(0x71b5c863, 0x8985, 0x4a8c, 0xbc, 0x68, 0xf5, 0x26, 0x4a, 0xee, 0x78, 0xf9);

		/* {7236c5a7-6fe2-4607-b707-9dbf401124af} */
		const tUUID ID_ReflectedNamespace = DefineUUID(0x7236c5a7, 0x6fe2, 0x4607, 0xb7, 0x07, 0x9d, 0xbf, 0x40, 0x11, 0x24, 0xaf);

		/* {0ba8f85d-9886-40ff-8cf0-90cb1f9a6d8a} */
		const tUUID ID_ReflectedClass = DefineUUID(0x0ba8f85d, 0x9886, 0x40ff, 0x8c, 0xf0, 0x90, 0xcb, 0x1f, 0x9a, 0x6d, 0x8a);

		/* {c12d55c6-5c90-41a9-aa43-5ee2715c40a5} */
		const tUUID ID_Scripting = DefineUUID(0xc12d55c6, 0x5c90, 0x41a9, 0xaa, 0x43, 0x5e, 0xe2, 0x71, 0x5c, 0x40, 0xa5);

		/* {36d26f66-3e21-4251-858b-91d01dcb9cae} */
		const tUUID ID_Stream = DefineUUID(0x36d26f66, 0x3e21, 0x4251, 0x85, 0x8b, 0x91, 0xd0, 0x1d, 0xcb, 0x9c, 0xae);

		/* {79de2284-9ec8-4e74-bacc-d74239344f73} */
		const tUUID ID_ReturnValue = DefineUUID(0x79de2284, 0x9ec8, 0x4e74, 0xba, 0xcc, 0xd7, 0x42, 0x39, 0x34, 0x4f, 0x73);

		/* {4bcab029-1f06-4d41-9625-21d27750b501} */
		const tUUID ID_Function = DefineUUID(0x4bcab029, 0x1f06, 0x4d41, 0x96, 0x25, 0x21, 0xd2, 0x77, 0x50, 0xb5, 0x01);

		/* {4ac30720-6d9f-4dfd-bc80-6d6a023dd0d1} */
		const tUUID ID_ArgumentFactory = DefineUUID(0x4ac30720, 0x6d9f, 0x4dfd, 0xbc, 0x80, 0x6d, 0x6a, 0x02, 0x3d, 0xd0, 0xd1);

		/* {0562748a-7392-4489-b2f5-1060327fa2c8} */
		const tUUID ID_DefineHandlers = DefineUUID(0x0562748a, 0x7392, 0x4489, 0xb2, 0xf5, 0x10, 0x60, 0x32, 0x7f, 0xa2, 0xc8);

		class IBaseObject;
		class IReflectedClass;
		class INativeState;
		class IFunction;
		class IStream;
		class IDefineHandlers;

		typedef void(*IDestructorProc)(IBaseObject* pObject, void* pUser);
		typedef bool(*IScriptFunctionCallback) (INativeState* pState, int32_t argc, void* pUser);

		class GALACTIC_NOVTABLE IUnknown
		{
		public:
			virtual Result GetInterface(const tUUID& ID, void** ppObject) = 0;
			virtual uint32_t AddRef() = 0;
			virtual uint32_t Release() = 0;
		};

		class GALACTIC_NOVTABLE IBaseObject : public IUnknown
		{
		public:
			virtual Result SetPrivate(void* pPrivate) = 0;
			virtual void* GetPrivate() = 0;
			virtual Result IsA(IReflectedClass* pClass) = 0;
			virtual Result AddDestructor(IDestructorProc Callback, void* pUser) = 0;
			virtual Result AssignClass(IReflectedClass* pClass) = 0;
			virtual Result GetReflectedClass(IReflectedClass** ppClass) = 0;
		};

		class GALACTIC_NOVTABLE IArgument : public IUnknown
		{
		public:
			virtual Result Clone(IArgument** ppArgument) = 0;
			virtual uint8_t GetType() = 0;
			virtual const char* GetTypeName() = 0;
			virtual bool IsNull() = 0;
			virtual bool IsBoolean() = 0;
			virtual bool IsString() = 0;
			virtual bool IsObject() = 0;
			virtual bool IsVector2D() = 0;
			virtual bool IsVector3D() = 0;
			virtual bool IsVector4D() = 0;
			virtual bool IsMatrix4x4() = 0;
			virtual bool IsNumber() = 0;
			virtual bool IsArray() = 0;
			virtual bool IsDictionary() = 0;
			virtual bool IsFunction() = 0;
			virtual Result ToBoolean() = 0;
			virtual Result ToString(const char** ppsz, size_t* pLength) = 0;
			virtual Result ToObject(IBaseObject** ppObject, IReflectedClass* pClass = nullptr) = 0;
			virtual Result ToVector2D(float& x, float& y) = 0;
			virtual Result ToVector3D(float& x, float& y, float& z) = 0;
			virtual Result ToVector4D(float& x, float& y, float& z, float& w) = 0;
			virtual Result ToMatrix4x4(float* prgfRowMajorEntries) = 0;
			virtual Result ToInt32(int32_t& Value) = 0;
			virtual Result ToUInt32(uint32_t& Value) = 0;
			virtual Result ToInt64(int64_t& Value) = 0;
			virtual Result ToUInt64(uint64_t& Value) = 0;
			virtual Result ToSingle(float& Value) = 0;
			virtual Result ToDouble(double& Value) = 0;
			virtual Result ToFunction(IFunction** ppFunction) = 0;
			virtual Result ToStream(IStream** ppStream) = 0;
		};

		class GALACTIC_NOVTABLE IArray : public IUnknown
		{
		public:
			virtual size_t GetCount() = 0;
			virtual Result GetAt(size_t Index, IArgument** ppArgument) = 0;
			virtual Result Clear() = 0;
			virtual Result Resize(size_t Size) = 0; // Will clear
			virtual Result Insert(IArgument* pArgument) = 0;
			virtual Result Move(IArray* pTargetArray) = 0;
		};

		class GALACTIC_NOVTABLE IDictionary : public IUnknown
		{
		public:
			virtual Result Clear() = 0;
			virtual Result Resize(size_t Size) = 0; // Will clear
			virtual Result Remove(const char* pszKey) = 0;
			virtual Result Get(const char* pszKey, IArgument** ppArgument) = 0;
			virtual Result Set(const char* pszKey, IArgument* pArgument) = 0;
		};

		class GALACTIC_NOVTABLE INativeState : public IUnknown
		{
		public:
			virtual bool SetError(const char* pszFormat, ...) = 0;
			virtual Result GetArgument(size_t Index, IArgument** ppArgument) = 0;
			virtual bool CheckBoolean(size_t Index, bool& bValue) = 0;
			virtual bool CheckInt32(size_t Index, int32_t& Value) = 0;
			virtual bool CheckUInt32(size_t Index, uint32_t& Value) = 0;
			virtual bool CheckInt64(size_t Index, int64_t& Value) = 0;
			virtual bool CheckUInt64(size_t Index, uint64_t& Value) = 0;
			virtual bool CheckSingle(size_t Index, float& Value) = 0;
			virtual bool CheckDouble(size_t Index, double& Value) = 0;
			virtual const char* CheckString(size_t Index, size_t* pLength = nullptr) = 0;
			virtual bool CheckVector2D(size_t Index, float& x, float& y) = 0;
			virtual bool CheckVector3D(size_t Index, float& x, float& y, float& z) = 0;
			virtual bool CheckVector4D(size_t Index, float& x, float& y, float& z, float& w) = 0;
			virtual bool CheckMatrix4x4(size_t Index, float* prgfRowMajorEntries) = 0;
			virtual bool CheckClass(IReflectedClass* pClass, size_t Index, bool bNull, IBaseObject** ppBaseObject) = 0;
			virtual bool CheckObject(size_t Index, IBaseObject** ppObject) = 0;
			virtual bool CheckFunction(size_t Index, IFunction** ppFunction) = 0;
			virtual bool CheckStream(size_t Index, IStream** ppStream) = 0;
			virtual bool CheckThis(IBaseObject** ppObject, IReflectedClass* pClass = nullptr) = 0;
			virtual Result GetThis(IBaseObject** ppObject, IReflectedClass* pClass = nullptr) = 0;
			virtual Result Return(IArgument* pArgument) = 0;
			virtual Result ReturnNull() = 0;
			virtual Result ReturnUndefined() = 0;
			virtual Result ReturnBoolean(bool b) = 0;
			virtual Result ReturnString(const char* psz, size_t Length) = 0;
			virtual Result ReturnStringZ(const char* psz) = 0;
			virtual Result ReturnInt32(int32_t Value) = 0;
			virtual Result ReturnUInt32(uint32_t Value) = 0;
			virtual Result ReturnInt64(int64_t Value) = 0;
			virtual Result ReturnUInt64(uint64_t Value) = 0;
			virtual Result ReturnSingle(float Value) = 0;
			virtual Result ReturnDouble(double Value) = 0;
			virtual Result ReturnVector2D(float x, float y) = 0;
			virtual Result ReturnVector3D(float x, float y, float z) = 0;
			virtual Result ReturnVector4D(float x, float y, float z, float w) = 0;
			virtual Result ReturnMatrix4x4(const float* prgfRowMajorEntries) = 0;
			virtual Result ReturnObject(IBaseObject* pObject) = 0;
			virtual Result ReturnStream(IStream* pStream) = 0;
		};

		class GALACTIC_NOVTABLE IReflectedNamespace : public IUnknown
		{
		public:
			virtual Result Clear() = 0;
			virtual Result GetNamespace(const char* pszName, IReflectedNamespace** ppNamespace) = 0;
			virtual Result AddNamespace(const char* pszName, IReflectedNamespace** ppNamespace) = 0;
			virtual Result RegisterFunction(const char* pszName, const char* pszMask, IScriptFunctionCallback Callback, void* pUser = nullptr) = 0;
			virtual Result AddProperty(void* pUser, const char* pszName, uint8_t Type, IScriptFunctionCallback pGetter, IScriptFunctionCallback pSetter = nullptr) = 0;
			virtual Result NewClass(IReflectedClass** ppClass, const char* pszName, IReflectedClass* pParent = nullptr) = 0;
			virtual const char* GetName() = 0;
		};

		class GALACTIC_NOVTABLE IReflectedClass : public IUnknown
		{
		public:
			virtual const char* GetName() = 0;
			virtual Result Clear() = 0;
			virtual Result IsA(IReflectedClass* pOtherClass) = 0;
			virtual Result RegisterConstructor(const char* pszMask, IScriptFunctionCallback pFunction, void* pUser = nullptr) = 0;
			virtual Result RegisterClone(IScriptFunctionCallback pFunction, void* pUser = nullptr) = 0;
			virtual Result RegisterMove(IScriptFunctionCallback pFunction, void* pUser = nullptr) = 0;
			virtual Result RegisterToString(IScriptFunctionCallback pFunction, void* pUser = nullptr) = 0;
			virtual Result RegisterFunction(const char* pszName, const char* pszMask, IScriptFunctionCallback pFunction, void* pUser = nullptr) = 0;
			virtual Result AddProperty(void* pUser, const char* pszName, uint8_t Type, IScriptFunctionCallback pGetter, IScriptFunctionCallback pSetter = nullptr) = 0;
		};

		class GALACTIC_NOVTABLE IScripting : public IUnknown
		{
		public:
			virtual Result GetGlobal(IReflectedNamespace** ppNamespace) = 0;
			virtual Result CollectGarbage() = 0;
			virtual Result GetDefineHandlers(IDefineHandlers** ppDefineHandlers) = 0;
		};

		class GALACTIC_NOVTABLE IStream : public IUnknown
		{
		public:
		};

		class GALACTIC_NOVTABLE IFunction : public IUnknown
		{
		public:
			virtual Result Call(IArray* pArguments = nullptr, IArray* pReturnArguments = nullptr) = 0;
		};

		class GALACTIC_NOVTABLE IArgumentFactory : public IUnknown
		{
		public:
			virtual Result CreateNull(IArgument** ppArgument) = 0;
			virtual Result CreateBoolean(IArgument** ppArgument, bool b) = 0;
			virtual Result CreateString(IArgument** ppArgument, const char* psz, size_t Length) = 0;
			virtual Result CreateStringZ(IArgument** ppArgument, const char* psz) = 0;
			virtual Result CreateObject(IArgument** ppArgument, IBaseObject* pObject) = 0;
			virtual Result CreateVector2D(IArgument** ppArgument, float x, float y) = 0;
			virtual Result CreateVector3D(IArgument** ppArgument, float x, float y, float z) = 0;
			virtual Result CreateVector4D(IArgument** ppArgument, float x, float y, float z, float w) = 0;
			virtual Result CreateMatrix4x4(IArgument** ppArgument, const float* prgfRowMajorEntries) = 0;
			virtual Result CreateInt32(IArgument** ppArgument, int32_t Value) = 0;
			virtual Result CreateUInt32(IArgument** ppArgument, uint32_t Value) = 0;
			virtual Result CreateInt64(IArgument** ppArgument, int64_t Value) = 0;
			virtual Result CreateUInt64(IArgument** ppArgument, uint64_t Value) = 0;
			virtual Result CreateSingle(IArgument** ppArgument, float Value) = 0;
			virtual Result CreateDouble(IArgument** ppArgument, double Value) = 0;
			virtual Result CreateArray(IArgument** ppArgument) = 0;
			virtual Result CreateDictionary(IArgument** ppArgument) = 0;
			virtual Result CreateUndefined(IArgument** ppArgument) = 0;
			virtual Result CreateFunction(IArgument** ppArgument, IScripting* pScripting, IScriptFunctionCallback Callback, void* pUser = nullptr) = 0;
		};

		class GALACTIC_NOVTABLE IDefineHandlers : public IUnknown
		{
		public:
			virtual Result DefineSigned(const char* pszName, int32_t Value) = 0;
			virtual Result DefineUnsigned(const char* pszName, uint32_t Value) = 0;
		};

		enum class LogPriority : uint32_t
		{
			Verbose = 1,
			Debug,
			Info,
			Warn,
			Error,
			Critical,
		};

		class GALACTIC_NOVTABLE IModuleInterface : public IUnknown
		{
		public:
			virtual void GetAPIVersion(uint32_t& MajorVersion, uint32_t& MinorVersion) = 0;
			virtual Result CreateInstance(const tUUID& ID, void** ppObject) = 0;
			virtual Result GetScripting(IScripting** ppScripting) = 0;
			virtual Result GetNamespace(IReflectedNamespace** ppNamespace) = 0;
			virtual Result SetInternalName(const char* pszName) = 0;
			virtual Result Log(uint32_t Priority, const char* psz) = 0;
			virtual void SetError(const char* psz) = 0; // Thread local
			virtual const char* GetError() = 0; // Thread local
		};
	}
}
