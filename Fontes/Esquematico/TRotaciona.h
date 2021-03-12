//---------------------------------------------------------------------------
#ifndef TRotacionaH
#define TRotacionaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRotaciona.h"

//---------------------------------------------------------------------------
//class VTConsulta;
class VTEsqRadial;
class VTEsqMalha;
class VTEsqViz;
//class VTReta;
//class VTSecao;
class VTBarra;
class VTCorte;
class VTReducao;
class VTRotaciona;

//---------------------------------------------------------------------------
class TRotaciona : public VTRotaciona
   {
   public:
           __fastcall TRotaciona(VTApl *apl);
           __fastcall ~TRotaciona(void);
      void __fastcall Executa(VTRede *rede, int align);

   private: //métodos
      void __fastcall MontaEsqNormal(VTRede *rede);
      void __fastcall MontaHoriLeftRight(VTRede *rede);
      void __fastcall MontaHoriRightLeft(VTRede *rede);
      void __fastcall MontaVertBottomTop(VTRede *rede);
      void __fastcall MontaVertTopBottom(VTRede *rede);

   private: //dados externos
      VTApl   *apl;

   private: //dados locais
      VTEsqRadial *esq_radial;
   };

//---------------------------------------------------------------------------
#endif
//eof

