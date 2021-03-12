//---------------------------------------------------------------------------
#ifndef VTArquivosH
#define VTArquivosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TArqMedicao;
class TArqRede;

//---------------------------------------------------------------------------
enum enumTIPO_ARQ_REDE {arqSINAP, arqGIS, arqLIS_GIS};

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTArquivos : public TObject
   {
   public:  //property
      __property int TipoArqMedicao = {read=tipo_arq.medicao, write=tipo_arq.medicao};
      __property int TipoArqRede    = {read=tipo_arq.rede,    write=tipo_arq.rede   };

   public:
                           __fastcall  VTArquivos(void) {};
                           __fastcall ~VTArquivos(void) {};
      virtual void         __fastcall  Clear(void) = 0;
      virtual TArqMedicao* __fastcall  ExisteArqMedicao(TDateTime dt) = 0;
      virtual TArqRede*    __fastcall  ExisteArqRede(TDateTime dt) = 0;
      virtual TArqRede*    __fastcall  ExisteArqRede(TDateTime dt_ini, TDateTime dt_fim) = 0;
      virtual void         __fastcall  IniciaArqMedicao(TDateTime dt_ini, TDateTime dt_fim) = 0;
      virtual void         __fastcall  IniciaArqRedePorMes(TDateTime dt_ini, TDateTime dt_fim) = 0;
      virtual void         __fastcall  IniciaArqRedeUnico(TDateTime dt_ini, TDateTime dt_fim) = 0;
      virtual TList*       __fastcall  LisArqMedicao(void) = 0;
      virtual TList*       __fastcall  LisArqRede(void) = 0;

   protected:  //dados
      struct{
            int medicao;
            int rede;
            }tipo_arq;
   };

//---------------------------------------------------------------------------
VTArquivos* __fastcall NewObjArquivos(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof

