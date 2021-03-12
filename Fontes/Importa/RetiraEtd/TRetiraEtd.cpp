//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <locale>
#include "TRetiraEtd.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Edita\VTEdita.h"
#include "..\..\Importa\Monta\VTMonta.h"
//#include "..\..\InterfaceTXT\Importa\VTImportaTXT.h"
//#include "..\..\Rede\VTArea.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
//#include "..\..\Rede\VTCarga.h"
//#include "..\..\Rede\VTDominio.h"
//#include "..\..\Rede\VTEqbar.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTSuprimento.h"
//#include "..\..\Rede\VTTrafo.h"
//#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\..\DLL_Inc\Curvas.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Rede.h"

//-----------------------------------------------------------------------------
VTRetiraEtd* NewObjRetiraEtd(VTApl *apl)
   {
   try{
      return(new TRetiraEtd(apl));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//-----------------------------------------------------------------------------
__fastcall TRetiraEtd::TRetiraEtd(VTApl *apl)
   {
   //salva ponteiros
   this->apl = apl;
   //cria objetos
   lisBAR_INI = new TList();
   lisETD     = new TList();
   lisPRI     = new TList();
   lisEQP     = new TList();
   lisDEL     = new TList();
   }

//-----------------------------------------------------------------------------
__fastcall TRetiraEtd::~TRetiraEtd(void)
   {
   //destrói lista sem destruir objetos
   if (lisBAR_INI) {delete lisBAR_INI; lisBAR_INI = NULL;}
   if (lisETD)     {delete lisETD;     lisETD     = NULL;}
   if (lisPRI)     {delete lisPRI;     lisPRI     = NULL;}
   if (lisEQP)     {delete lisEQP;     lisEQP     = NULL;}
   if (lisDEL)     {delete lisDEL;     lisDEL     = NULL;}
   }
   
//-----------------------------------------------------------------------------
void __fastcall TRetiraEtd::AlteraTensao(VTBarra *barra_ini)
   {
   //variaveis locais
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));;

   //lista ligações comuns à barra inicial
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, barra_ini);
   for(int nl = 0; nl < lisEQP->Count; nl++)
      {
      ligacao = (VTLigacao*)lisEQP->Items[nl];
      if(ligacao->rede->TipoRede->Segmento != redePRI) continue;
      //determina qual é a outra barra da ligação
      if(ligacao->pbarra1 == barra_ini) barra = ligacao->pbarra2;
      else                              barra = ligacao->pbarra1;
      //altera a tensão se for diferente
      if(! IsDoubleZero(barra->vnom - barra_ini->vnom)) barra_ini->vnom = barra->vnom;
      }
   }

