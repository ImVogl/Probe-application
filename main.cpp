#include <string>
#include <windows.h>
#include "Header.h"

#define ID_B_OK		100
#define ID_B_Q		101
#define ID_RB_V     102
#define ID_RB_C     103

#define Q_d			1.60218e-19

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int RegClass(WNDPROC, LPCTSTR, HBRUSH);
int read_data(wchar_t[], int[]);
int cut_arr(double[], int, double[], int);
double root_of_equation(double, double, double, double, double, int);

const int NoCh = 5;
const int new_size = 250;

HINSTANCE hInst;
char szClassName[] = "mainClass";
std::string num[6] = { "I_1", "I_2", "I_3" , "V12" , "V13" , "NC" };
// Îñíîâíûå ôóíêöèè
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg; HWND hwnd;
	hInst = hInstance;
	char szTitle[] = "AfP";
	if (!RegClass(WndProc, (LPCTSTR)szClassName, (HBRUSH)(COLOR_WINDOW + 0)))
		return FALSE;
	hwnd = CreateWindowA(szClassName, (LPCSTR)szTitle, WS_VISIBLE,
		50, 50, 545, 218, 0, 0, hInstance, NULL);
	// DWORD ERR = GetLastError();
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	if (!hwnd)
	{
		MessageBoxA(hwnd, "Can't create window!", "Warning!", MB_OK);
		return FALSE;
	}
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);				// Äàííàÿ ôóíêöèÿ íóæíà äëÿ òîãî, ÷òîáû ìîæíî áûëî ðàáîòàòü ñ ñèìâîëüíûìè ñîîáùåíèÿìè
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
// Register class function
int RegClass(WNDPROC proc, LPCTSTR szName, HBRUSH CWin)
{
	WNDCLASS WC;
	WC.style = CS_VREDRAW | CS_HREDRAW;
	WC.cbClsExtra = 0;
	WC.cbWndExtra = 0;
	WC.lpfnWndProc = proc;
	WC.hInstance = hInst;
	WC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WC.hCursor = LoadCursor(NULL, IDC_ARROW);
	WC.hbrBackground = CWin;
	WC.lpszMenuName = (LPCTSTR)NULL;
	WC.lpszClassName = szName;
	return RegisterClass(&WC); // Åñëè êëàññ çàðåãèñòðèðîâàí òî ýòî çíà÷åíèå íåíóëåâîå.
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hedit[12], hData_e, hOutput_e, hLabel_s, hMult, hEDT, hERes[3], hESQ, hEM, hENoB;
	static HWND  hLabel_o, hLabel[6], hLabel_d, hLabT, hACC[2], hOp[2], hLStat, hCh, hLRes, hLSQ, hLM, hLNoB;
	static HWND hOK, hClose, hRad[2];
	static wchar_t Out[1024];
	static wchar_t Data[1024];
	static char CH_E[12];
	static std::string BUF_S;
	static double T_root, accur, a, b, c, k_M[6];
	static int N_op, Numb_max;
	static RECT RC;
	static LPCSTR N_o_Ch[6] = {"1", "2", "3", "4", "5", "6"};
	static int radio_stat; // -1 is current mode; 1 is voltage mode
	switch (msg)
	{
	case WM_CREATE:
	{
		GetWindowRect(hwnd, &RC);
		for(int i = 0; i < 6; i++)
		{
			hLabel[i] = CreateWindowA("static", num[i].c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				50 + i * 35, 60, 30, 20, hwnd, NULL, hInst, NULL);
			hedit[i] = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				50 + i * 35, 85, 30, 20, hwnd, NULL, hInst, NULL);
			hedit[i + 6] = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				50 + i * 35, 110, 30, 20, hwnd, NULL, hInst, NULL);
		}
		for (int i = 0; i < 3; i++)
			hERes[i] = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				70 + i * 70, 135, 45, 20, hwnd, NULL, hInst, NULL);
		hCh = CreateWindowA("static", "Ch", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			10, 85, 35, 20, hwnd, NULL, hInst, NULL);
		hMult = CreateWindowA("static", "Mult", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			10, 110, 35, 20, hwnd, NULL, hInst, NULL);
		hOutput_e = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			160, 10, 250, 20, hwnd, NULL, hInst, NULL);
		hData_e = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			160, 35, 250, 20, hwnd, NULL, hInst, NULL);
		hEDT = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			265, 85, 85, 20, hwnd, NULL, hInst, NULL);
		hESQ = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			265, 135, 85, 20, hwnd, NULL, hInst, NULL);
		hEM = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			360, 135, 80, 20, hwnd, NULL, hInst, NULL);
		hACC[0] = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			360, 85, 80, 20, hwnd, NULL, hInst, NULL);
		hOp[0] = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			450, 85, 70, 20, hwnd, NULL, hInst, NULL);
		hENoB = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			450, 135, 70, 20, hwnd, NULL, hInst, NULL);
		hLabel_o = CreateWindowA("static", "Way to the output file", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			10, 10, 150, 20, hwnd, NULL, hInst, NULL);
		hLabel_d = CreateWindowA("static", "Way to the data file", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			10, 35, 150, 20, hwnd, NULL, hInst, NULL);
		hLabT = CreateWindowA("static", "T_approx, K", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			265, 60, 85, 20, hwnd, NULL, hInst, NULL);
		hLSQ = CreateWindowA("static", "Square, m^2", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			265, 110, 85, 20, hwnd, NULL, hInst, NULL);
		hLM = CreateWindowA("static", "Mass, kg", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			360, 110, 80, 20, hwnd, NULL, hInst, NULL);
		hACC[1] = CreateWindowA("static", "Accuraty, K", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			360, 60, 80, 20, hwnd, NULL, hInst, NULL);
		hOp[1] = CreateWindowA("static", "Iter numb", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			450, 60, 70, 20, hwnd, NULL, hInst, NULL);
		hLNoB = CreateWindowA("static", "Max points", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			450, 60, 70, 20, hwnd, NULL, hInst, NULL);
		hLStat = CreateWindowA("static", "Complete", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			RC.right - 136, RC.bottom - 108, 70, 20, hwnd, NULL, hInst, NULL);
		hLRes = CreateWindowA("static", "R, Om", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			10, 135, 50, 20, hwnd, NULL, hInst, NULL);
		hLNoB = CreateWindowA("static", "BV", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			450, 110, 70, 20, hwnd, NULL, hInst, NULL);
		hOK = CreateWindowA("button", "Calculate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420, 10, 100, 20, hwnd, (HMENU)ID_B_OK, hInst, NULL);
		hClose = CreateWindowA("button", "Quit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420, 35, 100, 20, hwnd, (HMENU)ID_B_Q, hInst, NULL);
        hRad[0] = CreateWindowA("button", "Voltage mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			10, RC.bottom - 108, 140, 20, hwnd, (HMENU)ID_RB_V, hInst, NULL);
        hRad[1] = CreateWindowA("button", "Current mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, RC.bottom - 108, 140, 20, hwnd, (HMENU)ID_RB_C, hInst, NULL);
		radio_stat = -1;
		SetWindowTextA(hOutput_e, (LPCSTR)"G:\\Experience\\OUT.txt");
		SetWindowTextA(hData_e, (LPCSTR)"G:\\Experience\\1.csv");
		SetWindowTextA(hACC[0], (LPCSTR)"40");
		SetWindowTextA(hOp[0], (LPCSTR)"32000");
		SetWindowTextA(hEM, (LPCSTR)"11.21e-27");
		SetWindowTextA(hESQ, (LPCSTR)"4e-6");
		SetWindowTextA(hENoB, (LPCSTR)"10000");
		SetWindowTextA(hEDT, (LPCSTR)"11000");
		for(int i = 0 ; i < 3; i++)
			SetWindowTextA(hERes[i], (LPCSTR)"10");
		for (int i = 0; i < 6; i++)
		{
			SetWindowTextA(hedit[i], N_o_Ch[i]);
			SetWindowTextA(hedit[i + 6], (LPCSTR)"10");
		}
		return 0;
	}
	case WM_COMMAND:
	{
        //if(LOWORD(wParam) == ID_RB_C)
            //radio_stat = 1;
        //if(LOWORD(wParam) == ID_RB_V)
            //radio_stat = -1;
		switch (LOWORD(wParam))
		{
        case ID_RB_C:
            {
                radio_stat = -1;
                return 0;
            }
        case ID_RB_V:
            {
                radio_stat = 1;
                return 0;
            }
		case ID_B_OK:
		{
			GetWindowRect(hwnd, &RC);
			hLStat = CreateWindowA("static", "Calculate", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				RC.right - 136, RC.bottom - 108, 70, 20, hwnd, NULL, hInst, NULL);
			GetWindowTextW(hOutput_e, Out, 1024);
			GetWindowTextW(hData_e, Data, 1024);
			if (wcslen(Out) < 2)
			{
				MessageBox(hwnd, "Write the way to the file for writing!", "Warning!", MB_OK);
				return 0;
			}
			if (wcslen(Data) < 2)
			{
				MessageBox(hwnd, "Write the way to the file with oscilloscop data!", "Warning!", MB_OK);
				return 0;
			}

			// Ïðîâåðèì, ñóùåñòâóåò ëè òàêîé ôàéë
			int dim[2];
			if(DWORD(GetFileAttributesW(Data)) != DWORD(-1))
				read_data(Data, dim);
			else
			{
				MessageBox(hwnd, "Invalid way to the file with oscilloscop data!", "Warning!", MB_OK);
				return 0;
			}
			// matrix <double> OSC_ARR(dim[0], dim[1]);
			double** OSC_ARR = new double *[dim[0]];
			for (int i = 0; i < dim[0]; i++)
				OSC_ARR[i] = new double[dim[1]];
			for (int i = 0; i < dim[0]; i++)
				for (int j = 0; j < dim[1]; j++)
					CHANGE >> OSC_ARR[i][j];
			int* ch_num = new int[dim[1] - 1];
			for (int i = 0; i < dim[1] - 1; i++)
			{
				GetWindowTextA(hedit[i], CH_E, 12);
				if (strlen(CH_E) < 1)
				{
					BUF_S = "Fill the field \'" + num[i] + "\' for number of channel";
					MessageBox(hwnd, BUF_S.c_str(), "Warning!", MB_OK);
					return 0;
				}
				else
				{
					CHANGE.str(L"");
					CHANGE.clear();
					CHANGE << CH_E;
					CHANGE >> ch_num[i];
				}
				GetWindowTextA(hedit[i + 6], CH_E, 12);
				if (strlen(CH_E) < 1)
				{
					BUF_S = "Fill the field \'" + num[i] + "\' for multiplication koefficient!";
					MessageBox(hwnd, BUF_S.c_str(), "Warning!", MB_OK);
					return 0;
				}
				else
				{
					CHANGE.str(L"");
					CHANGE.clear();
					CHANGE << CH_E;
					CHANGE >> k_M[i];
				}
			}
			// Ïðî÷òåì çíà÷åíèå íà÷àëüíîé òåìïåðàòóðû
			GetWindowTextA(hEDT, CH_E, 12);
			if (strlen(CH_E) < 1)
			{
				MessageBox(hwnd, "Temperature field is empty!", "Warning!", MB_OK);
			return 0;
			}
			else
			{
				CHANGE.str(L"");
				CHANGE.clear();
				CHANGE << CH_E;
				CHANGE >> T_root;
			}
			T_root = T_root / 11604;
			// Ïðî÷òåì òî÷íîñòü, ñ êîòîðîé íóæíî îïðåäåëèòü òåìïåðàòóðó
			GetWindowTextA(hACC[0], CH_E, 12);
			if (strlen(CH_E) < 1)
			{
				MessageBox(hwnd, "Fill accuraty field!", "Warning!", MB_OK);
				return 0;
			}
			else
			{
				CHANGE.str(L"");
				CHANGE.clear();
				CHANGE << CH_E;
				CHANGE >> accur;
			}
			accur = accur / 11604;
			// Ïðî÷òåì ìàêñèìàëüíîå ÷èñëî èòåðàöèé
			GetWindowTextA(hOp[0], CH_E, 12);
			if (strlen(CH_E) < 1)
			{
				MessageBox(hwnd, "Write number of iterations!", "Warning!", MB_OK);
				return 0;
			}
			else
			{
				CHANGE.str(L"");
				CHANGE.clear();
				CHANGE << CH_E;
				CHANGE >> N_op;
			}
			// Òåïåðü ïðîèçâåäåì ïåðåñòàíîâêó ñòîëáöîâ, ñîãëàñíî ðàñïðåäåëåííûì íîìåðàì
			double** buf_arr = new double*[dim[0]];
			for (int i = 0; i < dim[0]; i++)
				buf_arr[i] = new double[dim[1] - 1];
			for (int i = 0; i < dim[1] - 1; i++)
				for (int j = 0; j < dim[0]; j++)
					buf_arr[j][i] = OSC_ARR[j][ch_num[i]];
			for (int i = 0; i < dim[1] - 1; i++)
				for (int j = 0; j < dim[0]; j++)
					OSC_ARR[j][i + 1] = buf_arr[j][i];
			for (int i = 0; i < dim[0]; i++)
				delete[] buf_arr[i];
			delete[] buf_arr;
			// Òàê êàê íà ðóêàõ òîëüêî ñîîòíîøåíèÿ äëÿ òðåõçîíäîâîé ìåòîäèêå, òî è ñ÷èòàòü áóäåì òîëüêî äëÿ íåå
			double source_d[5];
			// Ïðî÷òåì çíà÷åíèÿ ñîïðîòèâëåíèé
			int I_J = 0;
			for (int i = 0; i < 3; i++) // for (int i = 0; i < int(dim[1]/2 + 0.5); i++)
			{

				GetWindowTextA(hERes[i], CH_E, 12);
				if (strlen(CH_E) < 1)
				{
					MessageBox(hwnd, "Resisters fields are empty!", "Warning!", MB_OK);
					return 0;
				}
				else
				{
					CHANGE.str(L"");
					CHANGE.clear();
					CHANGE << CH_E;
					CHANGE >> source_d[i];
					I_J++;
				}
			}
			// Ïðî÷òåì çíà÷åíèå ïëîùàäè
			GetWindowTextA(hESQ, CH_E, 12);
			if (strlen(CH_E) < 1)
			{
				MessageBox(hwnd, "Fill square field!", "Warning!", MB_OK);
				return 0;
			}
			else
			{
				CHANGE.str(L"");
				CHANGE.clear();
				CHANGE << CH_E;
				CHANGE >> source_d[I_J];
				I_J++;
			}
			GetWindowTextA(hEM, CH_E, 12);
			if (strlen(CH_E) < 1)
			{
				MessageBox(hwnd, "Fill mass field!", "Warning!", MB_OK);
				return 0;
			}
			else
			{
				CHANGE.str(L"");
				CHANGE.clear();
				CHANGE << CH_E;
				CHANGE >> source_d[I_J];
			}
			// Ïðî÷òåì çíà÷åíèå ìàêñèìàëüíîãî ðàçìåðà âûõîäíîãî ìàññèâà
			GetWindowTextA(hENoB, CH_E, 12);
			if (strlen(CH_E) < 1)
			{
				MessageBox(hwnd, "Write lenght of vector!", "Warning!", MB_OK);
				return 0;
			}
			else
			{
				CHANGE.str(L"");
				CHANGE.clear();
				CHANGE << CH_E;
				CHANGE >> Numb_max;
			}
			// source_d = { ñîïðîòèâëíèå ìåæäó Ñ1 è çåìëåé, ñîïðîòèâëíèå ìåæäó Ñ2 è çåìëåé, ïëîùàäü çîíäà, ïðèâåäåííàÿ ìàññà }
			// Ïîñêîëüêó ðåøàòü ïëàíèðóåòñÿ èòåðàöèîííûìè ìåòîäàìè, òî íà êàæäóþ òî÷êó èç ìàññèâà ïðèäåòñÿ ïî íåñîëüêî îïåðàöèé
			// Äëÿ ýêîíîìèè âðåìåíè ïðîðåäèì ìàññèâ, ñíèçèâ ÷èñëî òî÷åê äî 250.
			int new_size_d = dim[0];
			double OSC_ARR_small[new_size][NoCh];
			if (dim[0] > Numb_max)
			{
				// reinterpret_cast - ïðåîáðàçóåò â îäíîìåðíûé ìàññèâ
				// cut_arr(reinterpret_cast<double *>(OSC_ARR), dim[0], reinterpret_cast<double *>(OSC_ARR_small), 250, NoCh);
				// Ñ ïðåîáðàçîâàíèåì â îäíîåðíûé âèä ôóíêöèÿ ïîëó÷àåòñÿ ñëîæíîé, ïîýòîìó âîñïîëüçóåìñÿ áóôåðíûìè ìàññèâàìè
				double* buf_arr_1 = new double[dim[0]];
				double* buf_arr_2 = new double[new_size];
				for (int i = 0; i < NoCh; i++)
				{
					for (int j = 0; j < dim[0]; j++)
						buf_arr_1[j] = OSC_ARR[j][i];
					cut_arr(buf_arr_1, dim[0], buf_arr_2, new_size);
					for (int j = 0; j < new_size; j++)
						OSC_ARR_small[j][i] = buf_arr_2[j];
				}
				delete[] buf_arr_1;
				delete[] buf_arr_2;
				new_size_d = new_size;
			}
			double* I_1 = new double[new_size_d];
			double* I_2 = new double[new_size_d];
			double* I_3 = new double[new_size_d];
			double* V_12 = new double[new_size_d];
			double* V_13 = new double[new_size_d];
			double* time = new double[new_size_d];
			double* T = new double[new_size_d];
			if (dim[0] > Numb_max)
				for(int i = 0; i < new_size_d; i++)
				{
					time[i] = OSC_ARR_small[i][0];
					I_1[i] = k_M[0]*OSC_ARR_small[i][1] / source_d[0];
					I_2[i] = k_M[1] * OSC_ARR_small[i][2] / source_d[2];
					I_3[i] = k_M[2] * OSC_ARR_small[i][3] / source_d[3];
					V_12[i] = k_M[3] * OSC_ARR_small[i][4];
					V_13[i] = k_M[3] * OSC_ARR_small[i][5];
				}
			else
				for (int i = 0; i < new_size_d; i++)
				{
					time[i] = OSC_ARR[i][0];
					I_1[i] = k_M[0]*OSC_ARR[i][1] / source_d[0];
					I_2[i] = k_M[1] * OSC_ARR[i][2] / source_d[1];
					I_3[i] = k_M[2] * OSC_ARR[i][3] / source_d[2];
					V_12[i] = k_M[3] * OSC_ARR[i][4];
					V_13[i] = k_M[4] * OSC_ARR[i][5];
				}
            double* n_e = new double[new_size_d];
            if(radio_stat == -1)
            {

                for (int i = 0; i < new_size_d; i++)
                {
                    if ((abs(I_3[i]) < 1e-9) & (abs(I_2[i]) < 1e-9))
                        T[i] = -1;
                    else
                    {
                        a = (I_2[i] - I_1[i])/(I_3[i] - I_1[i]);
                        //std::cout << "Time: " << time[i] << " (I2-I1)/(I3-I1): " << a << std::endl;
                        T[i] = root_of_equation(a, abs(V_12[i]), abs(V_13[i]), T_root, accur, N_op);
                    }
                }
                for (int i = 0; i < new_size_d; i++)
                    if (T[i] > 0)
                    {
                        a = abs(V_13[i] - V_12[i]) / T[i];
                        b = abs(I_3[i]) - abs(I_2[i] * exp(-a));
                        c = source_d[3] * 0.6 * Q_d * sqrt(Q_d*T[i]/ source_d[4]);
                        n_e[i] = b/c/(1 - exp(-a));
                    }
                    else
                        n_e[i] = -1;
            }
            if(radio_stat == 1)
            {
                for (int i = 0; i < new_size_d; i++)
                {
                    if ((abs(I_3[i]) < 1e-9) & (abs(I_2[i]) < 1e-9))
                        T[i] = -1;
                    else
                    {
                        a = - I_1[i]/(I_3[i] - I_1[i]);
                        T[i] = root_of_equation(a, abs(V_12[i]), abs(V_13[i]), T_root, accur, N_op);
                    }
                }
                for (int i = 0; i < new_size_d; i++)
                    if (T[i] > 0)
                    {
                        a = abs(V_13[i] - V_12[i]) / T[i];
                        c = source_d[3] * 0.6 * Q_d * sqrt(Q_d*T[i]/ source_d[4]);
                        n_e[i] = I_3[i]/c/(1 - exp(-a));
                    }
                    else
                        n_e[i] = -1;
            }
            if(radio_stat == 0)
            {
                MessageBox(hwnd, "Select probes mode!", "Warning!", MB_OK);
                return 0;
            }
            if (DWORD(GetFileAttributesW(Out)) == DWORD(-1))
                CreateFileW(Out, GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                //std::wofstream f_out(Out);//, std::ios_base::out);

            boost::filesystem::wofstream f_out;
            f_out.open(Out);
            if(radio_stat == -1)
                f_out << "Current mode\n";
            else
                f_out << "Voltage mode\n";
            f_out << "Time, s;\tTemperature, K;\tDensity, m^-3;\tTemperature, eV;\tDensity, sm^-3;\tCurrent density, A/sm^2\n";
            for (int i = 0; i < new_size_d; i++)
                if (T[i] > 0)
                    f_out << time[i] << ";\t" << 11604*T[i] << ";\t" << n_e[i] << ";\t" << T[i] << ";\t" << (1e-6)*n_e[i] << ";\t" << (1e-4)*n_e[i]*source_d[3] * 0.6 * Q_d * sqrt(Q_d*T[i]/ source_d[4]) << std::endl;
                else
                    f_out << time[i] << ";\t" << -1 << ";\t" << -1 << ";\t" << -1 << ";\t" << -1 << ";\t" << (1e-4)*abs(I_2[i] + I_3[i]) / source_d[3] << std::endl;
            f_out.close();
			// Óäàëèì âñå çíà÷åíèÿ, êîòîðûå äàëè íåêîððåêòíûå ðåçóëüòàòû
			delete[] n_e;
			delete[] time;
			delete[] I_1;
			delete[] I_2;
			delete[] I_3;
			delete[] V_12;
			delete[] V_13;
			delete[] T;
			for (int i = 0; i < dim[0]; i++)
				delete[] OSC_ARR[i];
			delete[] OSC_ARR;
			delete[] ch_num;
			hLStat = CreateWindowA("static", "Complete", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				RC.right - 136, RC.bottom - 108, 70, 20, hwnd, NULL, hInst, NULL);
			return 0;
		}
		case ID_B_Q:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
