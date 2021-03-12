//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifRegulador.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TFormGraf.h"
#include "VTCfgGraf.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTUnif* NewObjUnifRegulador(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifRegulador(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifRegulador::TUnifRegulador() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifRegulador::TUnifRegulador(VTApl *apl, int tipo_graf, AnsiString nome)
                      :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawCurto
   if (draw) delete draw;
   draw = NewObjDrawCurto(apl);
   }

//---------------------------------------------------------------------------
__fastcall TUnifRegulador::~TUnifRegulador(void)
   {
   //nada a fazer
   }
//---------------------------------------------------------------------------
void __fastcall TUnifRegulador::Redraw(VTEdt *editor)
   {
   //desabilita animação
   animacao->Stop();
   //utiliza método da classe base
   TUnifEditor::Redraw(editor);
   //atualiza gráfico
   form->RedrawTelaReal();
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifRegulador::Redraw(int op, VTEdt *editor)
   {
   //utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifRegulador::RedrawAllBarra(void)
   {
   //variáveis locais
   TList      *lisBAR;
   VTBarra    *barra;
   VTCfgGraf  *cfg = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));

   //determina lista de Barras das Redes
   if ((lisBAR = LisEqptoRede(eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
      for (int nbar = 0; nbar < lisBAR->Count; nbar++)
         {
         barra = (VTBarra*)lisBAR->Items[nbar];
         if (! barra->Show[eqp_VISIBLE]) continue;
         RedrawBarra(barra, cfg->CorBarra(barra));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifRegulador::RedrawAllCarga(void)
   {
   //variáveis locais
   TList     *lisCAR;
   VTCarga   *carga;
   VTCfgGraf *cfg = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));

   //determina lista de Cargas
   if ((lisCAR = LisEqptoRede(eqptoCARGA)) != NULL)
      {//loop p/ todas Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //verifica se deve exibir as Cargas
         if (! carga->Show[eqp_VISIBLE]) continue;
         RedrawCarga(carga, cfg->CorCarga(carga));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifRegulador::RedrawAllLigacao(bool chave_enabled)
   {
   //variáveis locais
   TList      *lisEQP;
   VTLigacao  *ligacao;
   VTCfgGraf  *cfg = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));

   //determina lista de Capserie
   if ((lisEQP = LisEqptoRede(eqptoLIGACAO)) != NULL)
      {//loop p/ todas ligações
      for (int n = 0; n < lisEQP->Count; n++)
         {
         ligacao = (VTLigacao*)lisEQP->Items[n];
         if (! ligacao->Show[eqp_VISIBLE]) continue;
         //draw->LinhaLigacao(ligacao, cfg->CorLigacao(ligacao));
         draw->Ligacao(ligacao, cfg->CorLigacao(ligacao));
         }
      }
   }

//---------------------------------------------------------------------------
//eof

