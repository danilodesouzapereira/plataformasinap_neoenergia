//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TInterfaceCelPlanner.h"
#include "TCPCapacitor.h"
#include "TCPCarga.h"
#include "TCPChave.h"
#include "TCPEqpto.h"
#include "TCPGerador.h"
#include "TCPRegulador.h"
#include "TCPSubestacao.h"
#include "TCPTrafo.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Radial\VTSubestacao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTInterfaceCelPlanner* __fastcall NewObjInterfaceCelPlanner(VTApl *apl)
   {
   return(new TInterfaceCelPlanner(apl));
   }

//---------------------------------------------------------------------------
__fastcall TInterfaceCelPlanner::TInterfaceCelPlanner(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   lisEQP = new TList();
   lisBARRAS = new TList();
   lisLIG = new TList();
   lisSEDS = new TList();
   lisRedesCarregadas = new TList(); //FKM 2016.04.27
   radial = DLL_NewObjRadial(apl);
   //inicializa o maioID
   maiorId = 0;
   //inicializa cfg
   cfg.exporta_cargas      = true;
   cfg.exporta_chaves      = true;
   cfg.exporta_geradores   = true;
   cfg.exporta_religadores = true;
   cfg.exporta_reguladores = true;
   cfg.exporta_capacitores = true;
   cfg.exporta_subestacao  = true;
   cfg.exporta_seccionadoras = true;
   cfg.exporta_trafos      = true;
   cfg.utm                 = false;
   //cria uma lsita de strings
   lines_eqptos = new TStringList;
   }

//---------------------------------------------------------------------------
__fastcall TInterfaceCelPlanner::~TInterfaceCelPlanner(void)
   {
   //destrói objetos
   if (radial) {delete radial; radial = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisBARRAS) {delete lisBARRAS; lisBARRAS = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisSEDS) {delete lisSEDS; lisSEDS = NULL;}

   if (lisRedesCarregadas) {delete lisRedesCarregadas; lisRedesCarregadas = NULL;} //FKM 2016.04.27
   }

//---------------------------------------------------------------------------
strCPAntena  __fastcall  TInterfaceCelPlanner::AntenaDefault(void)
   {//variaveis locais
   strCPAntena antena;

   antena.ota   = 0;
   antena.tal   = 0;
   antena.typ   = "TR";
   antena.aht   = 6;
   antena.pwr   = 10;
   antena.txl   = 2;
   antena.rxl   = 2;
   antena.rxs   = 0;
   antena.rnf   = 12.4;
   antena.trd   = 0.05;
   antena.tru   = 0.05;
   antena.drd   = 0;
   antena.dru   = 0;
   antena.prb   = 100;
   antena.ant   = "751637_390";
   antena.dst   = 0;
   antena.azt   = 0;
   antena.azm   = 0;
   antena.ind   = 0;
   antena.inu   = 0;
   antena.celld = "DF-1";
   antena.cellu = "DF-1";

   return (antena);
   }
//---------------------------------------------------------------------------
AnsiString __fastcall  TInterfaceCelPlanner::AntenaDefaultAsString(void)
   {//variáveis locais
   AnsiString  separador = "\t";
   AnsiString  txt;
   strCPAntena antena;

   antena = AntenaDefault();
   txt  = IntToStr(antena.ota) + separador;
   txt += IntToStr(antena.tal) + separador;
   txt += antena.typ + separador;
   txt += IntToStr(antena.aht) + separador;
   txt += IntToStr(antena.pwr) + separador;
   txt += IntToStr(antena.txl) + separador;
   txt += IntToStr(antena.rxl) + separador;
   txt += " " +separador;
   txt += DoubleToStr("%2.1f", antena.rnf) + separador;
   txt += DoubleToStr("%2.1f", antena.trd) + separador;
   txt += DoubleToStr("%2.1f", antena.tru) + separador;
   txt += " " +separador;
   txt += " " +separador;
   txt += IntToStr(antena.prb) + separador;
   txt += antena.ant + separador;
   txt += antena.celld + separador;
   txt += antena.cellu + separador;
   txt += DoubleToStr("%2.1f", antena.dst) + separador;
   txt += DoubleToStr("%2.1f", antena.azt) + separador;
   txt += DoubleToStr("%2.1f", antena.azm) + separador;
   txt += DoubleToStr("%2.1f", antena.ind) + separador;
   txt += DoubleToStr("%2.1f", antena.inu) + separador;

   return(txt);
   }
