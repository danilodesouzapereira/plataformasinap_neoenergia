//---------------------------------------------------------------------------
#ifndef VTEsquematicoH
#define VTEsquematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum enumSENTIDO {esqHLR, esqHRL, esqVBT, esqVTB};

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCluster;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTEsquematico : public TObject
   {
   public:
                   __fastcall VTEsquematico(void) {};
						 __fastcall ~VTEsquematico(void) {};
		virtual bool __fastcall AjustaCoordBarraSemCoordUtm(VTRedes *redes) = 0;
      virtual void __fastcall Align(VTRede *rede, int align) = 0;
		virtual bool __fastcall ExisteBarraSemCoordEsq(VTRede *rede) = 0;
		virtual bool __fastcall ExisteBarraSemCoordUtm(VTRede *rede) = 0;
		virtual bool __fastcall ExisteBarraSemCoordEsq(VTRedes *redes) = 0;
		virtual bool __fastcall ExisteBarraSemCoordUtm(VTRedes *redes) = 0;
      virtual void __fastcall IniciaEsquematico(VTRedes *redes) = 0;
      virtual bool __fastcall IniciaEsquematicoIgualGeo(VTRedes *redes) = 0;
      virtual void __fastcall InverteCoordX(VTRedes *redes) = 0;
      virtual void __fastcall InverteCoordY(VTRedes *redes) = 0;
      virtual void __fastcall MontaEsquematicoGeral(VTRede *rede) = 0;
      virtual void __fastcall MontaEsquematicoGeral(VTRedes *redes) = 0;
      virtual void __fastcall MontaEsquematicoParcial(VTCluster *cluster) = 0;
      virtual void __fastcall MontaEsquematicoParcial(VTRede *rede) = 0;
      virtual bool __fastcall RotacionaBarras(VTBarra *barra_ref, double ang_grau) = 0;
      virtual void __fastcall SalvaEsquematicoGeral(VTRedes *redes) = 0;
      virtual void __fastcall SalvaEsquematicoGeral(VTCluster *cluster) = 0;
      virtual void __fastcall SalvaEsquematicoGeral(VTRede *rede) = 0;
      virtual void __fastcall SalvaEsquematicoParcial(VTCluster *cluster) = 0;
      virtual void __fastcall SalvaEsquematicoParcial(VTRede *rede) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsquematico
//---------------------------------------------------------------------------
VTEsquematico* __fastcall NewObjEsquematico(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 