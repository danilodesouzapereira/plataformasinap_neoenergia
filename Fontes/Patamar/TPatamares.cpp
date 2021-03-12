//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPatamares.h"
#include "VTPatamar.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPatamares* __fastcall NewObjPatamares(VTApl *apl)
   {
   try{
      return(new TPatamares(apl));
      }catch(Exception &e)
         { }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TPatamares::TPatamares(VTApl *apl)
   {
   //salva ponteiro p/ objeto
	this->apl = apl;
	try{//cria lista de Patamar
      lisPAT = new TList();
      }catch(Exception &e)
			{
         }
   //inicia Tipo como de 1Hora
   Tipo = pat01HORA;
   }

//---------------------------------------------------------------------------
// ~TPatamares() - Destrutor
//---------------------------------------------------------------------------
__fastcall TPatamares::~TPatamares(void)
   {
   //destrói lista e seus objetos
   if (lisPAT) {LimpaTList(lisPAT); delete lisPAT; lisPAT = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CopiaAtributosDe(VTPatamares &patamares)
   {
   //variáveis locais
   VTPatamar *patamar, *patamar_orig;

   //redefine tipo (que reinicia os objetos VTPatamar)
   Tipo = patamares.Tipo;
   //copia dados de cada Patamar
   for (int np = 0; np < lisPAT->Count; np++)
      {//determina objetos VTPatamar
      patamar      = Patamar[np];
      patamar_orig = patamares.Patamar[np];
      //copia dados
      patamar->Enabled      = patamar_orig->Enabled;
      patamar->HoraCal      = patamar_orig->HoraCal;
      patamar->MinutoCal    = patamar_orig->MinutoCal;
      patamar->FluxoCorreto = patamar_orig->FluxoCorreto;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar01HORA(void)
   {
   //variáveis locais
   int index  = 0;

   //define nome
   PD.nome = "024 patamares(1 hora)";
	try{//cria Patamar
      for (int hora = 0; hora <= 23; hora++)
         {//cria um Patamar e insere em lisPAT
         lisPAT->Add(NewObjPatamar(index, hora, 0, hora+1, 0));
         index++;
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar02HORA(void)
   {
   //variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

	//define nome
   PD.nome = "012 patamares(2 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=2)
         {//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+2, 0));
			h_cal = hora+1;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
			index++;
			}
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar03HORA(void)
   {
   //variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

   //define nome
   PD.nome = "008 patamares(3 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=3)
         {//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+3, 0));
			h_cal = hora+1;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
			index++;
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar04HORA(void)
   {
   //variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

   //define nome
   PD.nome = "006 patamares(4 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=4)
         {//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+4, 0));
			h_cal = hora+2;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
			index++;
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar06HORA(void)
   {
   //variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

   //define nome
   PD.nome = "004 patamares(6 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=6)
         {//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+6, 0));
			h_cal = hora+3;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
         index++;
			}
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar08HORA(void)
   {
   //variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

   //define nome
   PD.nome = "003 patamares(8 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=8)
			{//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+8, 0));
			h_cal = hora+4;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
			index++;
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar12HORA(void)
   {
	//variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

   //define nome
   PD.nome = "002 patamares(12 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=12)
         {//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+12, 0));
			h_cal = hora+6;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
			index++;
         }
      }catch(Exception &e)
			{ }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar24HORA(void)
   {
	//variáveis locais
	int index = 0, h_cal, m_cal;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar	*patamar;

   //define nome
   PD.nome = "001 patamar  (24 horas)";
   try{//cria Patamar
      for (int hora = 0; hora <= 23; hora+=24)
         {//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, hora, 0, hora+24, 0));
			h_cal = hora+12;
			m_cal = 0;
			if(geral)
				{
				if(geral->Infoset->LoadFromFile("TPatamares"))
					{//recupera do Infoset
					geral->Infoset->GetInfo(patamar->Nome+"_h_cal", h_cal);
					geral->Infoset->GetInfo(patamar->Nome+"_m_cal", m_cal);
					}
				}
			patamar->HoraCal = h_cal;
			patamar->MinutoCal = m_cal;
			}
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar05MIN(void)
   {
	//variáveis locais
   int       index   = 0;
   int       num_min = 5;
   int       hora_fim, minuto_fim;

   //define nome
   PD.nome = "288 patamares(5 minutos)";
   try{//cria Patamar
      for (int hora_ini = 0; hora_ini <= 23; hora_ini++)
         {
         for (int minuto_ini = 0; minuto_ini < 60; minuto_ini+=num_min)
            {//define minuto_fim e hora_fim
            hora_fim = hora_ini;
            if ((minuto_fim = minuto_ini + num_min) >= 60)
               {
               minuto_fim = 0;
               hora_fim++;
               }
            //cria um Patamar e insere em lisPAT
            lisPAT->Add(NewObjPatamar(index, hora_ini, minuto_ini, hora_fim, minuto_fim));
            index++;
            }
         }
		}catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar10MIN(void)
   {
   //variáveis locais
   int       index   = 0;
   int       num_min = 10;
   int       hora_fim, minuto_fim;

   //define nome
   PD.nome = "144 patamares(10 minutos)";
   try{//cria Patamar
      for (int hora_ini = 0; hora_ini <= 23; hora_ini++)
         {
         for (int minuto_ini = 0; minuto_ini < 60; minuto_ini+=num_min)
            {//define minuto_fim e hora_fim
            hora_fim = hora_ini;
            if ((minuto_fim = minuto_ini + num_min) >= 60)
               {
               minuto_fim = 0;
					hora_fim++;
               }
            //cria um Patamar e insere em lisPAT
            lisPAT->Add(NewObjPatamar(index, hora_ini, minuto_ini, hora_fim, minuto_fim));
            index++;
            }
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamar15MIN(void)
   {
   //variáveis locais
   int       index   = 0;
   int       num_min = 15;
   int       hora_fim, minuto_fim;

   //define nome
   PD.nome = "96 patamares(15 minutos)";
   try{//cria Patamar
      for (int hora_ini = 0; hora_ini <= 23; hora_ini++)
			{
         for (int minuto_ini = 0; minuto_ini < 60; minuto_ini+=num_min)
            {//define minuto_fim e hora_fim
            hora_fim = hora_ini;
            if ((minuto_fim = minuto_ini + num_min) >= 60)
               {
               minuto_fim = 0;
               hora_fim++;
               }
            //cria um Patamar e insere em lisPAT
            lisPAT->Add(NewObjPatamar(index, hora_ini, minuto_ini, hora_fim, minuto_fim));
            index++;
            }
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamarMMTN(void)
	{
	//variáveis locais
	int 			index = 0;
	VTGeral 		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar 	*patamar;
	struct   { //valores padrão
				char* nome;
				int   hora_ini;
				int   min_ini;
				int   hora_fim;
				int   min_fim;
				int   hora_cal;
				int   min_cal;
				} pat[4] = {{"Madrugada", 1, 0, 7, 0, 3, 0},
								{"Manhã",     7, 0,13, 0,10, 0},
								{"Tarde",    13, 0,19, 0,18, 0},
								{"Noite",    19, 0, 1, 0,23, 0}};

	if(geral)
		{//define valores do Infoset
		if(geral->Infoset->LoadFromFile("TPatamares"))
			{
			for(int np = 0; np < 4; np++)
				{//atualiza pat com os horarios do Infoset
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_h_ini", pat[np].hora_ini);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_m_ini", pat[np].min_ini);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_h_fim", pat[np].hora_fim);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_m_fim", pat[np].min_fim);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_h_cal", pat[np].hora_cal);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_m_cal", pat[np].min_cal);
				}
			}
		}
	//define nome
	PD.nome = "Madrugada/Manhã/Tarde/Noite";
	try{//cria Patamar
		for (int np = 0; np < 4; np++)
			{//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, pat[np].hora_ini, pat[np].min_ini,
																	pat[np].hora_fim, pat[np].min_fim));
			patamar->Nome       = pat[np].nome;
			patamar->HoraCal    = pat[np].hora_cal;
			patamar->MinutoCal  = pat[np].min_cal;
			index++;
			}
		}catch(Exception &e)
         { }
	}

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamarLMP(void)
   {
	//variáveis locais
   char*       str_aux;
	int index = 0;
	VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamar   *patamar;
   struct   {
				char* nome;
				int   hora_ini;
				int   min_ini;
				int   hora_fim;
				int   min_fim;
				int   hora_cal;
				int   min_cal;
				} pat[4] = {{"Leve",   22, 0, 6, 0, 3, 0},
								{"Média",   6, 0,15, 0,12, 0},
								{"Pesada", 15, 0,22, 0,17, 0},
                        {"Aux",     0, 0, 0, 0, 0, 0}};

	if(geral)
		{//define valores do Infoset
		if(geral->Infoset->LoadFromFile("TPatamares"))
			{
			for(int np = 0; np < 3; np++)
				{//atualiza pat com os horarios do Infoset
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_h_ini", pat[np].hora_ini);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_m_ini", pat[np].min_ini);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_h_fim", pat[np].hora_fim);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_m_fim", pat[np].min_fim);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_h_cal", pat[np].hora_cal);
				geral->Infoset->GetInfo(AnsiString(pat[np].nome)+"_m_cal", pat[np].min_cal);
				}
			}
      //reordena patamares
	  /* if (pat[1].hora_fim != pat[2].hora_ini)
			{
			pat[3] = pat[2];
			pat[2] = pat[1];
			pat[1] = pat[3];
         }*/
      }
	//define nome
   PD.nome = "Carga: Leve/Média/Pesada";
   try{//cria Patamar
		for (int np = 0; np < 3; np++)
			{//cria um Patamar e insere em lisPAT
			lisPAT->Add(patamar = NewObjPatamar(index, pat[np].hora_ini, pat[np].min_ini,
																	pat[np].hora_fim, pat[np].min_fim));
			patamar->Nome       = pat[np].nome;
			patamar->HoraCal    = pat[np].hora_cal;
			patamar->MinutoCal  = pat[np].min_cal;
         index++;
			}
      }catch(Exception &e)
			{ }
	}/*
//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamarMMTN(void)
   {
//CÓDIGO TEMPORÁRIO
   VTPatamar *patamar;
   struct   {
            char* nome;
            int   hora_ini;
            int   hora_fim;
            int   hora_cal;
				} pat[4] = {{"Madrugada", 1, 7, 3},
								{"Manhã",     7,13,10},
								{"Tarde",    13,19,18},
								{"Noite",    19, 1,23}};
   //variáveis locais
   int index = 0;

   //define nome
   PD.nome = "Madrugada/Manhã/Tarde/Noite";
   try{//cria Patamar
      for (int np = 0; np < 4; np++)
         {//cria um Patamar e insere em lisPAT
         lisPAT->Add(patamar = NewObjPatamar(index, pat[np].hora_ini, 0, pat[np].hora_fim, 0));
         patamar->Nome       = pat[np].nome;
         patamar->HoraCal    = pat[np].hora_cal;
         patamar->MinutoCal  = 0;
         index++;
         }
      }catch(Exception &e)
         { }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::CriaPatamarLMP(void)
   {
//CÓDIGO TEMPORÁRIO
   VTPatamar *patamar;
   struct   {
            char* nome;
            int   hora_ini;
            int   hora_fim;
            int   hora_cal;
				} pat[3] = {{"Leve",   22, 6, 3},
								{"Média",   6,15,12},
								{"Pesada", 16,22,17}};
   //variáveis locais
   int index = 0;

   //define nome
   PD.nome = "Carga: Leve/Média/Pesada";
   try{//cria Patamar
      for (int np = 0; np < 3; np++)
         {//cria um Patamar e insere em lisPAT
         lisPAT->Add(patamar = NewObjPatamar(index, pat[np].hora_ini, 0, pat[np].hora_fim, 0));
         patamar->Nome       = pat[np].nome;
         patamar->HoraCal    = pat[np].hora_cal;
         patamar->MinutoCal  = 0;
         index++;
         }
      }catch(Exception &e)
         { }
	} */
/*
//---------------------------------------------------------------------------
void __fastcall TPatamares::Enable(VTPatamar *patamar, bool enabled)
   {
   patamar->Enabled = enabled;
   }
*/
//---------------------------------------------------------------------------
VTPatamar*   __fastcall TPatamares::ExistePatamar(int hora, int minuto)
   {
   //variáveis locais
   int       min, min_ini, min_fim;
   VTPatamar *patamar;

   //converte horário indicado p/ minutos
   min = (60 * hora) + minuto;
   //verifica se o período do Patamar abrange o horário indicado
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      //determina minuto inicial e final do Patamar
      min_ini = (60 * patamar->HoraIni) + patamar->MinutoIni;
      min_fim = (60 * patamar->HoraFim) + patamar->MinutoFim;
      //verifica se intervalo inclui a virada do dia
      if (min_ini < min_fim)
         {//intervalo NÃO INCLUI virada do dia
         if ((min >= min_ini)&&(min < min_fim)) return(patamar);
         }
      else
         {//intervalo INCLUI virada do dia
         if ((min < min_fim)||(min >= min_ini)) return(patamar);
         }
      }
   return(NULL);
   }


//---------------------------------------------------------------------------
TList* __fastcall TPatamares::LisPatamar(void)
   {
   return(lisPAT);
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::LisPatamar(TList *lisEXT)
   {
   //copia lisPAT em lisEXT
   lisEXT->Clear();
   CopiaTList(lisPAT, lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::LisPatamarHabilitado(TList *lisEXT)
   {
   //variáveis locais
   VTPatamar *patamar;

   //copia Patamar habilitado em lisEXT
   lisEXT->Clear();
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (patamar->Enabled) lisEXT->Add(patamar);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TPatamares::NumPatamar(void)
   {
   return(lisPAT->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TPatamares::NumPatamarHabilitado(void)
   {
   //variáveis locais
   VTPatamar *patamar;
   int       num_pat = 0;

   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (patamar->Enabled) num_pat++;
      }
   return(num_pat);
   }

//---------------------------------------------------------------------------
int __fastcall TPatamares::PM_GetTipo(void)
   {
   return(PD.tipo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TPatamares::PM_GetNome(void)
   {
   return(PD.nome);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TPatamares::PM_GetPatamar(int np)
   {
   //proteção
   if ((np < 0)||(np >= lisPAT->Count)) return(NULL);
   return((VTPatamar*)(lisPAT->Items[np]));
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TPatamares::PM_GetPatamarAtivo(void)
	{
	return(PD.patamar_ativo);
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::PM_SetTipo(int tipo)
   {
   PD.tipo = tipo;
   //cancela patamar ativo
   Patamar_ativo = NULL;
   //destrói conjunto de Patamar atual
   LimpaTList(lisPAT);
   //reinicia conjunto de Patamar
   switch(tipo)
		{
		case pat05MIN:    CriaPatamar05MIN();    break;
      case pat10MIN:    CriaPatamar10MIN();    break;
      case pat15MIN:    CriaPatamar15MIN();    break;
		case pat01HORA:   CriaPatamar01HORA();   break;
		case pat02HORA:   CriaPatamar02HORA();   break;
      case pat03HORA:   CriaPatamar03HORA();   break;
      case pat04HORA:   CriaPatamar04HORA();   break;
      case pat06HORA:   CriaPatamar06HORA();   break;
      case pat08HORA:   CriaPatamar08HORA();   break;
      case pat12HORA:   CriaPatamar12HORA();   break;
      case pat24HORA:   CriaPatamar24HORA();   break;
      case patMMTN:     CriaPatamarMMTN();     break;
      case patLMP:      CriaPatamarLMP();      break;
      default:          CriaPatamar01HORA();   break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::PM_SetPatamarAtivo(VTPatamar *patamar)
   {
   PD.patamar_ativo = patamar;
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TPatamares::PrimeiroPatamarHabilitado(void)
   {
   //variáveis locais
   VTPatamar *patamar;

   //retorna primeiro patamar habilitado
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (patamar->Enabled) return(patamar);
      }
   //não há nenhum Patamar habilitado
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TPatamares::ReiniciaDiagFluxo(bool fluxo_correto)
   {
   //variáveis locais
   VTPatamar *patamar;

   //copia Patamar habilitado em lisEXT
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      patamar->FluxoCorreto = fluxo_correto;
      }
   }

//---------------------------------------------------------------------------
//eof
