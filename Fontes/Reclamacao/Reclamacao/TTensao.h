//---------------------------------------------------------------------------
#ifndef TTensaoH
#define TTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTTensao.h"

//---------------------------------------------------------------------------
class TTensao  : public VTTensao
   {
   public:
           __fastcall  TTensao(int num_pat);
           __fastcall ~TTensao(void);
      bool __fastcall  InsereTensao(int ind_pat, double vfn_kv[MAX_FASE]);
      bool __fastcall  InsereTensao(int ind_pat, complex<double>vfn_kv[MAX_FASE]);

   protected:  //métodos acessados vias property
      TDateTime       __fastcall PM_GetData(void);
      AnsiString      __fastcall PM_GetDataAsString(void);
      bool            __fastcall PM_GetEnabled(int ind_pat);
      double          __fastcall PM_GetModVfn_kv(int ind_pat, int ind_fase);
      complex<double> __fastcall PM_GetVfn_kv(int ind_pat, int ind_fase);

   private: //métodos

   private: //dados
      struct strTENSAO  {
                        bool            enabled;
                        complex<double> vfn_kv[MAX_FASE];
                        }*tensao;

   };

//---------------------------------------------------------------------------
#endif
//eof


