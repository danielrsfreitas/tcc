// pclan.cpp : Defines the entry point for the application.


#include "stdafx.h"
#include "resource.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "winsock2.h"
#include "ws2tcpip.h"

#define MAX_LOADSTRING 100

#define DEFAULT_BUFLEN  512
#define DEFAULT_PORT	5024
//#define DEFAULT_PORT2	1024
#define sPortUDP	50280 //Agora com UDP
//#define sPortTCP	50560 //Agora com TCP
#define WM_SOCKUDP	(WM_USER +10) //Agora com UDP
//#define WM_SOCKTCP	(WM_USER +10) //Agora com TCP


// Global Variables:
HINSTANCE hInst;						// current instance
HWND	hWnd;
TCHAR szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	// The title bar text

// Mais Variáveis Globais
bool bIDN = FALSE;
bool bCLS = FALSE;
bool bRST = FALSE;

bool bJAN = FALSE;
bool bPOR = FALSE;
bool bFUN = FALSE;
bool bPIA = FALSE;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Rede(HWND, UINT, WPARAM, LPARAM);

bool				EnviaIDNCLSRST(HWND,int);
bool				CmdSCPI(HWND,LPCSTR);
bool				EnviaCMD(HWND);
bool				UtilGerais(HWND,int);
bool				TXBatePapoUDP(HWND,int);	//Transmite BATEPAPO
bool				ArqFonte(HWND,LPCSTR);

//int	nPort=5555;		//Agora vars. globais
//int nPortTX=8888;
int		N;
//char	RXBuf[512]="";	
//bool	bRX=FALSE;
//bool	bPriRX=FALSE;
SOCKET	RecvSocket;					
//int		Port = 27015;
SOCKET	RXBPSocket=NULL;
SOCKET	TXBPSocket=NULL;
char	*szServer="localhost";
sockaddr	sockAddrClient;
SOCKADDR_IN	Remoto;
SOCKADDR_IN ServAddr,ListenAddr;   //Para SERV. OUVIR
struct	hostent	*host;
char	szchegando[512];
WSADATA	wsaData,wsaDat;
char	szArq[40]={0};

//HMENU	hMenuDialogo;
//HMENU	hMenuDial;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	//HWND	hWnd;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PCLAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PCLAN);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_PCLAN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_PCLAN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT	rc1 = {0,0,0,0};

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_REDE:
				   DialogBox(hInst, (LPCTSTR)IDD_REDEBOX, hWnd, (DLGPROC)Rede);
				   break;

				case IDM_PROC:
		MessageBox(hWnd, "** Realizado no DECOM/FEEC/UNICAMP **\r\n"
						" Versão 1.0 - Windows 2000/Me/XP/Vista,7,8,..  \r\n\n"
						"Profs:	Afonso de Oliveira Alonso \r\n"
						"	Michel Daoud Yacoub \r\n"
						"	João Marcos Travassos Romano\r\n"
						"	José Candido dos Santos \r\n"
						"	Gustavo Fraidenraich\r\n"
						"	Renato Lopes\r\n"
						"	Darli Augusto de Mello\r\n\r\n"
						"##  Abril de 2014 ##\r\n"
								,"FEEC/UNICAMP-LE34", MB_OK | MB_ICONINFORMATION);
				   break;

				case IDM_INFOCABOS:
		MessageBox(hWnd, "Comanda Geradores de Funções\r\n"
						"** Agilent:33220A e 33210A **\r\n\n"
						"Ligação DIRETA >> CABO LAN CRUZADO (Cross-0ver) ****\r\n"
						"Ligação com HUB >>CABO LAN NORMAL ****\r\n\n"
						,"COMO LIGAR GERADOR ?", MB_OK | MB_ICONINFORMATION);
				   break;

				case IDM_INFOGERAL:
		MessageBox(hWnd, "Escrito em linguagem C para \r\n"
						 "Windows.Compilado no Visual \r\n"	
						 " C/C++ 6.0 da Microsoft.\r\n\r\n"	
						 " Uso restrito nas aulas de laboratório\r\n"
						 " de EE882-Lab. de Comunicações.\r\n\r\n"
						"Bloco E : FEEC/UNICAMP\r\n\n"
						,"IMPORTANTE", MB_OK | MB_ICONINFORMATION);
				   break;

				case IDM_INFOREDE:
		MessageBox(hWnd, "###COMANDO p/ GERADOR usa  Protocolo TCP ###\r\n"
						" ***BATEPAPO no TECLADO usa Protocolo UDP **\r\n\n"
						"       #### %%% ####     "
						,"PCLAN.EXE > COMANDA GERADOR & BATE-PAPO TECLADO", MB_OK | MB_ICONINFORMATION);
				   break;

				case IDM_SCPI:
				   CmdSCPI(hWnd,"notepad scpi.txt");
				   break;

				case IDM_CALC:
				   UtilGerais(hWnd,1);
				   break;

				case IDM_NOTE:
				   UtilGerais(hWnd,2);
				   break;

				case IDM_RES:
				   ArqFonte(hWnd,"notepad resource.h");
				   break;

				case IDM_RC:
				   ArqFonte(hWnd,"notepad pclan.rc");
				   break;

				case IDM_CPP:
				   ArqFonte(hWnd,"notepad pclan.cpp");
				   break;
				
				case IDM_SAIR:
				   DestroyWindow(hWnd);
				   break;

				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


