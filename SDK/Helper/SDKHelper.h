#pragma once

#include <inttypes.h>
#include <exception>
#include <GalacticInterfaces.h>
#include <GalacticStrongPtr.h>

#pragma region "Internal"
#if defined(_MSC_VER)
#define DLL_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define DLL_EXPORT __attribute__((visibility("default")))
#else
#define DLL_EXPORT
#endif

class GException : public std::exception
{
public:
	inline GException(Galactic3D::Result Result) :
		m_Result(Result)
	{
	}

	Galactic3D::Result m_Result;

	virtual char const* what() const throw () override
	{
		switch (m_Result)
		{
			case Galactic3D::RESULT_ERROR:
				return "An error occurred.";
			case Galactic3D::RESULT_NOTIMPLEMENTED:
				return "Not implemented.";
			case Galactic3D::RESULT_OUTOFMEMORY:
				return "Out of memory.";
			case Galactic3D::RESULT_BADARGUMENT:
				return "Bad argument.";
			case Galactic3D::RESULT_NOTFOUND:
				return "Not found.";
			case Galactic3D::RESULT_BADPOINTER:
				return "Bad pointer.";
			case Galactic3D::RESULT_BADCALL:
				return "Bad call.";
			case Galactic3D::RESULT_OK:
				return "The operation completed successfully.";
			case Galactic3D::RESULT_FALSE:
				return "False.";
			default:
				break;
		}
		return "An unknown error occurred.";
	}
};

// Handle errors
#define GALACTIC_CALL(x) \
{ \
	Galactic3D::Result r = x; \
	if (Galactic3D::Failed(r)) \
	{ \
		throw GException(r); \
	} \
}

#define SDK_TRY try {
#define SDK_ENDTRY } \
	catch (std::exception& e) \
	{ \
		pState->SetError("%s", e.what()); \
		return false; \
	} \
	catch (std::nullptr_t) \
	{ \
		return false; \
	}
#define SDK_PROPAGATE_ERROR throw nullptr;

class CModule
{
public:
	Galactic3D::Strong<Galactic3D::Interfaces::IModuleInterface> m_pModuleInterface;
	Galactic3D::Strong<Galactic3D::Interfaces::IArgumentFactory> m_pArgumentFactory;

	Galactic3D::Strong<Galactic3D::Interfaces::IScripting> m_pScripting;
	Galactic3D::Strong<Galactic3D::Interfaces::IDefineHandlers> m_pDefineHandlers;

	Galactic3D::Strong<Galactic3D::Interfaces::IReflectedNamespace> m_pNamespace;
};

extern void ModuleRegister();
extern void ModuleUnregister();

static CModule* g_pModule;

inline void* RegisterModule(void*, Galactic3D::Interfaces::IModuleInterface* pModuleInterface, const char* pszModuleName)
{
	CModule* pModule = new CModule;

	try
	{
		GALACTIC_CALL(pModuleInterface->SetInternalName(pszModuleName));

		pModule->m_pModuleInterface = pModuleInterface;

		g_pModule = pModule;

		GALACTIC_CALL(pModuleInterface->CreateInstance(Galactic3D::Interfaces::ID_ArgumentFactory, (void**)&pModule->m_pArgumentFactory));

		GALACTIC_CALL(pModuleInterface->GetScripting(&pModule->m_pScripting));
		GALACTIC_CALL(pModule->m_pScripting->GetDefineHandlers(&pModule->m_pDefineHandlers));

		GALACTIC_CALL(pModuleInterface->GetNamespace(&pModule->m_pNamespace));

		ModuleRegister();

		return pModule;
	}
	catch (GException& e)
	{
		pModuleInterface->SetError(e.what());
	}
	catch (std::nullptr_t)
	{
	}

	delete pModule;

	return nullptr;
}

#define MODULE_MAIN(MODULE_NAME) \
	extern "C" DLL_EXPORT void* RegisterModule(void* p, Galactic3D::Interfaces::IModuleInterface* pModuleInterface) { return RegisterModule(p, pModuleInterface, MODULE_NAME); } \
	extern "C" DLL_EXPORT void UnregisterModule(void* pUser) { ModuleUnregister(); delete (CModule*)pUser; }
#pragma endregion

namespace SDK
{
	inline void RegisterFunction(const char* pszName, Galactic3D::Interfaces::IScriptFunctionCallback pFunction, void* pUser = nullptr)
	{
		GALACTIC_CALL(g_pModule->m_pNamespace->RegisterFunction(pszName, "", pFunction, pUser));
	}

	inline void AddProperty(const char* pszName, Galactic3D::Interfaces::IScriptFunctionCallback pGetter, Galactic3D::Interfaces::IScriptFunctionCallback pSetter = nullptr, void* pUser = nullptr)
	{
		GALACTIC_CALL(g_pModule->m_pNamespace->AddProperty(pUser, pszName, 0, pGetter, pSetter));
	}

