// diag_acc.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "stdafx.h"
#include "time.h"

//#include <WinBase.h>
#include <Windows.h>
#include <mmsystem.h>
#include <process.h>
#include <conio.h>
#pragma comment(lib, "winmm.lib")

int main()
{
	long end_time, begin_time, diff_time;
	long _idx, _idx2;


	printf("diag_acc\n");
	timeBeginPeriod(1);

	Sleep(500);


	begin_time = GetTickCount();

	Sleep(0);
	end_time = GetTickCount();
	diff_time = end_time - begin_time;
	printf("   0ms begin:%u  end:%u diff:%u\n", begin_time, end_time, diff_time);

	Sleep(1);
	end_time = GetTickCount();
	diff_time = end_time - begin_time;
	printf("   1ms begin:%u  end:%u diff:%u\n", begin_time, end_time, diff_time);

	Sleep(1);
	end_time = GetTickCount();
	diff_time = end_time - begin_time;
	printf("   1ms begin:%u  end:%u diff:%u\n", begin_time, end_time, diff_time);

	Sleep(10);
	end_time = GetTickCount();
	diff_time = end_time - begin_time;
	printf("  10ms begin:%u  end:%u diff:%u\n", begin_time, end_time, diff_time);

	Sleep(100);
	end_time = GetTickCount();
	diff_time = end_time - begin_time;
	printf(" 100ms begin:%u  end:%u diff:%u\n", begin_time, end_time, diff_time);

	Sleep(1000);
	end_time = GetTickCount();
	diff_time = end_time - begin_time;
	printf("1000ms begin:%u  end:%u diff:%u\n", begin_time, end_time, diff_time);


	printf("10x 1000ms\n");
	for (_idx = 0; _idx < 10; _idx++)
	{
		Sleep(1000);
		end_time = GetTickCount();
		diff_time = end_time - begin_time;
		printf("idx:%d 1000ms begin:%u  end:%u diff:%u\n", _idx, begin_time, end_time, diff_time);
	};


	printf("10x 1000*1ms\n");
	for (_idx = 0; _idx < 10; _idx++)
	{
		begin_time = GetTickCount();
		for (_idx2 = 0; _idx2 < 1000; _idx2++)
		{
			Sleep(1);
		};

		end_time = GetTickCount();
		diff_time = end_time - begin_time;
		printf("idx:%d 1000ms begin:%u  end:%u diff:%u\n", _idx, begin_time, end_time, diff_time);
	};

	printf("press any key...\n");

	getch();
    return 0;
}

