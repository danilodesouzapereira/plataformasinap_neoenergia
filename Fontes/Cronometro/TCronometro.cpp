#include <vcl.h>
#include <time.h>
#pragma hdrstop
#include "TCronometro.h"
#include "TFormCronometro.h"
#include "..\..\DLL_Inc\Funcao.h"

//-----------------------------------------------------------------------------
VTCronometro* NewObjCronometro(VTApl *apl)
   {
   return(new TCronometro(apl));
   }

//-----------------------------------------------------------------------------
__fastcall TCronometro::TCronometro(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria lista de relogios
   lisREL = new TList();
   }

//-----------------------------------------------------------------------------
__fastcall TCronometro::~TCronometro(void)
   {
   //destrói lista e seus objetos
   if (lisREL) {LimpaTList(lisREL); delete lisREL; lisREL = NULL;}
   }

//-----------------------------------------------------------------------------
TRelogio* __fastcall TCronometro::ExisteRelogio(AnsiString codigo)
   {
   //variáveis locais
   TRelogio *relogio;

   //procura o Relogio na lista
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      if (relogio->codigo.AnsiCompareIC(codigo) == 0) return(relogio);
      }
   //não existe o Relogio
   return(NULL);
   }

//-----------------------------------------------------------------------------
int __fastcall TCronometro::NumAcionamentos(AnsiString codigo)
   {
   //variáveis locais
   TRelogio *relogio;

   //determina o Relogio
   if ((relogio = ExisteRelogio(codigo)) == NULL) return(-1);
   //reset Relogio
   return(relogio->acionamentos);
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Reinicia(void)
   {
   LimpaTList(lisREL);
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Reset(void)
   {
   //variáveis locais
   TRelogio *relogio;

   //loop p/ todos Relogios
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      relogio->Reset();
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Reset(AnsiString codigo)
   {
   //variáveis locais
   TRelogio *relogio;

   //determina o Relogio
   if ((relogio = ExisteRelogio(codigo)) == NULL) return;
   //reset Relogio
   relogio->Reset();
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::SaveToFile(AnsiString filename)
   {
   //variáveis locais
   int        tmseg;
   double     taxa, total;
   AnsiString txt;
   TRelogio  *relogio;
   TStrings  *lines;

   lines = new TStringList();
   //determina tempo total cronometrado
   total = 0.;
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      total  += relogio->TempoMSeg();
      }
   //monta StringList com dados dos relógios
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      tmseg   = relogio->TempoMSeg();
      taxa    = (total == 0.) ? 0. : (double(tmseg)/total);
      txt.sprintf("%s;%s;%d;%d;%3.2f",
                  relogio->codigo.c_str(),
                  relogio->descricao.c_str(),
                  relogio->acionamentos,
                  tmseg,
                  taxa
                  );
      lines->Add(txt);
      }
   //grava arquivo
   lines->SaveToFile(filename);
   //destrói objeto
   delete lines;
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Show(void)
   {
   //variáveis locais
   TFormCronometro *form;
   TRelogio   *relogio;
   double     taxa, total;

   //cria FormCronometro p/ exibir os vários Relogios
   if ((form = new TFormCronometro(NULL, apl)) == NULL) return;
   //determina tempo total cronometrado
   total = 0.;
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      total  += relogio->TempoMSeg();
      }
   //insere Relogios no form
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      taxa = (total == 0.) ? 0. : (relogio->TempoMSeg()/total);
      form->Insere(relogio->codigo, relogio->descricao, relogio->acionamentos,  relogio->TempoMSeg(), taxa);
      }
   //exibe form modal
   form->ShowModal();
   //destrói o form
   delete form;
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Start(void)
   {
   //variáveis locais
   TRelogio *relogio;

   //loop p/ todos Relogios
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      relogio->Start();
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Start(AnsiString codigo, AnsiString descricao)
   {
   //variáveis locais
   TRelogio *relogio;

   //verifica se já existe o Relogio
   if ((relogio = ExisteRelogio(codigo)) == NULL)
      {//cria um novo Relogio
      if ((relogio = new TRelogio(codigo, descricao)) == NULL) return;
      //insere Relogio na lista
      lisREL->Add(relogio);
      }
   //inicia o Relogio
   relogio->Start();
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Stop(void)
   {
   //variáveis locais
   TRelogio *relogio;

   //loop p/ todos Relogios
   for (int n = 0; n < lisREL->Count; n++)
      {
      relogio = (TRelogio*)lisREL->Items[n];
      relogio->Stop();
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TCronometro::Stop(AnsiString codigo)
   {
   //variáveis locais
   TRelogio *relogio;

   //determina o Relogio
   if ((relogio = ExisteRelogio(codigo)) == NULL) return;
   //interrompe Relogio
   relogio->Stop();
   }

//-----------------------------------------------------------------------------
int __fastcall TCronometro::TempoMSeg(AnsiString codigo)
   {
   //variáveis locais
   TRelogio *relogio;

   //determina o Relogio
   if ((relogio = ExisteRelogio(codigo)) == NULL) return(0);
   //interrompe Relogio
   return(relogio->TempoMSeg());
   }

//-----------------------------------------------------------------------------
__fastcall TRelogio::TRelogio(AnsiString codigo, AnsiString descricao)
   {
   this->codigo    = codigo;
   this->descricao = descricao;
   //inicia contadores de tempo
   Reset();
   }

//-----------------------------------------------------------------------------
__fastcall TRelogio::~TRelogio(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TRelogio::Reset(void)
   {
   //inicia t1 e t2 c/ data/hora atual
   status       = stRESET;
   t1 = t2      = Now();
   mseg         = 0;
   acionamentos = 0;
   }

//-----------------------------------------------------------------------------
void __fastcall TRelogio::Start(void)
   {
   //inicia t1 e t2 c/ data/hora atual
   status  = stSTART;
   t1 = t2 = Now();
   //NÃO cancela tempo já cronometrado
   //mseg = 0;
   //incrementa número de acionamentos
   acionamentos++;
   }

//-----------------------------------------------------------------------------
void __fastcall TRelogio::Stop(void)
   {
   //variáveis locais
   unsigned short hour, min, sec, msec;

   //atualiza t2 c/ data/hora atual
   status = stSTOP;
   t2     = Now();
   //acumula intervalo de tempo em mseg
   TDateTime dif = t2 - t1;
   dif.DecodeTime(&hour, &min, &sec, &msec);
   mseg += ((1000*((60*((60*hour) + min)) + sec)) + msec);
   }

//---------------------------------------------------------------------------
int __fastcall TRelogio::TempoMSeg(void)
   {
   return(mseg);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TRelogio::TempoStringMSeg(void)
   {
   return(IntToStr(TempoMSeg()) + " mseg") ;
   }

//---------------------------------------------------------------------------
//eof