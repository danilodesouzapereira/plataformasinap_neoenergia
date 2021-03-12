//---------------------------------------------------------------------------
#ifndef TSegmentoH
#define TSegmentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTResumo;

//---------------------------------------------------------------------------
class TSegmento : public TObject
   {
   public:  //property
      __property AnsiString Level      = {read=PD.level,       write=PD.level       };
      __property int        TipoRedeId = {read=PD.tiporede_id, write=PD.tiporede_id };
      __property VTResumo*  Resumo     = {read=PD.resumo,      write=PD.resumo      };

   public:  //métodos
                 __fastcall  TSegmento(int tiporede_id);
                 __fastcall ~TSegmento(void);
      bool       __fastcall  AgregaResumos(TList *lisRESUMO);
      TSegmento* __fastcall  ExisteSegmento(int tiporede_id);
      TSegmento* __fastcall  InsereSegmento(int tiporede_id);
      TList*     __fastcall  LisSegmento(void);
      AnsiString __fastcall  TipoRedeAsString(void);

   private:  //dados acessados via property
      struct{
            int        tiporede_id;
            AnsiString level;
            VTResumo   *resumo;
            }PD;

   private:  //dados locais
      TList    *lisSEGMENTO;   //lista de Segmentos filhos
   };

//---------------------------------------------------------------------------
#endif
//eof
