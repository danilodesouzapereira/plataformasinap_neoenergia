//---------------------------------------------------------------------------
#ifndef VTGrupoH
#define VTGrupoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//enum tipoGRUPO{grupoFLUXO_FAS_VFN, grupoFLUXO_FAS_VFF, grupoFLUXO_FAS_IF,
//               g};

//---------------------------------------------------------------------------
class VTFasor;

//---------------------------------------------------------------------------
class VTGrupo : public TObject
   {
   public:  //property
      __property bool       Visible  = {read=PD.visible, write=PD.visible};
      __property AnsiString Codigo   = {read=PD.codigo,  write=PD.codigo};
      __property int        Tipo     = {read=PD.tipo,    write=PD.tipo};

   public:
                     __fastcall  VTGrupo(void) {};
      virtual        __fastcall ~VTGrupo(void) {};
      virtual bool   __fastcall  ExisteFasorEnabled(void) = 0;
      virtual bool   __fastcall  InsereFasor(VTFasor *fasor) = 0;
      virtual TList* __fastcall  LisFasor(void)=0;
      virtual void   __fastcall  LisFasor(TList *lisEXT)=0;

   protected:  //dados acessados via property
      struct{
            AnsiString codigo;
            int        tipo;
            bool       visible;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTGrupo
//---------------------------------------------------------------------------
VTGrupo* __fastcall NewObjGrupo(void);

//---------------------------------------------------------------------------
#endif
//eof
