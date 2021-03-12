//---------------------------------------------------------------------------
#ifndef TTipoChaveH
#define TTipoChaveH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTTipoChave.h"

//---------------------------------------------------------------------------
class TTipoChave : public VTTipoChave
   {
   public:
                   __fastcall  TTipoChave(void);
                   __fastcall ~TTipoChave(void);
      VTTipoChave* __fastcall  Clone(void);
      void         __fastcall  CopiaAtributosDe(VTTipoChave &tipochave);

   protected: //métodos acessados via property
      AnsiString __fastcall PM_GetCodigo(void);
      TColor     __fastcall PM_GetColor(void);
      AnsiString __fastcall PM_GetExtern_id(void);
      int        __fastcall PM_GetId(void);
      TObject*   __fastcall PM_GetObj(void);
      int        __fastcall PM_GetOperacao(void);
      bool       __fastcall PM_GetStatus(unsigned bit);
      unsigned   __fastcall PM_GetStatusAsUnsigned(void);
      int        __fastcall PM_GetTipo(void);
      void       __fastcall PM_SetCodigo(AnsiString codigo);
      void       __fastcall PM_SetColor(TColor color);
      void       __fastcall PM_SetExtern_id(AnsiString extern_id);
      void       __fastcall PM_SetId(int id);
      void       __fastcall PM_SetObj(TObject* obj);
      void       __fastcall PM_SetOperacao(int modo);
      void       __fastcall PM_SetStatus(unsigned bit, bool enabled);
      void       __fastcall PM_SetStatusAsUnsigned(unsigned status);
      void       __fastcall PM_SetTipo(int tipo);

   protected: //dados acessados via property
      struct   {
               int        id;
               int        tipo;
               int        modo_operacao;
               unsigned   status;
               TColor     color;
               AnsiString codigo;
               AnsiString extern_id;
               TObject    *obj;           //ponteiro auxiliar
               } PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

