//---------------------------------------------------------------------------
#ifndef TPtocargaH
#define TPtocargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTRede;

//---------------------------------------------------------------------------
class TPtocarga : public TObject
   {
   public:
      __fastcall  TPtocarga(VTRede *rede, VTBarra *barra);
      __fastcall ~TPtocarga(void);

   public:  //dados
      VTRede     *rede;
      VTBarra    *barra;
      int        num_cons_res;
      int        num_cons_com;
      int        num_cons_ind;
      int        num_cons_rur;
      int        num_cons_out;
      int        num_cons_a4;
      int        num_cons_ip;
      VTCarga    *cons_res;
      VTCarga    *cons_com;
      VTCarga    *cons_ind;
      VTCarga    *cons_rur;
      VTCarga    *cons_out;
      VTCarga    *cons_a4;
      VTCarga    *cons_ip;
   };

//---------------------------------------------------------------------------
#endif
//eof

