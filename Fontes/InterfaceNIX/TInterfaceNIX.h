//---------------------------------------------------------------------------
#ifndef TInterfaceNIXH
#define TInterfaceNIXH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTInterfaceNIX.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTRadial;
class VTPrimario;
class VTSecundario;
class VTTrafo;
class VTChave;
class VTNixAl;
class VTNixPr;

//---------------------------------------------------------------------------
class TInterfaceNIX : public VTInterfaceNIX
   {
   public:
           __fastcall  TInterfaceNIX(VTApl *apl);
           __fastcall ~TInterfaceNIX(void);
      bool __fastcall  ExportaArqNix(TStrings *lines);
//      bool __fastcall  ExportaArqNix(TStrings *lines, bool pr_com_corrente,
//                                                          bool al_com_corrente,
//                                                          int patamar);
   private: //métodos
      bool __fastcall  AcertaIdBarras(void);
      bool __fastcall  BarraInicialIDFalso(VTPrimario *primario);
      bool __fastcall  BarraInicialIDOriginal(VTPrimario *primario);
	  bool __fastcall  BarrasIniciaisCriaFalsoID(void);
	  VTNixAl* __fastcall CriaNIXAl(VTPrimario *primario, strIMP &z0,
														 strIMP &z1, strIMP &z2,
														 bool al_com_corrente,
														 int   patamar);
	  VTNixPr* __fastcall CriaNIXPr(VTChave *chave, int index, bool pr_com_corrente);
      bool __fastcall  DeterminaZeqv(VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2);
      bool __fastcall  ExportaBlocoAL(bool al_com_corrente, int patamar);
      bool __fastcall  ExportaBlocoBC(void);
      bool __fastcall  ExportaBlocoCB(void);
      bool __fastcall  ExportaBlocoIT(void);
      bool __fastcall  ExportaBlocoNO(void);
      bool __fastcall  ExportaBlocoPR(bool pr_com_corrente);
      bool __fastcall  ExportaBlocoRG(void);
	  bool __fastcall  ExportaBlocoTF(void);
	  bool __fastcall  ExportaBlocoTFDefault(void);
	  bool __fastcall  ExportaBlocoTFELPA(void);
      bool __fastcall  ExportaBlocoTR(void);
      void __fastcall  LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT);
      bool __fastcall  PrimarioValido(VTPrimario *primario);
      bool __fastcall  SecundarioValido(VTSecundario *secundario);

   private: //dados externos
      VTApl      *apl;

   private: //dados locais
      VTRadial    *radial;
      TList       *lisEQP;
      TList       *lisBARRAS;
      TList       *lisRedesCarregadas; //FKM 2016.04.27
      TStrings    *lines;
      int         maiorId;
   };

#endif
//---------------------------------------------------------------------------
//eof

