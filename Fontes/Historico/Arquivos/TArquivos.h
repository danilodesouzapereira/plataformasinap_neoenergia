//---------------------------------------------------------------------------
#ifndef TArquivosH
#define TArquivosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTArquivos.h"

//---------------------------------------------------------------------------
class TArquivos : public VTArquivos
   {
   public:
                   __fastcall  TArquivos(VTApl *apl_owner);
                   __fastcall ~TArquivos(void);
      void         __fastcall  Clear(void);
      TArqMedicao* __fastcall  ExisteArqMedicao(TDateTime dt);
      TArqRede*    __fastcall  ExisteArqRede(TDateTime dt);
      TArqRede*    __fastcall  ExisteArqRede(TDateTime dt_ini, TDateTime dt_fim);
      void         __fastcall  IniciaArqMedicao(TDateTime dt_ini, TDateTime dt_fim);
      void         __fastcall  IniciaArqRedePorMes(TDateTime dt_ini, TDateTime dt_fim);
      void         __fastcall  IniciaArqRedeUnico(TDateTime dt_ini, TDateTime dt_fim);
      TList*       __fastcall  LisArqMedicao(void);
      TList*       __fastcall  LisArqRede(void);

   private:  //métodos
      void __fastcall InsereArqMedicaoMensal(TDateTime dt_ini, TDateTime dt_fim);
      void __fastcall InsereArqRedeMensal(TDateTime dt_ini, TDateTime dt_fim);
      void __fastcall RemoveArqMedicaoForaDoPeriodo(TDateTime dt_ini, TDateTime dt_fim);
      void __fastcall RemoveArqRedeForaDoPeriodo(TDateTime dt_ini, TDateTime dt_fim);

   private:  //objetos externos
      VTApl *apl;

   private:  //dados
      TList     *lisARQ_MEDICAO;
      TList     *lisARQ_REDE;
      struct{
            int tipo_arq_rede;
            }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

