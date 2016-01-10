// Test1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ReadWriteBinary.h"

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	ATLASSERT(sizeof(int) == sizeof(DWORD));
	ATLASSERT(sizeof(size_t) == sizeof(DWORD));

	//stackoverflow_20745184::Test1_WriteOneReadOne();
	//stackoverflow_20745184::Test2_WriteMoreReadMore();
	//stackoverflow_20745184::Test3_PrintNext();
	stackoverflow_20745184::Test4_ReadMore();
	//stackoverflow_20745184::Test5_UpdateItem();
	stackoverflow_20745184::Test4_ReadMore();

	return 0;
}

