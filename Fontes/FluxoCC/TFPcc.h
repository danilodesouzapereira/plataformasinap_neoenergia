//---------------------------------------------------------------------------
#ifndef TFPccH
#define TFPccH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFPcc.h"

//---------------------------------------------------------------------------
class VTData;
class VTSisLin;
class TBar;

//---------------------------------------------------------------------------
class TFPcc : public VTFPcc
   {
   public:
           __fastcall  TFPcc(VTApl *apl);
           __fastcall ~TFPcc(void);
      void __fastcall  EnableAviso(bool enabled);
      void __fastcall  EnableRelatorio(bool enabled);
      bool __fastcall  Executa(VTData     *data,
                               AnsiString NomeRede,
                               int        ind_pat);
      void __fastcall  Relatorios(TStringList *StrList);

   private: //dados locais
	  AnsiString __fastcall DirTmp(void);
	  TBar *     __fastcall ExisteBar(TList *lisBAR,
									  int   tipo);
	  bool       __fastcall ExecutaCalculo(int ind_pat);
	  void       __fastcall ImprimeResultados(int ind_pat);
	  void       __fastcall PreparaBarSwing(void);
      void       __fastcall PreparaIndiceBar(void);

   private: //objetos externos
      VTApl  *apl;
      VTData *data;

   private: //dados locais
      double     RadGrau;
      double     *vetorB;
      AnsiString NomeRede;
      VTSisLin   *bnodal;
      struct   {
               bool enabled;
               } aviso, relatorio;
   };

#endif
//---------------------------------------------------------------------------
//eof
