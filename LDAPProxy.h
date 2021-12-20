/*
$Workfile: /TOP/CM/SW/LDAPProxy/LDAPProxy.h jmruiz $

DESCRIPTION:		Interface for the LDAPProxy class
NOTICE:				Copyright 2020, Werfen Clinical Software, S.A.
REVISION HISTORY:

$Log: LDAPProxy.h $
Headers

*/

#pragma once

#include "stdafx.h"
//#include "stDefinitions.h"



class ManagedLDAPClassWrapper;
class ILog;

#ifdef NATIVEDLL_EXPORTS
#define NATIVEDLL_API __declspec(dllexport)
#else
#define NATIVEDLL_API __declspec(dllimport)
#endif


struct NATIVEDLL_API stsdLDAPCfg
{
	stsdLDAPCfg();

	bool			enabled;				// LDAP authentication enabled/disabled

	std::wstring	hostname;				// The hostname for the LDAP server
	unsigned long	port;					// The IP port for the LDAP server

	std::wstring	pathCertRoot;			// The path for the CA certificate

	std::wstring	chainAdminGroup;			// The chain to map the LDAP Admin Group

	std::wstring	chainSupervisorGroup;		// The chain to map the LDAP Supervisor Group

	std::wstring	chainSeniorGroup;			// The chain to map the LDAP Senior Operator Group

	std::wstring	chainOperatorGroup;			// The chain to map the LDAP Operator Group

	std::wstring	chainAuthSignerGroup;			// The chain to map the LDAP AuthSigner Group

	bool operator == (const stsdLDAPCfg& obj) const
	{
		return
			enabled == obj.enabled &&
			hostname == obj.hostname &&
			port == obj.port &&
			pathCertRoot == obj.pathCertRoot &&
			chainAdminGroup == obj.chainAdminGroup &&
			chainSupervisorGroup == obj.chainSupervisorGroup &&
			chainSeniorGroup == obj.chainSeniorGroup &&
			chainOperatorGroup == obj.chainOperatorGroup &&
			chainAuthSignerGroup == obj.chainAuthSignerGroup;
	}
};


class NATIVEDLL_API LDAPProxy
{
public:
	LDAPProxy(ILog* pLog);
	virtual ~LDAPProxy();

	/**
	 * Call the logon function into the DLL. 
	 * Interface between native code and manage code.
	 *
	 * @param user string with the user to login.
	 * @param password string with the password for the user
	 * @param sdLDAPCfg Container with the LDAP configuration in order to connect.
	 * @param userData Container to save the user data gathered from the LDAP.
	 * @param groupMapping Container with the groups and a bool to save if the user belongs to this group or not.
	 * @param errorCode int with the result of the LDAP process. 
	 * @return result of the logon, true for success. false for failure.
	 */
	bool LogOn(wstring& user, wstring& password, const stsdLDAPCfg& sdLDAPCfg, std::map<wstring, wstring>& userData, std::map<wstring, bool>& groupMapping, int& errorCode);

	bool CheckConnection(wstring& user, wstring& password, const stsdLDAPCfg& sdLDAPCfg, const wstring& sLDAPFilter, int& errorCode);
	
	static ILog* GetLog() { return pLog_; }

private:
	ManagedLDAPClassWrapper* privateWrapper_;
	static ILog* pLog_;
};