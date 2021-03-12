//---------------------------------------------------------------------------
#ifndef TFasoresH
#define TFasoresH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasores.h"

//---------------------------------------------------------------------------
class TFasores : public VTFasores
   {
   public:
             __fastcall TFasores(void);
             __fastcall ~TFasores(void);
      void   __fastcall DefinePatamar(int ind_pat);
      void   __fastcall Insere(VTBarra *barra);
      void   __fastcall Insere(VTBarCC *barCC);
      void   __fastcall Insere(VTLigacao *ligacao);
      void   __fastcall Insere(VTLigCC *ligCC);
      TList* __fastcall LisFasor(void);
      void   __fastcall Reinicia(void);

   protected:  //métodos acessados via property
      bool __fastcall PM_GetTipoCurto(void);
      bool __fastcall PM_GetTipoFluxo(void);

   private: //métodos
      void __fastcall IniciaVetorCores(TColor *vet_color, int dim_vet);

   private: //dados locais
      TList *lisFASOR;
   };

//---------------------------------------------------------------------------
#endif
//eof