//-----------------------------------------------------------------------------
bool __fastcall TRetiraEtd::Executa(void)
   {
   //variáveis locais
   int      count = 0;
   VTBarra *barra;
   VTRede  *rede_etd, *rede_pri;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   try{//inicia lista de Eqptos a serem excluídos
      lisDEL->Clear();
      //obtém lista de Redes do tipo ETD
      lisETD->Clear();
      redes->LisRede(lisETD, redeETD);
      //loop p/ todas redes ETD
      for (int nr = 0; nr < lisETD->Count; nr++)
         {
         rede_etd = (VTRede*)lisETD->Items[nr];
         //determina barras da rede a serem eliminadas
         InsereToDel(rede_etd->LisBarra());
         //ETD deverá ser eliminada
         InsereToDel(rede_etd);
         }
      //obtém lista de Redes do tipo REDE_PRI
      lisPRI->Clear();
      redes->LisRede(lisPRI, redePRI);
      //obtém lista de Barras iniciais das redes primárias
      lisBAR_INI->Clear();
      for (int n = 0; n < lisPRI->Count; n++)
         {
         rede_pri = (VTRede*)lisPRI->Items[n];
         if ((barra = rede_pri->BarraInicial()) == NULL) continue;
         if (lisBAR_INI->IndexOf(barra) < 0) lisBAR_INI->Add(barra);
         }
      //retira barras iniciais da lista de barras a serem excluidas, se houverem
      RetiraLis1DeLis2(lisBAR_INI, lisDEL);
      //loop p/ todas barras iniciais das redes primárias
      for (int nb = 0; nb < lisBAR_INI->Count; nb++)
         {
         barra = (VTBarra*)lisBAR_INI->Items[nb];
         //DVK 2013.08.28 altera tensão da barra conforme tensão do primário
         AlteraTensao(barra);
         //insere um Suprimento na Barra
         if(! InsereSuprimento(barra)) continue;
         //incrementa contador de suprimentos incluídos
         count++;
         }
      //recalcula área das Redes
      redes->CalculaArea();
      //inclui Barras isoladas na lista de eqptos a serem eliminados
      CopiaTList(redes->LisBarraIsolada(), lisDEL);
      //elimina todos os eqptos em lisDEL
      if (lisDEL->Count > 0) edita->RetiraLisEqpto(lisDEL);
      //recalcula área das Redes
      redes->CalculaArea();
   }catch(Exception &e)
      {
      return(false);
      }
//   return(count > 0);
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TRetiraEtd::InsereSuprimento(VTBarra *barra)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   double    valor[nvCURVA_PQVT];
   VTCurva      *curva;
   VTEqbar      *eqbar;
   VTSuprimento *suprimento;
   VTComentario *comentario;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//verifica se já possui suprimento
      lisEQP->Clear();
      barra->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      if(lisEQP->Count > 0)
         {
         return(false);
         }
      //cria um novo Eqpto
      if ((suprimento = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(false);
      //inicia dados do Eqpto
      suprimento->Id        = FalsoId();
      suprimento->Codigo    = "";
      suprimento->Extern_id = barra->Id;
//      suprimento->ShowAsUnsigned = eqp_FIXO | sup_TIPO;
      suprimento->vnom      = barra->vnom;
      suprimento->smax      = 100.;
      suprimento->tiposup   = supVF;
      suprimento->zeq0.r      = 0.;
      suprimento->zeq0.x      = 0.05;
      suprimento->zeq1.r      = 0.;
      suprimento->zeq1.x      = 0.05;
      suprimento->pcc_3f.p    = 0.;
      suprimento->pcc_3f.q    = 2000.;
      suprimento->pcc_ft.p    = 0.;
      suprimento->pcc_ft.q    = 2000.;
      //cria Curva própria p/ o Suprimento: define curva com 24 pontos
      curva                = DLL_NewObjCurva();
      curva->Id            = -1;
      curva->Codigo        = "";
      curva->Tipo          = curvaPQVT;
      curva->Unidade       = unidMVA;
      curva->Escala        = patEDITAVEL;
      curva->NumeroValores = nvCURVA_PQVT;
      curva->Tipica        = false;
      curva->Classe        = NULL;
      curva->LimiteInferior = 0;
      curva->LimiteSuperior = 0;
      //preenche dados de 1 patamar
      hm_ini.hora       = 0;
      hm_ini.minuto     = 0;
      hm_fim.hora       = 24;
      hm_fim.minuto     = 0;
      valor[indP] = 0;
      valor[indQ] = 0;
      valor[indV] = 1;
      valor[indT] = 0;
      if (! curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQVT)) return(false);
      suprimento->DefineCurva(curva);
      //valida dados de equivalente: assume Pcc default
      suprimento->pcc_3f.p =   0.;
      suprimento->pcc_3f.q = 600.;
      suprimento->pcc_ft.p =   0.;
      suprimento->pcc_ft.q = 400.;
      //esquematico
      suprimento->esq.posbar  =  0;
      suprimento->esq.dxbar   =  2000;
      suprimento->esq.dybar   = -2000;
      //insere Eqpbar na Barra
      barra->InsereEqbar(suprimento);
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(true);
   }
/*
//-----------------------------------------------------------------------------
bool __fastcall TRetiraEtd::InsereSuprimento(VTBarra *barra)
   {
   //variáveis locais
   strSUPRIMENTO str_sup;
   VTMonta       *monta = (VTMonta*)apl->GetObject(__classid(VTMonta));

  try{//insere suprimento
      str_sup.sup_id     = FalsoId();
      str_sup.bar_id     = barra->Id;
      str_sup.codigo     = "";
      str_sup.tiposup    = supVF;
      str_sup.smax       = 100.;
      str_sup.vnom_kv    = barra->vnom;
      str_sup.zeq0.r       = 0.;
      str_sup.zeq0.x       = 0.05;
      str_sup.zeq1.r       = 0.;
      str_sup.zeq1.x       = 0.05;
      str_sup.pcc_3f.p   = 0.;
      str_sup.pcc_3f.q   = 2000.;
      str_sup.pcc_ft.p   = 0.;
      str_sup.pcc_ft.q   = 2000.;
      //define curva com 24 pontos
      str_sup.curva.id        = -1;
      str_sup.curva.tipica    = false;
      str_sup.curva.num_pat   = 1;
      str_sup.curva.num_valor = 4;
      for (int np = 0; np < str_sup.curva.num_pat; np++)
         {//preenche dados do patamar
         str_sup.curva.patamar[np].hm_ini.hora      = 0;
         str_sup.curva.patamar[np].hm_ini.minuto    = 0;
         str_sup.curva.patamar[np].hm_fim.hora      = 24;
         str_sup.curva.patamar[np].hm_fim.minuto    = 0;
         str_sup.curva.patamar[np].valor[indP] = 0;
         str_sup.curva.patamar[np].valor[indQ] = 0;
         str_sup.curva.patamar[np].valor[indV] = 1;
         str_sup.curva.patamar[np].valor[indT] = 0;
         }
      //insere Suprimento
      monta->InsereSuprimento(str_sup);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);       
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TRetiraEtd::InsereToDel(VTEqpto *eqpto)
   {
   //verifica se Eqpto  já existe em lisDEL
   if (lisDEL->IndexOf(eqpto) < 0) lisDEL->Add(eqpto);
   }

//-----------------------------------------------------------------------------
void __fastcall TRetiraEtd::InsereToDel(TList *lisEQP)
   {
   for (int n = 0; n < lisEQP->Count; n++)
      {
      InsereToDel((VTEqpto*)lisEQP->Items[n]);
      }
   }

//---------------------------------------------------------------------------
//eof
