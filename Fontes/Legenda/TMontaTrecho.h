//---------------------------------------------------------------------------
#ifndef TMontaTrechoH
#define TMontaTrechoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTFases;
class VTTrecho;

//---------------------------------------------------------------------------
class TMontaTrecho : public TMontaLigacao
   {
   public:
                 __fastcall TMontaTrecho(VTApl *apl);
                 __fastcall ~TMontaTrecho(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      //AnsiString __fastcall TipoMonoBiTrifasico(int fases);
      void       __fastcall ValorArranjo(void);
      void       __fastcall ValorComprimento(void);
      void       __fastcall ValorFases(void);
      //void       __fastcall ValorTipo(void);
      void       __fastcall ValorIadm(void);
      void       __fastcall ValorZseq(int unidade);
      void       __fastcall ValorZseq0(int unidade);
      void       __fastcall ValorZseq1(int unidade);

   private: //objetos externos
      VTTrecho *trecho;
      VTFases  *fases;

   private: //dados locais
      enum     ueZ {ueZ_OHM=1, ueZ_PU};
      VTMonta *monta_arranjo;
   };

//---------------------------------------------------------------------------
#endif
//eof