//---------------------------------------------------------------------------
void __fastcall TInterfaceCelPlanner::CriaCabecalho(int nSb)
   {//variáveis locais
   AnsiString       separador = "\t";
   AnsiString txt;

   //adiciona o nome
   lines->Add("[CelPlan DT SBC]");
   //quebra mais uma linha
   lines->Add("");
   //unidade
   txt  = "Uni" + separador;
   txt += "MT";
   lines->Add(txt);
   //descobrir o que é
   txt = "Cst" + separador;
   txt += "0" + separador;
   txt += "0" + separador;
   txt += "0" + separador;
   txt += "0" + separador;
   txt += "0" + separador;
   txt += "0" + separador;
   lines->Add(txt);
   //quebra mais uma linha
   lines->Add("");
   //acho que é descrição
   txt = "Dsc" + separador;
   txt += "Ud1" + separador;
   txt += "Ud2" + separador;
   txt += "Ud3" + separador;
   txt += "Ud4" + separador;
   txt += "Ud5" + separador;
   txt += "Ud6" + separador;
   txt += "Ud7" + separador;
   txt += "Ud8" + separador;
   txt += "nSB" + separador;
   lines->Add(txt);
   //campos do usuario
   txt = "Subscriber CelPlan" + separador;
   txt += "UserData1" + separador;
   txt += "UserData2" + separador;
   txt += "UserData3" + separador;
   txt += "UserData4" + separador;
   txt += "UserData5" + separador;
   txt += "UserData6" + separador;
   txt += "UserData7" + separador;
   txt += "UserData8" + separador;
   txt += IntToStr(nSb) + separador;
   lines->Add(txt);
   //quebra mais uma linha
   lines->Add("");
   //cabecalho da tabela
   txt = "Sbc" +separador;
   txt += "Ud1" +separador;
   txt += "Ud2" +separador;
   txt += "Ud3" +separador;
   txt += "Ud4" +separador;
   txt += "Ud5" +separador;
   txt += "Ud6" +separador;
   txt += "Ud7" +separador;
   txt += "Ud8" +separador;
   txt += "Act" +separador;
   txt += "Phs" +separador;
   txt += "Are" +separador;
   txt += "Fg1" +separador;
   txt += "Fg2" +separador;
   txt += "Fg3" +separador;
   txt += "Fg4" +separador;
   txt += "Fg5" +separador;
   txt += "Fg6" +separador;
   txt += "Fg7" +separador;
   txt += "Fg8" +separador;
   txt += "Col" +separador;
   if(cfg.utm)
      {
      txt += "Eas" +separador;
      txt += "Nor" +separador;
      }
   else
      {
      txt += "Lat" +separador;
      txt += separador;
      txt += separador;
      txt += separador;
      txt += "Lon" +separador;
      txt += separador;
      txt += separador;
      txt += separador;
      }
   txt += "Ota" +separador;
   txt += "Tal" +separador;
   txt += "Typ" +separador;
   txt += "Aht" +separador;
   txt += "Pwr" +separador;
   txt += "Txl" +separador;
   txt += "Rxl" +separador;
   txt += "Rxs" +separador;
   txt += "Rnf" +separador;
   txt += "TrD" +separador;
   txt += "TrU" +separador;
   txt += "DrD" +separador;
   txt += "DrU" +separador;
   txt += "Prb" +separador;
   txt += "Ant" +separador;
   txt += "CellD" +separador;
   txt += "CellU" +separador;
   txt += "Dst" +separador;
   txt += "AzT" +separador;
   txt += "AzM" +separador;
   txt += "InD" +separador;
   txt += "InU" +separador;
   lines->Add(txt);
//   txt  = "[CelPlan DT SBC]"                 + "\n";
//   txt += ""                                 + "\n";
//   txt += "Uni" + separador + "MT"           + "\n";
//   //descobrir o que é
//   txt += "Cst" + separador;
//   txt += "0" + separador;
//   txt += "0" + separador;
//   txt += "0" + separador;
//   txt += "0" + separador;
//   txt += "0" + separador;
//   txt += "0" + separador;
//   txt += "\n";
//   //quebra mais uma linha
//   txt += "\n";
//   //acho que é descrição
//   txt += "Dsc" + separador;
//   txt += "Ud1" + separador;
//   txt += "Ud2" + separador;
//   txt += "Ud3" + separador;
//   txt += "Ud4" + separador;
//   txt += "Ud5" + separador;
//   txt += "Ud6" + separador;
//   txt += "Ud7" + separador;
//   txt += "Ud8" + separador;
//   txt += "nSB" + separador;
//   txt += "\n";
//   //campos do usuario
//   txt += "Subscriber CelPlan" + separador;
//   txt += "UserData1" + separador;
//   txt += "UserData2" + separador;
//   txt += "UserData3" + separador;
//   txt += "UserData4" + separador;
//   txt += "UserData5" + separador;
//   txt += "UserData6" + separador;
//   txt += "UserData7" + separador;
//   txt += "UserData8" + separador;
//   txt += "15" + separador;
//   txt += "\n";
//   //quebra mais uma linha
//   txt += "\n";
//   //cabecalho da tabela
//   txt += "Sbc" +separador;
//   txt += "Ud1" +separador;
//   txt += "Ud2" +separador;
//   txt += "Ud3" +separador;
//   txt += "Ud4" +separador;
//   txt += "Ud5" +separador;
//   txt += "Ud6" +separador;
//   txt += "Ud7" +separador;
//   txt += "Ud8" +separador;
//   txt += "Act" +separador;
//   txt += "Phs" +separador;
//   txt += "Are" +separador;
//   txt += "Fg1" +separador;
//   txt += "Fg2" +separador;
//   txt += "Fg3" +separador;
//   txt += "Fg4" +separador;
//   txt += "Fg5" +separador;
//   txt += "Fg6" +separador;
//   txt += "Fg7" +separador;
//   txt += "Fg8" +separador;
//   txt += "Col" +separador;
//   txt += "Lat" +separador;
//   txt += separador;
//   txt += separador;
//   txt += separador;
//   txt += "Lon" +separador;
//   txt += separador;
//   txt += separador;
//   txt += separador;
//   txt += "Ota" +separador;
//   txt += "Tal" +separador;
//   txt += "Typ" +separador;
//   txt += "Aht" +separador;
//   txt += "Pwr" +separador;
//   txt += "Txl" +separador;
//   txt += "Rxl" +separador;
//   txt += "Rxs" +separador;
//   txt += "Rnf" +separador;
//   txt += "TrD" +separador;
//   txt += "TrU" +separador;
//   txt += "DrD" +separador;
//   txt += "DrU" +separador;
//   txt += "Prb" +separador;
//   txt += "Ant" +separador;
//   txt += "CellD" +separador;
//   txt += "CellU" +separador;
//   txt += "Dst" +separador;
//   txt += "AzT" +separador;
//   txt += "AzM" +separador;
//   txt += "InD" +separador;
//   txt += "InU" +separador;
//   txt += "\n";
//   return(txt);
   }
