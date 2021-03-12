//---------------------------------------------------------------------------
#ifndef TInterfaceCelPlannerH
#define TInterfaceCelPlannerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTInterfaceCelPlanner.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTRadial;
class VTPrimario;
class VTSecundario;
class VTTrafo;

//---------------------------------------------------------------------------
class TInterfaceCelPlanner : public VTInterfaceCelPlanner
   {
   public:
                     __fastcall  TInterfaceCelPlanner(VTApl *apl);
                     __fastcall ~TInterfaceCelPlanner(void);
      strCPAntena    __fastcall  AntenaDefault(void);
      AnsiString     __fastcall  AntenaDefaultAsString(void);
      bool           __fastcall  ExportaArqCelPlanner(TStrings *lines);

//      bool __fastcall  ExportaArqNix(TStrings *lines, bool pr_com_corrente,
//                                                          bool al_com_corrente,
//                                                          int patamar);
   private: //métodos
      void __fastcall CriaCabecalho(int nSb);
//      bool __fastcall  AcertaIdBarras(void);
//      bool __fastcall  BarraInicialIDFalso(VTPrimario *primario);
//      bool __fastcall  BarraInicialIDOriginal(VTPrimario *primario);
//      bool __fastcall  BarrasIniciaisCriaFalsoID(void);
//      bool __fastcall  DeterminaZeqv(VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2);
//      bool __fastcall  ExportaBlocoAL(bool al_com_corrente, int patamar);
//      bool __fastcall  ExportaBlocoBC(void);
//      bool __fastcall  ExportaBlocoCB(void);
//      bool __fastcall  ExportaBlocoIT(void);
      bool __fastcall  ExportaBlocoCargas(TList *listaBarras);
      bool __fastcall  ExportaBlocoCapacitores(TList *listaBarras);
      bool __fastcall  ExportaBlocoChaves(TList *listaLigacoes);
      bool __fastcall  ExportaBlocoGeradores(TList *listaBarras);
      bool __fastcall  ExportaBlocoReguladores(TList *listaLigacoes);
      bool __fastcall  ExportaBlocoSubestacoes(TList *listaRedes);
      bool __fastcall  ExportaBlocoTrafos(TList *listaLigacoes);
//      bool __fastcall  ExportaBlocoNO(void);

//      bool __fastcall  ExportaBlocoPR(bool pr_com_corrente);
//      bool __fastcall  ExportaBlocoRG(void);
//      bool __fastcall  ExportaBlocoTF(void);
//      bool __fastcall  ExportaBlocoTR(void);
//      void __fastcall  LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT);
//      bool __fastcall  PrimarioValido(VTPrimario *primario);
//      bool __fastcall  SecundarioValido(VTSecundario *secundario);

   private: //dados externos
      VTApl      *apl;

   private: //dados locais
      VTRadial    *radial;
      TList       *lisEQP;
      TList       *lisBARRAS;
      TList       *lisSEDS;
      TList       *lisLIG;
      TList       *lisRedesCarregadas; //FKM 2016.04.27
      TStrings    *lines;
      TStringList *lines_eqptos;
      int         maiorId;
   };

#endif
//---------------------------------------------------------------------------
//eof

