#pragma once

#include <msclr\lock.h>

using namespace System::Collections::Generic;

using namespace System;
using namespace System::Collections;
using namespace System::IO;

namespace Systelab
{
	namespace LDAPDllProxy
	{
		void Log(String^ msgManaged, String^ stFile, int line);
	}
}

namespace Systelab
{
	namespace LDAPDllProxy
	{
		// enum to inform about the result for the LDAP request			
		public enum LDAPStatus 
		{ 
			Success = 0, 
			Connection_Error = -1, 
			User_Data_Error = -2, 
			Generic_Error = -3,
			Configuration_error = -4
		};
		


		public ref class ManageLDAPCfg
		{
		public:

			String^	hostname;				// The hostname for the LDAP server
			int port = 0;							// The IP port for the LDAP server
			String^ pathCertRoot;

			~ManageLDAPCfg()
			{
				delete hostname;
				delete pathCertRoot;
			}
		};

		public ref class ManageLDAPSingleLogonCallArguments
		{
		public:
			String^ user;
			String^ password;
			ManageLDAPCfg^ LDAPCfg;
			Dictionary <String^, String^>^ userData;
			String^ ldapFilter;

			~ManageLDAPSingleLogonCallArguments()
			{
				delete user;
				delete password;
				delete LDAPCfg;
				delete userData;
				delete ldapFilter;
			}
		};

		public ref class ManageLDAPSingleLogonResults
		{
		public:
			LDAPStatus status;
			String^ user;
			String^ password;
			ManageLDAPCfg^ LDAPCfg;
			Dictionary <String^, String^>^ userData;
			String^ ldapFilter;

			~ManageLDAPSingleLogonResults()
			{
				delete user;
				delete password;
				delete LDAPCfg;
				delete userData;
				delete ldapFilter;
			}
		};

		

		public ref class ManagedLDAPClass /*: public IDisposable*/
		{
		public:
			
			static ManagedLDAPClass();


			/**
			 * This function calls the LDAPLogOn function using a task previously created.
			 * all the task are starting at the same time to make the process in parallel for all the filters.
			 *
			 * @param args Object where all the params to call the LDAPLogon are encapsulated. (see class ManageLDAPSingleLogonCallArguments)
			 * @return Object where the result of the call is encapsulated. (see class ManageLDAPSingleLogonResults).
			 */
			Object^ singleLogon(Object^ args);

			/**
			 * This function calls the singleLogon function through a task. One task for each filter into groupMapping.
			 * all the task are starting at the same time to make the process in parallel for all the filters.
			 *
			 * @param user string with the user to login.
			 * @param password string with the password for the user
			 * @param LDAPCfg Container with the LDAP configuration in order to connect.
			 * @param userData Container to save the user data gathered from the LDAP.
			 * @param groupMapping Container with the LDAP filter for all the groups and a bool to inform about the membership.
			 * @return result of the logon, enum LDAPStatus.
			 */
			LDAPStatus LDAPLogOnParallel(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, Dictionary <String^, bool>^% groupMapping);

			/**
			 * This function calls the logon process directly N times. One call for each filter into groupMapping.
			 *
			 * @param user string with the user to login.
			 * @param password string with the password for the user
			 * @param LDAPCfg Container with the LDAP configuration in order to connect.
			 * @param userData Container to save the user data gathered from the LDAP.
			 * @param groupMapping Container with the LDAP filter for all the groups and a bool to inform about the membership.
			 * @return result of the logon, enum LDAPStatus.
			 */
			LDAPStatus LDAPLogOnNoParallel(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, Dictionary <String^, bool>^% groupMapping);

			/**
			 * This is the entry point from the interface.
			 *			 
			 * @param user string with the user to login.
			 * @param password string with the password for the user
			 * @param LDAPCfg Container with the LDAP configuration in order to connect.
			 * @param userData Container to save the user data gathered from the LDAP.
			 * @param groupMapping Container with the LDAP filter for all the groups and a bool to inform about the membership.
			 * @return result of the logon, enum LDAPStatus.
			 */
			LDAPStatus LDAPLogOn(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, Dictionary <String^, bool>^% groupMapping);

			/**
			 * Carries out the login process using LDAP.
			 *
			 * @param user string with the user to login.
			 * @param password string with the password for the user
			 * @param LDAPCfg Container with the LDAP configuration in order to connect.
			 * @param userData Container to save the user data gathered from the LDAP.
			 * @param ldapFilter string with the LDAP filter.
			 * @return result of the logon, enum LDAPStatus.
			 */
			LDAPStatus LDAPLogOn(String^ user, String^ password, ManageLDAPCfg^ LDAPCfg, Dictionary <String^, String^>^% userData, String^ ldapFilter);
		
			
		};
	}
}