//---------------------------------------------------------------------------
//bool __fastcall  TInterfaceCelPlanner::AcertaIdBarras(void)
//{  //variáveis locais
//   VTBarra    *barra;
//   VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
//   TList      *lisBARRASidNeg = new TList;
//
//   //monta lista com Barras dos Primarios
//   lisEQP->Clear();
//   lisBARRAS->Clear();
//   redes->LisBarra(lisEQP);
//   //verifica os ids de todas barras
//   maiorId = 0;
//   for (int nb = 0; nb < lisEQP->Count; nb++)
//   {
//      barra = (VTBarra*) lisEQP->Items[nb];
//      //verifica se a barra possu id positivo
//      if(barra->Id > 0)
//      {
//         lisBARRAS->Add(barra);
//         //verifica qual o maior id
//         if(barra->Id > maiorId) maiorId = barra->Id;
//      }
//      else
//      {
//         lisBARRASidNeg->Add(barra);
//      }
//   }
//   //apos verificar qual o maior id, transforma os ids negativos em positivos
//   for(int nbn = 0; nbn < lisBARRASidNeg->Count; nbn++)
//   {
//      barra = (VTBarra*) lisBARRASidNeg->Items[nbn];
//      //soma o modulo do id ao maior id encontrado.
//      barra->Id = maiorId + abs(barra->Id);
//      //adiciona à lsita de barras
//      lisBARRAS->Add(barra);
//   }
//   //destrói lista sem destruir seus objetos
//   if (lisBARRASidNeg) {delete lisBARRASidNeg; lisBARRASidNeg = NULL;}
//   return true;
//}

