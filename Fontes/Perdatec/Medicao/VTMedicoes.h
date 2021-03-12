//---------------------------------------------------------------------------
#ifndef VTMedicoesH
#define VTMedicoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class TMedicao;

//---------------------------------------------------------------------------
class VTMedicoes : public TObject
   {
   public:  //property
      __property bool Alterado = {read=PD.alterado, write=PD.alterado};

   public:
                        __fastcall  VTMedicoes(void) {};
      virtual           __fastcall ~VTMedicoes(void) {};
      virtual void      __fastcall  Clear(void) = 0;
      virtual TMedicao* __fastcall  ExisteMedicao(VTRede *rede) = 0;
      virtual bool      __fastcall  ImportaMedicao(TStrings *files) = 0;
      virtual TMedicao* __fastcall  InsereMedicao(VTRede *rede) = 0;
      virtual TList*    __fastcall  LisMedicao(void) = 0;
      virtual TList*    __fastcall  OrdenaLisMedicao(void) = 0;
      virtual void      __fastcall  RetiraMedicao(TMedicao *medicao) = 0;

   protected:   //dados acessados via property
      struct{//DVK 2016.07.28
            bool alterado;
            }PD;
   };

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTMedicoes* __fastcall NewObjMedicoes(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof

