

#include "stdafx.h"
#include <iostream>


UT_SUBSYSTEM(CM_LDAP, "LDAPProxy.cpp");
#define MANAGED_FILE "LDAPProxy.cpp"

#include "LDAPProxy.h"
#include "MixedLDAPDll.h"

#include <vcclr.h>
#include <msclr\auto_gcroot.h>
#include <msclr\lock.h>

ILog* LDAPProxy::pLog_ = nullptr;

using namespace System;
using namespace Systelab::LDAPDllProxy;
using namespace System::Windows::Forms;

stsdLDAPCfg::stsdLDAPCfg()
{
	enabled = false;
	hostname = _T("");
	port = 636;
	pathCertRoot = _T("");
}

class ManagedLDAPClassWrapper
{
public:
	msclr::auto_gcroot<ManagedLDAPClass^> managed;
	Systelab::LDAPDllProxy::ManageLDAPCfg^ ConvertParams(const stsdLDAPCfg& sdLDAPCfg, Dictionary <String^, bool>^& managedGroupMapping, const std::map<wstring, bool>& groupMapping);
	void ConvertManagedUserDataToNative(Dictionary <String^, String^>^ managedUserData, std::map<wstring, wstring>& userData, Dictionary <String^, bool>^& managedGroupMapping, std::map<wstring, bool>& groupMapping);
	wstring FromManagedStringToWstring(String^ managedString);

};

LDAPProxy::LDAPProxy(ILog* pLog)
{
	pLog_ = pLog;
	TRACE_ERR(L"LDAPProxy::LDAPProxy");
	privateWrapper_ = new ManagedLDAPClassWrapper();
	privateWrapper_->managed = gcnew ManagedLDAPClass();
}

LDAPProxy::~LDAPProxy()
{
	TRACE_INT2(L"LDAPProxy::~LDAPProxy");
	delete privateWrapper_;
}

bool LDAPProxy::CheckConnection(wstring& user, wstring& password, const stsdLDAPCfg& sdLDAPCfg, const wstring& sLDAPFilter, int& errorCode)
{
	TRACE_ERR(L"LDAPProxy::CheckConnection IN");
	LDAPStatus logged = LDAPStatus::Generic_Error;
	try
	{
		Dictionary <String^, String^>^ managedUserData = gcnew Dictionary <String^, String^>();
		String^ LDAPFilter = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)sLDAPFilter.c_str()));
		ManageLDAPCfg^ managedLDAPParams = gcnew Systelab::LDAPDllProxy::ManageLDAPCfg();
		managedLDAPParams->hostname = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)sdLDAPCfg.hostname.c_str()));
		managedLDAPParams->port = sdLDAPCfg.port;
		String^ userManaged = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)user.c_str()));
		String^ passManaged = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)password.c_str()));
		logged = privateWrapper_->managed->LDAPLogOn(userManaged, passManaged, managedLDAPParams, managedUserData, LDAPFilter);
		errorCode = logged;
		
		TRACE_ERR(L"LDAPProxy::CheckConnection OUT");
	}
	catch (Exception^ ex)
	{
		System::Windows::Forms::MessageBox::Show(ex->Message);
		TRACE_ERR(L".NET exception from LDAPProxy::CheckConnection");
		TRACE_EX(ex->Message);
		Console::WriteLine(ex->Message);
		return false;
	}
	catch (...) // the report not controlled exceptions
	{
		TRACE_ERR(L"exception from LDAPProxy::CheckConnection");
		Console::WriteLine(L"exception from LDAPProxy::CheckConnection");
		return false;
	}
	return (logged == LDAPStatus::Success ? true : false);
}

bool LDAPProxy::LogOn(wstring& user, wstring& password, const stsdLDAPCfg& sdLDAPCfg, std::map<wstring,wstring> &userData, std::map<wstring, bool>& groupMapping, int& errorCode )
{
	TRACE_ERR(L"LDAPProxy::LogOn IN");
	std::cerr << L"LDAPProxy::LogOn IN\n";
	LDAPStatus logged = LDAPStatus::Generic_Error;
	try
	{
		Dictionary <String^, String^>^ managedUserData = gcnew Dictionary <String^, String^>();
		Dictionary <String^, bool>^ managedGroupMapping = gcnew Dictionary <String^, bool>();
		ManageLDAPCfg^ managedLDAPParams = privateWrapper_->ConvertParams(sdLDAPCfg, managedGroupMapping ,groupMapping );
		String^ userManaged = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)user.c_str()));
		String^ passManaged = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)password.c_str()));
		logged = privateWrapper_->managed->LDAPLogOn(userManaged, passManaged, managedLDAPParams, managedUserData, managedGroupMapping);
		errorCode = logged;
		privateWrapper_->ConvertManagedUserDataToNative(managedUserData, userData, managedGroupMapping, groupMapping);
		TRACE_ERR(L"LDAPProxy::LogOn OUT");
	}
	catch (Exception^ ex)
	{
		System::Windows::Forms::MessageBox::Show(ex->Message);
		TRACE_ERR(L".NET exception from LDAPProxy::LogOn");
		TRACE_EX(ex->Message);
		Console::WriteLine(ex->Message);
		return false;
	}
	catch (...) // the report not controlled exceptions
	{
		TRACE_ERR(L"exception from LDAPProxy::LogOn");
		Console::WriteLine(L"exception from LDAPProxy::LogOn");
		return false;
	}
	return (logged == LDAPStatus::Success ? true : false);
}