// Mesage handler for about box.
LRESULT CALLBACK Rede(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, iResp;
	static bool bRetIDN,bRetCLS,bRetRST;
	char	szDestBP[8]="";
	static SOCKET	ClientSocket;		//É ISSO MESMO ?
	PAINTSTRUCT ps;
	HDC	hdc; 
	char	szmsg[50]="";
	HWND	hFocus;
	char	Temp[8]="";
	switch (message)
	{
		case WM_INITDIALOG:

			SetDlgItemText(hDlg,IDD_EDIP,"143.106.150.189");
			SetDlgItemText(hDlg,IDD_CMD,"APPL:SIN 2.7E+3, 2.0, -0.45");
			SetDlgItemText(hDlg,IDD_EDIPJAN,"143.106.150.177");
			SetDlgItemText(hDlg,IDD_EDIPPOR,"143.106.150.179");
			SetDlgItemText(hDlg,IDD_EDIPFUN,"143.106.150.178");
			SetDlgItemText(hDlg,IDD_EDIPPIA,"xxx.xxx.xxx.xxx");

			//hMenuDial=LoadMenu(hInst,(LPCTSTR)IDM_MENUDIALOG);
			//SetMenu(hDlg,hMenuDial);

//       Inicializando Winsock 
// ___________________________________

iResp = WSAStartup(MAKEWORD(2,2),&wsaDat);
if (iResp != NO_ERROR)
{
	sprintf(szmsg,"\tWSAStartup Serv. Falhou com ERRO: %d\n", iResp);
	MessageBox(hDlg, szmsg, "STARTUP SERVIDOR FALHOU",MB_OK | MB_ICONINFORMATION);
	return FALSE;
}

// _______________________________________

// Criando Socket para ligar ao Destinatário
// _______________________________________

	RXBPSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if (RXBPSocket == INVALID_SOCKET)
{
	sprintf(szmsg,"\tSocket Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szmsg, "SOCKET SERV.  FALHOU",MB_OK | MB_ICONINFORMATION);
	WSACleanup();
	return FALSE;
}
// _______________________________________

// A Estrutura sockaddr_in especifica a família
// de endereço, ender. IP e porta deste PC Servidor
// onde o cliente conecta.
// _______________________________________	

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);				//OU 
	//ListenSock.sin_addr.s_addr = inet_addr( (LPTSTR)szIP );	//se IDENTIFICA c/ o própio IP local??
	ServAddr.sin_port = htons(sPortUDP); //Esta é BOA (50280) para RECEBER de PC Remoto UDP TXBATEPAPO ?


	iResp=bind(RXBPSocket, (SOCKADDR *) &ServAddr, sizeof(SOCKADDR));

		if (iResp == SOCKET_ERROR)
		{
			sprintf(szmsg,"\tBIND Falhou com ERRO: %ld\n", WSAGetLastError());
			MessageBox(hDlg, szmsg, "BIND FALHOU AQUI",MB_OK | MB_ICONINFORMATION);
			closesocket(RXBPSocket);
			WSACleanup();
			return FALSE;
		}

	iResp=WSAAsyncSelect(RXBPSocket, hDlg,WM_SOCKUDP , FD_READ );

		if (iResp != 0)
		{
			sprintf(szmsg,"\tASyncSelect Falhou com ERRO: %ld\n", WSAGetLastError());
			MessageBox(hDlg, szmsg, "SYNCSELECT UDP FALHOU AQUI",MB_OK | MB_ICONINFORMATION);
			closesocket(RXBPSocket);
			WSACleanup();
			return FALSE;
		}

			MessageBox(hDlg,"*** PROTOCOLO TCP para Comandar GERADOR **\n"
							"* PROTOCOLO UDP para BATEPAPO pelo teclado **\r\n",
					"PCLAN AVISANDO > REDE INSTALADA!!",MB_OK | MB_ICONINFORMATION);
			SetWindowText(hDlg,"## PCLAN.EXE INSTALADO COM TCP E UDP  ##");
			MessageBeep(MB_OK);
			return(FALSE);	

		case WM_PAINT:
			hdc = BeginPaint(hDlg, &ps);

			EndPaint(hDlg, &ps);
			break;
		
		case WM_SOCKUDP:	//PARA Captura de eventos do BatePapo
			{
				//hDlg=SetFocus(hDlg);
				//SetWindowText(hDlg,"CHEGANDO...");

				if ( WSAGETSELECTERROR(lParam) )
				{
					iResp=WSAGetLastError();
					sprintf(szmsg,"\t GETSELECTERROR pegou: %ld\n", iResp);
				MessageBox(hDlg, szmsg, "GETSELECTERROR  AQUI",MB_OK | MB_ICONINFORMATION);
					return 0;
				}
				switch(WSAGETSELECTEVENT(lParam))
				{								//ABRINDO Switch WSAGETSELECTEVENT

					case FD_READ:

				char szchegando[512];
				SOCKADDR	from;
				int resul,tam;
				//int u=512;
				tam=sizeof(SOCKADDR);
				ZeroMemory(szchegando,sizeof(szchegando));
				memset(szchegando,'\0',512);

				SetDlgItemText(hDlg,(int)IDD_RXBP,"");
				resul=recvfrom(wParam,szchegando,sizeof(szchegando)-1,0,&from,&tam);
					if (resul == SOCKET_ERROR)
					{
						iResp=WSAGetLastError();
				sprintf(szmsg,"\t recvfrom Falhou com ERRO: %ld\n", iResp);
				MessageBox(hDlg, szmsg, "RECEIVEFROM FALHOU AQUI",MB_OK | MB_ICONINFORMATION);
				closesocket(RXBPSocket);
				WSACleanup();
						return TRUE;
					}

					if ( WSAGETSELECTERROR(lParam) )
				{
					iResp=WSAGetLastError();
					sprintf(szmsg,"\t GETSELECTERROR pegou: %ld\n", iResp);
				MessageBox(hDlg, szmsg, "GETSELECTERROR  AQUI",MB_OK | MB_ICONINFORMATION);
					return 0;
				}

				SetDlgItemText(hDlg,(int)IDD_RXBP,szchegando);  //TEM QUE POR!!!
				SetWindowText(hDlg,"PCLAN.EXE AVISA > NOVA MENSAGEM UDP CHEGOU INTEIRA??SERÁ??...");
					iResp=WSAGetLastError();
					//hWnd=GetParent(hDlg);
					//hWnd=GetFocus();
					iResp=WSAGetLastError();

				    break;	

					default:  break;
				}		//FECHANDO	Switch WSAGETSELECTEVENT
				break;
			}		//FECHANDO	case SOCKUDP
			break;


		case WM_DESTROY:

			//shutdown(RXBPSocket,SD_BOTH);
			closesocket(RXBPSocket);
			WSACleanup();
			return FALSE;


		case WM_COMMAND:

			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam);
	//SetWindowText(hDlg, itoa( (int)wmId,Temp,10));  //TIRAR DEPOIS
	//SetWindowText(hDlg, itoa( (int)wmEvent,Temp,10));  //TIRAR DEPOIS
	//if ( (int)wmEvent==256 )
	//{
	//SetWindowText(hDlg, itoa( (int)wmId,Temp,10));  //TIRAR DEPOIS
	//}
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));

				closesocket(RXBPSocket);
				WSACleanup();
				return TRUE;
			}
			
			// Parse the menu selections:
			switch (wmId)
			{

				case IDD_IDN:
					bIDN = TRUE;
					bCLS = bRST = FALSE;
					iResp = MessageBox(hDlg, "** VAi identificar Gerador ! **\r\n"
						,"IDENTIFICANDO", MB_YESNO );
					if(iResp == IDNO)
					{
						bIDN = FALSE;
						CheckDlgButton(hDlg,IDD_IDN,FALSE);
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						N=1;
						EnviaIDNCLSRST(hDlg,N);
						CheckDlgButton(hDlg,IDD_IDN,FALSE);
						return TRUE;
					}
					//return(TRUE);
					break;


				case IDD_CLS:
					bCLS = TRUE;
					bIDN = bRST = FALSE;
					iResp = MessageBox(hDlg, "Vai Limpar \r\n"
						,"LIMPAR ÊRRO NO GERADOR", MB_YESNO);
					
					if(iResp == IDNO)
					{
						bCLS = FALSE;
						CheckDlgButton(hDlg,IDD_CLS,FALSE);	
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						N=2;
						EnviaIDNCLSRST(hDlg,N);
						CheckDlgButton(hDlg,IDD_CLS,FALSE);
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDD_RST:
					bRST = TRUE;
					bIDN = bCLS = FALSE;
					iResp = MessageBox(hDlg, "Resetar Gerador\r\n"
						,"RESET GERADOR", MB_YESNO);
					if(iResp == IDNO)
					{
						bCLS = FALSE;
						CheckDlgButton(hDlg,IDD_RST,FALSE);	
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						N=3;
						EnviaIDNCLSRST(hDlg,N);
						CheckDlgButton(hDlg,IDD_RST,FALSE);	
						return TRUE;
					}
					//return(TRUE);
					break;


				case IDC_CMD:					
					iResp = MessageBox(hDlg, "Enviar o Comando para Gerador?\r\n"
						,"VAI COMANDO PARA O GERADOR ???", MB_YESNO);
					if(iResp == IDNO)
					{					
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						EnviaCMD(hDlg);						
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDD_JAN:
					bJAN = TRUE;
					bPOR = bFUN = bPIA = FALSE;
					iResp = MessageBox(hDlg, "VAI PARA JANELA ?\r\n"
						,"PCLAN >> ENVIAR PARA JANELA ?", MB_YESNO);
					if(iResp == IDNO)
					{
						bJAN = FALSE;
						CheckDlgButton(hDlg,IDD_JAN,FALSE);	
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy (szDestBP,"JAN");
						N=1;
						TXBatePapoUDP(hDlg,N);
						CheckDlgButton(hDlg,IDD_JAN,FALSE);	
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDD_POR:
					bPOR = TRUE;
					bJAN= bFUN = bPIA = FALSE;
					iResp = MessageBox(hDlg, "VAI PARA PORTA ?\r\n"
						,"PCLAN >> ENVIAR PARA PORTA ?", MB_YESNO);
					if(iResp == IDNO)
					{
						bPOR = FALSE;
						CheckDlgButton(hDlg,IDD_POR,FALSE);	
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy (szDestBP,"POR");
						N=2;
						TXBatePapoUDP(hDlg,N);
						CheckDlgButton(hDlg,IDD_POR,FALSE);	
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDD_FUN:
					bFUN = TRUE;
					bJAN= bPOR = bPIA = FALSE;
					iResp = MessageBox(hDlg, "VAI PARA FUNDO ?\r\n"
						,"PCLAN >> ENVIAR PARA FUNDO ?", MB_YESNO);
					if(iResp == IDNO)
					{
						bFUN = FALSE;
						CheckDlgButton(hDlg,IDD_FUN,FALSE);	
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy (szDestBP,"FUN");
						N=3;
						TXBatePapoUDP(hDlg,N);
						CheckDlgButton(hDlg,IDD_FUN,FALSE);	
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDD_PIA:
					bPIA = TRUE;
					bJAN= bPOR = bFUN = FALSE;
					iResp = MessageBox(hDlg, "VAI PARA PIA ?\r\n"
						,"PCLAN >> ENVIAR PARA PIA ?", MB_YESNO);
					if(iResp == IDNO)
					{
						bPIA = FALSE;
						CheckDlgButton(hDlg,IDD_PIA,FALSE);	
						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy (szDestBP,"PIA");
						N=4;
						TXBatePapoUDP(hDlg,N);
						CheckDlgButton(hDlg,IDD_PIA,FALSE);	
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDM_SCPI:

					iResp = MessageBox(hDlg, "** VER COMANDOS SCPI ? **\r\n"
						,"SCPI", MB_YESNO | MB_ICONINFORMATION );
					if(iResp == IDNO)
					{

						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy(szArq,"notepad scpi.txt");
						CmdSCPI(hDlg,(LPSTR)szArq);
						return TRUE;
					}
					//return(TRUE);
					break;
					
				case IDM_CALC:

					iResp = MessageBox(hDlg, "** FAZER CONTAS ?? **\r\n"
						,"CALC", MB_YESNO | MB_ICONINFORMATION | MB_ICONHAND);
					if(iResp == IDNO)
					{

						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy(szArq,"calc.exe");
						UtilGerais(hDlg,1);
						return TRUE;
					}
					//return(TRUE);
					break;

				case IDM_NOTE:

					iResp = MessageBox(hDlg, "** EDITOR SIMPLES ?? **\r\n"
						,"NOTE", MB_YESNO | MB_ICONINFORMATION | MB_ICONHAND);
					if(iResp == IDNO)
					{

						return FALSE;
					}
					if(iResp == IDYES) 
					{
						MessageBeep(MB_ICONEXCLAMATION);
						strcpy(szArq,"notepad.exe");
						UtilGerais(hDlg,2);
						return TRUE;
					}
					//return(TRUE);
					break; 

				default:
					break;
				   //return DefWindowProc(hDlg, message, wParam, lParam);
			}	//Fechando switch(wmId)
			//break;	//ERA break
				   return DefWindowProc(hDlg, message, wParam, lParam); //ADICIONEI ISSO
			//break;
	}	//Fechando switch(message)

    return 0; 
}	//TÉRMINO DESTE PROCEDURE



	/* Inicio de Utilidades Gerais */
bool UtilGerais(HWND hDlg,int num)
{
	int		iResp;
	char	bife[40],tmp[4];
	if (num==1) strcpy(bife,"calc.exe");
	if (num==2) strcpy(bife,"notepad.exe");
	//if (num==3) strcpy(bife,"mspaint.exe");
	//if (num==4) strcpy(bife,"explorer.exe");

		iResp = MessageBox(hDlg, (LPCTSTR)bife,
				"** EXECUTAR ARQUIVO ? **", MB_YESNO | MB_ICONHAND);
			if(iResp == IDNO)
			{
			return FALSE;
			}
			if(iResp == IDYES) 
			{
				MessageBeep(MB_ICONEXCLAMATION);
				strcpy(szArq,(LPCTSTR)bife);
			}

	if (iResp=WinExec((LPSTR)bife,SW_SHOWNORMAL)>31) return TRUE;
	if (iResp<=31)	//Tivemos algum êrro
	{
		switch(iResp)
		{
			case 0:
				strcpy(bife,"Fora da memória!");
				break;

			case 2:
				strcpy(bife,"Arquivo não achado!!");
				break;

			case 3:
				strcpy(bife,"Rota não achada!!");
				break;

			default:
				strcpy(bife,"Arquivo  não carregado:cod. de erro:");
				strcat(bife,itoa(iResp,tmp,10));
				break;
		}

	MessageBox(hDlg,bife,"Aplicativo não encontrado",
					MB_OK | MB_ICONEXCLAMATION);
	}
		return 0;
}
	/* Fim do Utilidades Gerais */

/* FUNÇÃO TRIPLA para Ident.,Limpar e Resetar o Gerador de Função 33220A */
bool EnviaIDNCLSRST(HWND hDlg,int n)
{
	int		iResult, i;
	int		recvbuflen = DEFAULT_BUFLEN;
	char	szMsg[200];
	char	recvbuf[DEFAULT_BUFLEN] = "";
	char	*sendbuf = "";
	if (n==1)  sendbuf="*IDN?\r\n";
	if (n==2)  sendbuf="*CLS\r\n";
	if (n==3)  sendbuf="*RST\r\n";


	char	szIP[20] = "";						// lê o IP do gerador a ser à identificar 
	char	szTemp[200];
	SOCKET	ConnectSocket;
	struct sockaddr_in	clientService;
	WSADATA	wsaData;
	HWND	hCtrl;

// _________________________________

// Pega o IP do Gerador à Identificar
// _________________________________

	hCtrl = GetDlgItem(hDlg,IDD_EDIP);
	GetWindowText(hCtrl,(LPTSTR)szIP,16);
	if(strlen(szIP) == 0) 
	{
			MessageBox(hDlg,"Teclar o IP do Gerador ANTES","PCLAN.EXE INFORMA",MB_ICONSTOP);
			return FALSE;
	}	

	MessageBox(hDlg,(LPCTSTR)szIP,"PCLAN.EXE VAI FAZER",MB_ICONSTOP);

// ___________________________________

//       Inicializando Winsock
// ___________________________________

iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
if (iResult != NO_ERROR)
{
	sprintf(szMsg,"\tWSAStartup Falhou com ERRO: %d\n", iResult);
	MessageBox(hDlg, szMsg, "STARTUP FALHOU",MB_OK | MB_ICONINFORMATION);
	return FALSE;
}

// _______________________________________

// Criando Socket para Conectar ao Gerador
// _______________________________________

	ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if (ConnectSocket == INVALID_SOCKET)
{
	sprintf(szMsg,"\tSocket Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SOCKET FALHOU",MB_OK | MB_ICONINFORMATION);
	WSACleanup();
	return FALSE;
}	
	
// _______________________________________

// A Estrutura sockaddr_in especifica a família
// de endereço, ender. IP e porta do servidor (Gerador)
// a ser conectado.
// _______________________________________	
	

	clientService.sin_family = AF_INET;
	//clientService.sin_addr.s_addr = inet_addr ("143.106.150.179");	
	clientService.sin_addr.s_addr = inet_addr((LPCTSTR)szIP);
	clientService.sin_port = htons(DEFAULT_PORT); //Esta é BOA (5024) para falar com gerador AGILENT

// _______________________________________

// Conecte ao Servidor (Gerador)
// _______________________________________	
	SetWindowText(hDlg,"PCLAN.EXE AGUARDANDO... +/-  20 [seg]  !!...");
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	iResult = connect(ConnectSocket,(SOCKADDR*)&clientService,sizeof(clientService));
if (iResult == SOCKET_ERROR)
{
	int j=WSAGetLastError(); 
	sprintf(szMsg,"\tConnect Falhou com ERRO: %ld\n", j);
	MessageBox(hDlg, szMsg, "CONNECT FALHOU",MB_OK | MB_ICONINFORMATION);

	if (j == WSAETIMEDOUT)
	{
	MessageBox(hDlg, "WSAETIMEDOUT", "CONEXÃO FALHOU",MB_OK | MB_ICONINFORMATION);
	SetWindowText(hDlg,"PCLAN.EXE AVISA : EXPIROU TEMPO DE ESPERA !!...");
	}

	if (j == WSAEHOSTUNREACH)
	{
	MessageBox(hDlg, "WSAEHOSTUNREACH)", "GERADOR FORA DA REDE!!",MB_OK | MB_ICONINFORMATION);
	SetWindowText(hDlg,"PCLAN.EXE AVISA : FORA DA REDE ??? !...");
	}
	SetCursor(LoadCursor(NULL,IDC_CROSS));
	closesocket(ConnectSocket);
	WSACleanup();
	return FALSE;
}

// _______________________________________

// Enviando o Buffer Inicial
// _______________________________________	

	iResult = send(ConnectSocket,sendbuf,(int)strlen(sendbuf),0);
if (iResult == SOCKET_ERROR)
{
	sprintf(szMsg,"\tSend Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SEND FALHOU",MB_OK | MB_ICONINFORMATION);
	closesocket(ConnectSocket);
	WSACleanup();
	return FALSE;
}
	sprintf(szMsg,"\tBytes Sent: %d\n", iResult);
	MessageBox(hDlg, szMsg, "Mandou Bytes",MB_OK | MB_ICONINFORMATION);

// ShutDown na Conecção (Nada mais para enviar)
	iResult = shutdown(ConnectSocket,SD_SEND);
if (iResult == SOCKET_ERROR)
{
	sprintf(szMsg,"\tShutDown Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SHUTDOWN FALHOU",MB_OK | MB_ICONINFORMATION);
	closesocket(ConnectSocket);
	WSACleanup();
	return FALSE;
}

	SetWindowText(hDlg,"PCLAN.EXE AVISA : COMANDO TCP PARA O GERADOR FOI ENVIADO !...");

// Recebe até o fim da Resposta da Conexão

	do {
		iResult = recv(ConnectSocket,recvbuf,recvbuflen,0);
		if (iResult > 0)
		{

			for(i=0;i<(int)strlen(recvbuf);i++)
			{
				szTemp[i] = recvbuf[i+6];
			}
			SetDlgItemText(hDlg,IDD_RESP,(LPCTSTR)szTemp);  //colocando a resposta no Edit_Resp

		}

		else if (iResult == 0)
		{
			sprintf(szMsg,"Conexão Fechada !!");
			MessageBox(hDlg, szMsg,"FECHOU ?",MB_OK);
		}
		else
		{
			sprintf(szMsg,"recv Falhou com ERRO: %ld\n",WSAGetLastError());
			MessageBox(hDlg, szMsg,"Que pena Falhou",MB_OK);
		}
	} while (iResult > 0);

	SetWindowText(hDlg,"PCLAN.EXE AVISA : RESPOSTA TCP RECEBIDA  DO GERADOR !...");
    // Limpando e Saindo 
	closesocket(ConnectSocket);
	WSACleanup();
	return TRUE;

}//Fechamento da Função

// ATÉ AQUI(FUNÇÃO TRIPLA)


/* Tutorial p/ alguns Comandos SCPI p/ o Gerador de Função 33220A */
bool	CmdSCPI(HWND hDlg,LPCSTR lpNome)
{
	int		iRes;
	char	bufe[40],tmp[4];
	if (iRes=WinExec("notepad scpi.txt",SW_SHOWNORMAL)>31) return TRUE;
	if (iRes<=31)	//Tivemos algum êrro
	{
		switch(iRes)
		{
			case 0:
				strcpy(bufe,"Fora da memória!");
				break;

			case 2:
				strcpy(bufe,"Arquivo não achado!!");
				break;

			case 3:
				strcpy(bufe,"Rota não achada!!");
				break;

			default:
				strcpy(bufe,"Arquivo scpi.txt não carregado:cod. de erro:");
				strcat(bufe,itoa(iRes,tmp,10));
				break;
		}

	MessageBox(hDlg,bufe,"Arquivo do scpi.txt não encontrado",
					MB_OK | MB_ICONEXCLAMATION);

	}
		return 0;
}
	/* Fim do Tutorial p/ alguns Comandos SCPI p/ o Gerador de Função 33220A */


/* Enviando Comandos para o Gerador de Função 33220A ou 33210A */
bool EnviaCMD(HWND hDlg)
{
	int		iResult, i;
	int		recvbuflen = DEFAULT_BUFLEN;
	char	szMsg[200];
	char	recvbuf[DEFAULT_BUFLEN] = "";
	char	*sendbuf = "";

	char	szIP[20] = "";						// lê o IP do gerador a ser à identificar 
	char	szCMD[80] = "";						// lê o Comando do gerador

	char	szTemp[260];
	SOCKET	ConnectSocket;
	struct sockaddr_in	clientService;
	WSADATA	wsaData;
	HWND	hCtrl;
	
// _________________________________

// Pega o IP do Gerador à Identificar
// _________________________________

	hCtrl = GetDlgItem(hDlg,IDD_EDIP);
	GetWindowText(hCtrl,(LPTSTR)szIP,16);
	if(strlen(szIP) == 0) 
	{
			MessageBox(hDlg,"Teclar o IP do Gerador ANTES","PCLAN.EXE INFORMA",MB_ICONSTOP);
			return FALSE;
	}	

	MessageBox(hDlg,(LPCTSTR)szIP,"PCLAN.EXE VAI FAZER",MB_ICONSTOP);

// ______________________________________

// Pega o Comando do Gerador para Enviar
// ______________________________________
	
	hCtrl = GetDlgItem(hDlg,IDD_CMD);
	GetWindowText(hCtrl,(LPTSTR)szCMD,80);
	if(strlen(szCMD) == 0) 
	{
			MessageBox(hDlg,"Teclar o COMANDO do Gerador ANTES","PCLAN.EXE INFORMA",MB_ICONSTOP);
			MessageBeep((UINT)MB_ICONSTOP);
			return FALSE;
	}
	
// ___________________________________

//       Inicializando Winsock
// ___________________________________

iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
if (iResult != NO_ERROR)
{
	sprintf(szMsg,"\tWSAStartup Falhou com ERRO: %d\n", iResult);
	MessageBox(hDlg, szMsg, "STARTUP FALHOU",MB_OK | MB_ICONINFORMATION);
	return FALSE;
}

// _______________________________________

// Criando Socket para Conectar ao Gerador
// _______________________________________

	ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if (ConnectSocket == INVALID_SOCKET)
{
	sprintf(szMsg,"\tSocket Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SOCKET FALHOU",MB_OK | MB_ICONINFORMATION);
	WSACleanup();
	return FALSE;
}	
// _______________________________________

// A Estrutura sockaddr_in especifica a família
// de endereço, ender. IP e porta do servidor (Gerador)
// a ser conectado.
// _______________________________________	

	clientService.sin_family = AF_INET;
	//clientService.sin_addr.s_addr = inet_addr ("143.106.150.179");	
	clientService.sin_addr.s_addr = inet_addr((LPCTSTR)szIP);
	//clientService.sin_addr.s_addr = (u_long)0x8F6A96B2;
	clientService.sin_port = htons(DEFAULT_PORT); //Esta é BOA (5024) para falar com gerador AGILENT
// _______________________________________

// Conecte ao Servidor (Gerador)
// _______________________________________	
	SetWindowText(hDlg,"PCLAN.EXE AGUARDANDO... +/-  20 [seg]  !!...");
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	iResult = connect(ConnectSocket,(SOCKADDR*)&clientService,sizeof(clientService));
if (iResult == SOCKET_ERROR)
{
	int j=WSAGetLastError(); 
	sprintf(szMsg,"\tConnect Falhou com ERRO: %ld\n", j);
	MessageBox(hDlg, szMsg, "CONNECT FALHOU",MB_OK | MB_ICONINFORMATION);
	if (j == WSAETIMEDOUT)
	{
	MessageBox(hDlg, "WSAETIMEDOUT", "CONEXÃO FALHOU",MB_OK | MB_ICONINFORMATION);
	SetWindowText(hDlg,"PCLAN.EXE AVISA : EXPIROU TEMPO DE ESPERA !!...");
	}

	if (j == WSAEHOSTUNREACH)
	{
	MessageBox(hDlg, "WSAEHOSTUNREACH)", "GERADOR FORA DA REDE!!",MB_OK | MB_ICONINFORMATION);
	SetWindowText(hDlg,"PCLAN.EXE AVISA : FORA DA REDE ??? !...");
	}
	SetCursor(LoadCursor(NULL,IDC_CROSS));
	closesocket(ConnectSocket);
	WSACleanup();
	return FALSE;
}
// _______________________________________

// Enviando o Buffer Inicial
// _______________________________________	
	strcat(szCMD,"\r\n");
	MessageBox(hDlg,szCMD,"Testando",MB_OK);

	iResult = send(ConnectSocket,szCMD,(int)strlen(szCMD),0);
if (iResult == SOCKET_ERROR)
{
	sprintf(szMsg,"\tSend Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SEND FALHOU",MB_OK | MB_ICONINFORMATION);
	closesocket(ConnectSocket);
	WSACleanup();
	return FALSE;
}
	sprintf(szMsg,"\tBytes Sent: %d\n", iResult);
	MessageBox(hDlg, szMsg, "Mandou Bytes",MB_OK | MB_ICONINFORMATION);

	iResult = shutdown(ConnectSocket,SD_SEND);	// ShutDown na Conecção (Nada mais para enviar)
if (iResult == SOCKET_ERROR)
{
	sprintf(szMsg,"\tShutDown Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SHUTDOWN FALHOU",MB_OK | MB_ICONINFORMATION);
	closesocket(ConnectSocket);
	WSACleanup();
	return FALSE;
}

	SetWindowText(hDlg,"PCLAN.EXE AVISA : COMANDO TCP PARA O GERADOR FOI ENVIADO !...");
// Recebe até o fim da Resposta da Conexão

	do {
		iResult = recv(ConnectSocket,recvbuf,recvbuflen,0);
		if (iResult > 0)
		{
			
			for(i=0;i<(int)strlen(recvbuf);i++)
			{
				szTemp[i] = recvbuf[i];
			}
			SetDlgItemText(hDlg,IDD_RESP,(LPCTSTR)szTemp); //colocando a resposta no Edit_Resp

		}

		else if (iResult == 0)
		{
			sprintf(szMsg,"Conexão Fechada Falsa !!");
			MessageBox(hDlg, szMsg,"FECHOU ?@$",MB_OK);
		}
		else
		{
			sprintf(szMsg,"recv Falhou com ERRO Dois: %ld\n",WSAGetLastError());
			WSASetLastError(0);
			MessageBox(hDlg, szMsg,"Que pena Falhou",MB_OK);
		}
	} while (iResult > 0);

	SetWindowText(hDlg,"PCLAN.EXE AVISA : RESPOSTA TCP RECEBIDA  DO GERADOR !...");
	return TRUE;

}//Fechamento da Função


/* Enviando Texto para o Receptor */
bool TXBatePapoUDP(HWND hDlg,int Dest)
{
	int		iResult;
	//int		recvbuflen = DEFAULT_BUFLEN;
	char	szMsg[50]={0};
	//char	recvbuf[DEFAULT_BUFLEN] = "";
	//char	*sendbuf = "";
	//char	szErro[30]="";
	char	szIP[20] = "";						// lê o IP do gerador a ser à identificar 
	struct	hostent	*remotoHost;
	WSADATA	wsaData;
	HWND	hCtrl;


		if  (Dest==1)  MessageBox(hDlg,"Para JAN","PCLAN.EXE INFORMA",MB_ICONSTOP);
		if  (Dest==2)  MessageBox(hDlg,"Para POR","PCLAN.EXE INFORMA",MB_ICONSTOP);
		if  (Dest==3)  MessageBox(hDlg,"Para FUN","PCLAN.EXE INFORMA",MB_ICONSTOP);
		if  (Dest==4)  MessageBox(hDlg,"Para PIA","PCLAN.EXE INFORMA",MB_ICONSTOP);
	
// _________________________________

// Pega o IP do Receptor à Identificar
// _________________________________

	if (Dest==1) hCtrl = GetDlgItem(hDlg,IDD_EDIPJAN);
	if (Dest==2) hCtrl = GetDlgItem(hDlg,IDD_EDIPPOR);
	if (Dest==3) hCtrl = GetDlgItem(hDlg,IDD_EDIPFUN);
	if (Dest==4) hCtrl = GetDlgItem(hDlg,IDD_EDIPPIA);

	GetWindowText(hCtrl,(LPTSTR)szIP,16);
	if(strlen(szIP) == 0) 
	{
			MessageBox(hDlg,"Teclar o IP do Receptor ANTES","PCLAN.EXE INFORMA",MB_ICONSTOP);
			return FALSE;
	}	

	MessageBox(hDlg,"Agora Vai??","PCLAN.EXE VAI FAZER",MB_ICONSTOP);

// ______________________________________

// Pega o Texto para Enviar
// ______________________________________
	char szTX[512]={0};
	hCtrl = GetDlgItem(hDlg,IDD_TXBP);
	GetWindowText(hCtrl,(LPTSTR)szTX,512);

	if(strlen(szTX) == 0) 
	{
			MessageBox(hDlg,"Tecle uma MENSAGEM ANTES!","PCLAN.EXE INFORMA",MB_ICONSTOP);
			MessageBeep((UINT)MB_ICONSTOP);
			return FALSE;
	}
	
			MessageBox(hDlg,szTX,"PCLAN.EXE INFORMA",MB_ICONSTOP);
// ___________________________________

//       Inicializando Winsock
// ___________________________________

iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
if (iResult != NO_ERROR)
{
	sprintf(szMsg,"\tWSAStartup Falhou com ERRO: %d\n", iResult);
	MessageBox(hDlg, szMsg, "STARTUP FALHOU",MB_OK | MB_ICONINFORMATION);
	return FALSE;
}

// _______________________________________

// Criando Socket para ligar ao Destinatário
// _______________________________________
	TXBPSocket = INVALID_SOCKET;
	TXBPSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if (TXBPSocket == INVALID_SOCKET)
{
	sprintf(szMsg,"\tSocket Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SOCKET FALHOU",MB_OK | MB_ICONINFORMATION);
	WSACleanup();
	return FALSE;
}
//INICIO de Resolvendo o IP para hostname
	//struct	hostent	*remotoHost;
	//char	nomeRem[40];

	remotoHost=gethostbyname(szIP);
		if(remotoHost==NULL)
	{
	int j=GetLastError();
	sprintf(szMsg,"\tIncapaz de resolver remotoHost > ÊRRO %ld no IP ? \n", j);	// ...remotoHost > ÊRRO no IP ? %ld\n", j);
	SetWindowText(hDlg,"PCLAN.EXE AVISA > IP ?? ");	
	if ( j == 11001 ) MessageBox(hDlg, szMsg, "HOSTNAME ?? > IP NÃO ACHADO",MB_OK | MB_ICONINFORMATION);
		//CheckDlgButton(hDlg,IDD_FUN,FALSE);
	j=GetLastError();
	TXBPSocket = INVALID_SOCKET;
	closesocket(TXBPSocket);
	WSACleanup();

	return TRUE;
	}
	MessageBox(hDlg, "PASSOU AQUI", "HOSTNAME ??",MB_OK | MB_ICONINFORMATION);
	sprintf(szMsg,"\tHOST REMOTO > :%s", remotoHost->h_name);
	MessageBox(hDlg, szMsg, "HOSTNAME ??",MB_OK | MB_ICONINFORMATION);
//FIM de  Resolvendo o IP para hostname
	
// _______________________________________

// A Estrutura sockaddr_in especifica a família
// de endereço, ender. IP e porta do PC Remoto
// a ser conectado.
// _______________________________________	


	Remoto.sin_family = AF_INET;
	//clientService.sin_addr.s_addr = inet_addr ("143.106.150.179");	
	Remoto.sin_addr.s_addr = inet_addr( szIP );
	//Remoto.sin_addr.s_addr = *((u_long*)host->h_addr_list);
	//clientService.sin_addr.s_addr = (u_long)0x8F6A96B2;
	Remoto.sin_port = htons((int)sPortUDP); //Esta é BOA (50280) para enviar p/ PC Remoto TXBATEPAPO ?

// _______________________________________

// Enviando o Buffer Inicial
// _______________________________________	
	strcat(szTX,"\r\n");
	iResult = sendto(TXBPSocket,szTX,(int)strlen(szTX),0,(SOCKADDR *)&Remoto,sizeof(SOCKADDR));
if (iResult == SOCKET_ERROR)
{
	sprintf(szMsg,"\tsendto Falhou com ERRO: %ld\n", WSAGetLastError());
	MessageBox(hDlg, szMsg, "SEND FALHOU",MB_OK | MB_ICONINFORMATION);
	closesocket(TXBPSocket);
	WSACleanup();
	return FALSE;
}
	sprintf(szMsg,"\tBYTES ENVIADOS: %d\n", iResult);
	MessageBox(hDlg, szMsg, "Mandou Bytes",MB_OK | MB_ICONINFORMATION);
	SetWindowText(hDlg,"PCLAN.EXE INFO: GARANTIA DE ENTREGA DA MENSAGEM... (UDP)??");
	iResult=WSAGetLastError();
	closesocket(TXBPSocket);	//ADICIONEI ISSO
    // Limpando e Saindo 
	//TXBPSocket=INVALID_SOCKET;

	WSACleanup();
	return FALSE;
	//return TRUE;	//ERA ISSO
	
} //Fechamento da Função TXBatePapoUDP	
	
	
/* Arquivo fonte  */
bool	ArqFonte(HWND hDlg,LPCSTR lpArq)
{
	int		iRes;
	char	bufe[40],tmp[4];
	char	bif[60]={0};
	if(lpArq=="notepad resource.h")  strcpy(bif,"resource.h");
	if(lpArq=="notepad pclan.rc")    strcpy(bif,"pclan.rc");
	if(lpArq=="notepad pclan.cpp")  strcpy(bif,"pclan.cpp");

		iRes = MessageBox(hDlg, bif,
				"**  ARQUIVO FONTE ? **", MB_YESNO | MB_ICONINFORMATION | MB_ICONHAND);
			if(iRes == IDNO)
			{
			return FALSE;
			}
			if(iRes == IDYES) 
			{
				MessageBeep(MB_ICONHAND);
			}

	if (iRes=WinExec(lpArq,SW_SHOWNORMAL)>31) return TRUE;
	if (iRes<=31)	//Tivemos algum êrro
	{
		switch(iRes)
		{
			case 0:
				strcpy(bufe,"Fora da memória!");
				break;

			case 2:
				strcpy(bufe,"Arquivo não achado!!");
				break;

			case 3:
				strcpy(bufe,"Rota não achada!!");
				break;

			default:
				strcpy(bufe,"Arquivo pedido mas não carregado:cod. de erro:");
				strcat(bufe,itoa(iRes,tmp,10));
	}

	MessageBox(hDlg,bufe,lpArq,
					MB_OK | MB_ICONEXCLAMATION);

	}
		return FALSE;
}
	/* Fim de Arquivo Fonte */