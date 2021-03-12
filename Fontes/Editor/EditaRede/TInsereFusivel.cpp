//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "TInsereFusivel.h"
#include "TFusivel.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrafo.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
__fastcall TInsereFusivel::TInsereFusivel(VTApl *apl_owner)
   {
   //salva ponteiros p/ objetos
   apl = apl_owner;
   //cria lista
   lisEQP        = new TList();
   lisFUSIVEL    = new TList();
   lisEQP_INSERE = new TList();
   lisEQP_RETIRA = new TList();
   lisEQP_ALTERA = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TInsereFusivel::~TInsereFusivel(void)
   {
   //destroi lista sem destruir obj
   if(lisEQP)         {delete lisEQP;        lisEQP = NULL;}
   if(lisEQP_INSERE)  {delete lisEQP_INSERE; lisEQP_INSERE = NULL;}
   if(lisEQP_RETIRA)  {delete lisEQP_RETIRA; lisEQP_RETIRA = NULL;}
   if(lisEQP_ALTERA)  {delete lisEQP_ALTERA; lisEQP_ALTERA = NULL;}
   //destroi lista e objetos
   if(lisFUSIVEL) {LimpaTList(lisFUSIVEL); delete lisFUSIVEL; lisFUSIVEL = NULL;}
   }

//---------------------------------------------------------------------------
TFusivel* __fastcall TInsereFusivel::CriaFusivel(TStringList *campos)
   {
   //variáveis locais
   TFusivel *fusivel;

   //verifica se o numero de campos é válido
   if(campos->Count < 4) return(NULL);
   try{//cria obj fusivel
      fusivel            = new TFusivel();
      fusivel->TipoTrafo = StrToInt(campos->Strings[0]);
      fusivel->Snom_kva  = StrToDouble(campos->Strings[1]);
      fusivel->Iadm_a    = StrToDouble(campos->Strings[2]);
      fusivel->Elo       = (campos->Strings[3]);
   }catch(Exception &e)
      {
      if(fusivel) delete fusivel;
      return(NULL);
      }
   return(fusivel);
   }

//---------------------------------------------------------------------------
TFusivel* __fastcall TInsereFusivel::ExisteFusivel(VTTrafo *trafo)
   {
   //variáveis locais
   TFusivel *fusivel;
   double   snom_kva = trafo->snom * 1000.;

   for(int nf = 0; nf < lisFUSIVEL->Count; nf++)
      {
      fusivel = (TFusivel*)lisFUSIVEL->Items[nf];
      //verifica fases
      if(trafo->Monofasico && (fusivel->TipoTrafo != 1))      continue;
      if((! trafo->Monofasico) && (fusivel->TipoTrafo != 3))  continue;
      //verifica potencia nominal
      if(IsDoubleZero(fusivel->Snom_kva - snom_kva, 1e-2)) return(fusivel);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTChave* __fastcall TInsereFusivel::ExisteFusivelConectandoBarra(VTRede *rede, VTBarra *barra)
   {
   //variáveis locais
   VTChave   *chave;
   VTLigacao *ligacao;

   //inicia lisEQP com o conjunto de ligações que conectam a Barra indicada
   lisEQP->Clear();
   rede->LisLigacao(lisEQP, barra);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      ligacao = (VTLigacao*)lisEQP->Items[n];
      if (ligacao->Tipo() == eqptoCHAVE)
         {
         chave = ((VTChave*)ligacao);
         if (chave->TipoChave->Tipo == chaveBF) return(chave);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TInsereFusivel::ImportaFusiveis(AnsiString arquivo)
   {
   //variáveis locais
   bool         sucesso = true;
   AnsiString   linha;
   TStringList *linhas, *campos;
   TFusivel    *fusivel;

   //limpa lista
   lisFUSIVEL->Clear();
   try{//cria linhas e campos
      linhas = new TStringList();
      campos = new TStringList();
      //le linhas
      linhas->LoadFromFile(arquivo);
      for(int nl = 0; (nl < linhas->Count) && sucesso; nl++)
         {//verifica linha de comentário
         linha = linhas->Strings[nl];
         if(VerificaComentario(linha)) continue;
         //separa campos
         campos->Clear();
         ExtraiStrings(linha, ";", campos);
         //cria objetos fusivel
         fusivel = CriaFusivel(campos);
         if(fusivel == NULL) sucesso = false;
         else                lisFUSIVEL->Add(fusivel);
         }
   }catch(Exception &e)
      {
      if(linhas) delete linhas;
      if(campos) delete campos;
      return(false);
      }
   //destroi linhas e campos
   delete linhas;
   delete campos;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TInsereFusivel::Insere(AnsiString arquivo)
   {
   //variáveis locais
   bool        sucesso    = true;
   VTEdita     *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
//   VTGrafico   *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRede      *rede;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoRede  *tiporede;
//   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
   TList       *lisREDE   = redes->LisRede();
   TList       *lisTRAFO;

   //limpa listas usadas aqui
   lisEQP_ALTERA->Clear();
   lisEQP_INSERE->Clear();
   try{//importa Fusiveis
      if(! ImportaFusiveis(arquivo)) return(false);
      //cria lista
      lisTRAFO  = new TList();
      //verifica se existe tiporede Subestação
      tiporede = tipos->ExisteTipoRede(redeETD);
      for(int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //não insere em trafos de subestação
         if(rede->TipoRede == tiporede) continue;
         //limpa lista
         lisTRAFO->Clear();
         rede->LisLigacao(lisTRAFO, eqptoTRAFO);
         //executa inserção de chaves em cada trafo
         if(! InsereFusivelEmTrafos(lisTRAFO))
            {
            sucesso = false;
            break;
            }
         }
      //atualiza grafico
//      topologia->Reconfigura();
//      grafico->AreaRedeDefine();
      //insere e altera equipamentos
      edita->InsereLisEqpto(lisEQP_INSERE);
      edita->AlteraLisEqpto(lisEQP_ALTERA);
      //destroi lista
      delete lisTRAFO;
   }catch(Exception &e)
      {//destroi lista
      if(lisTRAFO) delete lisTRAFO;
      return(false);
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TInsereFusivel::InsereBarra(VTBarra *barra_pri)
   {
   //variáveis locais
   int         num_pat;
   VTBarra     *barra_nova;

   try{//insere nova barra
      barra_nova     = barra_pri->Clone();
      barra_nova->Id = FalsoId();
      barra_nova->Codigo = barra_nova->Codigo + "_1";
      barra_nova->esq.y1 += 100.;
      barra_nova->esq.y2 += 100.;
      barra_nova->Status[sttNOVO] = true;
      //insere barra na lista de eqptos novos
      lisEQP_INSERE->Add(barra_nova);
   }catch(Exception &e)
         {
         barra_nova = NULL;
         }
   return(barra_nova);
   }

//---------------------------------------------------------------------------
VTChave* __fastcall TInsereFusivel::InsereChaveFusivelEntradaET(VTRede *rede, VTBarra *barra_1, VTBarra *barra_2)
   {
   //variáveis locais
   int         num_pat;
   VTBarra     *barra_nova;
   VTChave     *chave;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoChave *tipochave;

   //verifica se existe tipo chave Fusivel
   tipochave = tipos->ExisteTipoChave(chaveBF);
   if(tipochave == NULL) return(NULL);
   try{//cria chave
      num_pat = patamares->NumPatamar();
      chave   = DLL_NewObjChave(num_pat);
      chave->Id                = FalsoId();
      chave->Codigo            = "FU";
      chave->Telecomandada     = false;
      chave->Extern_id         = "";
      chave->TipoChave         = tipochave;
      chave->Inom_amp          = 1;
      chave->EstadoNormal      = chvFECHADA;
      chave->ProtecaoAtiva     = false;
      chave->TecnoComunicacao  = comNAO_DISPONIVEL;
      chave->Status[sttNOVO]   = true;
      chave->ShowAsUnsigned    = eqp_VISIBLE | lig_I_MAX;
      //define posição de ligação nas barras
      chave->esq.posbar1 = 50;
      chave->esq.posbar2 = 50;
      //define barras da chave
      chave->DefineObjBarra(barra_1, barra_2);
      //insere chave na lista de eqptos novos
      lisEQP_INSERE->Add(chave);
      }catch(Exception &e)
         {
         chave = NULL;
         }
   return(chave);
   }

//---------------------------------------------------------------------------
bool __fastcall TInsereFusivel::InsereFusivelEmTrafos(TList *lisTRAFO)
   {
   //variáveis locais
   bool       sucesso = true;
   VTBarra    *barra_pri, *barra_nova;
   VTChave    *chave;
   VTTrafo    *trafo, *clone;
   VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TFusivel   *fusivel;

   for(int nt = 0; nt < lisTRAFO->Count; nt++)
      {
      trafo = (VTTrafo*)lisTRAFO->Items[nt];
      barra_pri = trafo->BarraPrimario();
      //verifica se ja existe chave no trafo
//      chave = ExisteFusivelConectandoBarra(trafo->rede, barra_pri);
//      if(chave != NULL) continue; //pode ser que essa linha não seja comentada
      //insere barra
      barra_nova = InsereBarra(barra_pri);
      //insere chave entre barra nova e primário do trafo
      chave      = InsereChaveFusivelEntradaET(trafo->rede, barra_pri, barra_nova);
      if(chave == NULL)
         {
         sucesso = false;
         break;
         }
      //move trafo para barra nova
      clone = trafo->Clone();
      trafo->Obj = clone;
      clone->DefineObjBarra(barra_nova, trafo->BarraSecundario(), NULL);
      clone->RedefineBarraDoPrimario(barra_nova);
      //insere trafo na lista de eqptos alterados
      lisEQP_ALTERA->Add(trafo);
      //verifica se existe fusivel compatível com trafo
      if((fusivel = ExisteFusivel(trafo)) != NULL)
         {//atualiza chave
         chave->Inom_amp  = fusivel->Iadm_a;
         chave->Extern_id = fusivel->Elo;
         }
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TInsereFusivel::Remove(void)
   {
   //variáveis locais
   bool        sucesso    = true;
   VTEdita     *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
//   VTGrafico   *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRede      *rede;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoRede  *tiporede;
//   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
   TList       *lisREDE   = redes->LisRede();
   TList       *lisTRAFO;

   //limpa listas usadas aqui
   lisEQP_ALTERA->Clear();
   lisEQP_RETIRA->Clear();
   try{//cria lista
      lisTRAFO  = new TList();
      //verifica se existe tiporede Subestação
      tiporede = tipos->ExisteTipoRede(redeETD);
      for(int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //não insere em trafos de subestação
         if(rede->TipoRede == tiporede) continue;
         //limpa lista
         lisTRAFO->Clear();
         rede->LisLigacao(lisTRAFO, eqptoTRAFO);
         //executa inserção de chaves em cada trafo
         if(! RemoveFusivelEmTrafos(lisTRAFO))
            {
            sucesso = false;
            break;
            }
         }
      //atualiza grafico
//      topologia->Reconfigura();
//      grafico->AreaRedeDefine();
      //retira e altera equipamentos
      edita->AlteraLisEqpto(lisEQP_ALTERA);
      edita->RetiraLisEqpto(lisEQP_RETIRA);
      //destroi lista
      delete lisTRAFO;
   }catch(Exception &e)
      {//destroi lista
      if(lisTRAFO) delete lisTRAFO;
      return(false);
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TInsereFusivel::RemoveFusivelEmTrafos(TList *lisTRAFO)
   {
   //variáveis locais
   int        ind_bar;
   bool       sucesso = true;
   VTBarra    *barra_pri, *barra;
   VTChave    *chave;
   VTEdita    *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTTrafo    *trafo, *clone;

   try{
      for(int nt = 0; nt < lisTRAFO->Count; nt++)
         {
         trafo = (VTTrafo*)lisTRAFO->Items[nt];
         barra_pri = trafo->BarraPrimario();
         //verifica se existe chave no trafo
         chave = ExisteFusivelConectandoBarra(trafo->rede, barra_pri);
         if(chave == NULL) continue;
         if(chave->Codigo.AnsiCompareIC("FU") != 0) continue;
         //determina outra barra da chave
         ind_bar = chave->IndiceBarra(barra_pri);
         barra = chave->Barra(1 - ind_bar);
         //clona trafo
         clone = trafo->Clone();
         trafo->Obj = clone;
         //redefine barras do trafo
         clone->DefineObjBarra(barra, trafo->BarraSecundario(), NULL);
         clone->RedefineBarraDoPrimario(barra);
         //insere trafo na lista de eqptos alterados
         lisEQP_ALTERA->Add(trafo);
         //insere barra_pri na lista de eqptos a serem removidos
         //(a chave será removida automaticamente)
         lisEQP_RETIRA->Add(barra_pri);
         }
   }catch(Exception &e)
      {
      sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TInsereFusivel::VerificaComentario(AnsiString linha)
   {
   if (linha.IsEmpty())   return(true);
   if (linha.Length() < 2) return(false);
   if((linha[1] == '/')&&(linha[2] == '/'))
      {//é linha de comentário
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
//eof