void Systelab::LDAPDllProxy::Log(String^ msgManaged, String^ stFile, int line)
{
	if (LDAPProxy::GetLog())
	{
		wstring st1, st2;
		{
			wchar_t* stringPointer = (wchar_t*)Marshal::StringToHGlobalAuto(msgManaged).ToPointer();
			st1 = stringPointer;
			Marshal::FreeHGlobal(IntPtr(stringPointer));
		}
		{
			wchar_t* stringPointer = (wchar_t*)Marshal::StringToHGlobalAuto(stFile).ToPointer();
			st2 = stringPointer;
			Marshal::FreeHGlobal(IntPtr(stringPointer));
		}
		LDAPProxy::GetLog()->Log((_THIS_SUBSYSTEM_), (st1), (st2), (T_ERROR), (line));
	}
}

Systelab::LDAPDllProxy::ManageLDAPCfg^ ManagedLDAPClassWrapper::ConvertParams(const stsdLDAPCfg& sdLDAPCfg, Dictionary <String^, bool>^& managedGroupMapping, const std::map<wstring, bool>& groupMapping)
{
	TRACE_ERR(L"ManagedLDAPClassWrapper::ConvertParams IN");
	Systelab::LDAPDllProxy::ManageLDAPCfg^ LDAPCfg = gcnew Systelab::LDAPDllProxy::ManageLDAPCfg();
	try
	{
		LDAPCfg->hostname = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)sdLDAPCfg.hostname.c_str()));
		LDAPCfg->port = sdLDAPCfg.port;

		for (auto entry : groupMapping)
		{
			String^ filterGroup = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)entry.first.c_str()));
			managedGroupMapping[filterGroup] = entry.second;
		}
		TRACE_ERR(L"ManagedLDAPClassWrapper::ConvertParams OUT");
	}
	catch (Exception^ ex)
	{
		System::Windows::Forms::MessageBox::Show(ex->Message);
		TRACE_ERR(L".NET exception from LDAPProxy::ManagedLDAPClassWrapper::ConvertParams");
		TRACE_EX(ex->Message);
		Console::WriteLine(ex->Message);
	}
	catch (...)
	{
		TRACE_ERR(L"exception from LDAPProxy::ManagedLDAPClassWrapper::ConvertParams");
		Console::WriteLine(L"exception from LDAPProxy::ManagedLDAPClassWrapper::ConvertParams");
	}
	return LDAPCfg;
}

void ManagedLDAPClassWrapper::ConvertManagedUserDataToNative(Dictionary <String^, String^>^ managedUserData, std::map<wstring, wstring>& userData, Dictionary <String^, bool>^& managedGroupMapping, std::map<wstring, bool>& groupMapping)
{
	TRACE_ERR(L"ManagedLDAPClassWrapper::ConvertToNative IN");
	try
	{
		for each (KeyValuePair<String^, String^> ^ entry in managedUserData)
		{
			wstring key = FromManagedStringToWstring(entry->Key->ToString());
			wstring value = FromManagedStringToWstring(entry->Value->ToString());
			userData[key] = value;
		}

		for (auto entry : groupMapping)
		{
			String^ filterGroup = Marshal::PtrToStringAuto((IntPtr)((wchar_t*)entry.first.c_str()));
			bool result = managedGroupMapping[filterGroup];
			groupMapping[entry.first] = result;
		}

		TRACE_ERR(L"ManagedLDAPClassWrapper::ConvertToNative OUT");
	}
	catch (Exception^ ex)
	{
		System::Windows::Forms::MessageBox::Show(ex->Message);
		TRACE_ERR(L".NET exception from ManagedLDAPClassWrapper::ConvertToNative");
		TRACE_EX(ex->Message);
		Console::WriteLine(ex->Message);
	}
	catch (...) // the report not controlled exceptions
	{
		TRACE_ERR(L"exception from ManagedLDAPClassWrapper::ConvertToNative");
		Console::WriteLine(L"exception from ManagedLDAPClassWrapper::ConvertToNative");
	}
}

wstring ManagedLDAPClassWrapper::FromManagedStringToWstring(String^ managedString)
{
	wchar_t* stringPointer = (wchar_t*)Marshal::StringToHGlobalAuto(managedString).ToPointer();
	wstring nativeString = stringPointer;
	// Always free the unmanaged string.
	Marshal::FreeHGlobal(IntPtr(stringPointer));
	return nativeString;
}

