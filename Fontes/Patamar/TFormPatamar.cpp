//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormPatamar.h"
#include "VTPatamar.h"
#include "VTPatamares.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormPatamar::TFormPatamar(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objeto
   lisPAT = new TList();
   //cria objetos Patamares
   CriaPatamares();
   //inicia RGroupPatamar
	RGroupPatamarInicia();
	//posiciona o Form
	FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormPatamar::~TFormPatamar(void)
	{
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	//salva parâmetros do Form
	if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista e seus objetos
   if (lisPAT) {LimpaTList(lisPAT); delete lisPAT; lisPAT = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::ActionConfirmaExecute(TObject *Sender)
	{
	//salva valores se a aba Definir instante de ref. estiver habilitada
	if(TabSheetHoraCal->TabVisible)
		{//salva valores da configuração
		if (! SalvaValores()) return;
		}
   //salva Patamares selecionado
   if (! SalvaPatamares()) return;
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::ActionDisablePatamarExecute(TObject *Sender)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares;

   //determina Patamares selecionado em RGroupPatamares
   if (RGroupPatamares->ItemIndex < 0) return;
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //desabilita todo Patamar de Pataamres
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = (VTPatamar*)patamares->Patamar[ind_pat];
      patamar->Enabled = false;
      }
   //reinicia CLBoxPatamar
   CLBoxPatamarInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::ActionEnablePatamarExecute(TObject *Sender)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares;

   //determina Patamares selecionado em RGroupPatamares
   if (RGroupPatamares->ItemIndex < 0) return;
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //desabilita todo Patamar de Pataamres
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = (VTPatamar*)patamares->Patamar[ind_pat];
      patamar->Enabled = true;
      }
   //reinicia CLBoxPatamar
   CLBoxPatamarInicia();
   }
//---------------------------------------------------------------------------
bool __fastcall TFormPatamar::AtualizaPatamar(VTPatamar *patamar, TDateTime date_time_ini,TDateTime date_time_fim,TDateTime date_time_cal)
	{
	//variaveis locais
	unsigned short  hour, min, sec, msec;

	//obtém hora e minuto editado: inicial
	date_time_ini.DecodeTime(&hour, &min, &sec, &msec);
	patamar->HoraIni = int(hour);
	patamar->MinutoIni = int(min);
	//obtém hora e minuto editado: final
	date_time_fim.DecodeTime(&hour, &min, &sec, &msec);
	patamar->HoraFim = int(hour);
	patamar->MinutoFim = int(min);
	//obtém hora e minuto editado: referencia
	date_time_cal.DecodeTime(&hour, &min, &sec, &msec);
	patamar->HoraCal = int(hour);
	patamar->MinutoCal = int(min);

	return(true);
   }
