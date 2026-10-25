static DWORD cycle_readback(DWORD startup,int candidate)
{
 WCHAR saved[1050];DWORD e;memcpy(saved,native.command,sizeof(saved));
 if(candidate){native.command[0]=0;if(!lab_append(native.command,1050,L"\"")||!lab_append(native.command,1050,cycle_path)||!lab_append(native.command,1050,L"\" service host"))return ERROR_INVALID_DATA;}
 e=ps_virtual_readback_start(startup);
 memcpy(native.command,saved,sizeof(saved));
 if(!e&&native.query.aligned.dwErrorControl!=SERVICE_ERROR_NORMAL)e=ERROR_INVALID_DATA;
 return e;
}