	inline void Define(const char* pszName, int32_t Value)
	{
		GALACTIC_CALL(g_pModule->m_pDefineHandlers->DefineSigned(pszName, Value));
	}

	inline void Define(const char* pszName, uint32_t Value)
	{
		GALACTIC_CALL(g_pModule->m_pDefineHandlers->DefineUnsigned(pszName, Value));
	}

	inline void Log(Galactic3D::Interfaces::LogPriority Priority, const char* psz)
	{
		GALACTIC_CALL(g_pModule->m_pModuleInterface->Log((uint32_t)Priority, psz));
	}

	inline void Log(const char* psz)
	{
		Log(Galactic3D::Interfaces::LogPriority::Info, psz);
	}

	inline void Fatal(const char* pszReason)
	{
		g_pModule->m_pModuleInterface->SetError(pszReason);

		throw nullptr;
	}

	class Class
	{
	public:
		inline Class()
		{
		}

		inline Class(const char* pszName, Galactic3D::Interfaces::IReflectedClass* pParent = nullptr)
		{
			GALACTIC_CALL(g_pModule->m_pNamespace->NewClass(&m_pClass, pszName, pParent));
		}

		Galactic3D::Strong<Galactic3D::Interfaces::IReflectedClass> m_pClass;

		inline operator Galactic3D::Interfaces::IReflectedClass*()
		{
			return m_pClass;
		}

		inline void RegisterConstructor(Galactic3D::Interfaces::IScriptFunctionCallback pFunction, void* pUser = nullptr)
		{
			GALACTIC_CALL(m_pClass->RegisterConstructor("", pFunction, pUser));
		}

		inline void RegisterFunction(const char* pszName, Galactic3D::Interfaces::IScriptFunctionCallback pFunction, void* pUser = nullptr)
		{
			GALACTIC_CALL(m_pClass->RegisterFunction(pszName, "", pFunction, pUser));
		}

		inline void AddProperty(const char* pszName, Galactic3D::Interfaces::IScriptFunctionCallback pGetter, Galactic3D::Interfaces::IScriptFunctionCallback pSetter = nullptr, void* pUser = nullptr)
		{
			GALACTIC_CALL(m_pClass->AddProperty(pUser, pszName, 0, pGetter, pSetter));
		}
	};

#pragma region "Argument Values"
	class Argument
	{
	public:
		inline Argument()
		{
		}

		inline Argument(Galactic3D::Interfaces::IArgument* pArgument)
		{
			m_pArgument = pArgument;
		}

		Galactic3D::Strong<Galactic3D::Interfaces::IArgument> m_pArgument;

		inline operator Galactic3D::Interfaces::IArgument*()
		{
			return m_pArgument;
		}
	};

	class NullValue : public Argument
	{
	public:
		inline NullValue()
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateNull(&m_pArgument));
		}
	};

	class NumberValue : public Argument
	{
	public:
		inline NumberValue(int32_t Value)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateInt32(&m_pArgument, (int32_t)Value));
		}

		inline NumberValue(uint32_t Value)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateUInt32(&m_pArgument, (uint32_t)Value));
		}

		inline NumberValue(int64_t Value)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateInt64(&m_pArgument, (int64_t)Value));
		}

		inline NumberValue(uint64_t Value)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateUInt64(&m_pArgument, (uint64_t)Value));
		}

		inline NumberValue(float Value)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateSingle(&m_pArgument, (float)Value));
		}

		inline NumberValue(double Value)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateDouble(&m_pArgument, (double)Value));
		}
	};

	class StringValue : public Argument
	{
	public:
		inline StringValue(const char* psz, size_t Length)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateString(&m_pArgument, psz, Length));
		}

		inline StringValue(const char* psz)
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateStringZ(&m_pArgument, psz));
		}
	};

	class ArrayValue : public Argument
	{
	public:
		inline ArrayValue()
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateArray(&m_pArgument));

			GALACTIC_CALL(m_pArgument->GetInterface(Galactic3D::Interfaces::ID_Array, (void**)&m_pArray));
		}

		Galactic3D::Strong<Galactic3D::Interfaces::IArray> m_pArray;

		inline size_t Count()
		{
			return m_pArray->GetCount();
		}

		inline Argument GetAt(size_t Index)
		{
			Galactic3D::Strong<Galactic3D::Interfaces::IArgument> pArgument;
			GALACTIC_CALL(m_pArray->GetAt(Index, &pArgument));

			return Argument(pArgument);
		}

		inline void Clear()
		{
			GALACTIC_CALL(m_pArray->Clear());
		}

		inline void Insert(Argument& Value)
		{
			GALACTIC_CALL(m_pArray->Insert(Value));
		}
	};

	class DictionaryValue : public Argument
	{
	public:
		inline DictionaryValue()
		{
			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateDictionary(&m_pArgument));

			GALACTIC_CALL(m_pArgument->GetInterface(Galactic3D::Interfaces::ID_Dictionary, (void**)&m_pDictionary));
		}

		Galactic3D::Strong<Galactic3D::Interfaces::IDictionary> m_pDictionary;

		inline void Clear()
		{
			GALACTIC_CALL(m_pDictionary->Clear());
		}

		inline void Remove(const char* pszKey)
		{
			GALACTIC_CALL(m_pDictionary->Remove(pszKey));
		}

		inline Argument Get(const char* pszKey)
		{
			Galactic3D::Strong<Galactic3D::Interfaces::IArgument> pArgument;
			GALACTIC_CALL(m_pDictionary->Get(pszKey, &pArgument));

			return Argument(pArgument);
		}

		inline void Set(const char* pszKey, Galactic3D::Interfaces::IArgument* pValue)
		{
			GALACTIC_CALL(m_pDictionary->Set(pszKey, pValue));
		}
	};

	template<class T, Class& Class> class ClassValue : public Argument
	{
	public:
		inline ClassValue(T* pValue)
		{
			GALACTIC_CALL(g_pModule->m_pModuleInterface->CreateInstance(Galactic3D::Interfaces::ID_BaseObject, (void**)&m_pObject));
			GALACTIC_CALL(m_pObject->AssignClass(Class));
			GALACTIC_CALL(m_pObject->SetPrivate(pValue));
			GALACTIC_CALL(m_pObject->AddDestructor([](Galactic3D::Interfaces::IBaseObject* pObject, void* pUser) {
				delete (T*)pObject->GetPrivate();
			}, nullptr));

			GALACTIC_CALL(g_pModule->m_pArgumentFactory->CreateObject(&m_pArgument, m_pObject));
		}

		Galactic3D::Strong<Galactic3D::Interfaces::IBaseObject> m_pObject;

		inline operator T*()
		{
			return (T*)m_pObject->GetPrivate();
		}

		inline T* operator->()
		{
			return (T*)m_pObject->GetPrivate();
		}
	};
