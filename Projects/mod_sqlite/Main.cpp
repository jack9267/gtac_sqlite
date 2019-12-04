
#include "pch.h"
#include <sqlite3.h>

// The modules internal name (Also used for the namespace name)
MODULE_MAIN("sqlite");

SDK::Class g_DatabaseClass;
SDK::Class g_StatementClass;

class CDatabase
{
public:
	CDatabase(sqlite3* pDB);
	~CDatabase();

	sqlite3* m_pDatabase;

	void Close();
};

CDatabase::CDatabase(sqlite3* pDB) :
	m_pDatabase(pDB)
{
}

CDatabase::~CDatabase()
{
	Close();
}

void CDatabase::Close()
{
	if (m_pDatabase != nullptr)
	{
		sqlite3_close(m_pDatabase);
		m_pDatabase = nullptr;
	}
}

void ModuleRegister()
{
	SDK::Define("SQLITE_INTEGER", SQLITE_INTEGER);
	SDK::Define("SQLITE_FLOAT", SQLITE_FLOAT);
	SDK::Define("SQLITE_BLOB", SQLITE_BLOB);
	SDK::Define("SQLITE_NULL", SQLITE_NULL);
	SDK::Define("SQLITE_TEXT", SQLITE_TEXT);

	g_DatabaseClass = SDK::Class("Database");

	g_DatabaseClass.RegisterConstructor([](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		const char* pszPath = State.CheckString(0);
		sqlite3* pDB;
		int nResult = sqlite3_open(pszPath, &pDB);
		if (nResult != SQLITE_OK)
			return pState->SetError("%s", sqlite3_errstr(nResult));

		SDK::ClassValue<CDatabase, g_DatabaseClass> Value(new CDatabase(pDB));
		State.Return(Value);

		return true;

		SDK_ENDTRY;
	});

	g_DatabaseClass.RegisterFunction("close", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CDatabase, g_DatabaseClass>();

		pThis->Close();

		return true;

		SDK_ENDTRY;
	});

	g_DatabaseClass.RegisterFunction("backup", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CDatabase, g_DatabaseClass>();

		const char* pszFilename = State.CheckString(0);

		sqlite3* pDstDB;
		if (sqlite3_open(pszFilename, &pDstDB) != SQLITE_OK) {
			pState->SetError("%s", sqlite3_errmsg(pDstDB));

			sqlite3_close(pDstDB);

			return false;
		}

		sqlite3_backup* backupDB = sqlite3_backup_init(pDstDB, "main", pThis->m_pDatabase, "main");
		if (backupDB == nullptr) {
			pState->SetError("%s", sqlite3_errmsg(pDstDB));

			sqlite3_close(pDstDB);

			return false;
		}

		int rc = 0;
		do
		{
			rc = sqlite3_backup_step(backupDB, -1);
			if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED)
				sqlite3_sleep(250);
		} while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

		sqlite3_backup_finish(backupDB);

		rc = sqlite3_backup_step(backupDB, -1);

		sqlite3_close(pDstDB);

		if (rc != SQLITE_DONE)
			pState->SetError("%s", sqlite3_errmsg(pDstDB));

		return true;

		SDK_ENDTRY;
	});

	g_DatabaseClass.RegisterFunction("query", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CDatabase, g_DatabaseClass>();

		const char* pszQuery = State.CheckString(0);

		if (pThis->m_pDatabase == nullptr)
			return pState->SetError("database closed");

		sqlite3_stmt* pQuery = NULL;

		const char* pszTail = NULL;
		int i = sqlite3_prepare_v3(pThis->m_pDatabase, (char*)pszQuery, -1, 0, &pQuery, &pszTail);

		if (i == SQLITE_OK)
		{
			i = sqlite3_step(pQuery);

			if (i != SQLITE_OK && i != SQLITE_DONE && i != SQLITE_ROW)
			{
				pState->SetError("%s", sqlite3_errmsg(pThis->m_pDatabase));

				// Query failed
				sqlite3_finalize(pQuery);

				return false;
			}

			if (i == SQLITE_OK)
			{
				// Done query that does not return anything
				sqlite3_finalize(pQuery);

				SDK::NullValue Result;
				State.Return(Result);

				return true;
			}
		}
		else
		{
			return pState->SetError("%s", sqlite3_errmsg(pThis->m_pDatabase));
		}

		int Count = sqlite3_column_count(pQuery);

		SDK::ArrayValue Result;
		for (int i = 0; i < Count; i++)
		{
			switch (sqlite3_column_type(pQuery, i))
			{
				case SQLITE_INTEGER:
					{
						SDK::NumberValue Value(sqlite3_column_int(pQuery, i));
						Result.Insert(Value);
					}
					break;
				case SQLITE_FLOAT:
					{
						SDK::NumberValue Value(sqlite3_column_double(pQuery, i));
						Result.Insert(Value);
					}
					break;
				case SQLITE_TEXT:
					{
						SDK::StringValue Value((const char*)sqlite3_column_text(pQuery, i));
						Result.Insert(Value);
					}
					break;
				default:
					{
						SDK::NullValue Value;
						Result.Insert(Value);
					}
					break;
			}
		}

		sqlite3_finalize(pQuery);

		State.Return(Result);

		return true;

		SDK_ENDTRY;
	});
}

void ModuleUnregister()
{
}