//---------------------------------------------------------------------------
void __fastcall TFormPatamar::CLBoxPatamarClickCheck(TObject *Sender)
   {
   //variáveis locais
   int         index;
   VTPatamar   *patamar;

   //proteção
   if ((index  = CLBoxPatamar->ItemIndex) < 0) return;
   //determina patamar associado
   if ((patamar = (VTPatamar*)CLBoxPatamar->Items->Objects[index]) == NULL) return;
   //atualiza Patamar
   patamar->Enabled = CLBoxPatamar->Checked[index];
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::CLBoxPatamarInicia(void)
   {
   //variáveis locais
   int         index;
   VTPatamar   *patamar;
   VTPatamares *patamares;

   //reinicia CLBoxPatamar
   CLBoxPatamar->Clear();
   //determina Patamares selecionado em RGroupPatamares
   if (RGroupPatamares->ItemIndex < 0) return;
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //exibe Patamar em CLBoxPatamar
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
		patamar = patamares->Patamar[ind_pat];
		index = CLBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      CLBoxPatamar->Checked[index] = patamar->Enabled;
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::CriaPatamares(void)
   {
   //variáveis locais
   int         tipo[] =   {pat01HORA,   pat02HORA,  pat03HORA,  pat04HORA,
                           pat06HORA,   pat08HORA,  pat12HORA,  pat24HORA,
                           pat15MIN,    pat10MIN,   pat05MIN,
                           patMMTN,     patLMP};
   struct   {
            VTPatamares *patamares;
            }orig, tmp;

   //determina objeto Patamares que está em Apl
	orig.patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //reinicia lisPAT
   LimpaTList(lisPAT);
   for (int n = 0; n < sizeof(tipo)/sizeof(int); n++)
      {//cria objeto Patamares e define seu tipo
      lisPAT->Add(tmp.patamares = NewObjPatamares(apl));
      tmp.patamares->Tipo = tipo[n];
      //verifica se o Patamares criado é do mesmo tipo do Patamares  atual
      if (tmp.patamares->Tipo == orig.patamares->Tipo)
         {//copia Patamares original
         tmp.patamares->CopiaAtributosDe(*orig.patamares);
         }
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
	}

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::RGroupPatamarInicia(void)
   {
   //variáveis locais
   struct   {
            VTPatamares *patamares;
            }orig, tmp;

   //determina objeto Patamares que está em Apl
   orig.patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //reinicia RGroupPatamares
   RGroupPatamares->Items->Clear();
   for (int n = 0; n < lisPAT->Count; n++)
      {//atualiza escala de Patamares p/ obter seu nome
      tmp.patamares = (VTPatamares*)lisPAT->Items[n];
      RGroupPatamares->Items->AddObject(tmp.patamares->Nome, tmp.patamares);
      }
   //pré seleciona Patamares igual ao original
   if (RGroupPatamares->Items->Count > 0)
      {
      RGroupPatamares->ItemIndex = RGroupPatamares->Items->IndexOf(orig.patamares->Nome);
      if (RGroupPatamares->ItemIndex < 0) RGroupPatamares->ItemIndex = 0;
      }
   //exibe Patamares em CLBoxPatamar
   CLBoxPatamarInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::RGroupPatamaresClick(TObject *Sender)
   {
   //variáveis locais
   VTPatamares *patamares;

   //determina VTPatamares selecionado
   if (RGroupPatamares->ItemIndex < 0) return;
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //exibe Patamares em CLBoxPatamar
   CLBoxPatamarInicia();
	StringGridConfigInicia();
   //verifica se deve esconder edição de data/hora de cálculo dos patamares
   if (patamares->NumPatamar() < 24)
      {//habilita edição de data/hora de cálculo dos patamares
		TabSheetHoraCal->TabVisible = true;
		}
   else
      {//desabilita edição de data/hora de cálculo dos patamares
		PageControl->ActivePage = TabSheetHabilita;
		TabSheetHoraCal->TabVisible = false;
		}
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormPatamar::StringGridConfigInicia(void)
   {
   //variáveis locais
   AnsiString   hm_cal;
   VTPatamar   *patamar;
   VTPatamares *patamares;

   //Limpa StringGrid
   LimpaTStringGrid(StringGridConfig);
   
   //inicia StringGrid
   StringGridConfig->Cells[0][0] = "Patamar";
   StringGridConfig->Cells[1][0] = "Hora:Minuto";

   //determina patamar selecionado em RGroupPatamares
   if (RGroupPatamares->ItemIndex < 0) return;
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //verifica se Patamares c/ 24 ou mais Patamar
   if (patamares->NumPatamar() >= 24)
      {//não edita hora/minuto de cálculo
      StringGridConfig->RowCount = 2;
      StringGridConfig->FixedRows = 1;
      return;
      }
   //exibe patamar na StringGrid
   StringGridConfig->RowCount = patamares->NumPatamar()+1;
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = (VTPatamar*)patamares->Patamar[ind_pat];
      hm_cal.sprintf("%02d:%02d", patamar->HoraCal, patamar->MinutoCal);
		StringGridConfig->Cells[0][ind_pat+1] = patamar->Periodo;
		StringGridConfig->Cells[1][ind_pat+1] = hm_cal;
      }
   }
 //---------------------------------------------------------------------------
 */
 void __fastcall TFormPatamar::StringGridConfigInicia(void)
   {
   //variáveis locais
	AnsiString   hm_ini, hm_fim, hm_cal;
	VTPatamar   *patamar;
   VTPatamares *patamares;

   //Limpa StringGrid
	LimpaTStringGrid(StringGridConfig);
   
	//inicia StringGrid
	StringGridConfig->ColCount = 4;
	StringGridConfig->Cells[1][0] = "Hora Inicial";
	StringGridConfig->Cells[2][0] = "Hora Final";
	StringGridConfig->Cells[3][0] = "Referência";

   //determina patamar selecionado em RGroupPatamares
   if (RGroupPatamares->ItemIndex < 0) return;
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //verifica se Patamares c/ 24 ou mais Patamar
   if (patamares->NumPatamar() >= 24)
      {//não edita hora/minuto de cálculo
		StringGridConfig->RowCount = 4;
		StringGridConfig->FixedRows = 1;
      return;
      }
   //verifica se é patamar Leve/media/pesada
   if (patamares->Tipo == patLMP)
      {//edita nome Leve/media/pesada
		StringGridConfig->FixedCols = 0;
      }
   else 	StringGridConfig->FixedCols = 1;
   //exibe patamar na StringGrid
	StringGridConfig->RowCount = patamares->NumPatamar()+1;
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
		patamar = (VTPatamar*)patamares->Patamar[ind_pat];
		hm_ini.sprintf("%02d:%02d", patamar->HoraIni, patamar->MinutoIni);
		hm_fim.sprintf("%02d:%02d", patamar->HoraFim, patamar->MinutoFim);
		hm_cal.sprintf("%02d:%02d", patamar->HoraCal, patamar->MinutoCal);
		StringGridConfig->Cells[0][ind_pat+1] = patamar->Nome;
		StringGridConfig->Cells[1][ind_pat+1] = hm_ini;
		StringGridConfig->Cells[2][ind_pat+1] = hm_fim;
		StringGridConfig->Cells[3][ind_pat+1] = hm_cal;
		}
   }

//---------------------------------------------------------------------------
void __fastcall TFormPatamar::LimpaTStringGrid(TStringGrid *StringGrid)
   {
   //limpa todos os valores da StringGrid
   for (int lin = 1; lin < StringGrid->RowCount; lin ++)
      {
      for (int col = 0; col < StringGrid->ColCount; col++)
         {
         StringGrid->Cells[col][lin] = "";
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormPatamar::SalvaPatamares(void)
   {
   //variáveis locais
   struct   {
            VTPatamares *patamares;
            }orig, tmp;

   //proteção
   if (RGroupPatamares->ItemIndex < 0)
      {
      Aviso("Selecione um padrão de patamares");
      return(false);
      }
   //determina Patamares selecionado
   tmp.patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //atualiza Patamares existente em Apl
   orig.patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   orig.patamares->CopiaAtributosDe(*tmp.patamares);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormPatamar::SalvaValores(void)
	{
	//variaveis locais
	unsigned short  hour, min, sec, msec;
	AnsiString		 pat1, pat2;
	AnsiString		 str_tempo_ini, str_tempo_fim;
	TDateTime       date_time_ini,date_time_fim,date_time_cal;
	VTPatamar       *patamar;
	VTPatamares     *patamares;

	//valida valores antes de salvar
	if (! ValidaValores(&pat1, &pat2))
		{
		Erro("Patamares ou intervalos descontínuos: "+ pat1 + pat2);
		return(false);
		}
   //determina patamar selecionado em RGroupPatamares
	if (RGroupPatamares->ItemIndex < 0) return (false);
   patamares = (VTPatamares*)RGroupPatamares->Items->Objects[RGroupPatamares->ItemIndex];
   //verifica se Patamares possui 24 ou mais Patamar
   if (patamares->NumPatamar() >= 24) return (true);
   //lê hora/minuto de cálculo de cada Patamar
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = (VTPatamar*)patamares->Patamar[ind_pat];
		//obtém hora e minuto editado
		try{//lê hora/minuto de cálculo de cada patamar
			str_tempo_ini = StringGridConfig->Cells[1][ind_pat+1];
			str_tempo_fim = StringGridConfig->Cells[2][ind_pat+1];
			//corrige tempo (DateTime não reconhece 24:00)
			if(str_tempo_ini.AnsiCompareIC("24:00") == 0) str_tempo_ini = "00:00";
			if(str_tempo_fim.AnsiCompareIC("24:00") == 0) str_tempo_fim = "00:00";
			//converte para formato hora
			date_time_ini = TDateTime(str_tempo_ini);
			date_time_fim = TDateTime(str_tempo_fim);
			date_time_cal = TDateTime(StringGridConfig->Cells[3][ind_pat+1]);
         //caso Leve/media/pesada
         if(patamares->Tipo == patLMP)
            {
            patamar->Nome = StringGridConfig->Cells[0][ind_pat+1];
            if((patamar->Nome.AnsiPos("Leve")==0)&&
               (patamar->Nome.AnsiPos("Média")==0)&&
               (patamar->Nome.AnsiPos("Pesada")==0))
               {
               Erro("Os patamares devem ser chamados 'Leve', 'Média' ou 'Pesada'");
		         return(false);
               }
            }
			}catch(Exception &e)
				{
				Erro("Hora/minuto inválidos p/ o patamar "
						+ StringGridConfig->Cells[0][ind_pat+1]);
				return(false);
				}

		//obtém hora e minuto editado: referencia
		//date_time_cal.DecodeTime(&hour, &min, &sec, &msec);
		//valida hora/minuto
		if (! ValidaHoraMinutoCalculo(patamar, date_time_cal))
			{
			Erro("Hora/minuto de cálculo fora do intervalo do patamar "
					+ StringGridConfig->Cells[0][ind_pat+1]);
			return(false);
			}
      //atualiza Patamar
		//patamar->HoraCal   = int(hour);
		//patamar->MinutoCal = int(min);
		if(AtualizaPatamar(patamar, date_time_ini, date_time_fim, date_time_cal))
			SalvaValoresInfoset(patamar->Nome, date_time_ini, date_time_fim, date_time_cal);
		}
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TFormPatamar::SalvaValoresInfoset(AnsiString nome_pat, TDateTime date_time_ini,TDateTime date_time_fim,TDateTime date_time_cal)
	{
	//variaveis locais
	unsigned short  hour, min, sec, msec;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
		{
		//obtém hora e minuto editado: inicial
		date_time_ini.DecodeTime(&hour, &min, &sec, &msec);
		//if (geral) geral->Infoset->SaveToFile("TPatamares");
		geral->Infoset->AddInfo(nome_pat+"_h_ini", hour);
		geral->Infoset->AddInfo(nome_pat+"_m_ini", min);
		//obtém hora e minuto editado: final
		date_time_fim.DecodeTime(&hour, &min, &sec, &msec);
		geral->Infoset->AddInfo(nome_pat+"_h_fim", hour);
		geral->Infoset->AddInfo(nome_pat+"_m_fim", min);
		//obtém hora e minuto editado: referencia
		date_time_cal.DecodeTime(&hour, &min, &sec, &msec);
		geral->Infoset->AddInfo(nome_pat+"_h_cal", hour);
		geral->Infoset->AddInfo(nome_pat+"_m_cal", min);
		//salva
		return(geral->Infoset->SaveToFile("TPatamares"));
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormPatamar::ValidaHoraMinutoCalculo(VTPatamar *patamar, TDateTime date_time_cal)
	{
	//variaveis locais
	AnsiString	str_tempo_ini, str_tempo_fim;
	int 			ind_pat =	patamar->Index;
	TDateTime   date_time_ini, date_time_fim;

	str_tempo_ini = StringGridConfig->Cells[1][ind_pat+1];
	str_tempo_fim = StringGridConfig->Cells[2][ind_pat+1];
	//corrige tempo (DateTime não reconhece 24:00)
	if(str_tempo_ini.AnsiCompareIC("24:00") == 0) str_tempo_ini = "00:00";
	if(str_tempo_fim.AnsiCompareIC("24:00") == 0) str_tempo_fim = "00:00";
	//converte para formato hora
	date_time_ini = TDateTime(str_tempo_ini);
	date_time_fim = TDateTime(str_tempo_fim);
	//verifica se Patamar inclui virada do dia
	if (date_time_ini < date_time_fim)
      {//valida hora_cal/minuto_cal >= hora_ini/minuto_ini do Patamar
		if  (date_time_cal < date_time_ini) return(false);
		//if ((hora_cal == patamar->HoraIni)&&(minuto_cal < patamar->MinutoIni)) return(false);
      //valida hora_cal/minuto_cal < hora_fim/minuto_fim do Patamar
		if  (date_time_cal > date_time_fim) return(false);
		//if ((hora_cal == patamar->HoraFim)&&(minuto_cal >= patamar->MinutoFim)) return(false);
		//hora_cal/minuto_cal válido
      return(true);
      }
   else
		{//valida qualquer hora se for 00:00 a 24:00
		if  (str_tempo_ini.AnsiCompareIC(str_tempo_fim) == 0) return(true);
		//valida hora_cal/minuto_cal >= hora_ini/minuto_ini do Patamar
		if  (date_time_cal > date_time_ini) return(true);
		//if ((hora_cal == patamar->HoraIni)&&(minuto_cal >= patamar->MinutoIni)) return(true);
      //valida hora_cal/minuto_cal < hora_fim/minuto_fim do Patamar
		if  (date_time_cal < date_time_fim) return(true);
		//if ((hora_cal == patamar->HoraFim)&&(minuto_cal < patamar->MinutoFim)) return(true);
      //hora_cal/minuto_cal inválido
      return(false);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormPatamar::ValidaValores(AnsiString *pat1, AnsiString *pat2)
	{
	//variaveis locais
	AnsiString		 str_tempo_ini, str_tempo_fim;
	TDateTime       date_time_ini,date_time_fim,date_time_cal;

	//só preenche pat1 e pat2 se houver valores inválidos
	for(int np = 1; np < StringGridConfig->RowCount; np++)
		{//verifica se é o ultimo
		if(! (np == StringGridConfig->RowCount-1))
			{//não é o ultimo patamar
			try{//lê hora/minuto de cada patamar
				str_tempo_ini = StringGridConfig->Cells[2][np];
				str_tempo_fim = StringGridConfig->Cells[1][np+1];
				//corrige tempo (DateTime não reconhece 24:00)
				if(str_tempo_ini.AnsiCompareIC("24:00") == 0) str_tempo_ini = "00:00";
				if(str_tempo_fim.AnsiCompareIC("24:00") == 0) str_tempo_fim = "00:00";
				date_time_fim = TDateTime(str_tempo_fim);
				date_time_ini = TDateTime(str_tempo_ini);
				if(date_time_fim != date_time_ini)
					{//verifica se o horario final do patamar é o inicial do próximo
					*pat1 = AnsiString(StringGridConfig->Cells[0][np]);
					*pat2 = " e " + AnsiString(StringGridConfig->Cells[0][np+1]);
					return(false);
					}
				}catch(Exception &e)
				{				}
			}
		else
			{//é o ultimo patamar
			try{//lê hora/minuto de cada patamar (primeiro e ultimo)
				str_tempo_ini = StringGridConfig->Cells[2][np];
				str_tempo_fim = StringGridConfig->Cells[1][1];
				//corrige tempo (DateTime não reconhece 24:00)
				if(str_tempo_ini.AnsiCompareIC("24:00") == 0) str_tempo_ini = "00:00";
				if(str_tempo_fim.AnsiCompareIC("24:00") == 0) str_tempo_fim = "00:00";
				date_time_fim = TDateTime(str_tempo_fim);
				date_time_ini = TDateTime(str_tempo_ini);
				if(date_time_fim != date_time_ini)
					{//verifica se o horario final do patamar é o inicial do próximo
					*pat1 = AnsiString(StringGridConfig->Cells[0][np]);
					*pat2 = " e " + AnsiString(StringGridConfig->Cells[0][1]);
					return(false);
					}
				}catch(Exception &e)
				{				}
			}
		}
	return(true);
	}
//---------------------------------------------------------------------------
//eof

