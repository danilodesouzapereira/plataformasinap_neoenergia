//---------------------------------------------------------------------------
#ifndef VTGeraRedeH
#define VTGeraRedeH
//---------------------------------------------------------------------------
class VTApl;
class VTBlocos;
//---------------------------------------------------------------------------
class VTGeraRede: public TObject
   {
   public:
                        __fastcall VTGeraRede(void) {};
      virtual           __fastcall ~VTGeraRede(void) {};
      virtual   bool    __fastcall Executa(TList* lisLiga, TList *lisFora) = 0;
      virtual   bool    __fastcall ExecutaBlocos(TList* lisLiga, VTBlocos* blocos) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTConfiab
//---------------------------------------------------------------------------
VTGeraRede* __fastcall NewObjGeraRede(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

 