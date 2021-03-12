//---------------------------------------------------------------------------
#ifndef VTTroncoH
#define VTTroncoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTCabo;
class VTPrimario;
class VTSecundario;

//---------------------------------------------------------------------------
class VTTronco : public TObject
   {
   public:  //property
      __property bool   CorrenteRamalLimitada = {read=PD.i_ramal_limitada, write=PD.i_ramal_limitada};
      __property double CorrenteRamalMax_pu   = {read=PD.i_ramal_max_pu,   write=PD.i_ramal_max_pu};

   public:
                         __fastcall VTTronco(void) {};
      virtual            __fastcall ~VTTronco(void) {};
      virtual VTArranjo* __fastcall ArranjoRamalBifasico(void) = 0;
      virtual VTArranjo* __fastcall ArranjoRamalMonofasico(void) = 0;
      virtual VTArranjo* __fastcall ArranjoRamalTrifasico(void) = 0;
      virtual VTArranjo* __fastcall ArranjoTroncoBifasico(void) = 0;
      virtual VTArranjo* __fastcall ArranjoTroncoMonofasico(void) = 0;
      virtual VTArranjo* __fastcall ArranjoTroncoTrifasico(void) = 0;
      virtual VTCabo*    __fastcall CaboRamalBifasico(void) = 0;
      virtual VTCabo*    __fastcall CaboRamalMonofasico(void) = 0;
      virtual VTCabo*    __fastcall CaboRamalTrifasico(void) = 0;
      virtual VTCabo*    __fastcall CaboTroncoBifasico(void) = 0;
      virtual VTCabo*    __fastcall CaboTroncoMonofasico(void) = 0;
      virtual VTCabo*    __fastcall CaboTroncoTrifasico(void) = 0;
      virtual AnsiString __fastcall CodigoCaboRamalBifasico(void) = 0;
      virtual AnsiString __fastcall CodigoCaboRamalMonofasico(void) = 0;
      virtual AnsiString __fastcall CodigoCaboRamalTrifasico(void) = 0;
      virtual AnsiString __fastcall CodigoCaboTroncoBifasico(void) = 0;
      virtual AnsiString __fastcall CodigoCaboTroncoMonofasico(void) = 0;
      virtual AnsiString __fastcall CodigoCaboTroncoTrifasico(void) = 0;
      virtual int        __fastcall CompRamalBifasico(void) = 0;
      virtual int        __fastcall CompRamalMonofasico(void) = 0;
      virtual int        __fastcall CompRamalTrifasico(void) = 0;
      virtual int        __fastcall CompTroncoBifasico(void) = 0;
      virtual int        __fastcall CompTroncoMonofasico(void) = 0;
      virtual int        __fastcall CompTroncoTrifasico(void) = 0;
      virtual bool       __fastcall DefinePrimario(VTPrimario *primario, int ind_pat) = 0;
      virtual bool       __fastcall DefineSecundario(VTSecundario *secundario, int ind_pat) = 0;
      virtual TList*     __fastcall LisBarraTronco(void) = 0;
      virtual TList*     __fastcall LisLigacaoRamal(void) = 0;
      virtual TList*     __fastcall LisLigacaoTronco(void) = 0;
      virtual double     __fastcall MaiorTensaoTronco(int ind_pat) = 0;
      virtual double     __fastcall MenorTensaoTronco(int ind_pat) = 0;

   protected:
      struct{
            bool   i_ramal_limitada;
            double i_ramal_max_pu;  //pu da corrente no início da rede
            }PD;
   };

//---------------------------------------------------------------------------
//método global p/ criar objetos VTTronco
//---------------------------------------------------------------------------
VTTronco* __fastcall NewObjTronco(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof