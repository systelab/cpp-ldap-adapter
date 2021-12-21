#include "stdafx.h"

#pragma unmanaged 
UT_SUBSYSTEM(CM_LDAP, "MixedLDAPDll.cpp");
#pragma managed 
#define MANAGED_FILE "MixedLDAPDll.cpp"

#include "MixedLDAPDll.h"
#include <msclr\lock.h>
#include <array>


using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::DirectoryServices;
using namespace System::Collections::Generic;
using namespace System::Collections;
using namespace System::Text::RegularExpressions;
using namespace System::Threading;
using namespace System::Threading::Tasks;


/*
*	525 - user not found
	52e - invalid credentials
	530 - not permitted to logon at this time
	531 - not permitted to logon at this workstation
	532 - password expired
	533 - account disabled
	534 - The user has not been granted the requested logon type at this machine
	701 - account expired
	773 - user must reset password
	775 - user account locked
*/
namespace Systelab
{
	namespace LDAPDllProxy
	{
		 
		static ManagedLDAPClass::ManagedLDAPClass()
		{
			Systelab::LDAPDllProxy::Log(_T(".NET executing"), MANAGED_FILE, __LINE__);
		}


		Object^ ManagedLDAPClass::singleLogon(Object^ args)
		{
			ManageLDAPSingleLogonCallArguments^ arguments = gcnew ManageLDAPSingleLogonCallArguments();
			ManageLDAPSingleLogonResults^ result = gcnew ManageLDAPSingleLogonResults();
			Dictionary <String^, String^>^ userData = gcnew Dictionary <String^, String^>();
			arguments = (ManageLDAPSingleLogonCallArguments^)args;
			result->ldapFilter = arguments->ldapFilter;
			result->user = arguments->user;
			result->password = arguments->password;
			result->LDAPCfg = arguments->LDAPCfg;
			
			LDAPStatus status = LDAPStatus::Generic_Error;
			status = LDAPLogOn(arguments->user, arguments->password, arguments->LDAPCfg, userData, arguments->ldapFilter);
			if (status == LDAPStatus::Success)
			{
				result->userData = userData;
			}
			result->status = status;
			return result;
		}

		LDAPStatus ManagedLDAPClass::LDAPLogOnParallel(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, Dictionary <String^, bool>^% groupMapping)
		{
			LDAPStatus status = LDAPStatus::Generic_Error;
			List < ManageLDAPSingleLogonCallArguments^>^ LDAPSingleLogonArgsList = gcnew List < ManageLDAPSingleLogonCallArguments^>();
			List<Task^>^ tasks = gcnew List<Task^>();


			for each (KeyValuePair<String^, bool> ^ group in groupMapping)
			{
				ManageLDAPSingleLogonCallArguments^ LDAPSingleLogonArgs = gcnew ManageLDAPSingleLogonCallArguments();
				LDAPSingleLogonArgs->user = user;
				LDAPSingleLogonArgs->password = password;
				LDAPSingleLogonArgs->LDAPCfg = LDAPCfg;
				LDAPSingleLogonArgs->ldapFilter = group->Key;
				LDAPSingleLogonArgsList->Add(LDAPSingleLogonArgs);
				Task<Object^>^ logonTask = Task<Object^>::Factory->StartNew(gcnew Func<Object^, Object^>(this, &ManagedLDAPClass::singleLogon), LDAPSingleLogonArgs);
				tasks->Add(logonTask);
			}

			Task::WaitAll(tasks->ToArray());

			for each (Task<Object^> ^ task in tasks)
			{
				ManageLDAPSingleLogonResults^ result = gcnew ManageLDAPSingleLogonResults();
				result = (ManageLDAPSingleLogonResults^)task->Result;
				status = result->status;
				if (result->status == LDAPStatus::Success)
				{
					userData = result->userData;
					groupMapping[result->ldapFilter] = true;
					break;
				}
				else if (result->status == LDAPStatus::Connection_Error || result->status == LDAPStatus::Configuration_error)
				{
					break;
				}
			}
			return status;
			
		}

		LDAPStatus ManagedLDAPClass::LDAPLogOn(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, Dictionary <String^, bool>^% groupMapping)
		{
			return LDAPLogOnParallel(user, password, LDAPCfg, userData, groupMapping);
		}

		LDAPStatus ManagedLDAPClass::LDAPLogOnNoParallel(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, Dictionary <String^, bool>^% groupMapping)
		{
			LDAPStatus status = LDAPStatus::Generic_Error;;

			for each (KeyValuePair<String^, bool> ^ group in groupMapping)
			{
				status = LDAPLogOn(user, password, LDAPCfg, userData, group->Key);
				if (status == LDAPStatus::Success)
				{
					groupMapping[group->Key] = true;
					break;
				}
				else if (status == LDAPStatus::Connection_Error)
				{
					break;
				}
			}
			return status;
		}

