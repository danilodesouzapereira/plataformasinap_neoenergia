//---------------------------------------------------------------------------
#ifndef TPerdaH
#define TPerdaH

//---------------------------------------------------------------------------
#include "VTPerda.h"

//---------------------------------------------------------------------------
class TPerda : public VTPerda
   {
   public:
           __fastcall  TPerda(void);
           __fastcall ~TPerda(void);
      void __fastcall  Soma(VTPerda *perda);

   protected:  //métodos acessados via property
      AnsiString __fastcall PM_GetDescricao(void);
      double     __fastcall PM_GetEnergia_mwh(int seg_id);
      bool       __fastcall PM_GetExisteSegmento(int seg_id);
      TObject*   __fastcall PM_GetObject(void);
      void       __fastcall PM_SetDescricao(AnsiString descricao);
      void       __fastcall PM_SetObject(TObject *object);
      void       __fastcall PM_SetEnergia_mwh(int seg_id, double Energia_mwh);

   private: //dados locais acessados via property
      struct   {
               TObject    *object;
               AnsiString descricao;
               double     cabo;
               double     trf;
               double     trf_cu;
               double     trf_fe;
               double     ramal;
               double     medidor;
               double     outros;
               double     fluxo;    //perda calculado pelo fluxo
               double     fora;     //perda nãO calculada pelo fluxo
               double     total;    //perda total, incluindo parcelas extras não calculadas pelo fluxo
               } PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

