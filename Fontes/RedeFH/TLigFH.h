//---------------------------------------------------------------------------
#ifndef TLigFHH
#define TLigFHH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTLigFH.h"
#include "..\Rede\VTLigacao.h"

class VTMatriz;

//---------------------------------------------------------------------------
class TLigFH : public VTLigFH
   {
   public:
           __fastcall  TLigFH(VTLigacao *ligacao,
                              int       num_pat,
                              int       num_freq);
           __fastcall ~TLigFH(void);
      void __fastcall  ReiniciaValores(void);

   private: //m�todos acessados via property
      VTEqpto *       __fastcall PM_GetBarRef(void);
      double          __fastcall PM_GetDH_Corr(int ind_pat, int ind_fase, int ind_freq);
      double          __fastcall PM_GetDHT_Corr(int ind_pat, int ind_fase);
      complex<double> __fastcall PM_GetIamp(int ind_pat, int ind_fase, int ind_freq);
      double          __fastcall PM_GetIef_amp(int ind_pat, int ind_fase);
      VTLigacao *     __fastcall PM_GetLigacao(void);
      complex<double> __fastcall PM_GetPerda_kva(int ind_pat, int ind_fase, int indfreq);
      //
      void            __fastcall PM_SetBarRef(VTEqpto *bar_ref);
      void            __fastcall PM_SetDH_Corr(int ind_pat, int ind_fase, int ind_freq, double dh);
      void            __fastcall PM_SetDHT_Corr(int ind_pat, int ind_fase, double dht);
      void            __fastcall PM_SetIamp(int ind_pat, int ind_fase, int ind_freq, complex<double> iamp);
      void            __fastcall PM_SetIef_amp(int ind_pat, int ind_fase, double ief_amp);
      void            __fastcall PM_SetPerda_kva(int ind_pat, int ind_fase, int ind_freq, complex<double> perda_kva);

   private: //dados acessados via property
      struct
         {
         VTLigacao *ligacao;
         VTEqpto   *bar_ref;

         //dados locais
         int      num_pat;    // N�mero de patamares armazenados
         int      num_freq;   // N�mero de frequ�ncias harm�nicas armazenadas
         int      tamanho2;   // Valor m�ximo da segunda dimens�o (fases da liga��o)
         VTMatriz *dht_corr;  // Distor��o harm�nica total de corrente em % [ind_pat][ind_fase]
         VTMatriz *ief_amp;   // Valor eficaz de corrente em A              [ind_pat][ind_fase]
         VTMatriz *dh_corr;   // Distor��o harm�nica de corrente em %       [ind_pat][ind_fase][ind_freq]
         VTMatriz *iamp;      // Corrente harm�nica em A                    [ind_pat][ind_fase][ind_freq]
         VTMatriz *perda_kva; // Perda em kVA                               [ind_pat][ind_fase][ind_freq]
         }PD;
   };

#endif
//-----------------------------------------------------------------------------
// eof