#pragma endregion

	class State
	{
	public:
		inline State(Galactic3D::Interfaces::INativeState* pState)
		{
			m_pNativeState = pState;
		}

		Galactic3D::Strong<Galactic3D::Interfaces::INativeState> m_pNativeState;

		inline operator Galactic3D::Interfaces::INativeState*()
		{
			return m_pNativeState;
		}

		inline Argument GetArgument(size_t Index)
		{
			Galactic3D::Strong<Galactic3D::Interfaces::IArgument> pArgument;
			GALACTIC_CALL(m_pNativeState->GetArgument(Index, &pArgument));

			return Argument(pArgument);
		}

		inline void CheckBoolean(size_t Index, bool& bValue)
		{
			if (!m_pNativeState->CheckBoolean(Index, bValue))
				SDK_PROPAGATE_ERROR;
		}

		inline void CheckNumber(size_t Index, int32_t& Value)
		{
			if (!m_pNativeState->CheckInt32(Index, Value))
				SDK_PROPAGATE_ERROR;
		}

		inline void CheckNumber(size_t Index, uint32_t& Value)
		{
			if (!m_pNativeState->CheckUInt32(Index, Value))
				SDK_PROPAGATE_ERROR;
		}

		inline void CheckNumber(size_t Index, int64_t& Value)
		{
			if (!m_pNativeState->CheckInt64(Index, Value))
				SDK_PROPAGATE_ERROR;
		}

		inline void CheckNumber(size_t Index, uint64_t& Value)
		{
			if (!m_pNativeState->CheckUInt64(Index, Value))
				SDK_PROPAGATE_ERROR;
		}

		inline void CheckNumber(size_t Index, float& Value)
		{
			if (!m_pNativeState->CheckSingle(Index, Value))
				SDK_PROPAGATE_ERROR;
		}

		inline void CheckNumber(size_t Index, double& Value)
		{
			if (!m_pNativeState->CheckDouble(Index, Value))
				SDK_PROPAGATE_ERROR;
		}

		inline const char* CheckString(size_t Index, size_t* pLength = nullptr)
		{
			const char* psz = m_pNativeState->CheckString(Index, pLength);
			if (!psz)
				SDK_PROPAGATE_ERROR;
			return psz;
		}

		template<class T, Class& Class> inline T* CheckThis()
		{
			Galactic3D::Strong<Galactic3D::Interfaces::IBaseObject> pObject;
			if (!m_pNativeState->CheckThis(&pObject, Class.m_pClass))
				SDK_PROPAGATE_ERROR;
			T* p = (T*)pObject->GetPrivate();
			if (p == nullptr)
			{
				m_pNativeState->SetError("%s deleted", Class.m_pClass->GetName());
				SDK_PROPAGATE_ERROR;
			}
			return p;
		}

		inline void Return(Galactic3D::Interfaces::IArgument* pArgument)
		{
			GALACTIC_CALL(m_pNativeState->Return(pArgument));
		}
	};
}
