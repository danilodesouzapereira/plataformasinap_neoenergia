//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMicro.h"
//#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTMicro* __fastcall NewObjMicro(void)
   {
   return(new TMicro());
   }

//---------------------------------------------------------------------------
__fastcall TMicro::TMicro(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMicro::~TMicro(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TMicro::CPU_DataAvailable(void)
   {
   /*
   //variáveis locais
   #define ID_BIT  0x200000
   bool    result = false;

   asm{
	   PUSHFD						//{direct access to flags no possible, only via stack}
      POP     EAX					//{flags to EAX}
      MOV     EDX,EAX			//{save current flags}
      XOR     EAX,ID_BIT    	//{not ID bit}
      PUSH    EAX					//{onto stack}
      POPFD							//{from stack to flags, with not ID bit}
      PUSHFD						//{back to stack}
      POP     EAX					//{get back to EAX}
      XOR     EAX,EDX			//{check if ID bit affected}
	  JZ      @exit				//{no, CPUID not availavle}
      MOV     AL,True			//{Result=True}
      MOV     result, AL
      @exit:
      }
   return(result);
   */
   return true;
   }

//---------------------------------------------------------------------------
void __fastcall TMicro::CPU_GetID(unsigned *cpu_id)
   {
   /*
   asm{
      PUSH    EAX          //Save affected register
      PUSH    EBX          //Save affected register
      PUSH    ECX          //Save affected register
      PUSH    EDX          //Save affected register
      PUSH    EDI          //Save affected register
      MOV     EAX, cpu_id
      MOV     EDI,EAX      //{cpu_id}
      MOV     EAX,1
      DW      0xA20F       //{CPUID Command}
      STOSD			         //{cpu_id[0]}
      MOV     EAX,EBX
      STOSD                //{cpu_id[1]}
      MOV     EAX,ECX
      STOSD                //{cpu_id[2]}
      MOV     EAX,EDX
      STOSD                //{cpu_id[3]}
      POP     EDI			   //{Restore registers}
      POP     EDX
      POP     ECX
      POP     EBX
      POP     EAX
	  }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TMicro::CPU_GetVendor(char *vendor)
   {
   /*
   asm{
      PUSH    EAX					//{Save affected register}
      PUSH    EBX					//{Save affected register}
      PUSH    EDI
      MOV     EAX, vendor
      MOV     EDI,EAX			//{@Result (TVendor)}
      MOV     EAX,0
      DW      0xA20F				//{CPUID Command}
      MOV     EAX,EBX
      XCHG	  EBX,ECX         //{save ECX result}
      MOV	  ECX,4
      @1:
      STOSB
      SHR     EAX,8
      LOOP    @1
      MOV     EAX,EDX
      MOV	  ECX,4
      @2:
      STOSB
      SHR     EAX,8
      LOOP    @2
      MOV     EAX,EBX
      MOV     ECX,4
      @3:
      STOSB
      SHR     EAX,8
      LOOP    @3
      POP     EDI					//{Restore registers}
      POP     EBX
      POP     EAX
	  }
   */
   vendor = "12345";
   }

/*
//---------------------------------------------------------------------------
AnsiString __fastcall TMicro::PM_GetCPU_Id(void)
	{
	//variáveis locais
	AnsiString str_id;
	unsigned   cpu_id[4] = {0,0,0,0};

	//verifica se a informação de CPU_ID é disponível
	if (CPU_DataAvailable())
		{//obtém ID da CPU
		CPU_GetID(cpu_id);
		//monta str_id c/ ID da CPU
		str_id.sprintf("%04x.%04x.%04x.%04x", cpu_id[0], cpu_id[1], cpu_id[2], cpu_id[3]);
//Aviso("CPU ID = " + str_id);
		}
	return(str_id);
	}
*/
//---------------------------------------------------------------------------
AnsiString __fastcall TMicro::PM_GetCPU_Id(void)
   {
   //variáveis locais
   AnsiString str_id;
   unsigned   cpu_id[4] = {0,0,0,0};

   //verifica se a informação de CPU_ID é disponível
   if (CPU_DataAvailable())
      {//obtém ID da CPU
      CPU_GetID(cpu_id);
//PROTEÇÃO MICRO DUOL CORE: zera cpu_id[1]
cpu_id[1] = 0;
      //monta str_id c/ ID da CPU
		str_id.sprintf("%04x.%04x.%04x.%04x", cpu_id[0], cpu_id[1], cpu_id[2], cpu_id[3]);
		}
   return(str_id);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMicro::PM_GetCPU_Vendor(void)
   {
   //variáveis locais
   AnsiString str_vendor;
   char       vendor[64];

   //inicia vetor vendor
   memset(vendor, '\0', sizeof(vendor));
   //verifica se há informação na CPU
   if (CPU_DataAvailable())
      {//obtém Vendor da CPU
      CPU_GetVendor(vendor);
      str_vendor = AnsiString(vendor);
      }
   return(str_vendor);
   }

//---------------------------------------------------------------------------
//eof