//---------------------------------------------------------------------------
//bool __fastcall  TInterfaceCelPlanner::ArranjosCodigoFalso(TList *lisARRANJOS)
//{  //variaveis locais
//   VTArranjo *arranjo;
//
//   //proteção
//   if (!lisARRANJOS) {return false;}
//   //percorre todos arranjos
//   for (int na = 0; na < lisARRANJOS->Count ; nc++)
//      {
//      arranjo = (VTArranjo*) lisARRANJOS->Items[nc];
//      }
//
//}
////---------------------------------------------------------------------------
//bool __fastcall  TInterfaceCelPlanner::ArranjosCodigoOriginal(TList *lisARRANJOS)
//{
//
//}
//---------------------------------------------------------------------------
//bool __fastcall  TInterfaceCelPlanner::BarraInicialIDFalso(VTPrimario *primario)
//{  //variáveis locais
//   VTBarra    *barra;
//   VTRede     *rede;
//   int        falso_id;
//
//   //proteção
//   if(primario == NULL) return false;
//   //recupera a rede
//   rede = primario->Rede;
//   //recupera o falso id
//   falso_id = rede->Tag;
//   //altera a barra inicial
//   barra = primario->BarraInicial ;
//   //guarda o id original
//   barra->Tag = barra->Id;
//   //muda pro falso
//   barra->Id = falso_id;
//   return true;
//}
//---------------------------------------------------------------------------
//bool __fastcall  TInterfaceCelPlanner::BarraInicialIDOriginal(VTPrimario *primario)
//{  //variáveis locais
//   VTBarra    *barra;
//   int        orig_id;
//
//   //proteção
//   if(primario == NULL) return false;
//   //altera a barra inicial
//   barra = primario->BarraInicial ;
//   //recupera o id original
//   barra->Id = barra->Tag;
//   return true;
//}
//---------------------------------------------------------------------------
//bool __fastcall  TInterfaceCelPlanner::BarrasIniciaisCriaFalsoID(void)
//{  //variáveis locais
//   VTBarra    *barra;
//   VTRede     *rede;
//   VTPrimario *primario;
//   TList      *lisPRIMARIO;
//
//   lisPRIMARIO = radial->LisPrimario();
//   //verifica se ja foi acertados os ids negativos
//   if(maiorId == 0) AcertaIdBarras();
//   //percorre todos primarios
//   for (int np = 0; np < lisPRIMARIO->Count; np++)
//   {
//      primario = (VTPrimario*) lisPRIMARIO->Items[np];
//      rede = primario->Rede;
//      //guarda no tag da rede, um id provisorio
//      rede->Tag = maiorId + abs(FalsoId());
//   }
//   return true;
//}
//---------------------------------------------------------------------------
//bool __fastcall TInterfaceCelPlanner::DeterminaZeqv(VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2)
//   {
//   //variáveis locais
//   VTSuprimento *suprimento = NULL;
//
//   //código temporário
//   if (primario->Subestacao != NULL)
//      {//determina Suprimento da Subestacao
//      lisEQP->Clear();
//      primario->Subestacao->Rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
//      if (lisEQP->Count > 0) suprimento = (VTSuprimento*)lisEQP->First();
//      }
//   //verifica se foi encontrado um SUprimento
//   if (suprimento == NULL)
//      {//assume valores default
//      z0.r = 0.04167;
//      z0.x = 0.12500;
//      z1.r = 0.01667;
//      z1.x = 0.05000;
//      z2   = z1;
//      }
//   else
//      {//assume impedância equivalente do Suprimento da Subestacao
//      z0 = suprimento->zeq0;
//      z1 = suprimento->zeq1;
//      z2 = z1;
//      }
//   return(true);
//   }
//---------------------------------------------------------------------------
//FKM 2016.01.28 - subia valores zerados
//bool __fastcall TInterfaceCelPlanner::DeterminaZeqv(VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2)
//   {
//   //variáveis locais
//   VTSuprimento *suprimento = NULL;
//
//   //código temporário
//   if (primario->Subestacao != NULL)
//      {//determina Suprimento da Subestacao
//      lisEQP->Clear();
//      primario->Subestacao->Rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
//      if (lisEQP->Count > 0) suprimento = (VTSuprimento*)lisEQP->First();
//      else
//         {
//         primario->BarraInicial->LisEqbar(lisEQP, eqptoSUPRIMENTO);
//         if (lisEQP->Count > 0) suprimento = (VTSuprimento*)lisEQP->First();
//         }
//      }
//   else
//      {//determina Suprimento do Primario
//      lisEQP->Clear();
//      if(primario->BarraInicial != NULL)
//         {primario->BarraInicial->LisEqbar(lisEQP, eqptoSUPRIMENTO);}
//      if (lisEQP->Count > 0) suprimento = (VTSuprimento*)lisEQP->First();
//      }
//   //verifica se foi encontrado um SUprimento
//   if (suprimento == NULL)
//      {//assume valores default   // z0 = 0.85*z1
//      z0.r = 0.01;//0.04167;     //0.01
//      z0.x = 0.04;//0.12500;     //0.4
//      z1.r = 0.01;//0.01667;     //0.01
//      z1.x = 0.5;//0.05000;     //0.5
//      z2   = z1;
//      }
//   else
//      {//caso os valores sejam zerados...
//      if(IsDoubleZero(suprimento->zeq0.r) && IsDoubleZero(suprimento->zeq0.x))
//         {//assume valores default
//         z0.r = 0.01;//0.04167;
//         z0.x = 0.04;//0.12500;
//         }
//      else
//         {//assume impedância equivalente do Suprimento da Subestacao
//         z0 = suprimento->zeq0;
//         }
//      if(IsDoubleZero(suprimento->zeq1.r) && IsDoubleZero(suprimento->zeq1.x))
//         {//assume valores default
//         z1.r = 0.01;//0.01667;
//         z1.x = 0.5;//0.05000;
//         }
//      else
//         {//assume impedância equivalente do Suprimento da Subestacao
//         z1 = suprimento->zeq1;
//         }
//      z2 = z1;
//      }
//   return(true);
//   }
//---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaArqCelPlanner(TStrings *lines)
   {//variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   int nSb = 0;

   //obtem a lista de redes carregadas
   //redes->LisRedeCarregada(lisRedesCarregadas);
   //salva ponteiro para objeto
   this->lines = lines;
   //inicia objeto Radial
//   if (! radial->Inicia(redes))
//      {
//      Aviso("A rede selecionada não é radial");
//      return(false);
//      }
   //exporta blocos de dados do arquivo CelPlanner
   lines->Clear();
   lines_eqptos->Clear();
   //Gera lista de todas barras
   lisBARRAS->Clear();
   redes->LisBarra(lisBARRAS);
   //gera a lista de todas chaves
   lisLIG->Clear();
   redes->LisLigacao(lisLIG,eqptoCHAVE);
   //gera os blocos
   if(cfg.exporta_cargas)
      {
      if (! ExportaBlocoCargas(lisBARRAS))
         {return(false);}
      }
   if(cfg.exporta_chaves)
      {
      if (! ExportaBlocoChaves(lisLIG))
         {return(false);}
      }
   if(cfg.exporta_geradores)
      {
      if (! ExportaBlocoGeradores(lisBARRAS))
         {return(false);}
      }
   //gera a lista de todos reguladores
   lisLIG->Clear();
   redes->LisLigacao(lisLIG,eqptoREGULADOR);
   if(cfg.exporta_reguladores)
      {
      if (! ExportaBlocoReguladores(lisLIG))
         {return(false);}
      }
   //gera a lista de todos trafos
   lisLIG->Clear();
   redes->LisLigacao(lisLIG,eqptoTRAFO);
   if(cfg.exporta_trafos)
      {
      if (! ExportaBlocoTrafos(lisLIG))
         {return(false);}
      }
   if(cfg.exporta_capacitores)
      {
      if (! ExportaBlocoCapacitores(lisBARRAS))
         {return(false);}
      }
     //Gera lista de todas barras
   lisSEDS->Clear();
   redes->LisRede(lisSEDS,redeETD);
   if(cfg.exporta_subestacao)
      {
      if (! ExportaBlocoSubestacoes(lisSEDS))
         {return(false);}
      }
   //conta quantos subscribers
   nSb = lines_eqptos->Count;
   //cria cabecçalho
   CriaCabecalho(nSb);
   //copia o lines_eqpto no lines
   for(int nl = 0; nl < lines_eqptos->Count; nl++)
      {
      lines->Add(lines_eqptos->Strings[nl]);
      }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoCargas(TList *listaBarras)
   {//variáveis locais
   VTCarga    *carga;
   VTBarra    *barra;
   VTRede     *rede;
   TList      *lisCargas = new TList;
   AnsiString antena;

   try
      {//cria a string da antena
      antena = AntenaDefaultAsString();
      //percorre todas barras e lista as cargas
      for(int nb = 0; nb < listaBarras->Count; nb++)
         {
         barra = (VTBarra*) listaBarras->Items[nb];
         barra->LisEqbar(lisCargas,eqptoCARGA);
         }
      //percorre todas as cargas e adiciona no txt
      for (int n = 0; n < lisCargas->Count; n++)
         {
         carga = (VTCarga*)lisCargas->Items[n];
         //se a carga nao for ip
         if (!carga->IP)
            {
            TCPCarga cp_carga(apl, carga);
            //insere linha em StringLista
            lines_eqptos->Add(cp_carga.LinhaArqCsv(cfg.utm) + antena);
            }
         }
      //destroi lista
      if(lisCargas){delete lisCargas; lisCargas = NULL;}
      return(true);
      }
   catch(Exception &e)
      {
      return (false);
      }
   }
//---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoCapacitores(TList *listaBarras)
   {//variáveis locais
   VTCapacitor    *capacitor;
   VTBarra    *barra;
   VTRede     *rede;
   TList      *lisCap = new TList;
   AnsiString antena;

   try
      {//cria a string da antena
      antena = AntenaDefaultAsString();
      //percorre todas barras e lista as cargas
      for(int nb = 0; nb < listaBarras->Count; nb++)
         {
         barra = (VTBarra*) listaBarras->Items[nb];
         barra->LisEqbar(lisCap,eqptoCAPACITOR);
         }
      //percorre todas os capacitores e adiciona no txt
      for (int n = 0; n < lisCap->Count; n++)
         {
         capacitor = (VTCapacitor*)lisCap->Items[n];
         TCPCapacitor cp_capacitor(apl, capacitor);
         //insere linha em StringLista
         lines_eqptos->Add(cp_capacitor.LinhaArqCsv(cfg.utm) + antena);
         }
      //destroi lista
      if(lisCap){delete lisCap; lisCap = NULL;}
      return(true);
      }
   catch(Exception &e)
      {
      return (false);
      }
   }
//---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoChaves(TList *listaLigacoes)
   {//variáveis locais
   VTLigacao   *ligacao;
   VTChave     *chave;
   TList       *lisChaves = new TList;
   AnsiString antena;

   try
      {//cria a string da antena
      antena = AntenaDefaultAsString();
      //percorre todas barras e lista as cargas
      for(int nb = 0; nb < listaLigacoes->Count; nb++)
         {
         ligacao = (VTLigacao*) listaLigacoes->Items[nb];
         if(ligacao->Tipo() == eqptoCHAVE)
            {//se for somente RAs
            chave = (VTChave*)ligacao;
            if(cfg.exporta_religadores)
               {
               if(chave->TipoChave->Tipo == chaveRA)
                  {lisChaves->Add(ligacao);}
               else
                  {/*faz Nada*/}
               }
            if(cfg.exporta_religadores)
               {
               if(chave->TipoChave->Tipo == chaveSEC)
                  {lisChaves->Add(ligacao);}
               else
                  {/*faz Nada*/}
               }
            else
               {lisChaves->Add(ligacao);}
            }
         }
      //percorre todas as cargas e adiciona no txt
      for (int n = 0; n < lisChaves->Count; n++)
         {
         chave = (VTChave*)lisChaves->Items[n];
         TCPChave cp_chave(apl, chave);
         //insere linha em StringLista
         lines_eqptos->Add(cp_chave.LinhaArqCsv(cfg.utm) + antena);
         }
      //destroi lista
      if(lisChaves){delete lisChaves; lisChaves = NULL;}
      return(true);
      }
   catch(Exception &e)
      {
      return (false);
      }
   }
//---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoGeradores(TList *listaBarras)
   {//variáveis locais
   VTGerador    *gerador;
   VTBarra    *barra;
   VTRede     *rede;
   TList      *lisGeradores = new TList;
   AnsiString antena;

   try
      {//cria a string da antena
      antena = AntenaDefaultAsString();
      //percorre todas barras e lista as cargas
      for(int nb = 0; nb < listaBarras->Count; nb++)
         {
         barra = (VTBarra*) listaBarras->Items[nb];
         barra->LisEqbar(lisGeradores,eqptoGERADOR);
         }
      //percorre todas as cargas e adiciona no txt
      for (int n = 0; n < lisGeradores->Count; n++)
         {
         gerador = (VTGerador*)lisGeradores->Items[n];
         TCPGerador cp_gerador(apl,gerador);
         //insere linha em StringLista
         lines_eqptos->Add(cp_gerador.LinhaArqCsv(cfg.utm) + antena);
         }
      //destroi lista
      if(lisGeradores){delete lisGeradores; lisGeradores = NULL;}
      return(true);
      }
   catch(Exception &e)
      {
      return (false);
      }
   }
//---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoReguladores(TList *listaLigacoes)
   {//variáveis locais
   VTLigacao   *ligacao;
   VTRegulador     *regulador;
   TList       *lisReg = new TList;
   AnsiString antena;

   try
      {//cria a string da antena
      antena = AntenaDefaultAsString();
      //percorre todas barras e lista as cargas
      for(int nb = 0; nb < listaLigacoes->Count; nb++)
         {
         ligacao = (VTLigacao*) listaLigacoes->Items[nb];
         if(ligacao->Tipo() == eqptoREGULADOR)
            {//se for somente RAs
            regulador = (VTRegulador*)ligacao;
            lisReg->Add(ligacao);
            }
         }
      //percorre todas as cargas e adiciona no txt
      for (int n = 0; n < lisReg->Count; n++)
         {
         regulador = (VTRegulador*)lisReg->Items[n];
         TCPRegulador cp_regulador(apl, regulador);
         //insere linha em StringLista
         lines_eqptos->Add(cp_regulador.LinhaArqCsv(cfg.utm) + antena);
         }
      //destroi lista
      if(lisReg){delete lisReg; lisReg = NULL;}
      return(true);
      }
   catch(Exception &e)
      {
      return (false);
      }
   }
//---------------------------------------------------------------------------
bool __fastcall  TInterfaceCelPlanner::ExportaBlocoSubestacoes(TList *listaRedes)
   {//variaveis locais
   VTRede *rede;
   AnsiString antena;
   try
      {//cria a string da antena
      antena = AntenaDefaultAsString();
       //percorre todas barras e lista das redes de subestacao
      for(int nb = 0; nb < listaRedes->Count; nb++)
         {
         rede = (VTRede*) listaRedes->Items[nb];
         if(rede->TipoRede->Segmento == redeETD)
            {//se for subestacao
            TCPSubestacao cp_subestacao(apl, rede);
            //insere linha em StringLista
            lines_eqptos->Add(cp_subestacao.LinhaArqCsv(cfg.utm) + antena);
            }
         }
      return (true);
      }
   catch(Exception &e)
      {
      return (false);
      }

   }
//---------------------------------------------------------------------------
bool __fastcall  TInterfaceCelPlanner::ExportaBlocoTrafos(TList *listaLigacoes)
   {//variáveis locais
   VTLigacao   *ligacao;
   VTTrafo     *trafo;
   TList       *lisTrafos = new TList;
   AnsiString antena;
   VTRede      *rede;

   try
      {
      //cria a string da antena
      antena = AntenaDefaultAsString();
      //percorre todas barras e lista as cargas
      for(int nb = 0; nb < listaLigacoes->Count; nb++)
         {
         ligacao = (VTLigacao*) listaLigacoes->Items[nb];
         if(ligacao->Tipo() == eqptoTRAFO)
            {//se for somente RAs
            lisTrafos->Add(ligacao);
            }
         }
      //percorre todas as cargas e adiciona no txt
      for (int n = 0; n < lisTrafos->Count; n++)
         {
         trafo = (VTTrafo*)lisTrafos->Items[n];
         //se for trafo de força não exporta
         if (trafo->rede->TipoRede->Segmento != redeETD)
            {
            TCPTrafo cp_trafo(apl, trafo);
            //insere linha em StringLista
            lines_eqptos->Add(cp_trafo.LinhaArqCsv(cfg.utm) + antena);
            }
         }
      //destroi lista
      if(lisTrafos){delete lisTrafos; lisTrafos = NULL;}
      return(true);
      }
   catch(Exception &e)
      {
      return (false);
      }

   }
//---------------------------------------------------------------------------
//eof

