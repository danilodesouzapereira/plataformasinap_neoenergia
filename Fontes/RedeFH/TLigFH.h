//---------------------------------------------------------------------------
#ifndef TLigFHH
#define TLigFHH

//arquivos incluídos-----------------------------------------------------------
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

   private: //métodos acessados via property
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
         int      num_pat;    // Número de patamares armazenados
         int      num_freq;   // Número de frequências harmônicas armazenadas
         int      tamanho2;   // Valor máximo da segunda dimensão (fases da ligação)
         VTMatriz *dht_corr;  // Distorção harmônica total de corrente em % [ind_pat][ind_fase]
         VTMatriz *ief_amp;   // Valor eficaz de corrente em A              [ind_pat][ind_fase]
         VTMatriz *dh_corr;   // Distorção harmônica de corrente em %       [ind_pat][ind_fase][ind_freq]
         VTMatriz *iamp;      // Corrente harmônica em A                    [ind_pat][ind_fase][ind_freq]
         VTMatriz *perda_kva; // Perda em kVA                               [ind_pat][ind_fase][ind_freq]
         }PD;
   };

#endif
//-----------------------------------------------------------------------------
// eof
