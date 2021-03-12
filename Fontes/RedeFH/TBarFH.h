//---------------------------------------------------------------------------
#ifndef TBarFHH
#define TBarFHH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include ".\VTBarFH.h"

class VTMatriz;

//---------------------------------------------------------------------------
class TBarFH : public VTBarFH
   {
   public:
           __fastcall  TBarFH(VTBarra *barra,
                              int     num_pat,
                              int     num_freq);
           __fastcall ~TBarFH(void);
      void __fastcall  ReiniciaValores(void);

   private: //m�todos acessados via property
      VTBarra *       __fastcall PM_GetBarra(void);
      double          __fastcall PM_GetDH_Corr(int ind_pat, int ind_fase, int ind_freq);
      double          __fastcall PM_GetDH_Tensao(int ind_pat, int ind_fase, int ind_freq);
      double          __fastcall PM_GetDHT_Corr(int ind_pat, int ind_fase);
      double          __fastcall PM_GetDHT_Tensao(int ind_pat, int ind_fase);
      complex<double> __fastcall PM_GetIamp(int ind_pat, int ind_fase, int ind_freq);
      double          __fastcall PM_GetIef_amp(int ind_pat, int ind_fase);
      double          __fastcall PM_GetVef_pu(int ind_pat, int ind_fase);
      complex<double> __fastcall PM_GetVpu(int ind_pat, int ind_fase, int indfreq);
      //
		void            __fastcall PM_SetBarra(VTBarra *barra);
      void            __fastcall PM_SetDH_Corr(int ind_pat, int ind_fase, int ind_freq, double dh);
      void            __fastcall PM_SetDH_Tensao(int ind_pat, int ind_fase, int ind_freq, double dh);
      void            __fastcall PM_SetDHT_Corr(int ind_pat, int ind_fase, double dht);
      void            __fastcall PM_SetDHT_Tensao(int ind_pat, int ind_fase, double dht);
      void            __fastcall PM_SetIamp(int ind_pat, int ind_fase, int ind_freq, complex<double> iamp);
      void            __fastcall PM_SetIef_amp(int ind_pat, int ind_fase, double ief_amp);
      void            __fastcall PM_SetVef_pu(int ind_pat, int ind_fase, double vef_pu);
      void            __fastcall PM_SetVpu(int ind_pat, int ind_fase, int ind_freq, complex<double> vpu);

   private: //dados acessados via property
      struct
         {
         VTBarra  *barra;
         //dados locais
         int      num_pat;     // N�mero de patamares armazenados
         int      num_freq;    // N�mero de frequ�ncias harm�nicas armazenadas
         VTMatriz *dht_corr;   // Distor��o harm�nica total de corrente em % [ind_pat][ind_fase]
         VTMatriz *dht_tensao; // Distor��o harm�nica total de tens�o   em % [ind_pat][ind_fase]
         VTMatriz *ief_amp;    // Valor eficaz de corrente em A              [ind_pat][ind_fase]
         VTMatriz *vef_pu;     // Valor eficaz de tens�o em pu               [ind_pat][ind_fase]
         VTMatriz *dh_corr;    // Distor��o harm�nica de corrente em %       [ind_pat][ind_fase][ind_freq]
         VTMatriz *dh_tensao;  // Distor��o harm�nica de tensao   em %       [ind_pat][ind_fase][ind_freq]
         VTMatriz *iamp;       // Corrente harm�nica em A                    [ind_pat][ind_fase][ind_freq]
         VTMatriz *vpu;        // Tens�o   harm�nica em pu                   [ind_pat][ind_fase][ind_freq]
         }PD;
   };

#endif
//-----------------------------------------------------------------------------
// eof