		LDAPStatus ManagedLDAPClass::LDAPLogOn(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, String^ ldapFilter)
		{
			
			Systelab::LDAPDllProxy::Log(_T(".NET executing LDAPLogOn IN") , MANAGED_FILE, __LINE__);
			LDAPStatus status= LDAPStatus::Success;
			try
			{
				String^ filter = "(sAMAccountName=" + user +")";
				IPHostEntry^ hostEntry = Dns::GetHostEntry(LDAPCfg->hostname);
				String^ path = _T("LDAP://") + hostEntry->HostName + _T(":") + LDAPCfg->port;
				DirectoryEntry^ de = gcnew DirectoryEntry(path, user, password, AuthenticationTypes::Secure);
				//DirectorySearcher^ ds = gcnew DirectorySearcher(de, _T("(&(objectCategory=person)(objectClass=user)(sAMAccountName=") + user + _T("))"));
				if (!String::IsNullOrEmpty(ldapFilter))
				{
					filter = "(&(sAMAccountName=" + user+")" +ldapFilter +")";
				}
				DirectorySearcher^ ds = gcnew DirectorySearcher(de, filter);

				SearchResult^ result = ds->FindOne();
				//Systelab::LDAPDllProxy::Log(_T(".NET executing LDAPLogOn result"), MANAGED_FILE, __LINE__);
				if (result != nullptr)
				{
					for each (String ^ props in result->Properties->PropertyNames)
					{
						for each (Object ^ myCollection in result->Properties[props])
						{
							//Systelab::LDAPDllProxy::Log(_T("KEY: ") + props + _T(" value: ")+ myCollection, MANAGED_FILE, __LINE__);
							userData[props->ToLower()] = myCollection->ToString();
						}
					}
					Systelab::LDAPDllProxy::Log(_T(".NET executing LDAPLogOn true OUT"), MANAGED_FILE, __LINE__);
					return status;
				}
				else
				{
					Systelab::LDAPDllProxy::Log(_T(".NET executing LDAPLogOn false OUT"), MANAGED_FILE, __LINE__);
					status = LDAPStatus::User_Data_Error;
					return status;
				}
			}
			catch (ArgumentNullException^ ex)
			{
				Systelab::LDAPDllProxy::Log(_T("Argument Null Exception caught:\n\n") + ex->ToString(), MANAGED_FILE, __LINE__);
				status = LDAPStatus::Generic_Error;
				return status;
			}
			catch (ArgumentOutOfRangeException^ ex)
			{
				Systelab::LDAPDllProxy::Log(_T("Argument more 255 chars Exception caught:\n\n") + ex->ToString(), MANAGED_FILE, __LINE__);
				status = LDAPStatus::Generic_Error;
				return status;
			}
			catch (SocketException^ ex)
			{
				//malformated host
				Systelab::LDAPDllProxy::Log(_T("Socket Exception caught:\n\n") + ex->ToString(), MANAGED_FILE, __LINE__);
				status = LDAPStatus::Connection_Error;
				return status;
			}
			catch (ArgumentException^ ex)
			{
				Systelab::LDAPDllProxy::Log(_T("Error in configuration:\n\n") + ex->ToString(), MANAGED_FILE, __LINE__);
				status = LDAPStatus::Configuration_error;
				return status;
			}
			catch (DirectoryServicesCOMException^ ex)
			{
				//wrong user or password 
				Systelab::LDAPDllProxy::Log(_T("Exception caught: ") + ex->ToString(),MANAGED_FILE, __LINE__);

				cli::array<Char>^ sep = gcnew cli::array<Char>{','};
				cli::array<String^>^ splitted;
				splitted = ex->ExtendedErrorMessage->Split(sep, StringSplitOptions::None);
				String^ error;
				if (splitted->Length - 2 >= 0)
				{
					String^ s1 = splitted[splitted->Length - 2]->Trim(); // This will be " data 701"
					error = s1->Substring(s1->IndexOf(" ")+1);
				}

				Systelab::LDAPDllProxy::Log(_T("Extended Error : ") + error->ToString(), MANAGED_FILE, __LINE__);

				status = LDAPStatus::User_Data_Error;
				return status;
			}
			catch (COMException^ ex)
			{
				//server is not operational (network/no certificate)
				Systelab::LDAPDllProxy::Log(_T("Exception caught:\n\n") + ex->ToString(), MANAGED_FILE, __LINE__);
				status = LDAPStatus::Connection_Error;
				return status;
			}
			catch (Exception^ ex)
			{
				Systelab::LDAPDllProxy::Log(_T("Exception caught:\n\n") + ex->ToString(), MANAGED_FILE, __LINE__);
				status = LDAPStatus::Generic_Error;
				return status;
			}
		}

		
	}
}