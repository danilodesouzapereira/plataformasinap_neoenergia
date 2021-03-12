//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtFaseTrecho.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\Estrutura.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtFaseTrecho::TFormEdtFaseTrecho(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //cria apl próprio
   apl = NewObjApl(this, apl_owner);
   //cria objetos
   apl->Add(DLL_NewObjArvore(apl));
   //inicia redes
   TViewRedesInicia();
   //inicia cboxfases
   CBoxFasesInicia();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtFaseTrecho::~TFormEdtFaseTrecho(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destroi apl
   if (apl)     {delete apl;      apl  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::ActionAplicarExecute(TObject *Sender)
   {
   //variáveis locais
   VTRede      *rede;
   TList       *lisTRECHO;
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   //verifica se há progresso
   if(progresso == NULL)
      {
      apl->Add(progresso = DLL_NewObjProgresso(NULL, this));
      }
   //inicia progresso simples
   progresso->Start(progDEFAULT);
   //determina rede
   rede = TViewRedeSelecionada();
   if(rede == NULL)
      {
      Aviso("Selecione uma rede");
      //finaliza progresso
      progresso->Stop();
      return;
      }
   //verifica se alguma alteração foi selecionada
   if((CBoxFases->ItemIndex == -1) || (RadioGroupNeutro->ItemIndex == -1))
      {
      Aviso("Selecione algum tipo de alteração (neutro, fases ou ambos)");
      //finaliza progresso
      progresso->Stop();
      return;
      }
   //verifica se alguma alteração será feita
   if((CBoxFases->ItemIndex == 0) && (RadioGroupNeutro->ItemIndex == 0))
      {
      Aviso("Nenhuma alteração foi feita na rede");
      //finaliza progresso
      progresso->Stop();
      return;
      }
   //cria lista
   lisTRECHO = new TList();
   //lista todos os trechos da rede
   rede->LisLigacao(lisTRECHO, eqptoTRECHO);
   try{//verifica se as fases serão alteradas (trata neutro em cada função)
      if(CBoxFases->ItemIndex == 0) AlteraNeutro(lisTRECHO);
      else                          AlteraFases(lisTRECHO);
      }catch(Exception &e)
         {//finaliza progresso
         progresso->Stop();
         }
   //destroi lista
   delete lisTRECHO;
   //finaliza progresso
   progresso->Stop();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::AlteraFases(TList  *lisTRECHO)
   {
   //variáveis locais
   bool       com_neutro;
   int        fase_tag;
   TFase      *fase;
   VTArranjo  *arranjo_orig, *arranjo_novo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTEdita    *edita    = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTFases    *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   VTTrecho   *trecho, *trecho_alt;
   TList      *lisALT;

   //cria lista
   lisALT = new TList();
   //traduz fase selecionada para fase_tag
   fase = (TFase*)(CBoxFases->Items->Objects[CBoxFases->ItemIndex]);
   fase_tag = fase->tag;
   //define se deve inserir neutro
   com_neutro = (RadioGroupNeutro->ItemIndex == 1);
   if(com_neutro) fase_tag = fase->tag | faseN;
   for(int nt = 0; nt < lisTRECHO->Count; nt++)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[nt];
      arranjo_orig = trecho->arranjo;
      if(arranjo_orig->Fases == fase_tag) continue;
      //localiza ou cria arranjo compatível com fase_tag
      arranjo_novo = ExisteArranjo(arranjo_orig, fase_tag);
      if(arranjo_novo == NULL) continue;
      //insere trecho na lista de alterados
      trecho_alt = trecho->Clone();
      trecho->Obj = trecho_alt;
      trecho_alt->DefineObjArranjo(arranjo_novo);
      lisALT->Add(trecho);
      }
   //recondutora todos os trechos
   edita->AlteraLisEqpto(lisALT);
   //destroi lista
   delete lisALT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::AlteraNeutro(TList  *lisTRECHO)
   {
   //variáveis locais
   bool       com_neutro;
   int        fase_tag;
   VTArranjo  *arranjo_orig, *arranjo_novo;
   VTEdita    *edita    = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTTrecho   *trecho, *trecho_alt;
   TList      *lisALT;

   //cria lista
   lisALT = new TList();
   //define se deve inserir ou retirar neutro
   com_neutro = (RadioGroupNeutro->ItemIndex == 1);
   for(int nt = 0; nt < lisTRECHO->Count; nt++)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[nt];
      arranjo_orig = trecho->arranjo;
      //determina fase do trecho com ou sem neutro
      if(com_neutro) fase_tag = arranjo_orig->Fases | faseN;
      else           fase_tag = (arranjo_orig->Fases & faseABC);
      if(arranjo_orig->Fases == fase_tag) continue;
      //localiza ou cria arranjo compatível com fase_tag
      arranjo_novo = ExisteArranjo(arranjo_orig, fase_tag);
      if(arranjo_novo == NULL) continue;
      //insere trecho na lista de alterados
      trecho_alt = trecho->Clone();
      trecho->Obj = trecho_alt;
      trecho_alt->DefineObjArranjo(arranjo_novo);
      lisALT->Add(trecho);
      }
   //recondutora todos os trechos
   edita->AlteraLisEqpto(lisALT);
   //destroi lista
   delete lisALT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::ActionFechaExecute(TObject *Sender)
   {
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::CBoxFasesInicia(void)
   {
   //variáveis locais
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //insere opções de fases
   CBoxFases->Clear();
   CBoxFases->Items->AddObject("Originais",NULL);
   CBoxFases->Items->AddObject("ABC",  fases->TagToFase[faseABC]);
   CBoxFases->Items->AddObject("A",    fases->TagToFase[faseA]);
   CBoxFases->Items->AddObject("B",    fases->TagToFase[faseB]);
   CBoxFases->Items->AddObject("C",    fases->TagToFase[faseC]);
   CBoxFases->Items->AddObject("AB",   fases->TagToFase[faseAB]);
   CBoxFases->Items->AddObject("BC",   fases->TagToFase[faseBC]);
   CBoxFases->Items->AddObject("CA",   fases->TagToFase[faseCA]);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TFormEdtFaseTrecho::CriaArranjo(VTSuporte *suporte, VTCabo *caboA,
                                                      VTCabo *caboB, VTCabo *caboC, VTCabo *caboN)
   {
   //variaveis locais
   VTArranjos   *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTArranjo    *arranjo;
   VTFases      *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   VTMontaZ     *montaZ   = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
   VTPtoFixacao *pto_fixacao;
   AnsiString   cod_arranjo;
   int          fase_tag = 0;
   int          ptoA, ptoB, ptoC, ptoN;

   //inicia indices dos ptos de fixação
   if((suporte->Codigo.AnsiCompareIC("Poste Primario") == 0) ||
      (suporte->Codigo.AnsiCompareIC("Poste Primário") == 0))
      {
      ptoA = 1; ptoB = 2; ptoC = 3; ptoN = 4;
      }
   else
      {
      ptoA = 0; ptoB = 1; ptoC = 2; ptoN = 3;
      }
   //determina fase_tag
   if(caboA) fase_tag |= faseA;
   if(caboB) fase_tag |= faseB;
   if(caboC) fase_tag |= faseC;
   if(caboN) fase_tag |= faseN;
   //determina um código para o arranjo
   cod_arranjo = arr_original->Codigo + "_" + fases->AsStrABCN[fase_tag];
   try{//cria um novo objeto Arranjo
      if ((arranjo       = DLL_NewObjArranjo()) == NULL) return(NULL);
      arranjo->Id        = FalsoId();
      arranjo->Codigo    = cod_arranjo;
      arranjo->Extern_id = "";
      arranjo->Owner     = NULL;
      arranjo->Data      = Date();
      arranjo->Fases     = fase_tag;
      arranjo->TipoCfg   = arranjoCABOxFASE;
      arranjo->ramal     = arr_original->ramal;
      arranjo->ResTerra  = arr_original->ResTerra;
      arranjo->Vmin_KV   = arr_original->Vmin_KV;
      arranjo->Vmax_KV   = arr_original->Vmax_KV;
      arranjo->Iadm_amp  = arr_original->Iadm_amp;
      arranjo->Color     = arr_original->Color;
      arranjo->Status[sttNOVO] = true;
      //
      arranjo->suporte  = suporte;
      if(caboA != NULL)
         {
         if ((pto_fixacao = suporte->ExistePtoFixacao(ptoA)) != NULL)
            {//define PtoFixacao e Cabo p/ fase do Arranjo
            arranjo->DefPtoFixacaoFase(indFASE_A, pto_fixacao);
            arranjo->DefCaboFase(indFASE_A, caboA);
            }
         }
      if(caboB != NULL)
         {
         if ((pto_fixacao = suporte->ExistePtoFixacao(ptoB)) != NULL)
            {//define PtoFixacao e Cabo p/ fase do Arranjo
            arranjo->DefPtoFixacaoFase(indFASE_B, pto_fixacao);
            arranjo->DefCaboFase(indFASE_B, caboB);
            }
         }
      if(caboC != NULL)
         {
         if ((pto_fixacao = suporte->ExistePtoFixacao(ptoC)) != NULL)
            {//define PtoFixacao e Cabo p/ fase do Arranjo
            arranjo->DefPtoFixacaoFase(indFASE_C, pto_fixacao);
            arranjo->DefCaboFase(indFASE_C, caboC);
            }
         }
      if(caboN != NULL)
         {
         if ((pto_fixacao = suporte->ExistePtoFixacao(ptoN)) != NULL)
            {//define PtoFixacao e Cabo p/ fase do Arranjo
            arranjo->DefPtoFixacaoFase(indNEUTRO, pto_fixacao);
            arranjo->DefCaboFase(indNEUTRO, caboN);
            }
         }
      //monta matriz de impedâncias
      if (! montaZ->Executa(arranjo))
         {//destrói Arranjo
         delete arranjo;
         return(NULL);
         }
      //salva arranjo
      arranjos->InsereArranjo(arranjo);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TFormEdtFaseTrecho::CriaArranjo(VTCabo *cabo, int fase_nova)
   {
   //variaveis locais
   AnsiString   cod_arranjo;
   VTArranjos   *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTArranjo    *arranjo;
   VTFases      *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   VTMontaZ     *montaZ   = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //determina um código para o arranjo
   cod_arranjo = arr_original->Codigo + "_" + fases->AsStrABCN[fase_nova];
   try{//cria um novo objeto Arranjo
      if ((arranjo       = DLL_NewObjArranjo()) == NULL) return(NULL);
      arranjo->Id        = FalsoId();
      arranjo->Codigo    = cod_arranjo;
      arranjo->Extern_id = "";
      arranjo->Owner     = NULL;
      arranjo->Data      = Date();
      arranjo->Fases     = fase_nova;
      arranjo->TipoCfg   = arranjoCABO_Z0Z1;
      arranjo->ramal     = arr_original->ramal;
      arranjo->ResTerra  = arr_original->ResTerra;
      arranjo->Vmin_KV   = arr_original->Vmin_KV;
      arranjo->Vmax_KV   = arr_original->Vmax_KV;
      arranjo->Iadm_amp  = arr_original->Iadm_amp;
      arranjo->Color     = arr_original->Color;
      arranjo->Status[sttNOVO] = true;
      //
      //define CaboZ0Z1
      arranjo->caboZ0Z1 = cabo;
      //determina Suporte (opcional => pode ser NULL)
      arranjo->suporte = arr_original->suporte;
      //monta matriz de impedâncias
      if (! montaZ->Executa(arranjo))
         {//destrói Arranjo
         delete arranjo;
         return(NULL);
         }
      //salva arranjo
      arranjos->InsereArranjo(arranjo);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TFormEdtFaseTrecho::CriaArranjo(strIMP &z0, strIMP &z1,
                                          double c0, double c1, int fase_nova)
   {
   //variaveis locais
   AnsiString   cod_arranjo;
   VTArranjos   *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTArranjo    *arranjo;
   VTFases      *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   VTMontaZ     *montaZ   = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //determina um código para o arranjo
   cod_arranjo = arr_original->Codigo + "_" + fases->AsStrABCN[fase_nova];
   try{//cria um novo objeto Arranjo
      if ((arranjo       = DLL_NewObjArranjo()) == NULL) return(NULL);
      arranjo->Id        = FalsoId();
      arranjo->Codigo    = cod_arranjo;
      arranjo->Extern_id = "";
      arranjo->Owner     = NULL;
      arranjo->Data      = Date();
      arranjo->Fases     = fase_nova;
      arranjo->TipoCfg   = arranjoZ0Z1;
      arranjo->ramal     = arr_original->ramal;
      arranjo->ResTerra  = arr_original->ResTerra;
      arranjo->Vmin_KV   = arr_original->Vmin_KV;
      arranjo->Vmax_KV   = arr_original->Vmax_KV;
      arranjo->Color     = arr_original->Color;
      arranjo->Iadm_amp  = arr_original->Iadm_amp;
      arranjo->Status[sttNOVO] = true;
      //
      arranjo->z0 = z0;
      arranjo->z1 = z1;
      arranjo->c0 = c0;
      arranjo->c1 = c1;
      //determina Suporte (opcional => pode ser NULL)
      arranjo->suporte = arr_original->suporte;
      //monta matriz de impedâncias
      if (! montaZ->Executa(arranjo))
         {//destrói Arranjo
         delete arranjo;
         return(NULL);
         }
      //salva arranjo
      arranjos->InsereArranjo(arranjo);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TFormEdtFaseTrecho::ExisteArranjo(VTArranjo *original, int fase_tag)
   {
   //variaveis locais
   VTArranjos   *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTArranjo    *novo;
   VTCabo       *caboA, *caboB, *caboC, *caboN;
   VTSuporte    *suporte;
   strIMP       z0, z1;
   double       c0, c1;

   //salva ponteiro
   arr_original = original;
   //verifica tipo de arranjo
   switch(original->TipoCfg)
      {
      case arranjoCABOxFASE:
         caboA = original->fase[indFASE_A].cabo;
         caboB = original->fase[indFASE_B].cabo;
         caboC = original->fase[indFASE_C].cabo;
         caboN = original->fase[indNEUTRO].cabo;
         suporte = original->suporte;
         //filtra fase_tag
         if((fase_tag & faseA) != faseA) caboA = NULL;
         if((fase_tag & faseB) != faseB) caboB = NULL;
         if((fase_tag & faseC) != faseC) caboC = NULL;
         if((fase_tag & faseN) != faseN) caboN = NULL;
         //localiza arranjo
         novo = arranjos->ExisteArranjoCABOxFASE(suporte, caboA, caboB, caboC, caboN);
         //verifica se existe
         if(novo == NULL) novo = CriaArranjo(suporte, caboA, caboB, caboC, caboN);
         break;
      case arranjoCABO_Z0Z1:
         //localiza arranjo
         novo = arranjos->ExisteArranjoCABO_Z0Z1(original->caboZ0Z1, fase_tag);
         //verifica se existe
         if(novo == NULL) novo = CriaArranjo(original->caboZ0Z1, fase_tag);
         break;
      case arranjoZ0Z1:
         z0 = original->z0;
         z1 = original->z1;
         c0 = original->c0;
         c1 = original->c1;
         //localiza arranjo
         novo = arranjos->ExisteArranjoZ0Z1(z0.r, z0.x, c0, z1.r, z1.x, c1, fase_tag);
         //verifica se existe
         if(novo == NULL) novo = CriaArranjo(z0, z1, c0, c1, fase_tag);
         break;
      default: break;
      }
   return(novo);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtFaseTrecho::ExisteRede(AnsiString cod_rede, TList *lisREDE)
   {
   //variaveis locais
   VTRede *rede;

   for(int nr = 0; nr < lisREDE->Count; nr++)
      {//verifica se existe uma rede com o cod_rede
      rede = (VTRede*)lisREDE->Items[nr];
      if(rede->Codigo.AnsiCompareIC(cod_rede) == 0) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //variáveis locais
   VTRede      *rede;
   VTGrafico   *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há alguma rede selecionada
   rede = TViewRedeSelecionada();
   if(rede != NULL)
      {//cancela eqptos destacados
//      grafico->DestacaEqpto(rede->LisLigacao(), rede->Color);
      grafico->DestacaEqpto(NULL, rede->Color);
      }
   //destrói este Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::TViewRedesClick(TObject *Sender)
   {
   //variáveis locais
   int         dim = 3;
   VTRede      *rede;
   VTGrafico   *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há alguma rede selecionada
   rede = TViewRedeSelecionada();
   if(rede == NULL) return;
   //destaca rede
   grafico->DestacaEqpto(rede->LisLigacao(), rede->Color, dim);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::TViewRedesDblClick(TObject *Sender)
   {
   //variáveis locais
   VTRede    *rede;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há alguma rede selecionada
   rede = TViewRedeSelecionada();
   if(rede == NULL) return;
   //seleciona área de zoom
   grafico->SelecionaZoom(rede);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFaseTrecho::TViewRedesInicia(void)
   {
   //variáveis locais
   VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia TView
   TViewRedes->Items->Clear();
   //inicia Arvore e o TreeView
   arvore->Inicia(redes);
   arvore->TreeViewInicia(TViewRedes);
   //expande o TView
//   TViewRedes->FullExpand();
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtFaseTrecho::TViewRedeSelecionada(void)
   {
   //variáveis locais
   AnsiString  class_name;
   VTNode      *node;
   VTRede      *rede;
   TList       *lisREDE;

   //verifica se há um TreeNode selecionado
   if ((TViewRedes->Selected == NULL)||(TViewRedes->Selected->Data == NULL)) return(NULL);
   //verifica se objeto associado ao TreeNode é um VTNode
   class_name = ((TObject*)(TViewRedes->Selected->Data))->ClassName();
   if (class_name.AnsiCompareIC("TNode") == 0)
      {//determina Rede associada ao TreeNode
      node = (VTNode*)TViewRedes->Selected->Data;
      lisREDE = node->LisObjectNode();
      if (lisREDE->Count == 1)
         {
         return((VTRede*)lisREDE->First());
         }
      if(lisREDE->Count > 1)
         {//localiza a rede selecionada na lista de redes do node
         rede = ExisteRede(TViewRedes->Selected->Text, lisREDE);
         return(rede);
         }
      }
   return(NULL);
   }


//---------------------------------------------------------------------------

